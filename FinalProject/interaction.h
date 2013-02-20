#pragma once

#include "glut.h"
#include <cmath>
#define BASE 10
#define TIMER_INTERVAL 33
#define ORTHO 2000
class interaction
{
private:
	int selectx,selecty;
	bool colorup;
	GLfloat *blockcolor;
	bool blockvalid;

public:
	int gridsize;

	interaction(void);
	~interaction(void);
	void change_selection(int x,int y);
	void draw_selection();
	void animation_loop();
	bool check_xy(int x,int y);
	void set_valid(bool);
	bool get_valid(){ return blockvalid;}

	friend class GLWidget;

};
