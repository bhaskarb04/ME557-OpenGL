#ifndef BALL_CPP

#define BALL_CPP
#include "vector.h"
#include "glut.h"
#include <Windows.h>

#define EPS 0.0001
#define PI 3.1415926535

extern GLfloat speed;
extern GLfloat gap;

struct Color
{
	GLfloat r,g,b;
};

class Ball
{
	vec3_t ctr;
	vec3_t vel;
	Color color;
	GLfloat size;
	void reflect(vec3_t);
public:
	Ball(GLfloat);
	~Ball();
	void display();
	void update();
	GLboolean avoid(vec3_t p1,vec3_t p2);
	void setvelctr(vec3_t ctr,vec3_t vel);
	vec3_t showvel(){return vel;}
	vec3_t showctr(){return ctr;}
};


#endif BALL_CPP