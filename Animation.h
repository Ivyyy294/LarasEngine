#pragma once
#include "GameObject.h"
#include <memory>
#include <qdatetime.h>
#include <set>

namespace LarasEngine
{
	class Animation
	{
	protected:
		std::string uniqeId;
		qint64 activeSince;
		qint64 elapsed;
		size_t durationFrames;
		size_t frameNr;
		bool done;
		virtual void Run (double lag) = 0;
		GameObject* object;

	public:
		Animation(const size_t f, GameObject* _object, const std::string id)
			: durationFrames(f)
			, frameNr (0)
			, object (_object)
			, done (false)
			, activeSince (QDateTime::currentMSecsSinceEpoch())
			, uniqeId (id)
		{};

		void Update(double lag);
		inline bool Done() const { return done; };
		inline std::string GetUniqeId() const { return uniqeId; };

		typedef std::shared_ptr <Animation> Ptr;
		typedef std::vector <Animation::Ptr> Vec;
		typedef std::set <std::string> Set;
	};

	class AnimationHandler
	{
	public:
		inline bool IsAnimationActive (const std::string& uniqeAnimationId) const { return (set.find(uniqeAnimationId) != set.end()); };
		bool AddAnimation(Animation* a, bool allowDuplicate = true);
		void RunAnimations(double lag);
	private:
		Animation::Vec vec;
		Animation::Set set;
	};
}


