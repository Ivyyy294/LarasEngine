#pragma once
#include <memory>
#include "CoreObject.h"
#include <qstatictext.h>
#include <qpixmap.h>
#include <qpen.h>

namespace LarasEngine
{
	class SimpleTextObject
		: public CoreObject
	{
	public:
		SimpleTextObject (const CoreObject::TypId& _typId)
			: CoreObject (_typId) {};
		SimpleTextObject (const int xPos, const int yPos, const QString _text, const CoreObject::TypId& _typId);
		virtual void Draw (QPainter* painter) override;
		inline void SetPen (const QPen& _pen) { pen = _pen; };
		void SetFont (const QFont& _font);
		void SetText (const QString& _text);
		inline QSizeF GetSize () const { return text.size (); };
	private:
		QStaticText text;
		QFont font;
		QPen pen;
	};

	class SimpleGraphicObject
		: public CoreObject
	{
	public:
		typedef std::vector <SimpleGraphicObject> Vec;

		SimpleGraphicObject (const CoreObject::TypId& _typId)
			: CoreObject (_typId) {};
		SimpleGraphicObject (const int xPos, const int yPos, const CoreObject::TypId& _typId, bool loadPixmap = true);
		virtual ~SimpleGraphicObject () {};

		inline void SetRectWidth (size_t w) { rect.setWidth (w); };
		inline void SetRectHeight (size_t h) { rect.setHeight (h); };
		void SetScale (double scale);
		
		//inline QRectF GetRect () const { return rect; };
		inline const QPixmap& GetImage () const { return image; };
		QRectF GetImageRect () const;
		QPointF GetCenter () const { return GetImageRect ().center (); };

		void MoveCenter (const QPointF& p);
		void Resize (size_t length);
		virtual void Draw (QPainter* painter) override;
		virtual QRectF GetCollisionRect () const override;
		typedef std::shared_ptr <SimpleGraphicObject> Ptr;

	protected:
		void LoadImage (const std::string& imagePath);
		void AddImage (const QString& imagePath);
		bool GetImageAtPos (const int pos, QImage& image);
		void SwitchImage (const TypId& id);
		size_t indexImage{ 0 };

	private:
		QPixmap image;
		QRectF rect;

		void ApplyRotation (QPainter* painter);
		std::vector <QImage> imageVec;
	};
}
