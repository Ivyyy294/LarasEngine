#include "stdafx.h"
#include "TimeInfo.h"
#include "GObjectHandler.h"

using namespace LarasEngine;

TimeInfo& TimeInfo::Me ()
{
	static TimeInfo me;
	return me;
}

void LarasEngine::TimeInfo::SetFrameTime (qint64 t)
{
	frameTime = t;
	factor = double (frameTime) / double (LE_OH.GetTargetMsPerUpdate ());
}
