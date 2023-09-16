#pragma once

#include "GameObject.h"
#include "Level.h"
#include "Event.h"
#include <QtMultimedia/qsoundeffect.h>
#include <qscreen.h>
#include <cmath>
#include "Factory.h"

namespace LarasEngine
{
	class GObjectHandler
		: public Factory
	{
	public:
		static GObjectHandler& Me();
		inline GameObject::ObjectMap& GetMap() { return map; };
		GameObject::Vec& GetObjectVec(GameObject::TypId typ);

		inline Event::Vec& GetEvents() { return events; };
		GameObject* AddObject(GameObject* obj);
		GameObject* AddObject (GameObject::TypId id);
		void AddEvent(Event* e);
		void reset();
		void PreLoadImage(const std::string& imageId, const std::string& path);
		const QPixmap& LoadImage(const std::string& imageId);

		void LoadSound(const std::string& soundId, const std::string& path);
		void PlaySound (const std::string& soundId, float volume, size_t loopCount = 0);
		void StopSound (const std::string& soundId);

		inline GameStatus GetGameStatus() const { return currentGameStatus; };

		void SetMiniPause (double duration);
		inline void SetActiveLevel (const Level::LevelId& levelId) { activeLevelId = levelId; };
		inline Level::Ptr GetActiveLevelPtr() { return level; };
		inline std::string GetActiveLevel () const { return activeLevelId;};
		void ResetLevel (const Level::LevelId& id);

		QRect GetPrimaryScreenGeometry () const { return QGuiApplication::primaryScreen ()->geometry (); };
		//inline qint64 GetTargetFPS () const { return 1000 / TARGET_MS_PER_UPDATE; };
		QTransform& GetCameraTransform () { return cameraTransform; };

		inline void SetGameStatus(GameStatus status) { currentGameStatus = status; };
		inline void SetMaxFPS (const qint64 maxFPS) { maxFPS == 0 ? MIN_MS_PER_UPDATE = 0 : MIN_MS_PER_UPDATE =  std::floor (1000.f / maxFPS); }
		inline void SetTargetFPS (const qint64 targetFPS) { TARGET_MS_PER_UPDATE = std::floor (1000 / targetFPS); }

		inline const qint64& GetMinMsPerUpdate () { return MIN_MS_PER_UPDATE; };
		inline const qint64& GetTargetMsPerUpdate () {return TARGET_MS_PER_UPDATE; };
		inline const double& GetMiniPauseDuration () const { return miniPauseDuration; };
	private:
		typedef std::shared_ptr <QPixmap> PixmapPtr;
		typedef std::shared_ptr <QSoundEffect> SoundEffectPtr;
		typedef std::map <std::string, PixmapPtr> QPixmapMap;
		typedef std::map <std::string, SoundEffectPtr> QSoundEffectMap;

		GObjectHandler();
		~GObjectHandler() {};

		void PreLoadTextures();
		void PreLoadSounds();

		void Error (std::string msg, bool abort);

		QPixmapMap imageMap;
		QSoundEffectMap soundMap;
		Event::Vec events;
		GameObject::ObjectMap map;
		GameStatus currentGameStatus;
		Level::Ptr level{nullptr};
		Level::LevelId activeLevelId;
		std::string activeLevel;
		qint64 MIN_MS_PER_UPDATE{ 0 };
		qint64 TARGET_MS_PER_UPDATE;
		QTransform cameraTransform;
		double miniPauseDuration{ 0.f };
	};
}

