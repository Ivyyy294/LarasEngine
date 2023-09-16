#pragma once
#include <qjsonobject.h>

class JsonObject
{
public:
	JsonObject (const std::string& _fileName);

	bool LoadJson ();
	bool LoadJson (const std::string& fileName);

	virtual QJsonObject GetAsJsonObject () const = 0;
	virtual void ReadJson (const QJsonObject& json) = 0;
	void SaveToJson (const std::string& fileName);
private:
	std::string fileName;
};

