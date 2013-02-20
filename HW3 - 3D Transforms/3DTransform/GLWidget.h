#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui/QMessageBox>
#include <QMouseEvent>
#include <QImage>
#include "glut.h"
#include <vector>
using namespace std;

#define TEAPOT 1
#define TORUS 2
#define XVAL 0
#define YVAL 1
#define ZVAL 2
#define PI 3.1415926535
struct object
{
	int type;
	GLfloat matrix[16];
	GLfloat r,g,b;
	int innerr,outerr,nsides,nrings;
};

class GLWidget : public QGLWidget
{
Q_OBJECT
public:
explicit GLWidget(QWidget *parent = 0);

protected:
	QWidget *parentwidget;
	vector<object> list;
	int orgx,orgy;
	int listselect;
	GLfloat rotate[3],translate[3],scale[3],rotatelocal[3];
	int mousex,mousey;
	GLfloat lpos[3],rpos,directed;
	bool addlightflag;

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:

	void drawAxes();
	void drawObjects();
	void drawTeapot(object,bool);
	void drawTorus(object,bool );
	void doTransformLocal();
	void drawLights();
	void drawScreenAxis(GLfloat*);
	void getScreenAxis(GLfloat matrix[],GLfloat x_axis[],GLfloat y_axis[],GLfloat z_axis[],GLfloat origin[]);

public slots:
	void addTorus();
	void addTeapot();
	void doTransform();
	void getTLX(QString s);
	void getTLY(QString s);
	void getTLZ(QString s);
	void getRotX(QString s);
	void getRotY(QString s);
	void getRotZ(QString s);
	void getScaleX(QString s);
	void getScaleY(QString s);
	void getScaleZ(QString s);
	void clear();
	void delete_object();
	void shiftplus();
	void shiftminus();
	void addLight();
	void rotplus();
	void rotminus();
	void lightype(int i);
};

#endif // GLWIDGET_H

