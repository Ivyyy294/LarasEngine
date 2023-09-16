#pragma once
#include <time.h>

namespace LarasEngine
{
	class TimeInfo
	{
	public:
		static 	class TimeInfo& Me ();
	
		inline const double& GetDeltaTime () const { return deltaTime; };
		inline const qint64& GetFrameTime () const { return frameTime; };
		inline const double& GetFactor () const { return factor; };

		inline void SetDeltaTime (double d) { deltaTime = d; };
		void SetFrameTime (qint64 t);
	private:
		double deltaTime{ 0.f };
		qint64 frameTime{ 0 };
		double factor{ 0.0 };
		TimeInfo () {};
	};
}
