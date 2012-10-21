#include "Ball.h"
#include "Background.h"
#include "Bricks.h"
#include <Windows.h>

#define TimerInterval 33
GLint window_width=768,window_height=500;
GLfloat gap=1.0;
GLint game_width=window_width - 150*gap;
GLfloat defsize=10.0;
GLfloat speed=4.0;
GLint lives=3;

Ball *ball;
Background *bg;
Brick *brick;
bool pause_flag=true;
bool done=false;
int main_window_,message_window;

void restart(bool);


void subdisplay(int substate)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	
	if(substate==1)
	{
		if(speed<8.0)
		{
			glRasterPos2i(window_width/2-25,window_height/2);
			string ss="Congrats You Leveled Up";
			
			for(unsigned int i=0;i<ss.length();i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ss[i]);
			speed+=2.0;
			PlaySound("excellent3.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		else
		{
			glRasterPos2i(window_width/2-20,window_height/2);
			string ss="Congrats You Won!!";
			for(unsigned int i=0;i<ss.length();i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ss[i]);
			PlaySound("woohoo.wav", NULL, SND_FILENAME | SND_ASYNC);
			speed=4.0;
		}
		//glutDestroyWindow(message_window);
		restart(true);
	}
	if(substate==2)
	{
		lives--;
		if(lives==0)
		{
			glRasterPos2i(window_width/2-18,window_height/2);
			string ss="Good Game Restarting";
			for(unsigned int i=0;i<ss.length();i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ss[i]);
			restart(false);
			PlaySound("you.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		else
		{
			glRasterPos2i(window_width/2-3,window_height/2);
			string ss="OOPS!!";
			for(unsigned int i=0;i<ss.length();i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ss[i]);
			restart(true);
			if(lives%2==0)
				PlaySound("hahah.wav", NULL, SND_FILENAME | SND_ASYNC);
			else
				PlaySound("doh.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		//glutDestroyWindow(message_window);
	}
	glutSwapBuffers();
}
void messagebox(int state)
{
	subdisplay(state);
	Sleep(2000);
	pause_flag=false;
}
void Initialize(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);    //set background color to Black
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(window_width), 0.0, GLdouble(window_height));
	ball= new Ball(defsize);
	bg = new Background(gap,gap,gap,window_height - gap,game_width,window_height - gap,game_width,gap);
	brick = new Brick();

}

void restart(bool keepscore)
{
	GLint score=0;
	if(keepscore)
		score=brick->showscore();
	else
	{
		speed=4.0;
		lives=3;
	}
	delete ball;
	delete bg;
	delete brick;
	ball= new Ball(defsize);
	bg = new Background(gap,gap,gap,window_height - gap,game_width,window_height - gap,game_width,gap);
	brick = new Brick();
	pause_flag=true;
	if(keepscore)
		brick->setscore(score);
}

void Collision_Avoidance()
{
	//avoid the walls
	vector<vec3_t> Wallvec=bg->Bgvecs();
	for(unsigned int i=0;i<Wallvec.size()-1;i++)
	{
		ball->avoid(Wallvec[i],Wallvec[i+1]);
	}
	//avoid the paddle
	vec3_t p(bg->paddle_pos());
	vec3_t pvec1(p.X(),p.Y(),0.0),pvec2(p.Z(),p.Y(),0.0);
	ball->avoid(pvec1,pvec2);
	//avoid bricks
	vector<rect> list=brick->getlist();
	done=true;
	for(unsigned int i=0;i<list.size();i++)
	{
		if(!list[i].visible)
			continue;
		done=false;
		vec3_t bvec1,bvec2,bvec3,bvec4;
		bvec1.Set(list[i].x,list[i].y,0);
		bvec2.Set(list[i].x+list[i].width,list[i].y,0);
		bvec3.Set(list[i].x+list[i].width,list[i].y+list[i].height,0);
		bvec4.Set(list[i].x,list[i].y+list[i].height,0);
		//bottom edge
		if(	ball->avoid(bvec1,bvec2) ||
		//right edge
			ball->avoid(bvec2,bvec3)    ||
		//top edge
			ball->avoid(bvec3,bvec4)    ||
		//left edge
			ball->avoid(bvec4,bvec1)    )
		{
			brick->set_invisible(i);
			PlaySound("Break.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

	}
}

void Check_state()
{
	if(done)
	{
		pause_flag=true;
		messagebox(1);
	}
	if(ball->showctr().Y() < 0)
	{
		pause_flag=true;
		messagebox(2);
	}
}

void myTimerFunction(int value)
{

	if(!pause_flag)
	{
		ball->update();
		Collision_Avoidance();
		Check_state();
		brick->update();
	}

	glutPostRedisplay();
	glutTimerFunc(TimerInterval, myTimerFunction,1);
}

void keypress(int key,int x,int y)
{
	switch (key)
	{
	case(GLUT_KEY_LEFT):
		bg->move_paddle(true);
		break;

	case(GLUT_KEY_RIGHT):
		bg->move_paddle(false);
		break;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	bg->display();
	ball->display();
	brick->display();

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0,0,width,height);
}


void mainmenu(GLint menunum)
{
	vec3_t v;
	switch(menunum)
	{
	case(1):
		speed=4.0;
		v=ball->showvel();
		v.Normalize();
		v*=speed;
		ball->setvelctr(ball->showctr(),v);
		break;
	case(2):
		speed=6.0;
		v=ball->showvel();
		v.Normalize();
		v*=speed;
		ball->setvelctr(ball->showctr(),v);
		break;
	case(3):
		speed=8.0;
		v=ball->showvel();
		v.Normalize();
		v*=speed;
		ball->setvelctr(ball->showctr(),v);
		break;
	case(4):
		restart(false);
		break;
	case(5):
		exit(0);
		break;
	}
}
void MakeMenus(void)
{
	glutCreateMenu(mainmenu);
	glutAddMenuEntry("Level1",1);
	glutAddMenuEntry("Level2",2);
	glutAddMenuEntry("Level3",3);
	glutAddMenuEntry("Restart",4);
	glutAddMenuEntry("Quit",5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void MouseFunc(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON)
	{
		pause_flag=false;
	}
	if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{
		pause_flag=true;
	}
}

void keypress2(unsigned char key,int x,int y)
{
	if(key==' ')
		pause_flag=!pause_flag;
}

void menustate(GLint status)
{
	if(status== GLUT_MENU_IN_USE)
		pause_flag=true;
	if(status==GLUT_MENU_NOT_IN_USE)
		pause_flag=false;

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);                         
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   
	glutInitWindowSize(window_width,window_height);
	glutInitWindowPosition(100, 150);              
	main_window_=glutCreateWindow("BreakOUT");                
  
	glutDisplayFunc(display);                  
	glutTimerFunc(TimerInterval, myTimerFunction,1);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress2);
	glutSpecialFunc(keypress);
	//glutMouseFunc(MouseFunc);
	glutMenuStateFunc(menustate);
	MakeMenus();
	Initialize();

	glutMainLoop(); 		                 // go into a perpetual loop
	return 0;
}