#pragma once
#include <qdialog.h>
#include <qobject.h>
#include "Layer.h"
#include "painter.h"
#include "GObjectHandler.h"

//ToDo
//verbindung zum Dialog über einen einheitlichen Slot steuern

namespace LarasEngine
{
	class Core
		: public QObject
	{
		Q_OBJECT
	public:
		Core();
		virtual ~Core();

		void Run ();

		inline void SetGameStatus(GameStatus status) { LE_OH.SetGameStatus (status); };
		inline void SetMenueDialog(QDialog* d) { menue = d; };
		inline void SetActiveLevel(const Level::LevelId& level) { LE_OH.SetActiveLevel (level); };
	signals:
		void Repaint();
		void ClosePainter();

	protected:
		typedef std::vector <GameObject::Pair> CrashList;

		GameObject::ObjectMap& objectVectorMap;
		Event::Vec& events;
		QDialog* menue;
		painter::Ptr w;

	private:
		void FillPipeline();
		void CheckCrash (CrashList& list, GameObject* currentObject);
		void HandleCrashList(CrashList crashList);
		void Frame();
		virtual void ProcessEvents();
		void Update ();
		void AutoDelete();
		int timer;
		std::vector <GameObject::TypIdVec> GetThreadToDoList ();
		void InitDeltaTime () const;
		
		void UpdateVecMap (const GameObject::ObjectMap& objVecMap, const GameObject::TypIdVec idVec);
		void UpdateVec (const GameObject::Vec& vec);
		void MiniPause ();
		double miniPauseTimepassed{0.f};
	private slots:
		void timerEvent (QTimerEvent* event);
	};

}


