#include "stdafx.h"
#include "DebugReport.h"
#include <qdebug.h>
#include <qdatetime.h>
#include "GObjectHandler.h"

int DebugReport::ebene = 0;

void DebugReport::Start ()
{
	qDebug () << GetTabs () + "START " << name.c_str ();
	timer = QDateTime::currentMSecsSinceEpoch ();
	++ebene;
}

void DebugReport::End ()
{
	const qint64 current = QDateTime::currentMSecsSinceEpoch ();
	const qint64 diff = current -timer;

	--ebene;
	qDebug() << GetTabs() + "END" << name.c_str() << " Executiontime: " << std::to_string (diff).c_str();
}

QString DebugReport::GetTabs ()
{
	QString tabs;

	for (int i = 0; i < ebene; ++i)
		tabs += "---";

	return tabs;
}

DebugReport::DebugReport (const std::string& _name)
	: name (_name)
{
#ifdef REPORT
	Start ();
#endif // REPORT
}

DebugReport::~DebugReport ()
{
#ifdef REPORT
	End ();
#endif // REPORT
}
