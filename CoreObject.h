#pragma once
#include <qrect.h>
#include <qpainter.h>

namespace LarasEngine
{
	class CoreObjectSimple
	{
	public:
		CoreObjectSimple (const std::string& _typId);
		typedef std::string TypId;
		typedef std::vector <TypId> TypIdVec;

		inline TypId GetTypId () const { return typId; };
	protected:
		std::string typId;
	};

	class CoreObject
		: public CoreObjectSimple
	{
	public:
		CoreObject (const TypId& _typId);

		virtual void Draw (QPainter* painter) = 0;

		//void MoveCenter (QPointF _point);
		inline void SetDeleteMe (bool val) { deleteMe = val; };
		inline void SetRotation (const double r) { rotation = r; };
		inline void SetPosition (const QPointF& pos) { position = pos; };
		inline void SetPosition (const int x, const int y) { position = QPointF (x, y); };
		inline void SetCollisionRect (const QRectF& r) { collisionRect = r; };
		inline void AddCollisionBox (const QRectF& r) { collisionBoxVec.push_back (r); };

		inline const QPointF& GetPosition () const { return position; };
		inline double GetRotation () const { return rotation; };
		inline bool DeleteMe () const { return deleteMe; };
		virtual QRectF GetCollisionRect () const;

		typedef std::vector <QRectF> CollisionBoxVec;
		CollisionBoxVec GetCollisionBoxVec () const;

		virtual void Translate (const QPointF& trans) { position += trans; };

	protected:
		double screenW;
		double screenH;
		bool deleteMe{ false};
		double rotation;

		double CalculateXViaRotation (int s, double r);

	private:
		CollisionBoxVec collisionBoxVec;
		QRectF collisionRect;
		QPointF position;
	};
}