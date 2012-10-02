#include <vector> 
#include <assert.h>
#include "glut.h"


///Global state Variables////////////////////////////////////////
static GLint      windowHeight    = 480;
static GLint      windowWidth     = 640;
GLint type;
GLboolean fill;
#define POINTS 1
#define SCRIBBLE 2
#define LINES 3
#define RECT 4
struct point
{
	GLint x;
	GLint y;
	GLfloat psize;
	GLfloat r,g,b;
};
struct scribble
{
	std::vector<point>scriblist;
	GLfloat scribsize;
	GLfloat r,g,b;
};
struct line
{
	GLint x1,x2,y1,y2;
	GLfloat lsize,r,g,b;
};
struct rect
{
	GLint x1,x2,y1,y2;
	GLfloat rsize,r,g,b;
	GLboolean fill;
};
struct ordercell
{
	GLint type;
	GLint num;
};

std::vector<point> points;
std::vector<line> lines;
std::vector<scribble> scribbles;
std::vector<rect> rects;
std::vector<ordercell> order;
GLfloat R,G,B;
GLfloat pointsize;
GLfloat linesize;
GLboolean line_click;
/************** myInit ***************/
void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);    //set background color to Black
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(windowWidth), 0.0, GLdouble(windowHeight));
	type=POINTS;
	R=G=B=1.0;
	pointsize=2.0;
	linesize=2.0;
	line_click=false;
	fill=false;
}


/************** Menus ***************/
void main_menu(GLint id)
{
    // Handle main menu

	switch(id) 
	{
		//case 1: points
	case (1):
		type=POINTS;
		break;
		//case 2:scribble
	case (2):
		type=SCRIBBLE;
		break;
		//case 3:line
	case (3):
		type=LINES;
		break;
		//case 4:clear
	case (4):
		points.clear();
		lines.clear();
		scribbles.clear();
		rects.clear();
		order.clear();
		glutPostRedisplay();
		break;
		// case 5: exit the program
	case (5): 
		exit(-1);
		break;
	}
}

void rect_menu(GLint id)
{
	// Handle outline or fill rectangle
	type=RECT;
	switch(id)
	{
	case(1):
		fill=false;
		break;
	case(2):
		fill=true;
		break;
	}
}

void color_menu(GLint id)
{
	// Handle color menu
	switch(id)
	{
	case(1):
		R=1.0;G=1.0;B=1.0;
		break;
	case(2):
		R=1.0;G=0.0;B=0.0;
		break;
	case(3):
		R=0.0;G=1.0;B=0.0;
		break;
	case(4):
		R=0.0;G=0.0;B=1.0;
		break;
	case(5):
		R=1.0;G=1.0;B=0.0;
		break;
	}
}

void pixel_menu(GLint id)
{
	// Handle pixel size
	switch(id)
	{
	case(1):
		pointsize=2.0;
		break;
	case(2):
		pointsize=4.0;
		break;
	case(3):
		pointsize=8.0;
		break;
	case(4):
		pointsize=16.0;
		break;
	}
}

void line_menu(GLint id)
{
	// Handle line width
	switch(id)
	{
	case(1):
		linesize=2.0;
		break;
	case(2):
		linesize=4.0;
		break;
	case(3):
		linesize=8.0;
		break;
	case(4):
		linesize=16.0;
		break;
	}
}


