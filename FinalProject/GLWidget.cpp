#include <cmath>
#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) :
QGLWidget(parent)
{
	//parentpointer=parent;
}

void GLWidget::initializeGL()
{
	gridsize=ORTHO/(BASE*2);
	blocksize=gridsize;
	find_updownparams();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glOrtho(-ORTHO/2, ORTHO/2, -ORTHO/2,ORTHO/2, 1.0, ORTHO);
	Camerapos.x=0;
	Camerapos.y=0;
	Camerapos.z=1*gridsize;
	gluLookAt(0,0,1*gridsize,0,0,0,0,1,0);
	glEnable(GL_NORMALIZE); 
	glGetFloatv (GL_MODELVIEW_MATRIX, mvmatrix);
	D2=true;
	D3=false;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer->start(TIMER_INTERVAL);
	blockx=0;blocky=0;
	type_selected=straight;
	rotation=0;
	drawball=false;
	moveangle=0;
	fpv=false;
	keyx=keyz=0;
	load_textures();
	zoomval=0;

	GLfloat amblight[]={0.5,0.5,0.5,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amblight);
	GLfloat lightcolor[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat pos0[]={0,0,ORTHO/2,0.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT0,GL_POSITION,pos0); //setup position
	glEnable(GL_LIGHT0);//setup the light
	
	GLfloat pos3[]={-(BASE/2)*blocksize,-(BASE/2)*blocksize,(BASE/2)*blocksize,1.0};
	glLightfv(GL_LIGHT3,GL_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT3,GL_POSITION,pos3); //setup position

	GLfloat pos4[]={(BASE/2)*blocksize,-(BASE/2)*blocksize,(BASE/2)*blocksize,1.0};
	glLightfv(GL_LIGHT4,GL_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT4,GL_POSITION,pos4); //setup position

	GLfloat pos1[]={(BASE/2)*blocksize,(BASE/2)*blocksize,(BASE/2)*blocksize,1.0};
	glLightfv(GL_LIGHT1,GL_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT1,GL_POSITION,pos1); //setup position

	GLfloat pos2[]={-(BASE/2)*blocksize,(BASE/2)*blocksize,(BASE/2)*blocksize,1.0};
	glLightfv(GL_LIGHT2,GL_DIFFUSE,lightcolor); //setup type
	glLightfv(GL_LIGHT2,GL_POSITION,pos2); //setup position

	glEnable(GL_LIGHTING);

	light_source[0]=true;light_source[1]=false;light_source[2]=false;
	light_source[3]=false;light_source[4]=false;

	QVector3D q(-11*gridsize*sin((double)moveangle)*((100-zoomval)/100),
				-11*gridsize*cos((double)moveangle)*((100-zoomval)/100),
				11*gridsize*((100-zoomval)/100)),
				z(0,0,1);
	orig_zangle=movezangle=acos(QVector3D::dotProduct(q,z)/(q.length()*z.length()))*180/PI;
	//glEnable(GL_NORMALIZE);
}
void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawLights();
	model_draw();
	if(drawball)
		draw_ball(waypoints[animationcount].x,waypoints[animationcount].y,waypoints[animationcount].z+2*RADIUS);
	if(!fpv)
	{
		drawAxes();
		drawBase();
	}
	else
	{
		draw_skybox();
		drawTransparentBase();
	}
	tree_model_draw();
}
void GLWidget::drawTransparentBase()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	GLfloat color[]={1.0f,1.0f,1.0f,0.2f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f((BASE/2)*blocksize,(BASE/2)*blocksize,0.0f);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(-(BASE/2)*blocksize,(BASE/2)*blocksize,0.0f);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(-(BASE/2)*blocksize,-(BASE/2)*blocksize,0.0f);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f((BASE/2)*blocksize,-(BASE/2)*blocksize,0.0f);
	glEnd();
	glDisable(GL_BLEND);
}
void GLWidget::drawAxes()
{
	GLfloat matcolor[]={1.0f,0.0f,0.0f,1.0f};
	glColor3f(1.0,0.0,0.0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(5.0*gridsize,0.0,0.0);
	glEnd();
	glColor3f(0.0,1.0,0.0);
	GLfloat matcolor2[]={0.0f,1.0f,0.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor2);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,5.0*gridsize,0.0);
	glEnd();
	glColor3f(0.0,0.0,1.0);
	GLfloat matcolor3[]={0.0f,0.0f,1.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matcolor3);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,5.0*gridsize);
	glEnd();
}

