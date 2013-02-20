#pragma once
#include <stdio.h>
#include <vector>
using namespace std;

#include <QVector3D>
#include <QImage>
#include <QMatrix4x4>
#include "glut.h"
#include "interaction.h"

#define PI 3.1415926535
#define NSIDES 30
#define RADIUS 30
#define RISE_H 50.0
#define Z_ADD 15.0
#define ZOOM_STEP 1

enum TYPE 
{
	start, //light blue
	straight, //red
	turn,//red
	up,//red
	down,//red
	end, //dark blue
	loop, //red
	tree, //green
	people //skin color
};
enum DIRECTION
{
	north,
	south,
	east,
	west
};

struct block
{
	int x,y,h;
	TYPE type;
	int rotation;
	GLfloat color[4];
	bool dir[4];
	int uptodown,lefttoright;
};
struct point 
{
	double x,y,z;
};
class model
{
	bool taken[BASE][BASE];
	block list[BASE][BASE];
	GLfloat type_color[9][4];
	bool dirmat[BASE][BASE][4];
	bool first_added;
	int beginx,beginy,endx,endy;
	int turnsteps,edgesteps;
	float updownlength,updownangle;
	GLUquadric* qobj;

	void load_color_model();
	bool spot_taken(int x,int y);
	void modify_dirmat();
	void add_waypoints();
	void fix_waypoints(int);
	void draw(block);
	void drawstraight(int x,int y);
	void drawturn();
	void drawup();
	void draw_treeorperson(TYPE t);
	void drawcylinder(GLfloat length);
	void draw_tree(block b);
public:

	model(void);
	~model(void);
	void model_draw();
	void add_model();
	bool preview_animation(TYPE,int ,int,int );
	bool check_preview();
	void find_updownparams();
	void load_textures();
	void draw_skybox();
	void tree_model_draw();

	int blocksize;
	TYPE type_selected;
	block preview;
	vector<point>waypoints;
	GLuint texture_id[5];
	point Camerapos;
	bool D2,D3;
	bool drawball,fpv;

	friend class GLWidget;
	
};
