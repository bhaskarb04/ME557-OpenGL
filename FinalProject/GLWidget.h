#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QQuaternion>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCursor>
#include <QPointF>
#include <QTimer>
#include "glut.h"
#include <vector>
#include <fstream>
using namespace std;

#include "interaction.h"
#include "model.h"

class GLWidget : public QGLWidget,public interaction,public model
{
Q_OBJECT
public:
explicit GLWidget(QWidget *parent = 0);

protected:
	//QWidget *parentpointer;
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	virtual void  keyPressEvent(QKeyEvent *event);
	void swapBuffers();
	double mousex,mousey,fpvmousex,fpvmousey;
	int blockx,blocky;
	GLfloat mvmatrix[16];
	int rotation;
	QTimer *timer;
	int animationcount;
	float moveangle,movezangle,fpvangle,orig_zangle;
	QQuaternion finalpos;
	float keyx,keyz;
	float zoomval;
	bool light_source[5];

private:
	void drawAxes();
	void drawLights();
	void drawBase();
	void drawTransparentBase();
	void drawbasequad();
	void draw_ball(float x, float y, float z);
	void draw_light_source();
	void getScreenAxis(GLfloat matrix[],GLfloat x_axis[],GLfloat y_axis[],GLfloat z_axis[],GLfloat origin[]);
	void scr2world(double& x,double& y,double& z);
	void printonfly(double x,double y, double z);
	void setCamera();
signals:
	void setlabel(QString);
public slots:
	void animate();
	void do_animation();
	void do_ball_animation();
	void make2D();
	void make3D();
	void add_model_();
	void straight_select();
	void up_select();
	void down_select();
	void loop_select();
	void turn_select();
	void tree_select();
	void people_select();
	void start_ride();
	void stop_ride();
	void start_ball();
	void save_coaster();
	void load_coaster();
	void directedlight(bool);
	void tleftlight(bool);
	void trightlight(bool);
	void bleftlight(bool);
	void brightlight(bool);
};

#endif // GLWIDGET_H

