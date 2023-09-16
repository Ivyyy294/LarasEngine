#include "stdafx.h"
#include "Input.h"
#include "LarasEngine.h"
#include <qdebug.h>
#include <thread>
#include "DebugReport.h"
#include "TimeInfo.h"

using namespace LarasEngine;

Core::Core()
	: objectVectorMap (LE_OH.GetMap())
	, events (LE_OH.GetEvents())
{
}

Core::~Core()
{
}

void Core::Run ()
{
	while (GAMESTATUS != GameStatus::STATUS_EXIT)
	{
		if (GAMESTATUS == GameStatus::STATUS_MENUE)
		{
			QCursor cursor (Qt::ArrowCursor);
			QApplication::setOverrideCursor (cursor);
			QApplication::changeOverrideCursor (cursor);

			menue->exec();;
			LE_OH.SetGameStatus((GameStatus)menue->result());

			cursor.setShape (Qt::BlankCursor);
			QApplication::setOverrideCursor (cursor);
			QApplication::changeOverrideCursor (cursor);
		}
		else
		{
			killTimer(timer);
			
			w.reset (new painter);

			bool ok = connect(this, &Core::Repaint, w.get(), &painter::Repaint)
				&& connect(this, &Core::ClosePainter, w.get(), &painter::Close);

			w->showFullScreen();

			timer = startTimer(LE_OH.GetMinMsPerUpdate());

			w->exec();
		}
	}
}

void LarasEngine::Core::FillPipeline()
{
	Layer::Map& layers = w->GetLayers ();
	layers.clear ();

	for (const auto& vector : LE_OH.GetMap())
	{
		for (const auto& a : vector.second)
		{
			if (GameObject* obj = a.get ())
			{
				size_t layerId = obj->GetLayerId();

				if (layerId != LayerId::LAYER_INVISIBLE)
				{
					if (layers.find(layerId) == layers.end())
						layers[layerId] = Layer();

					layers[layerId].AddImage(obj);
				}
			}
		}
	}
}

void LarasEngine::Core::CheckCrash(CrashList& list, GameObject* currentObject)
{
	if (!currentObject->GetCollisionOn())
		return;

	for (auto& vecTmp : objectVectorMap)
	{
		if (!currentObject->CanCollideWithObjectTyp (vecTmp.first))
			continue;

		for (GameObject::Shared tmpObj : vecTmp.second)
		{
			if (!tmpObj.get()->GetCollisionOn()
				|| !tmpObj->CanCollideWithObjectTyp (currentObject->GetTypId()))
				continue;

			if (currentObject != tmpObj.get())
			{
				if (currentObject->CheckCrash(tmpObj.get()))
				{
					GameObject::Pair tmpPair(tmpObj.get(), currentObject);

					if (std::find(list.begin(), list.end(), tmpPair) == list.end())
						list.push_back(GameObject::Pair(currentObject, tmpObj.get()));
				}
			}
		}
	}
}

void Core::ProcessEvents()
{
	for (auto& e : events)
	{
		if (e->IsActive() && e->CheckCondition())
			e->Run();
	}
}

void Core::UpdateVecMap (const GameObject::ObjectMap& objVecMap, const GameObject::TypIdVec idVec)
{
	//Update Camera last
	for (const GameObject::TypId& id : idVec)
			UpdateVec (objVecMap.at (id));
}

void Core::UpdateVec (const GameObject::Vec& vec)
{
	double lag = TimeInfo::Me().GetFactor ();
	for (size_t i = 0; i < vec.size (); ++i)
	{
		GameObject* obj = vec.at (i).get ();

		if (obj != nullptr)
			obj->Update ();
	}
}

void LarasEngine::Core::MiniPause ()
{
	miniPauseTimepassed += TimeInfo::Me ().GetDeltaTime ();

	if (miniPauseTimepassed >= LE_OH.GetMiniPauseDuration ())
	{
		LE_OH.SetGameStatus (GameStatus::STATUS_GAME);
		miniPauseTimepassed = 0.f;
	}
}

