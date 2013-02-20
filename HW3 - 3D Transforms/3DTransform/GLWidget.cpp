#include <cmath>
#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) :
QGLWidget(parent)
{
	parentwidget=parent;
}

void GLWidget::initializeGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -100.0, 100.0);
	gluLookAt(2,2,2,0,0,0,0,0,1);
	translate[0]=0;translate[1]=0;translate[2]=0;
	rotate[0]=0;rotate[1]=0;rotate[2]=0;
	scale[0]=1;scale[1]=1;scale[2]=1;
	glEnable(GL_DEPTH_TEST);
	addlightflag=false;
	lpos[0]=4.0f;
	lpos[1]=4.0f;
	lpos[2]=40.0f;
	rpos=0.0f;
	directed=1.0;
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(addlightflag)
		drawLights();
	drawAxes();
	drawObjects();
	glFlush();
}
void GLWidget::drawTeapot(object obj,bool selected)
{
	GLfloat matcolor[]={obj.r,obj.g,obj.b,1.0f};
	glMatrixMode(GL_MODELVIEW);
	if(selected)
	{
		matcolor[0]=1.0f;
		matcolor[1]=1.0f;
		matcolor[2]=1.0f;
		glColor3f(1.0,1.0,1.0);
	}
	else
		glColor3f(obj.r,obj.g,obj.b);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor);
	glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(obj.matrix);
		glutSolidTeapot(obj.innerr);
	glPopMatrix();
}

void GLWidget::drawTorus(object obj,bool selected)
{
	GLfloat matcolor[]={obj.r,obj.g,obj.b,1.0f};
	glMatrixMode(GL_MODELVIEW);
	if(selected)
	{
		matcolor[0]=1.0f;
		matcolor[1]=1.0f;
		matcolor[2]=1.0f;
		glColor3f(1.0,1.0,1.0);
	}
	else
		glColor3f(obj.r,obj.g,obj.b);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor);
	glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(obj.matrix);
		glutSolidTorus(obj.innerr,obj.outerr,obj.nsides,obj.nrings);
	glPopMatrix();
	drawScreenAxis(obj.matrix);
}
void GLWidget::drawAxes()
{
	GLfloat matcolor[]={1.0f,0.0f,0.0f,1.0f};
	glColor3f(1.0,0.0,0.0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(5.0,0.0,0.0);
	glEnd();
	glColor3f(0.0,1.0,0.0);
	GLfloat matcolor2[]={0.0f,1.0f,0.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor2);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,5.0,0.0);
	glEnd();
	glColor3f(0.0,0.0,1.0);
	GLfloat matcolor3[]={0.0f,0.0f,1.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor3);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,5.0);
	glEnd();
}
void GLWidget::drawObjects()
{
	for(unsigned int i=0;i<list.size();i++)
	{
		switch(list[i].type)
		{
		case(TEAPOT):
			drawTeapot(list[i],listselect==i);
			break;
		case(TORUS):
			drawTorus(list[i],listselect==i);
			break;
		}
	}
}
void GLWidget::addTorus()
{
	object o;
	o.innerr=5;
	o.outerr=10;
	o.nrings=10;
	o.nsides=20;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv (GL_MODELVIEW_MATRIX, o.matrix);
		o.type=TORUS;
	glPopMatrix();
	o.r=GLfloat(rand()%256) / 255.0f;
	o.g=GLfloat(rand()%256) / 255.0f;
	o.b=GLfloat(rand()%256) / 255.0f;
	list.push_back(o);
	listselect=list.size()-1;
	updateGL();
}

void GLWidget::addTeapot()
{
	object o;
	o.innerr=5;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv (GL_MODELVIEW_MATRIX, o.matrix);
		o.type=TEAPOT;
	glPopMatrix();
	o.r=GLfloat(rand()%256) / 255.0f;
	o.g=GLfloat(rand()%256) / 255.0f;
	o.b=GLfloat(rand()%256) / 255.0f;
	list.push_back(o);
	listselect=list.size()-1;
	updateGL();
}

void GLWidget::doTransform()
{
	if(list.size()== 0 || listselect<0)
		return;
	glMatrixMode(GL_MODELVIEW);
	object *obj=&list[listselect];
	GLfloat tlx=obj->matrix[12];
	GLfloat tly=obj->matrix[13];
	GLfloat tlz=obj->matrix[14];
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(tlx,tly,tlz);
		glTranslatef(translate[0],translate[1],translate[2]);
		glRotatef(rotate[2],0,0,1);
		glRotatef(rotate[1],0,1,0);
		glRotatef(rotate[0],1,0,0);
		glScalef(scale[0],scale[1],scale[2]);
		glTranslatef(-tlx,-tly,-tlz);
		glMultMatrixf(obj->matrix);
		glGetFloatv (GL_MODELVIEW_MATRIX, obj->matrix);
	glPopMatrix();
	updateGL();
}

