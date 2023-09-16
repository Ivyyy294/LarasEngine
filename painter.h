#ifndef PAINTER_H
#define PAINTER_H

#include "Layer.h"
#include "ui_painter.h"

namespace LarasEngine
{
	class painter : public QDialog
	{
		Q_OBJECT

	public:
		painter(QWidget *parent = 0);
		~painter();

		typedef std::shared_ptr <painter> Ptr;
		inline Layer::Map& GetLayers () { return layers; };

	signals:
		void Draw();

	private:
		Ui::painterClass ui;
		bool gewonnen;
		Layer::Map layers;

	protected: 
		void paintEvent (QPaintEvent *e);
		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent (QKeyEvent *r);

	public slots:
		void Repaint();
		void Close();
	};
}


#endif // PAINTER_H
