#include "Dot.h"
#include <iostream>
#include "glut.h"
#include <Windows.h>
#define PACSIZE 10
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define DO_NOTHING 4
#define MOVE_STEP 2
bool play=false;
dot d;
/************** myInit ***************/
void myInit(void)
{
    glClearColor(0.5, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(4.0);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
	point p;
	p.x=100;
	p.y=50;
	d.list.push_back(p);
	p.x=100;
	p.y=130;
	d.list.push_back(p);
	p.x=150;
	p.y=130;
	d.list.push_back(p);
}

//Draw Pacman
void draw_pacman(int x,int y,int dir,int mopen)
{
	glBegin(GL_POLYGON);
		glColor3f(1.0,0.0,0.0);
		glVertex2i(x-PACSIZE,y+PACSIZE);
		glVertex2i(x+PACSIZE,y+PACSIZE);
		glVertex2i(x+PACSIZE,y-PACSIZE);
		glVertex2i(x-PACSIZE,y-PACSIZE);
	glEnd();
	//glBegin(GL_POLYGON);
		glColor3f(0.0,0.0,1.0);
		switch(dir)
		{
		case(LEFT):
			glBegin(GL_POINTS);
				glVertex2i(x,y+PACSIZE/2);
			glEnd();
			glBegin(GL_POLYGON);
			if(mopen)
			{
				glVertex2i(x-PACSIZE,y+PACSIZE);
				glVertex2i(x,y);
				glVertex2i(x-PACSIZE,y-PACSIZE);
			}
			else
			{
				glVertex2i(x-PACSIZE,y+1);
				glVertex2i(x,y+1);
				glVertex2i(x,y-1);
				glVertex2i(x-PACSIZE,y-1);
			}
			glEnd();
			break;
		case(RIGHT):
			glBegin(GL_POINTS);
				glVertex2i(x,y+PACSIZE/2);
			glEnd();
			glBegin(GL_POLYGON);
			if(mopen)
			{
				glVertex2i(x+PACSIZE,y+PACSIZE);
				glVertex2i(x,y);
				glVertex2i(x+PACSIZE,y-PACSIZE);
			}
			else
			{
				glVertex2i(x+PACSIZE,y+1);
				glVertex2i(x,y+1);
				glVertex2i(x,y-1);
				glVertex2i(x+PACSIZE,y-1);
			}
			glEnd();
			break;
		case(UP):
			glBegin(GL_POINTS);
				glVertex2i(x+PACSIZE/2,y);
			glEnd();
			glBegin(GL_POLYGON);
			if(mopen)
			{
				glVertex2i(x-PACSIZE,y+PACSIZE);
				glVertex2i(x,y);
				glVertex2i(x+PACSIZE,y+PACSIZE);
			}
			else
			{
				glVertex2i(x+1,y+PACSIZE);
				glVertex2i(x+1,y);
				glVertex2i(x-1,y);
				glVertex2i(x-1,y+PACSIZE);
			}
			glEnd();
			break;
		case(DOWN):
			glBegin(GL_POINTS);
				glVertex2i(x-PACSIZE/2,y);
			glEnd();
			glBegin(GL_POLYGON);
			if(mopen)
			{
				glVertex2i(x-PACSIZE,y-PACSIZE);
				glVertex2i(x,y);
				glVertex2i(x+PACSIZE,y-PACSIZE);
			}
			else
			{
				glVertex2i(x+1,y-PACSIZE);
				glVertex2i(x+1,y);
				glVertex2i(x-1,y);
				glVertex2i(x-1,y-PACSIZE);
			}
			glEnd();
			break;
		default:
			glBegin(GL_POINTS);
				glVertex2i(x,y+PACSIZE/2);
			glEnd();
			glBegin(GL_POLYGON);
				glVertex2i(x-PACSIZE,y+PACSIZE);
				glVertex2i(x,y);
				glVertex2i(x-PACSIZE,y-PACSIZE);
			glEnd();
			break;
		}
	//glEnd();
}

int give_direction_to_nearest_point(int x,int y)
{
	if(d.list.size()==0)
		return (DO_NOTHING);
	int minval=(x-d.list[0].x)*(x-d.list[0].x)+(y-d.list[0].y)*(y-d.list[0].y);
	int minpoint=0;
	for(int i=1;i<d.list.size();i++)
	{
		int val=(x-d.list[i].x)*(x-d.list[i].x)+(y-d.list[i].y)*(y-d.list[i].y);
		if(minval > val)
		{
			minval=val;
			minpoint=i;
		}
	}
	if(minval < 100)
	{
		d.list.erase(d.list.begin()+minpoint);
		return(LEFT);
	}
	if(abs(d.list[minpoint].y - y) < MOVE_STEP || abs(d.list[minpoint].x - x) < abs(d.list[minpoint].y - y) && abs(d.list[minpoint].x - x) > MOVE_STEP-1)
	{
		if(d.list[minpoint].x - x > 0)
			return(RIGHT);
		else
			return(LEFT);
	}
	else
	{
		if(d.list[minpoint].y - y > 0)
			return(UP);
		else
			return(DOWN);
	}
}
/************** myDisplay ************/
void myDisplay(void)
{
	// Clear the colors on the screen from a previous display
	glClear(GL_COLOR_BUFFER_BIT);

	// Now, draw something
	glBegin(GL_POINTS);
	glColor3f(0.0,0.0,0.0);
		for(int i=0;i<d.list.size();i++)
			glVertex2i(d.list[i].x,d.list[i].y);
    glEnd();
	
		for(int i=0;i<d.paclist.size();i++)
		{
			//draw pacman
			glBegin(GL_POLYGON);
				glColor3f(0.0,0.0,0.0);
				glVertex2i(d.paclist[i].x-PACSIZE,d.paclist[i].y+PACSIZE);
				glVertex2i(d.paclist[i].x+PACSIZE,d.paclist[i].y+PACSIZE);
				glVertex2i(d.paclist[i].x+PACSIZE,d.paclist[i].y-PACSIZE);
				glVertex2i(d.paclist[i].x-PACSIZE,d.paclist[i].y-PACSIZE);
			glEnd();
			//Find nearest point and give direction
			int direction=give_direction_to_nearest_point(d.paclist[i].x,d.paclist[i].y);
			draw_pacman(d.paclist[i].x,d.paclist[i].y,direction,d.mouthopen[i]);
			if(d.mouthopen[i]==1)
				d.mouthopen[i]=0;
			else
				d.mouthopen[i]=1;
			switch(direction)
			{
			case(LEFT):
				d.paclist[i].x-=MOVE_STEP;
				break;
			case(RIGHT):
				d.paclist[i].x+=MOVE_STEP;
				break;
			case(UP):
				d.paclist[i].y+=MOVE_STEP;
				break;
			case(DOWN):
				d.paclist[i].y-=MOVE_STEP;
				break;
			}
		}
	// this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}

int near_dot(int x,int y)
{
	for(int i=0;i<d.list.size();i++)
	{
		if((x-d.list[i].x)*(x-d.list[i].x)+(y-d.list[i].y)*(y-d.list[i].y) < 25)
		{
			d.list.erase(d.list.begin()+i);
			return (1);
		}
	}
	for(int j=0;j<d.paclist.size();j++)
	{
		if((x-d.paclist[j].x)*(x-d.paclist[j].x)+(y-d.paclist[j].y)*(y-d.paclist[j].y) < PACSIZE*PACSIZE)
		{
			return (2);
		}
	}
	return (0);
}

void idlefunc(void)
{
	glutPostRedisplay();
	Sleep(100);
}
void myMouseFunc(int button,int state,int x,int y)
{
	y=480-y;
	switch(button)
	{
	case(GLUT_LEFT_BUTTON):
		if(state==GLUT_DOWN)
		{
			int dot_or_pacman=near_dot(x,y);
			if(dot_or_pacman==0)
			{
				point p;
				p.x=x;p.y=y;
				d.list.push_back(p);
				//cout<<x<<" "<<y<<endl;
			}
			else
			{
				if(dot_or_pacman==1)
				{
					point p;
					p.x=x;p.y=y;
					d.paclist.push_back(p);
					int x=1;
					d.mouthopen.push_back(x);
				}
			}
			glutPostRedisplay();
		}
		break;
	case(GLUT_RIGHT_BUTTON):
		//TODO: make it automatic and create pause button
		if(state==GLUT_DOWN)
		{
			if(!play)
			{
				glutIdleFunc(idlefunc);
				play=!play;
			}
			else
			{
				glutIdleFunc(NULL);
				play=!play;
			}
		}	
		break;
	}
}

/************** main *****************/
int main(int argc, char** argv)
{
	//	Initialize opengl & glut
	glutInit(&argc, argv);

	// Initialize display mode:
	// GLUT_SINGLE defines single buffering (i.e., display -> calculations -> display so on)
	// GLUT_DOUBLE defines double buffering (i.e., display something while do calculations in the background)
	// GLUT_RGB defines RGB color mode
	// GLUT_INDEX defines indexed color mode
	// GLUT_DEPTH defines that the display mode is 3D and depth is required
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Tell opengl how big your window size should be
	glutInitWindowSize(640, 480);

	// Tell opengl where on screen does the opengl window need to appear
	glutInitWindowPosition(100, 150);
    
	// Now tell opengl to create the opengl window
	glutCreateWindow("Competing PacMen");
    
    // init() consists of all the user defined initialization functions & variables
	// example, whether the lighting is enabled, what is the background color, etc
	myInit();

	// tell opengl window to draw the scene that you define in the function myDisplay()
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouseFunc);
	// Enter the opengl event processing loop
	// no draw events will be processed without glutMainLoop()
	// glut goes into an infinite loop that never returns. Hence the program doesnt return
	glutMainLoop();

	return 0;
}
