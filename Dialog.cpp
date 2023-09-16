#include "stdafx.h"
#include "Dialog.h"
#include <qjsonarray.h>
#include "Input.h"

using namespace LarasEngine;

Dialog::Dialog(const TypId& _typId)
	: GameObject (0, 0, _typId)
	, indexCurrentText (0)
	, maxCharCount (0)
	, framesCount (0)
	, drawNext (false)
	, textSpeed (1)
{
	font = QApplication::font();
	font.setPointSize(18);

	QRectF rect = GetImageRect ();

	textRect = rect;
	textRect.setWidth(textRect.width() * 0.9);
	textRect.setHeight(textRect.height() * 0.85);
	textRect.moveCenter(rect.center());
}

void Dialog::Draw (QPainter* painter)
{
	if (indexCurrentText < textVec.size())  
	{
		GameObject::Draw(painter);

		std::string  text = textVec.at(indexCurrentText);
		int charCount = std::min(maxCharCount, text.size());
		text = text.substr(0, charCount);

		painter->save();
		painter->setFont(font);
		painter->setPen(pen);

		painter->drawText(textRect, Qt::AlignCenter, text.c_str());
		painter->drawText(textRect, Qt::AlignBottom | Qt::AlignRight, "[ENTER]");
		painter->restore();
	}

	//textObj.Draw(painter);
}

void Dialog::Update ()
{
	if (drawNext)
	{
		if (Input::Me ().TestKey (Input::Key::Key_Enter))
		{
			indexCurrentText++;
			drawNext = false;
			maxCharCount = 0;
		}
	}
	else
	{
		if (framesCount % textSpeed == 0)
		{
			++maxCharCount;

			if (textVec.at(indexCurrentText).size() <= maxCharCount)
				drawNext = true;
		}
	}
	framesCount++;

	if (indexCurrentText >= textVec.size())
		deleteMe = true;
}

void Dialog::ReadJson(const QJsonObject& json)
{
	QJsonArray d = json["Dialogs"].toArray();

	for (size_t i = 0; i < d.size(); ++i)
	{
		std::string tmp = d[i].toString().toLatin1();
		textVec.push_back(tmp);
	}

	if (json.contains("xPos") && json.contains("yPos"))
	{
		int xPos = json["xPos"].toInt();
		int yPos = json["yPos"].toInt();
		SetPosition(xPos, yPos);
		textRect.moveCenter(GetCenter());
	}
	else
	{
		const QPixmap& image = GetImage ();
		int xPos = screenW / 2 - image.width() / 2;
		int yPos = screenH - image.height() * 1.1;
		SetPosition(xPos, yPos);
	}

	if (json.contains("fontSize"))
		font.setPointSize(json["fontSize"].toInt());

	if (json.contains("text_speed"))
		textSpeed = json["text_speed"].toInt();

	if (json.contains("fontColor"))
	{
		QJsonArray c = json["fontColor"].toArray();
		int r = c[0].toInt();
		int g = c[1].toInt();
		int b = c[2].toInt();
		int a = c[3].toInt();
		pen = QPen (QColor (r, g, b, a));
	}
}