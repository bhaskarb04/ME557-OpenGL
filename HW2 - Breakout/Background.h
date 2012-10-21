#ifndef BG_CPP
#define BG_CPP

#include <vector>
#include <iostream>
using namespace std;
#include "vector.h"
#include "glut.h"

extern GLfloat speed;

class Background
{
	vec3_t bl;
	vec3_t tl;
	vec3_t tr;
	vec3_t br;

	vec3_t paddle; //x,y and x which is added is z
	GLint paddlesize;
public:
	Background(GLfloat blx, GLfloat bly,GLfloat tlx, GLfloat tly,
		GLfloat trx, GLfloat tryy, GLfloat brx, GLfloat bry);
	~Background();
	void display();
	vector<vec3_t> Bgvecs();
	vec3_t paddle_pos(){return paddle;}
	void move_paddle(bool left);
};

#endif