#include "interaction.h"

interaction::interaction(void)
{
	selectx=0;selecty=0;
	blockcolor = new GLfloat[4];
	blockcolor[0]=0.0f;
	blockcolor[1]=0.0f;
	blockcolor[2]=0.0f;
	blockcolor[3]=1.0f;
	colorup=false;
	blockvalid=true;
}

interaction::~interaction(void)
{
}

void interaction::draw_selection()
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,blockcolor);
	glBegin(GL_QUADS);
		glVertex3f(selectx*gridsize,selecty*gridsize,0.0f);
		glVertex3f(selectx*gridsize,(selecty+1)*gridsize,0.0f);
		glVertex3f((selectx+1)*gridsize,(selecty+1)*gridsize,0.0f);
		glVertex3f((selectx+1)*gridsize,selecty*gridsize,0.0f);
	glEnd();
}
bool interaction::check_xy(int x, int y)
{
	if(x >=BASE/2 || y>=BASE/2 || x<=-BASE/2-1 || y<=-BASE/2-1)
		return true;
	else
		return false;
}
void interaction::change_selection(int x, int y)
{
	if(check_xy(x,y))
		return;
	selectx=x;selecty=y;
}
void interaction::animation_loop()
{
	int bit=0;
	if(!blockvalid)
		bit=2;
	if(colorup)
	{
		blockcolor[bit]+=0.1f;
	}
	else
	{
		blockcolor[bit]-=0.1f;
	}
	if(colorup && blockcolor[bit] > 1.0f)
	{
		colorup=false;
		blockcolor[bit]=1.0f;
		return;
	}
	if(!colorup && blockcolor[bit] < 0.0f)
	{	
		colorup=true;
		blockcolor[bit]=0.0f;
		return;
	}
}
void interaction::set_valid(bool t)
{
	if(blockvalid!=t)
	{
		blockcolor[0]=0.0f;
		blockcolor[1]=0.0f;
		blockcolor[2]=0.0f;
		blockcolor[3]=1.0f;
	}
	blockvalid=t;
}