void GLWidget::drawLights()
{
	GLfloat lightcolor[]={1.0f,1.0f,1.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,lightcolor);
	glMatrixMode(GL_MODELVIEW);
	for(int i=1;i<5;i++)
	{
		if(!light_source[i])
			continue;
		GLfloat pos[4];
		switch(i)
		{
		case 1:
			pos[0]=(BASE/2)*blocksize;pos[1]=(BASE/2)*blocksize;pos[2]=(BASE/2)*blocksize;
			break;
		case 2:
			pos[0]=-(BASE/2)*blocksize;pos[1]=(BASE/2)*blocksize;pos[2]=(BASE/2)*blocksize;
			break;
		case 3:
			pos[0]=-(BASE/2)*blocksize;pos[1]=-(BASE/2)*blocksize;pos[2]=(BASE/2)*blocksize;
			break;
		case 4:
			pos[0]=(BASE/2)*blocksize;pos[1]=-(BASE/2)*blocksize;pos[2]=(BASE/2)*blocksize;
			break;
		}
		//draw the "light source"
		glPushMatrix();
			glLoadIdentity();
			glTranslatef(pos[0],pos[1],pos[2]);
			glutSolidSphere(10.0,10,10);
		glPopMatrix();
	}
	lightcolor[0]=0.0f;
	lightcolor[1]=0.0f;
	lightcolor[2]=0.0f;
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,lightcolor);
}
void GLWidget::drawBase()
{
	GLfloat basecolor[]={1.0f,1.0f,0.0f,1.0f};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,basecolor);
	for(unsigned int i=0;i<BASE+1;i++)
	{
		for(unsigned int j=0;j<BASE+1;j++)
		{
			glPushMatrix();
			glTranslatef((GLfloat(i)-BASE/2)*gridsize,(GLfloat(j)-BASE/2)*gridsize,0.0f);
			if(i<BASE)
			{
				glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(1.0f*gridsize,0.0f,0.0f);	
				glEnd();
			}
			if(j<BASE)
			{
				glBegin(GL_LINES);
				glVertex3f(0.0f,0.0f,0.0f);
				glVertex3f(0.0f,1.0f*gridsize,0.0f);	
				glEnd();
			}
			glPopMatrix();
		}
	}
	draw_selection();
}
void GLWidget::scr2world(double& x,double& y,double& z)
{
	double xx,yy,zz;
	GLdouble modelmatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,modelmatrix);
	GLdouble projmatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	int viewmatrix[4];
	glGetIntegerv(GL_VIEWPORT,viewmatrix);
	gluUnProject(x,y,0.0,modelmatrix,projmatrix,viewmatrix,&xx,&yy,&zz);
	x=xx/gridsize;y=-yy/gridsize;z=zz/gridsize;
}
void GLWidget::printonfly(double x, double y, double z)
{
	QString str;
	str.append(QString("%1 ").arg(x));
	str.append(QString("%1 ").arg(-y));
	str.append(QString("%1").arg(z));
	emit setlabel(str);
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(D3)
		return;
	if(!first_added && (preview.type==tree || preview.type==people))
		return;
	double modelmatrix[16],projmatrix[16],x,y,z;
	int viewmatrix[4];
	if(event->button()==Qt::LeftButton)
	{
		double x=event->x(),y=event->y(),z=0.0;
		mousex =x;
		mousey =y;
		scr2world(x,y,z);
		if(check_xy(floor(x),floor(y)) || !get_valid())
			return;
		add_model();
	}
	updateGL();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(D2)
	{
		
	}
	if(D3 && fpv==false)
	{
		if(event->buttons() & Qt::LeftButton)
		{
			double xmov=mousex - event->x();
			double ymov=mousey - event->y();
			mousex=event->x();
			mousey=event->y();
			moveangle+=xmov>1?1:0;
			moveangle+=xmov<-1?-1:0;
			movezangle+=ymov>1?1:0;
			movezangle+=ymov<-1?-1:0;
			
			//moveangle=moveangle%360;
			/*Camerapos.x=;
			Camerapos.y=-11*gridsize*cos((double)moveangle)*((100-zoomval)/100);
			Camerapos.z=11*gridsize*((100-zoomval)/100);
			cross=QVector3D::normal(q,z);
			cross.setX(abs(cross.x()));cross.setY(abs(cross.y()));cross.setZ(abs(cross.z()));*/
			setCamera();
			updateGL();
		}
	}
	//if(fpv==true)
	//{
	//	if(event->buttons() & Qt::LeftButton)
	//	{
	//		double xmov=fpvmousex - event->x();
	//		double ymov=fpvmousey - event->y();
	//		fpvmousex=event->x();
	//		fpvmousey=event->y();
	//		fpvangle+=xmov>0?5:-5;
	//		//QQuaternion q=QQuaternion::fromAxisAndAngle( qreal x, qreal y, qreal z, qreal angle );
	//		updateGL();
	//	}
	//}
}
void GLWidget::wheelEvent(QWheelEvent *event)
{
	if(D2)
	{
		if(event->delta()>0)
			rotation=(rotation+90)%360;
		else
			rotation=(rotation-90)%360;
		printonfly(rotation,0.0f,0.0f);
	}
	if(D3)
	{
		if(event->delta()>0)
			zoomval+=ZOOM_STEP;
		else
			zoomval-=ZOOM_STEP;
		/*QVector3D q(-11*gridsize*sin((double)moveangle),-11*gridsize*cos((double)moveangle),11*gridsize);
		Camerapos.x=q.x()*((100-zoomval)/100);
		Camerapos.y=q.y()*((100-zoomval)/100);
		Camerapos.z=q.z()*((100-zoomval)/100);*/
		setCamera();
		updateGL();
	}
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
void GLWidget::make2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ORTHO/2, ORTHO/2, -ORTHO/2,ORTHO/2, 1.0, ORTHO);
	Camerapos.x=0;
	Camerapos.y=0;
	Camerapos.z=1*gridsize;
	gluLookAt(0,0,1*gridsize,0,0,0,0,1,0);
	D2=true;
	D3=false;
	drawball=false;
	fpv=false;
	updateGL();
}
void GLWidget::make3D()
{
	moveangle=0;
	zoomval=0;
	movezangle=orig_zangle;
	setCamera();
	D2=false;
	D3=true;
	fpv=false;
	updateGL();
}
void GLWidget::setCamera()
{
	QVector3D negy(0,-1,0);
	QMatrix4x4 m;
	m.rotate(moveangle,0,0,1);
	m.rotate(-movezangle,1,0,0);
	negy=negy*m;
	Camerapos.x=-11*gridsize*((100-zoomval)/100)*negy.x();
	Camerapos.y=11*gridsize*((100-zoomval)/100)*negy.y();
	Camerapos.z=-11*gridsize*((100-zoomval)/100)*negy.z();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1.0,1.0,2*ORTHO);
	gluLookAt(Camerapos.x,Camerapos.y,Camerapos.z,0,0,0,0,0,1);
}
void GLWidget::swapBuffers() 
{
  QGLWidget::swapBuffers();
}
void GLWidget::animate()
{
	if(D3)
		return;
	double x=this->mapFromGlobal(this->cursor().pos()).x(),y=this->mapFromGlobal(this->cursor().pos()).y(),z=0.0;
	scr2world(x,y,z);
	if(check_xy(floor(x),floor(y)))
			return;
	blockx=floor(x);
	blocky=floor(y);
	//printonfly(blockx,blocky,0.0f);
	change_selection(blockx,blocky);
	set_valid(preview_animation(type_selected,blockx,blocky,rotation));
	animation_loop();
	updateGL();
}
void GLWidget::add_model_()
{
	updateGL();
}
void GLWidget::straight_select()
{
	type_selected=straight;
	rotation=0;
}
void GLWidget::up_select()
{
	type_selected=up;
	rotation=0;
}
void GLWidget::down_select()
{
	type_selected=down;
	rotation=0;
}
void GLWidget::loop_select()
{
	type_selected=loop;
	rotation=0;
}
void GLWidget::turn_select()
{
	type_selected=turn;
	rotation=0;
}
void GLWidget::tree_select()
{
	type_selected=tree;
}
void GLWidget::people_select()
{
	type_selected=people;
}
void GLWidget::start_ride()
{
	if(!first_added)
		return;
	D2=false;
	D3=true;
	animationcount=0;
	setFocus();
	timer->stop();
	disconnect(timer,0,0,0);
	connect(timer, SIGNAL(timeout()), this, SLOT(do_animation()));
	timer->start(TIMER_INTERVAL);
}
void GLWidget::stop_ride()
{
	if(!first_added)
		return;
	make2D();
	timer->stop();
	disconnect(timer,0,0,0);
	connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer->start(TIMER_INTERVAL);
}
void GLWidget::start_ball()
{
	if(!first_added)
		return;
	animationcount=0;
	make3D();
	timer->stop();
	disconnect(timer,0,0,0);
	connect(timer, SIGNAL(timeout()), this, SLOT(do_ball_animation()));
	timer->start(TIMER_INTERVAL);
}
void GLWidget::do_animation()
{
	drawball=false;
	fpv=true;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*glOrtho(-ORTHO/2, ORTHO/2, -ORTHO/2,ORTHO/2, 1.0, ORTHO);*/
	gluPerspective(75,1.0,1.0,ORTHO*2);
	int i=animationcount;
	Camerapos.x=waypoints[i].x;
	Camerapos.y=waypoints[i].y;
	Camerapos.z=waypoints[i].z+RADIUS+Z_ADD;
	QVector3D q1(waypoints[i].x,waypoints[i].y,waypoints[i].z);
	QVector3D q2(waypoints[i+1].x,waypoints[i+1].y,waypoints[i+1].z);
	QVector3D look=q2-q1;
	look.normalize();
	QVector3D up(0,0,1);
	QVector3D right = QVector3D::crossProduct(up,look);
	right.normalize();
	QVector3D finalup = QVector3D::crossProduct(look,right);
	finalup.normalize();
	QMatrix4x4 transform;
	q2-=q1;
	transform.rotate(keyx,right);
	transform.rotate(keyz,finalup);
	q2=q2*transform;
	q2+=q1;
	gluLookAt(waypoints[i].x,waypoints[i].y,waypoints[i].z+RADIUS+Z_ADD,
			q2.x(),q2.y(),q2.z()+RADIUS+Z_ADD,
			0,0,1);
	animationcount++;
	animationcount=animationcount % (waypoints.size()-1);
	updateGL();
}
void GLWidget::do_ball_animation()
{
	drawball=true;
	fpv=false;
	animationcount++;
	animationcount=animationcount % (waypoints.size()-1);
	updateGL();
}
void GLWidget::draw_ball(float x, float y, float z)
{
	GLfloat ballcolor[]={1.0f,1.0f,1.0f,1.0f};
	glMatrixMode(GL_MODELVIEW);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,ballcolor);
	glEnable( GL_TEXTURE_2D );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture( GL_TEXTURE_2D, texture_id[3]);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x,y,z);
	glutSolidSphere(20.0,30,30);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void GLWidget::save_coaster()
{
	fstream fileptr;
	fileptr.open("Coaster.dat",fstream::out | fstream::binary);
	fileptr.write((char*)taken,sizeof(bool)*BASE*BASE);
	fileptr.write((char*)list,sizeof(block)*BASE*BASE);
	fileptr.write((char*)dirmat,sizeof(bool)*BASE*BASE*4);
	fileptr<<waypoints.size();
	for(int i=0;i<waypoints.size();i++)
	{
		fileptr.write((char*)&waypoints[i],sizeof(point));
	}
}

