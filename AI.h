#pragma once
#include <memory>

namespace LarasEngine
{
	class AIAction
	{
	public:
		virtual bool Check () = 0;
		virtual void Run (double lag) = 0;

		typedef std::shared_ptr <AIAction> Ptr;
		typedef std::map <char, AIAction::Ptr> Map;
	};

	class AIPrimaryAction
		: public AIAction
	{
	public:
		AIPrimaryAction (bool _allowSubActions);
		typedef std::shared_ptr <AIPrimaryAction> Ptr;
		typedef std::map <char, AIPrimaryAction::Ptr> Map;


		inline bool IsDone () const { return done; };
		inline bool AllowSubActions () const { return allowSubActions; };
	protected:
		bool done;
		bool allowSubActions;
	};

	class AI
	{
	public:
		AI ();

		AI& AddPrimaryAIAction (char weight, AIPrimaryAction* action);
		AI& AddSubAIAction (char weight, AIAction* action);

		void Run (double lag);
	private:
		void ChoosePrimaryAction ();
		void RunSubAction (double lag);

		AIPrimaryAction::Map primaryActionMap;
		AIAction::Map subActionMap;
		AIPrimaryAction* activePrimaryAction;
	};
}

