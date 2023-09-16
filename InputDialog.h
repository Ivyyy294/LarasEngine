#pragma once
#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include "GameObject.h"
#include "JsonObject.h"

namespace LarasEngine
{
	class InputDialog 
		: public QDialog
		, private JsonObject
	{
		Q_OBJECT
	public:
		static std::string GetInput (const QString& text);

	private:
		virtual QJsonObject GetAsJsonObject () const override { return QJsonObject(); };
		virtual void ReadJson (const QJsonObject& json) override;

		InputDialog (const QString& text, QString& val);
		QString& _val;
		QLineEdit* lineEdit;
		QLabel* labelText;
		QPixmap pixmap;
	private slots:
		void on_returnPressed ();
	};
}


