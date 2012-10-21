#include "Bricks.h"

Brick::Brick()
{
	int colornum=0;
	score=0;
	make_color_list();
	brick_width=(window_width-4*gap)/10;
	brick_height=20;
	printnum=0;
	for(int x=2*gap;x<window_width-2*gap;x+=brick_width+gap)
	{
		for(int y=window_height-60;y>window_height-120;y-=(20+gap))
		{
			if(x+brick_width > game_width)
				continue;
			rect r;
			r.r=color_list[colornum%6].X();
			r.g=color_list[colornum%6].Y();
			r.b=color_list[colornum%6].Z();
			r.visible=true;
			r.x=x;
			r.y=y;
			r.height=brick_height;
			r.width=brick_width;
			list.push_back(r);
			colornum++;
		}
	}
}

Brick::~Brick()
{

}

void Brick::printall(string s)
{
	glRasterPos2i(game_width+10*gap,window_height-20-24*printnum);
	for(unsigned int i=0;i<s.length();i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
	printnum++;
}

void Brick::display()
{
	for(unsigned int i=0;i<list.size();i++)
	{
		if(!list[i].visible)
			continue;
		glColor3f(list[i].r,list[i].g,list[i].b);
		glBegin(GL_QUADS);
		glVertex2f(list[i].x,list[i].y);
		glVertex2f(list[i].x+list[i].width,list[i].y);
		glVertex2f(list[i].x+list[i].width,list[i].y+list[i].height);
		glVertex2f(list[i].x,list[i].y+list[i].height);
		glEnd();
	}
	glColor3f(1.0,1.0,1.0);
	string s="SCORE";
	printall(s);
	char num[10];
	sprintf(num,"%d",score);
	s=num;
	printall(s);
	s="LIVES";
	printall(s);
	sprintf(num,"%d",lives);
	s=num;
	printall(s);
	printnum=0;

	display_broken();
}

void Brick::make_color_list()
{
	color_list.push_back(vec3_t(1.0,0.0,0.0));//RED
	color_list.push_back(vec3_t(0.0,1.0,0.0));//GREEN
	color_list.push_back(vec3_t(0.0,0.0,1.0));//BLUE
	color_list.push_back(vec3_t(0.5,0.5,0.0));//RG
	color_list.push_back(vec3_t(0.0,0.5,0.5));//GB
	color_list.push_back(vec3_t(0.5,0.0,0.5));//RB
}

void Brick::set_invisible(int i)
{
	
	if(i <list.size())
	{
		list[i].visible=false;
		broken_side=15;//sqrt((double)brick_width*brick_height);
		int num_broken=ceilf((double)brick_width*brick_height/(double)(broken_side*broken_side));
		int xstart=list[i].x;
		int ystart=list[i].y;
		int x=0,y=0;
		int neg=-1;
		for(unsigned int j=0;j<num_broken;j++)
		{
			if(x>brick_width)
			{
				x=0;
				y+=broken_side+1;
			}
			brokenlist.push_back(vec3_t(xstart+x,ystart+y,0.0));
			brokenlist.push_back(vec3_t(neg*float(rand()%5+1),float(rand()%5+1),i));
			x+=broken_side;
			neg=-neg;
		}
	}
	score++;

}

void Brick::display_broken()
{
	for(unsigned int i=0;i<brokenlist.size();i+=2)
	{
		int side=broken_side;
		GLfloat rot=brokenlist[i].Z();
		glColor3f(list[brokenlist[i+1].Z()].r,list[brokenlist[i+1].Z()].g,list[brokenlist[i+1].Z()].b);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(brokenlist[i].X()+broken_side/2,brokenlist[i].Y()+broken_side/2,0.0);
		glRotatef(rot,0,0,1);
		glBegin(GL_QUADS);
			glVertex2f(-side/2,-side/2);
			glVertex2f(side/2,-side/2);
			glVertex2f(side/2,side/2);
			glVertex2f(-side/2,side/2);
		glEnd();
		glPopMatrix();
	}
}

void Brick::update()
{
	for(unsigned int i=0;i<brokenlist.size();i+=2)
	{
		brokenlist[i].Set(brokenlist[i].X()+brokenlist[i+1].X(),
							brokenlist[i].Y()-brokenlist[i+1].Y(),int(brokenlist[i].Z()+10)%360);
		if(brokenlist[i].X() < gap || brokenlist[i].X() > game_width - 2*gap
			|| brokenlist[i].Y() < 0)
		{
			brokenlist.erase(brokenlist.begin()+i);
			brokenlist.erase(brokenlist.begin()+i);
			i-=2;
		}
	}
}