#include "stdafx.h"
#include "Input.h"
#include "painter.h"
#include "DebugReport.h"
#include "Input.h"

using namespace LarasEngine;

painter::painter(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	srand (time(NULL));
}

painter::~painter()
{

}

void painter::paintEvent (QPaintEvent *e)
{
	QPainter p;
	p.begin (this);

	for (auto& a : layers)
		a.second.DrawLayer (&p);

	p.end ();
}

void painter::keyReleaseEvent (QKeyEvent *r)
{
	Input::Me ().SetKey (Input::Key (r->key ()), false);
}

void painter::Close()
{
	close();
}

void painter::keyPressEvent(QKeyEvent *event)
{
	Input::Me().SetKey ( Input::Key (event->key()), true);
}

void painter::Repaint()
{
	DebugReport r ("Repaint");
	repaint();
}

