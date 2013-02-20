#ifndef FINALPROJECT_H
#define FINALPROJECT_H

#include <QtGui/QMainWindow>
#include "ui_finalproject.h"

class FinalProject : public QMainWindow
{
	Q_OBJECT

public:
	FinalProject(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FinalProject();
	Ui::FinalProjectClass ui;
};

#endif // FINALPROJECT_H
