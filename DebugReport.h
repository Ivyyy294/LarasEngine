#pragma once

//#define REPORT

class DebugReport
{
public:
	DebugReport (const std::string& _name);
	~DebugReport ();

private:
	void Start ();
	void End ();
	QString GetTabs ();
	std::string name;
	qint64 timer;
	static int ebene;
};