void GLWidget::doTransformLocal()
{
	if(list.size()== 0 || listselect<0)
		return;
	glMatrixMode(GL_MODELVIEW);
	object *obj=&list[listselect];
	GLfloat tlx=obj->matrix[12];
	GLfloat tly=obj->matrix[13];
	GLfloat tlz=obj->matrix[14];
	GLfloat x_axis[3],y_axis[3],z_axis[3],origin[3];
	getScreenAxis(obj->matrix,x_axis,y_axis,z_axis,origin);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(tlx,tly,tlz);
		glRotatef(45,0,0,1);
		glRotatef(fmod((float)rotatelocal[2],(float)360.0),0,0,1);
		glRotatef(fmod((float)rotatelocal[1],(float)360.0),0,1,0);
		glRotatef(-45,0,0,1);
		glTranslatef(-tlx,-tly,-tlz);
		glMultMatrixf(obj->matrix);
		glGetFloatv (GL_MODELVIEW_MATRIX, obj->matrix);
	glPopMatrix();
	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		QImage img=this->grabFrameBuffer();
		QColor colorval = QColor(img.pixel(event->x(),event->y()));
		int red=colorval.red();
		int green=colorval.green();
		int blue=colorval.blue();
		listselect=-1;
		for(int i=0;i<list.size();i++)
		{
			float r=float(red)/255.0;
			float g=float(green)/255.0;
			float b=float(blue)/255.0;
			if(list[i].r==r && list[i].g==g && list[i].b==b)
			{
				listselect=i;
				break;
			}
		}
	}
	if(event->button()== Qt::RightButton && listselect>=0)
	{
		mousex=event->x();
		mousey=event->y();
	}
	updateGL();
	
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	rotatelocal[0]=0;rotatelocal[1]=0;rotatelocal[2]=0;
	if((event->buttons() & Qt::RightButton) && listselect>=0)
	{
		rotatelocal[ZVAL]=(event->x() - mousex);
		mousex=event->x();
		//rotatelocal[XVAL]=(event->y() - mousey)/5*45;
		rotatelocal[YVAL]=(event->y() - mousey);
		mousey=event->y();
	}
	doTransformLocal();
}

void GLWidget::getTLX(QString s)
{
	translate[XVAL] = s.toFloat();
}
void GLWidget::getTLY(QString s)
{
	translate[YVAL] = s.toFloat();
}
void GLWidget::getTLZ(QString s)
{
	translate[ZVAL] = s.toFloat();
}
void GLWidget::getRotX(QString s)
{
	rotate[XVAL] = s.toFloat();
}
void GLWidget::getRotY(QString s)
{
	rotate[YVAL] = s.toFloat();
}
void GLWidget::getRotZ(QString s)
{
	rotate[ZVAL] = s.toFloat();
}
void GLWidget::getScaleX(QString s)
{
	scale[XVAL] = (s.toFloat()==0)?1:s.toFloat();
}
void GLWidget::getScaleY(QString s)
{
	scale[YVAL] = (s.toFloat()==0)?1:s.toFloat();
}
void GLWidget::getScaleZ(QString s)
{
	scale[ZVAL] = (s.toFloat()==0)?1:s.toFloat();
}
void GLWidget::clear()
{
	list.clear();
	listselect=-1;
	updateGL();
}
void GLWidget::delete_object()
{
	if(listselect<0)
		return;
	list.erase(list.begin()+listselect);
	listselect=-1;
	updateGL();
}
void GLWidget::shiftplus()
{
	if(!list.size())
		return;
	listselect=(listselect+1) % list.size();
	updateGL();
}
void GLWidget::shiftminus()
{
	if(!list.size())
		return;
	listselect=(listselect<=0?list.size()-1:listselect-1);
	updateGL();
}
void GLWidget::addLight()
{
	if(addlightflag)
		return;
	addlightflag=!addlightflag;
	updateGL();
}
void GLWidget::drawLights()
{
	GLfloat lightcolor[]={1.0f,1.0f,1.0f,0.0f};
	lpos[2]=40.0f*cos(rpos);
	GLfloat pos[]={lpos[0],lpos[1],lpos[2],directed};
	glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT0,GL_POSITION,pos); //setup position
	glEnable(GL_LIGHT0);//setup the light
	glEnable(GL_LIGHTING);//final render
	
	//draw the "light source"
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,lightcolor);
	glPushMatrix();
		glTranslatef(pos[0],pos[1],pos[2]);
		glutSolidSphere(1.0,10,10);
	glPopMatrix();
	lightcolor[0]=0.0f;
	lightcolor[1]=0.0f;
	lightcolor[2]=0.0f;
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,lightcolor);
}

void GLWidget::getScreenAxis(GLfloat matrix[],GLfloat x_axis[],GLfloat y_axis[],GLfloat z_axis[],GLfloat origin[])
{
	for(int i=0;i<3;i++)
	{
		x_axis[i]=matrix[i+4*0];
		y_axis[i]=matrix[i+4*1];
		z_axis[i]=matrix[i+4*2];
		origin[i]=matrix[i+4*3];
	}
}
void GLWidget::drawScreenAxis(GLfloat matrix[])
{
	GLfloat x_axis[3],y_axis[3],z_axis[3],origin[3];
	getScreenAxis(matrix,x_axis,y_axis,z_axis,origin);
	//GLfloat lightcolor[]={0.0f,1.0f,1.0f,0.0f};
	//glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,lightcolor);
	glBegin(GL_LINES);
		glVertex3f(origin[0],origin[1],origin[2]);
		glVertex3f(origin[0]+x_axis[0],origin[1]+x_axis[1],origin[2]+x_axis[2]);
		glVertex3f(origin[0],origin[1],origin[2]);
		glVertex3f(origin[0]+y_axis[0],origin[1]+y_axis[1],origin[2]+y_axis[2]);
		glVertex3f(origin[0],origin[1],origin[2]);
		glVertex3f(origin[0]+z_axis[0],origin[1]+z_axis[1],origin[2]+z_axis[2]);
	glEnd();
}
void GLWidget::rotplus()
{
	rpos+=PI / 20;
	//if(rpos > 2*PI)
		//rpos-=2*PI;
	updateGL();
}
void GLWidget::rotminus()
{
	rpos-=PI / 20;
	//if(rpos < -2*PI)
		//rpos+=2*PI;
	updateGL();
}
void GLWidget::lightype(int i)
{
	directed=(i>0)?0.0f:1.0f;
	updateGL();
}