#ifndef BRICKS_CPP
#define BRICKS_CPP

#include <vector>
#include <iostream>
#include <string>
using namespace std;
#include "vector.h"
#include "glut.h"


extern GLint window_width,window_height,game_width,lives;
extern GLfloat gap;
struct rect
{
	GLfloat x,y;
	GLfloat width,height;
	bool visible;
	GLfloat r,g,b;
};

class Brick
{
	vector<rect> list;
	vector<vec3_t> color_list;
	vector<vec3_t> brokenlist;
	GLint score;
	GLint printnum;
	int brick_width;
	int brick_height;
	int broken_side;

	void make_color_list();
	void display_broken();
public:
	Brick();
	~Brick();
	void display();
	vector<rect> getlist(){return list;}
	void set_invisible(int i);
	void printall(string);
	GLint showscore(){return score;}
	void setscore(GLint s){score=s;}
	void update();
};

#endif