#include "Ball.h"

Ball::Ball(GLfloat ss)
{
	ctr.Set(40.0,40.0,0.0);
	vel.Set(-speed,speed,0.0);
	color.r=1.0;
	color.g=0.0;
	color.b=0.0;
	size=ss;
}

Ball::~Ball()
{

}

void Ball::display()
{
	glPointSize(2.0);
	glColor3f(color.r,color.g,color.b);
	GLfloat delta=2*PI/40;
	glBegin(GL_POLYGON);
	for(GLfloat theta=0;theta<2*PI; theta+=delta)
	{
		GLfloat x=size*cos(theta) + ctr.X();
		GLfloat y=size*sin(theta) + ctr.Y();
		glVertex2f(x,y);
		
	}
	glEnd();
}

void Ball::update()
{
	ctr=ctr+vel;
}

void Ball::reflect(vec3_t n)
{
	float val = 2*n.Dot(vel);
	vec3_t veltmp = n*val;
	vel = vel - veltmp;
}

//returning dotp for debugging purpose
GLboolean Ball::avoid(vec3_t p1,vec3_t p2)
{
	vec3_t p12=p1-p2;				//find the vec to the two points
	p12.Normalize();				//find its unit vector
	vec3_t n(-p12.Y(),p12.X(),0.0); //find the normal to the unit vec

	vec3_t pc=ctr - p1;             //find the vec between ball and line segment
	GLfloat dotp=pc.Dot(n);			//project vec to the normal 
	if(abs(dotp) > size+gap)
		return false;
	if(abs((ctr-p1).Length()) < abs((p1-p2).Length()) && abs((ctr-p2).Length()) < abs((p1-p2).Length()))
	{
		reflect(n);
		PlaySound("Bounce.wav", NULL, SND_FILENAME | SND_ASYNC);
		return true;
	}
	return false;
}

void Ball::setvelctr(vec3_t ctrnew,vec3_t velnew)
{
	ctr=ctrnew;
	vel=velnew;
}