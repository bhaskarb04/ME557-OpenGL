#include "Background.h"

Background::Background(GLfloat blx, GLfloat bly,GLfloat tlx, GLfloat tly,GLfloat trx, GLfloat tryy, GLfloat brx, GLfloat bry)
{
	bl.Set(blx,bly,0.0);
	tl.Set(tlx,tly,0.0);
	tr.Set(trx,tryy,0.0);
	br.Set(brx,bry,0.0);

	paddlesize=60.0;
	paddle.Set((brx-blx)/2 - paddlesize/2,0.0,(brx-blx)/2 + paddlesize/2);
}

Background::~Background()
{

}

void Background::display()
{
	glColor3f(0.0,1.0,0.0); //green lines
	glLineWidth(4.0);
	glBegin(GL_LINES);
		glVertex2f(bl.X(),bl.Y());
		glVertex2f(tl.X(),tl.Y());
		glVertex2f(tl.X(),tl.Y());
		glVertex2f(tr.X(),tr.Y());
		glVertex2f(tr.X(),tr.Y());
		glVertex2f(br.X(), br.Y());
	glEnd();
	glLineWidth(8.0);
	glBegin(GL_LINES);
		glVertex2f(paddle.X(),paddle.Y());
		glVertex2f(paddle.Z(),paddle.Y());
	glEnd();
}

vector<vec3_t> Background::Bgvecs()
{
	std::vector<vec3_t> tmp(4);
	tmp[0]=bl;
	tmp[1]=tl;
	tmp[2]=tr;
	tmp[3]=br;
	return tmp;
}

void Background::move_paddle(bool left)
{
	GLfloat x;
	if(left)
		x=(0.0>paddle.X()-speed)?0.0:paddle.X()-speed;
	else
		x=(br.X()<paddle.X()+paddlesize+speed)?br.X()-paddlesize:paddle.X()+speed;
	paddle.Set(x,paddle.Y(),x+paddlesize);
}