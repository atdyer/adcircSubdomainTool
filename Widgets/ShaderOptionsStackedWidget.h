#ifndef SHADEROPTIONSSTACKEDWIDGET_H
#define SHADEROPTIONSSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QColorDialog>

namespace Ui {
	class ShaderOptionsStackedWidget;
}

class ShaderOptionsStackedWidget : public QStackedWidget
{
		Q_OBJECT
		
	public:
		explicit ShaderOptionsStackedWidget(QWidget *parent = 0);
		~ShaderOptionsStackedWidget();
		
	private:
		Ui::ShaderOptionsStackedWidget *ui;
};

#endif // SHADEROPTIONSSTACKEDWIDGET_H
