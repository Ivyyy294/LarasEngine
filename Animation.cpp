#include "stdafx.h"
#include "Animation.h"

using namespace LarasEngine;

void Animation::Update(double lag)
{
	elapsed = QDateTime::currentMSecsSinceEpoch() - activeSince;

	if (!done)
	{
		Run(lag);
		frameNr++;
	}
}

bool AnimationHandler::AddAnimation(Animation* a, bool allowDuplicate)
{
	bool ok = a != nullptr;

	if (ok)
	{
		const std::string id = a->GetUniqeId();

		if (!allowDuplicate)
			ok = set.find(id) == set.end();

		if (ok)
		{
			vec.push_back(Animation::Ptr(a));
			set.insert(id);
		}
	}

	return ok;
}

void AnimationHandler::RunAnimations(double lag)
{
	for (Animation::Vec::iterator i = vec.begin(); i != vec.end();)
	{
		i->get()->Update(lag);

		if (i->get()->Done())
			i = vec.erase(i);
		else
			++i;
	}

	set.clear();

	for (const auto& i : vec)
		set.insert(i->GetUniqeId());
}
