#include "stdafx.h"
#include "AI.h"

using namespace LarasEngine;

AIPrimaryAction::AIPrimaryAction (bool _allowSubActions)
	: done (false)
	, allowSubActions (_allowSubActions)
{}

AI::AI ()
	: activePrimaryAction (nullptr)
{
}

AI& AI::AddPrimaryAIAction (char weight, AIPrimaryAction* action)
{
	//weight already used
	assert (primaryActionMap.find (weight) == primaryActionMap.end ());

	primaryActionMap[weight].reset (action);
	return *this;
}

AI& AI::AddSubAIAction (char weight, AIAction* action)
{
	//weight already used
	assert (subActionMap.find (weight) == subActionMap.end());

	subActionMap[weight].reset (action);
	return *this;
}

void AI::Run (double lag)
{
	if (activePrimaryAction == nullptr)
		ChoosePrimaryAction ();

	if (activePrimaryAction != nullptr)
	{
		activePrimaryAction->Run (lag);

		if (activePrimaryAction->IsDone ())
			activePrimaryAction = nullptr;
	}
	
	if (activePrimaryAction == nullptr || activePrimaryAction->AllowSubActions ())
		RunSubAction (lag);
	
}

void AI::ChoosePrimaryAction ()
{
	for (auto& i : primaryActionMap)
	{
		if (i.second->Check ())
		{
			activePrimaryAction = i.second.get ();
			return;
		}
	}
}

void AI::RunSubAction (double lag)
{
	for (auto& i : subActionMap)
	{
		if (i.second->Check ())
			i.second->Run (lag);
	}
}
