#include "stdafx.h"
#include "GObjectHandler.h"
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qmessagebox.h>

using namespace LarasEngine;


GObjectHandler& GObjectHandler::Me()
{
	static GObjectHandler me;
	return me;
}

GameObject::Vec& GObjectHandler::GetObjectVec(GameObject::TypId typ)
{
	if (map.find(typ) == map.end())
	{
		GameObject::Vec a;
		map[typ] = a;
	}

	return map[typ];
}

GameObject* GObjectHandler::AddObject(GameObject* obj)
{
	obj->LoadJson();
	obj->Init();
	std::string typ = obj->GetTypId();

	if (map.find(typ) == map.end())
	{
		GameObject::Vec a;
		map[typ] = a;
	}

	map.at(typ).push_back(GameObject::Shared(obj));

	return map.at (typ).back ().get();
}

GameObject* LarasEngine::GObjectHandler::AddObject (GameObject::TypId id)
{
	if (GameObject* tmp = CreateObject (id))
		return AddObject (tmp);
	else
		Error ("TypId not registered to Factory!", false);

	return nullptr;
}

void GObjectHandler::AddEvent(Event* e)
{
	std::shared_ptr <Event> event(e);
	events.push_back(event);
}

void GObjectHandler::reset()
{
	events.clear();
	map.clear();
}

void LarasEngine::GObjectHandler::PreLoadImage(const std::string& imageId, const std::string& path)
{
	if (imageMap.find(imageId) == imageMap.end())
	{
		PixmapPtr i (new QPixmap);

		if (i->load (path.c_str ()))
			imageMap[imageId] = i;
		else
			Error ("Texture: " + path + " not found!", true);
	}
	else
		Error ("TextureId " + imageId + " already exists", true);
}

const QPixmap& LarasEngine::GObjectHandler::LoadImage(const std::string& imageId)
{
	if (imageMap.find (imageId) == imageMap.end ())
		Error ("Undefined ImageId used: " + imageId, true);

	return *imageMap.at(imageId);
}

void LarasEngine::GObjectHandler::LoadSound(const std::string& soundId, const std::string& path)
{
	SoundEffectPtr effect (new QSoundEffect);
	effect->setSource (QUrl::fromLocalFile (path.c_str ()));
	soundMap[soundId] = effect;
}

void LarasEngine::GObjectHandler::PlaySound(const std::string& soundId, float volume, size_t loopCount)
{
	if (soundMap.find (soundId) == soundMap.end())
		Error ("Undefined SoundId used: " + soundId, false);

	SoundEffectPtr effect = soundMap.at (soundId);

	if (effect->status () == QSoundEffect::Ready)
	{
		effect->setVolume (volume);

		if (loopCount > 0)
			effect->setLoopCount (loopCount);

		effect->play ();
	}
#ifndef _DEBUG
	else
		Error ("Sound " + soundId + " not ready!", false);
#endif // !_DEBUG
}

void LarasEngine::GObjectHandler::StopSound (const std::string& soundId)
{
	if (soundMap.find (soundId) == soundMap.end ())
		Error ("Undefined SoundId used: " + soundId, false);

	SoundEffectPtr effect = soundMap.at (soundId);

	effect->stop ();
}

void LarasEngine::GObjectHandler::SetMiniPause (double duration)
{
	currentGameStatus = STATUS_MINI_PAUSE;
	miniPauseDuration = duration;
}

void LarasEngine::GObjectHandler::ResetLevel (const Level::LevelId& id)
{
	level.reset (CreateLevel (id));
	
	if (level.get () != nullptr)
		level->Start ();
}

LarasEngine::GObjectHandler::GObjectHandler()
	: TARGET_MS_PER_UPDATE (16)
{
	PreLoadTextures();
	PreLoadSounds();
}

void LarasEngine::GObjectHandler::PreLoadTextures()
{
	std::string jsonFileName = "./JSON/textures.json";
	QFile loadFile(jsonFileName.c_str());

	bool ok = loadFile.open(QIODevice::ReadOnly);

	if (ok)
	{
		QByteArray saveData = loadFile.readAll();

		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

		QJsonObject json = loadDoc.object();

		if (json.contains("textures") && json["textures"].isArray())
		{
			QJsonArray textureArray = json["textures"].toArray();

			for (size_t i = 0; i < textureArray.size(); ++i)
			{
				if (textureArray[i].isArray())
				{
					QJsonArray texture = textureArray[i].toArray();
					std::string tName = texture[0].toString().toLatin1();
					std::string tPath = texture[1].toString().toLatin1();
					PreLoadImage(tName, tPath);
				}
			}
		}
	}
}

void LarasEngine::GObjectHandler::PreLoadSounds()
{
	std::string jsonFileName = "./JSON/sounds.json";
	QFile loadFile(jsonFileName.c_str());

	bool ok = loadFile.open(QIODevice::ReadOnly);

	if (ok)
	{
		QByteArray saveData = loadFile.readAll();

		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

		QJsonObject json = loadDoc.object();

		if (json.contains("sounds") && json["sounds"].isArray())
		{
			QJsonArray textureArray = json["sounds"].toArray();

			for (size_t i = 0; i < textureArray.size(); ++i)
			{
				if (textureArray[i].isArray())
				{
					QJsonArray texture = textureArray[i].toArray();
					std::string tName = texture[0].toString().toLatin1();
					std::string tPath = texture[1].toString().toLatin1();
					LoadSound(tName, tPath);
				}
			}
		}
	}
}

void LarasEngine::GObjectHandler::Error (std::string msg, bool abort)
{
	QCursor cursor (Qt::CursorShape::ArrowCursor);
	QApplication::setOverrideCursor (cursor);
	QApplication::changeOverrideCursor (cursor);

	QMessageBox::critical (QApplication::activeModalWidget (), "Error", QString::fromLatin1 (msg.c_str()));

	if (abort)
		QApplication::quit ();
}