void GLWidget::load_coaster()
{
	first_added=true;
	fstream fileptr;
	fileptr.open("Coaster.dat",fstream::in | fstream::binary);
	fileptr.read((char*)taken,sizeof(bool)*BASE*BASE);
	fileptr.read((char*)list,sizeof(block)*BASE*BASE);
	fileptr.read((char*)dirmat,sizeof(bool)*BASE*BASE*4);
	int ws;
	fileptr>>ws;
	waypoints.resize(ws);
	for(int i=0;i<waypoints.size();i++)
	{
		fileptr.read((char*)&waypoints[i],sizeof(point));
	}
	updateGL();
}
void GLWidget::keyPressEvent(QKeyEvent *event)
{
	if(D3)
	{
		
		switch(event->key())
		{
		case(Qt::Key_Up):
				keyx+=5;
				break;
		case(Qt::Key_Down):
				keyx-=5;
				break;
		case(Qt::Key_Right):
				keyz+=5;
				break;
		case(Qt::Key_Left):
				keyz-=5;
				break;
		}
	}
}
void GLWidget::bleftlight(bool on)
{
	if(!on)
	{
		glDisable(GL_LIGHT3);
		light_source[3]=false;
	}
	else
	{
		glEnable(GL_LIGHT3);
		light_source[3]=true;
	}
	updateGL();
}
void GLWidget::tleftlight(bool on)
{
	glDisable(GL_LIGHTING);
	if(!on)
	{
		glDisable(GL_LIGHT2);
		light_source[2]=false;
	}
	else
	{
		glEnable(GL_LIGHT2);//setup the light
		light_source[2]=true;
	}
	glEnable(GL_LIGHTING);
	updateGL();
}
void GLWidget::trightlight(bool on)
{
	if(!on)
	{
		glDisable(GL_LIGHT1);
		light_source[1]=false;
	}
	else
	{
		glEnable(GL_LIGHT1);//setup the light
		light_source[1]=true;
	}
	updateGL();
}
void GLWidget::brightlight(bool on)
{
	if(!on)
	{
		glDisable(GL_LIGHT4);
		light_source[4]=false;
	}
	else
	{
		glEnable(GL_LIGHT4);//setup the light
		light_source[4]=true;
	}
	updateGL();
}
void GLWidget::directedlight(bool on)
{
	if(!on)
	{
		glDisable(GL_LIGHT0);
		light_source[0]=false;
	}
	else
	{
		glEnable(GL_LIGHT0);
		light_source[0]=true;
	}
	updateGL();
}
