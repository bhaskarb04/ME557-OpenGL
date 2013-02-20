#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QWidget>
#include "ui_mainwidget.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWidget();

private:
	Ui::MainWidgetClass ui;
};

#endif // MAINWIDGET_H
