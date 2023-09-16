#pragma once
#include <vector>
#include <memory>

namespace LarasEngine
{
	class Event
	{
	public:
		Event()
			: active(true)
		{}

		virtual ~Event() {};

		typedef std::vector <std::shared_ptr <Event>> Vec;

		inline bool IsActive() const { return active; };
		virtual bool CheckCondition() = 0;
		virtual void Run() = 0;

	protected:
		bool active;
	};
}


