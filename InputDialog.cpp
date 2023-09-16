#include "stdafx.h"
#include "InputDialog.h"
#include "GObjectHandler.h"

using namespace LarasEngine;

std::string LarasEngine::InputDialog::GetInput (const QString& text)
{
	QString val;
	LarasEngine::InputDialog d (text, val);
	d.exec ();

	return val.toLatin1 ();
}

void LarasEngine::InputDialog::ReadJson (const QJsonObject& json)
{
	if (json.contains("DialogStyle"))
		setStyleSheet (json["DialogStyle"].toString());
	 
	if (json.contains("LineEditStyle"))
		lineEdit->setStyleSheet (json["LineEditStyle"].toString());
	 
	if (json.contains("LabelStyle"))
		labelText->setStyleSheet (json["LabelStyle"].toString());
	
	if (json.contains("TextureId"))
		 pixmap = LE_OH.LoadImage (json["TextureId"].toString().toStdString());
}

LarasEngine::InputDialog::InputDialog (const QString& text, QString& val)
	: _val (val)
	, JsonObject ("InputDialog")
{
	 setModal (true);
	 setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
	 
	 lineEdit = new QLineEdit (this);
	 labelText = new QLabel (text, this);
	 
	 LoadJson ();

	 //Background Image
	 setFixedSize (pixmap.size ());
	 QLabel* label = new QLabel (this);
	 label->setPixmap (pixmap);
	 label->move (0, 0);

	 int padding = pixmap.width () * 0.05;
	 int paddingV = pixmap.height () * 0.2;

	 labelText->setFixedSize (pixmap.width () - padding * 2, paddingV);
	 labelText->move (padding, paddingV);
	 
	 lineEdit->setFixedSize (pixmap.width() - padding * 2, paddingV);
	 lineEdit->move (padding, paddingV * 3);

	 connect (lineEdit, &QLineEdit::returnPressed, this, &InputDialog::on_returnPressed);
}

void LarasEngine::InputDialog::on_returnPressed ()
{
	_val = lineEdit->text ();

	if (!_val.isEmpty())
		accept ();
}