void Core::Update()
{
	std::vector <std::thread> tVec;
	
	std::vector <GameObject::TypIdVec> todoList = GetThreadToDoList ();

	//Update threads starten
	if (todoList.size() > 0)
		UpdateVecMap (objectVectorMap, todoList.at (0));

	////for (size_t i = 0; i < todoList.size(); ++i)
	////{
	////	tVec.push_back (std::thread (Update3, objectVectorMap, todoList.at (i), playerInput, lag));
	////	qDebug() << "\t thread " << i << " started";
	////}

	////qDebug ("\t joining threads");

	//////Auf das beenden der threads warten
	////for (std::thread& i : tVec)
	////	i.join ();

	////qDebug ("\t threads finished");

	//Check for collisions, after every object moved
	CrashList list;

	for (const auto& vector : objectVectorMap)
	{
		for (const auto& iter : vector.second)
		{
			GameObject* obj = iter.get ();

			if (obj->GetCollisionOn())
				CheckCrash(list, obj);
		}
	}
	
	HandleCrashList(list);
}

void Core::timerEvent(QTimerEvent* event)
{
	Frame();
}

void Core::Frame()
{
	static std::string previousLevel;

	DebugReport r ("Frame");

	GameStatus gameStatus = GAMESTATUS;
	
	if (gameStatus == GameStatus::STATUS_EXIT
		|| gameStatus == GameStatus::STATUS_MENUE)
	{
		LE_OH.SetActiveLevel ("");
		previousLevel = "";
		Input::Me ().ResetPlayerInput ();
		killTimer (timer);
		emit ClosePainter ();
		return;
	}

	//Aktives level bei bedarf wechseln
	Level::LevelId activeLevel = LE_OH.GetActiveLevel ();

	if (!activeLevel.empty ())
	{
		if (previousLevel != activeLevel)
		{
			LE_OH.ResetLevel (activeLevel);
			previousLevel = activeLevel;
		}
	}

	if (Level::Ptr level = LE_OH.GetActiveLevelPtr ())
	{
		InitDeltaTime ();
		const qint64 frameTime = TimeInfo::Me ().GetFrameTime ();
		const qint64& minMsPerUpdate = LE_OH.GetMinMsPerUpdate ();

		if (minMsPerUpdate == 0 || (frameTime + 1 >=  minMsPerUpdate))
		{
			qDebug () << "FrameTime " << frameTime;
			level->ProcessInput ();

			if (gameStatus == GameStatus::STATUS_MINI_PAUSE)
				MiniPause ();
			else if (gameStatus != GameStatus::STATUS_PAUSE)
			{
				//Anzahl der vector durchläufe reduzieren
				ProcessEvents ();

				Update ();
				level->DoPostWork ();
				AutoDelete ();
			}

			TimeInfo::Me ().SetFrameTime (0);
			FillPipeline ();
		}
		
		emit Repaint ();

	}
}

void LarasEngine::Core::HandleCrashList(CrashList crashList)
{
	for (const auto& i : crashList)
	{
		i.first->HandleCrash(i.second);
		i.second->HandleCrash(i.first);
	}
}

void LarasEngine::Core::AutoDelete()
{
	for (auto& a : objectVectorMap)
	{
		for (GameObject::Vec::iterator iter = a.second.begin (); iter != a.second.end ();)
		{
			if (iter->get ()->DeleteMe ())
				iter = a.second.erase (iter);
			else
				++iter;
		}
	}
}

std::vector<GameObject::TypIdVec> LarasEngine::Core::GetThreadToDoList ()
{
	size_t tMaxCount = 1; //std::min (size_t (1), std::thread::hardware_concurrency ());
	size_t ovm = objectVectorMap.size ();
	double tCount = std::max (1.0, std::ceil (ovm / tMaxCount));

	GameObject::TypIdVec idVec;
	std::vector <GameObject::TypIdVec> idVecVec;

	for (const auto& vector : objectVectorMap)
	{
		if (idVec.size () < tCount)
			idVec.push_back (vector.first);

		if (idVec.size () >= tCount || vector.first == objectVectorMap.rbegin ()->first)
		{
			idVecVec.push_back (idVec);
			idVec.clear ();
		}
	}

	return idVecVec;
}

void LarasEngine::Core::InitDeltaTime () const
{
	static qint64 previous = QDateTime::currentMSecsSinceEpoch ();
	qint64 lag = TimeInfo::Me().GetFrameTime();

	qint64 current = QDateTime::currentMSecsSinceEpoch ();
	qint64 elapsed = current - previous;

	previous = current;
	lag += elapsed;

	TimeInfo& time = TimeInfo::Me ();
	time.SetDeltaTime (elapsed / 1000.f);
	time.SetFrameTime (elapsed);
}
