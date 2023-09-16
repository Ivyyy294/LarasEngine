#include "stdafx.h"
#include "JsonObject.h"
#include <qjsondocument.h>

JsonObject::JsonObject (const std::string& _fileName)
	: fileName (_fileName)
{
}

bool JsonObject::LoadJson ()
{
	std::string jsonFileName = "./JSON/" + fileName + ".json";
	return LoadJson (jsonFileName);
}

bool JsonObject::LoadJson (const std::string& fileName)
{
	QFile loadFile (fileName.c_str ());

	bool ok = loadFile.open (QIODevice::ReadOnly);

	if (ok)
	{
		QByteArray saveData = loadFile.readAll ();

		QJsonDocument loadDoc (QJsonDocument::fromJson (saveData));

		ReadJson (loadDoc.object ());
	}

	return ok;
}

void JsonObject::SaveToJson (const std::string& fileName)
{
	QFile file (fileName.c_str ());

	bool ok = file.open (QIODevice::OpenModeFlag::WriteOnly);

	if (ok)
	{
		QJsonDocument doc (GetAsJsonObject());

		file.write (doc.toJson());
		file.close ();
	}
}