/************** defineMenus **********/
void defineMenus(void)
{		
	GLint r_menu = glutCreateMenu(rect_menu);
	glutAddMenuEntry("Outline", 1);
	glutAddMenuEntry("Fill",    2);
	
	GLint c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("White",   1);
	glutAddMenuEntry("Red",     2);
	glutAddMenuEntry("Green",   3);
	glutAddMenuEntry("Blue",    4);
	glutAddMenuEntry("Yellow",  5);
	
	GLint p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("2",  1);
	glutAddMenuEntry("4",  2);
	glutAddMenuEntry("8",  3);
	glutAddMenuEntry("16", 4);

	GLint l_menu = glutCreateMenu(line_menu);
	glutAddMenuEntry("2",  1);
	glutAddMenuEntry("4",  2);
	glutAddMenuEntry("8",  3);
	glutAddMenuEntry("16", 4);

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Point",       1);
	glutAddMenuEntry("Scribble",    2);
	glutAddMenuEntry("Line",        3);
	glutAddSubMenu("Rectangle",     r_menu);
	glutAddSubMenu("Color",         c_menu);
	glutAddSubMenu("Pixel Size",    p_menu);
	glutAddSubMenu("Line Weight",   l_menu);
	glutAddMenuEntry("Clear",       4);
	glutAddMenuEntry("Quit",        5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/************ myMouseButton *********/
void myMouseButton(GLint button, GLint state, GLint x, GLint y)
{	
	y = windowHeight - y;
	// Handle mouse click events
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		ordercell ord;
		ord.type=type;
		if(type==POINTS)
		{
			point p;
			p.x=x;p.y=y;
			p.psize=pointsize;
			p.r=R;p.g=G;p.b=B;
			points.push_back(p);
			ord.num=points.size()-1;
			order.push_back(ord);
		}
		if(type==SCRIBBLE)
		{
			scribble s;
			s.scribsize=pointsize;
			s.r=R;s.b=B;s.g=G;
			point p;
			p.x=x;p.y=y;
			s.scriblist.push_back(p);
			scribbles.push_back(s);
			ord.num=scribbles.size()-1;
			order.push_back(ord);
		}
		if(type==LINES)
		{
			if(!line_click)
			{
				line l;
				l.x1=x;
				l.y1=y;
				l.x2=x;
				l.y2=y;
				l.lsize=linesize;
				l.r=R;l.g=G;l.b=B;
				lines.push_back(l);
				line_click=true;
			}
			else
			{
				lines[lines.size()-1].x2=x;
				lines[lines.size()-1].y2=y;
				line_click=false;
				ord.num=lines.size()-1;
				order.push_back(ord);
			}
		}
		if(type==RECT)
		{
			rect r;
			r.x1=x;r.y1=y;
			r.x2=x;r.y2=y;
			r.rsize=linesize;
			r.r=R;r.g=G;r.b=B;
			r.fill=fill;
			rects.push_back(r);
			ord.num=rects.size()-1;
			order.push_back(ord);
		}
	}

    // Force redraw
	glutPostRedisplay();
}


/************ myMouseMotion **********/
void myMouseMotion(GLint x, GLint y)
{
	y = windowHeight - y;
	// Handle mouse motion events
	if(type==SCRIBBLE)
	{
		point p;
		p.x=x;p.y=y;
		scribbles[scribbles.size()-1].scriblist.push_back(p);
	}
	if(type==RECT)
	{
		rects[rects.size()-1].x2=x;
		rects[rects.size()-1].y2=y;
	}
    // Force redraw
	glutPostRedisplay();
}

void myPassiveMotion(GLint x, GLint y)
{
	y = windowHeight - y;
	if(type==LINES && line_click)
	{
		lines[lines.size()-1].x2=x;
		lines[lines.size()-1].y2=y;
	}
	glutPostRedisplay();
}
/************** myDisplay ************/
void myDisplay(void)
{
    // Handle draw events
	
    glClear(GL_COLOR_BUFFER_BIT);
	// Draw stuff here
	for(int i=0;i<points.size();i++)
	{
		glPointSize(points[i].psize);
		glBegin(GL_POINTS);
			glColor3f(points[i].r,points[i].g,points[i].b);
			glVertex2i(points[i].x,points[i].y);
		glEnd();
	}
	for(int i=0;i<scribbles.size();i++)
	{
		glPointSize(scribbles[i].scribsize);
		glBegin(GL_POINTS);
			glColor3f(scribbles[i].r,scribbles[i].g,scribbles[i].b);
			for(int j=0;j<scribbles[i].scriblist.size();j++)
			{
				glVertex2i(scribbles[i].scriblist[j].x,scribbles[i].scriblist[j].y);
			}
		glEnd();
	}
	for(int i=0;i<lines.size();i++)
	{
		//if(i+1==lines.size() && line_click)
			//continue;
		glLineWidth(lines[i].lsize);
		glBegin(GL_LINES);
			glColor3f(lines[i].r,lines[i].g,lines[i].b);
			glVertex2i(lines[i].x1,lines[i].y1);
			glVertex2i(lines[i].x2,lines[i].y2);
		glEnd();
	}
	for(int i=0;i<rects.size();i++)
	{
		glLineWidth(rects[i].rsize);
		if(rects[i].fill)
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_POLYGON);
			glColor3f(rects[i].r,rects[i].g,rects[i].b);
			glVertex2i(rects[i].x1,rects[i].y1);
			glVertex2i(rects[i].x2,rects[i].y1);
			glVertex2i(rects[i].x2,rects[i].y2);
			glVertex2i(rects[i].x1,rects[i].y2);
		glEnd();
	}
	
	glutSwapBuffers();
}

void myKeyFunc(unsigned char key,int x, int y)
{
	if(key=='u' && order.size()>0)
	{
		switch(order[order.size()-1].type)
		{
		case(POINTS):
			points.pop_back();
			break;
		case(SCRIBBLE):
			scribbles.pop_back();
			break;
		case(LINES):
			lines.pop_back();
			break;
		case(RECT):
			rects.pop_back();
			break;
		}
		order.pop_back();
	}
	glutPostRedisplay();
}
/************** main *****************/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("My Paint Program");
    
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouseButton);
	glutPassiveMotionFunc(myPassiveMotion);
    glutMotionFunc(myMouseMotion);
	glutKeyboardFunc(myKeyFunc);
	defineMenus();
	myInit();

    glutMainLoop();
	return 0;
}
