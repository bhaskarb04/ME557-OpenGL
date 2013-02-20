#include "model.h"

model::model(void)
{
	for(int i=0;i<BASE;i++)
	{
		for(int j=0;j<BASE;j++)
		{
			taken[i][j]=false;
			for(int k=0;k<4;k++)
				dirmat[i][j][k]=false;
		}

	}
	turnsteps=0;
	for(float theta=0;theta<PI/2;theta+=0.01)
		turnsteps++;
	load_color_model();
	first_added=false;
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

model::~model(void)
{
}
void model::find_updownparams()
{
	edgesteps=20;
	updownangle=atan(RISE_H/float(blocksize-2*edgesteps));
	updownlength=RISE_H/sin((double)updownangle);
}
void model::load_color_model()
{
	//TODO fix colors
	type_color[start][0]=173.0f/255.0f;
	type_color[start][1]=216.0f/255.0f;
	type_color[start][2]=230.0f/255.0f;
	type_color[start][3]=1.0f; //light blue
	type_color[straight][0]=1.0f; //red
	type_color[straight][1]=0.0f;
	type_color[straight][2]=0.0f;
	type_color[straight][3]=1.0f;
	type_color[turn][0]=1.0f; //red
	type_color[turn][1]=0.0f;
	type_color[turn][2]=0.0f;
	type_color[turn][3]=1.0f;
	type_color[up][0]=1.0f; //red
	type_color[up][1]=0.0f;
	type_color[up][2]=0.0f;
	type_color[up][3]=1.0f;
	type_color[down][0]=1.0f; //red
	type_color[down][1]=0.0f;
	type_color[down][2]=0.0f;
	type_color[down][3]=1.0f;
	type_color[end][0]=0.0f; //dark blue
	type_color[end][1]=0.0f;
	type_color[end][2]=0.5f;
	type_color[end][3]=1.0f;
	type_color[loop][0]=1.0f; //red
	type_color[loop][1]=0.0f;
	type_color[loop][2]=0.0f;
	type_color[loop][3]=1.0f;
	type_color[tree][0]=1.0f; //green
	type_color[tree][1]=1.0f;
	type_color[tree][2]=1.0f;
	type_color[tree][3]=0.0f;
	type_color[people][0]=1.0f; //skin color
	type_color[people][1]=1.0f;
	type_color[people][2]=1.0f;
	type_color[people][3]=0.0f;

}

void model::model_draw()
{
	if(D2)
		draw(preview);
	for(int i=0;i<BASE;i++)
	{
		for(int j=0;j<BASE;j++)
		{
			if(taken[i][j])
			{
				if(list[i][j].type!=tree)
					draw(list[i][j]);
			}
		}
	}
	//glutSolidTeapot(50.0);
}
bool model::spot_taken(int x, int y)
{
	return taken[x+BASE/2][y+BASE/2];
}
void model::add_model()
{
	if(spot_taken(preview.x,preview.y))
		return;
	//preview.color[0]=1.0f;
	//preview.color[1]=0.0f;
	list[preview.x+BASE/2][preview.y+BASE/2]=preview;
	taken[preview.x+BASE/2][preview.y+BASE/2]=true;
	modify_dirmat();
	if(!first_added)
	{
		first_added=!first_added;
		beginx=preview.x;
		beginy=preview.y;
	}
	endx=preview.x;
	endy=preview.y;
	add_waypoints();
}
void model::modify_dirmat()
{
	int x=preview.x+BASE/2;
	int y=preview.y+BASE/2;
	if(preview.type==straight || preview.type==up || preview.type==down)
	{
		if(abs(preview.rotation)==0 || abs(preview.rotation)==180)
		{
			dirmat[x][y][east]=true;
			dirmat[x][y][west]=true;
		}
		if(abs(preview.rotation)==90 || abs(preview.rotation)==270)
		{
			dirmat[x][y][north]=true;
			dirmat[x][y][south]=true;
		}
	}
	if(preview.type==turn)
	{
		if(abs(preview.rotation)==0)
		{
			dirmat[x][y][west]=true;
			dirmat[x][y][south]=true;
		}
		if(preview.rotation==90 || preview.rotation==-270)
		{
			dirmat[x][y][east]=true;
			dirmat[x][y][south]=true;
		}
		if(preview.rotation==180 || preview.rotation==-180)
		{
			dirmat[x][y][east]=true;
			dirmat[x][y][north]=true;
		}
		if(preview.rotation==270 || preview.rotation==-90)
		{
			dirmat[x][y][west]=true;
			dirmat[x][y][north]=true;
		}
	}
}
void model::add_waypoints()
{
	int wptend=waypoints.size();
	point p;
	bool flip=false;
	vector<point>temp;
	if(preview.type==straight)
	{
		if(preview.lefttoright==-1 || preview.uptodown==1)
			flip=true;
		
		if(abs(preview.rotation)==0 || abs(preview.rotation)==180)
		{
			for(int x=preview.x*blocksize;x<preview.x*blocksize+blocksize;x++)
			{
				p.x=x;p.y=preview.y*blocksize+blocksize/2;p.z=preview.h;
				if(flip)
					temp.push_back(p);
				else
					waypoints.push_back(p);
			}
		}
		if(abs(preview.rotation)==90 || abs(preview.rotation)==270)
		{
			for(int y=preview.y*blocksize;y<preview.y*blocksize+blocksize;y++)
			{
				p.x=preview.x*blocksize+blocksize/2;p.y=y;p.z=preview.h;
				if(flip)
					temp.push_back(p);
				else
					waypoints.push_back(p);
			}
		}
	}
	if(preview.type==turn)
	{
		float r=blocksize/2;
		float addx=0,addy=0;
		if(abs(preview.rotation)==0)
		{
			addx=0;
			addy=0;
			if(preview.lefttoright==1)
				flip=true;
		}
		if(preview.rotation==90 || preview.rotation==-270)
		{
			addx=blocksize;
			addy=0;
			if(preview.uptodown==-1)
				flip=true;
		}
		if(preview.rotation==180 || preview.rotation==-180)
		{
			addx=blocksize;
			addy=blocksize;
			if(preview.lefttoright==-1)
				flip=true;
		}
		if(preview.rotation==270 || preview.rotation==-90)
		{
			addx=0;
			addy=blocksize;
			if(preview.uptodown==1)
				flip=true;
		}
		point prev;
		float startangle=float(preview.rotation)*PI/180;
		for(float theta=startangle;theta<startangle+PI/2;theta+=0.01)
		{
			p.x=preview.x*blocksize+addx+r*cos(theta);//+(preview.x<0?blocksize:0);
			p.y=preview.y*blocksize+addy+r*sin(theta);//+(preview.y<0?blocksize:0);
			p.z=preview.h;
			if(prev.x==p.x && prev.y==p.y && prev.z==p.z)
				continue;
			prev.x=p.x; prev.y=p.y; prev.z=p.z;
			if(flip)
				temp.push_back(p);
			else
				waypoints.push_back(p);
		}	
	}
	if(preview.type==up || preview.type==down)
	{
		p.x=preview.x*blocksize;p.y=preview.y*blocksize;p.z=preview.h;
		QQuaternion startv,endv,middlev;
		if((preview.type==down && abs(preview.rotation)==180) || (preview.type==up && abs(preview.rotation)==0))
		{
			float thetastep=updownangle/(float)edgesteps;
			p.y+=blocksize/2;
			startv.setVector(p.x,p.y,p.z);endv.setVector(p.x+blocksize,p.y,p.z+RISE_H);
			for(float x=0;x<blocksize;x++)
			{
				middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
				middlev/=middlev.scalar();
				p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
				temp.push_back(p);
			}
			//startv.setVector(p.x,p.y,p.z);endv.setVector(preview.x*blocksize+blocksize-edgesteps,p.y,p.z+(blocksize-2*edgesteps)*tan(updownangle));
			//for(float x=0;x<blocksize-2*edgesteps;x++)
			//{
			//	//p.z=preview.h+z;
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)(blocksize-2*edgesteps));
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			//startv.setVector(p.x,p.y,p.z);endv.setVector(preview.x*blocksize+blocksize,p.y,p.z);
			//for(float x=0;x<edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			if(preview.type==down)
				flip=true;
			else
			{
				for(int i=0;i<temp.size();i++)
					waypoints.push_back(temp[i]);
			}
		}
		if((preview.type==down && abs(preview.rotation)==0) || (preview.type==up && abs(preview.rotation)==180))
		{
			float thetastep=updownangle/(float)edgesteps;
			p.y+=blocksize/2;
			p.x+=blocksize;
			startv.setVector(p.x,p.y,p.z);endv.setVector(preview.x*blocksize,p.y,p.z+RISE_H);
			for(float x=0;x<blocksize;x++)
			{
				middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
				middlev/=middlev.scalar();
				p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
				temp.push_back(p);
			}
			//for(float theta=updownangle,x=edgesteps;theta>0;theta-=thetastep,p.x--,x--,p.z=preview.h+tan(theta))
			//	temp.push_back(p);
			//startv.setVector(p.x,p.y,p.z);endv.setVector(preview.x*blocksize+edgesteps,p.y,p.z+(blocksize-2*edgesteps)*tan(updownangle));
			//for(float x=0;x<blocksize-2*edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			////for(float x=0,z=0;x<blocksize-2*edgesteps;x++,p.x--,z=tan(updownangle)*x)
			////{
			////	p.z=preview.h+z;
			////	temp.push_back(p);
			////}
			////float zorig=p.z;
			//startv.setVector(p.x,p.y,p.z);endv.setVector(preview.x*blocksize,p.y,p.z);
			//for(float x=0;x<edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			//for(float theta=0,x=0;theta<updownangle;theta+=thetastep,p.x--,p.z=zorig+tan(theta),x++)
				//temp.push_back(p);
			if(preview.type==down)
				flip=true;
			else
			{
				for(int i=0;i<temp.size();i++)
					waypoints.push_back(temp[i]);
			}
			
		}
		if((preview.type==down && (preview.rotation==-90 || preview.rotation==270 )) || 
			(preview.type==up &&  (preview.rotation==90 || preview.rotation==-270 )))
		{
			float thetastep=updownangle/(float)edgesteps;
			p.x+=blocksize/2;
			startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize+blocksize,p.z+RISE_H);
			for(float x=0;x<blocksize;x++)
			{
				middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
				middlev/=middlev.scalar();
				p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
				temp.push_back(p);
			}
			//for(float theta=0;theta<updownangle;theta+=thetastep,p.y++)
			//	temp.push_back(p);
			//startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize+blocksize-edgesteps,p.z+tan(updownangle)*(blocksize-2*edgesteps));
			//for(float x=0;x<blocksize-2*edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			///*for(float y=0,z=0;y<blocksize-2*edgesteps;y++,p.y++,z=tan(updownangle)*y)
			//{
			//	p.z=preview.h+z;
			//	temp.push_back(p);
			//}*/
			//startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize+blocksize,p.z);
			//for(float x=0;x<edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			//for(float theta=updownangle;theta>0;theta-=thetastep,p.y++)
			//	temp.push_back(p);
			if(preview.type==down)
				flip=true;
			else
			{
				for(int i=0;i<temp.size();i++)
					waypoints.push_back(temp[i]);
			}
		}
		if((preview.type==down && (preview.rotation==90 || preview.rotation==-270 )) || 
			(preview.type==up &&  (preview.rotation==-90 || preview.rotation==270 )))
		{
			float thetastep=updownangle/(float)edgesteps;
			p.x+=blocksize/2;
			p.y+=blocksize;
			startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize,p.z+RISE_H);
			for(float x=0;x<blocksize;x++)
			{
				middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
				middlev/=middlev.scalar();
				p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
				temp.push_back(p);
			}
			//for(float theta=updownangle;theta>0;theta-=thetastep,p.y--)
			//	temp.push_back(p);
			//startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize+edgesteps,preview.h+(blocksize-2*edgesteps)*tan(updownangle));
			//for(float x=0;x<blocksize - 2*edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			///*for(float y=0,z=0;y<blocksize-2*edgesteps;y++,p.y--,z=tan(updownangle)*y)
			//{
			//	p.z=preview.h+z;
			//	temp.push_back(p);
			//}*/
			//startv.setVector(p.x,p.y,p.z);endv.setVector(p.x,preview.y*blocksize,p.z);
			//for(float x=0;x<edgesteps;x++)
			//{
			//	middlev=QQuaternion::nlerp(startv,endv,(float)x/(float)edgesteps);
			//	middlev/=middlev.scalar();
			//	p.x=middlev.x();p.y=middlev.y();p.z=middlev.z();
			//	temp.push_back(p);
			//}
			//for(float theta=0;theta<updownangle;theta+=thetastep,p.y--)
			//	temp.push_back(p);
			if(preview.type==down)
				flip=true;
			else
			{
				for(int i=0;i<temp.size();i++)
					waypoints.push_back(temp[i]);
			}
			
		}
	}
	if(flip)
	{
		for(int i=temp.size()-1;i>=0;i--)
			waypoints.push_back(temp[i]);
	}
	fix_waypoints(wptend);
}
void model::fix_waypoints(int begin)
{
	if(begin!=0)
		begin-=2;
	for(int i=begin;i<waypoints.size()-1;i++)
	{
		QVector3D q1(waypoints[i].x,waypoints[i].y,waypoints[i].z);
		QVector3D q2(waypoints[i+1].x,waypoints[i+1].y,waypoints[i+1].z);
		if((q1-q2).length()<1)
		{
			waypoints.erase(waypoints.begin()+i+1);
			i--;
		}
	}
}
bool model::preview_animation(TYPE t, int x, int y,int rot)
{
	block b;
	b.type=t;
	b.x=x;
	b.y=y;
	b.h=5.0;
	memcpy(b.color,type_color[t],sizeof(float)*4);
	b.rotation=rot;
	if(t==straight || t==up || t==down)
	{
		if(abs(rot)==0 || abs(rot)==180)
		{
			b.dir[north]=false;
			b.dir[south]=false;
			b.dir[east]=true;
			b.dir[west]=true;
		}
		if(abs(rot)==90 || abs(rot)==270)
		{
			b.dir[north]=true;
			b.dir[south]=true;
			b.dir[east]=false;
			b.dir[west]=false;
		}
	}
	if(preview.type==turn)
	{
		if(abs(preview.rotation)==0)
		{
			b.dir[north]=false;
			b.dir[south]=true;
			b.dir[east]=false;
			b.dir[west]=true;
		}
		if(preview.rotation==90 || preview.rotation==-270)
		{
			b.dir[north]=false;
			b.dir[south]=true;
			b.dir[east]=true;
			b.dir[west]=false;
		}
		if(preview.rotation==180 || preview.rotation==-180)
		{
			b.dir[north]=true;
			b.dir[south]=false;
			b.dir[east]=true;
			b.dir[west]=false;
		}
		if(preview.rotation==270 || preview.rotation==-90)
		{
			b.dir[north]=true;
			b.dir[south]=false;
			b.dir[east]=false;
			b.dir[west]=true;
		}
	}
	b.lefttoright=0;
	b.uptodown=0;
	preview=b;
	return(check_preview());
}
bool model::check_preview()
{
	if(spot_taken(preview.x,preview.y))
		return false;
	if(!first_added || preview.type==tree || preview.type==people)
		return true;
	int falsematch=0;
	//left check
	int x=max(preview.x - 1,-BASE/2);
	int y=preview.y;
	if(spot_taken(x,y))
	{
		if(preview.type==up || preview.type==down)
		{
			if(abs(preview.rotation)!=0)
				return false;
		}
		if(dirmat[x+BASE/2][y+BASE/2][east]!=preview.dir[west])
			return false;
		else if(dirmat[x+BASE/2][y+BASE/2][east]==false)
			falsematch++;
		preview.lefttoright=1;
		if(list[x+BASE/2][y+BASE/2].type==up)
		{
			if(preview.type!=down)
				preview.h=list[x+BASE/2][y+BASE/2].h+RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
		else 
		{
			if(preview.type==down)
				preview.h=list[x+BASE/2][y+BASE/2].h-RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
	}
	else
		falsematch++;
	//down check
	x=preview.x;
	y=max(preview.y - 1,-BASE/2);
	if(spot_taken(x,y))
	{
		if(preview.type==up || preview.type==down)
		{
			if(!((preview.type==down && (preview.rotation==90 || preview.rotation==-270 )) || 
			(preview.type==up &&  (preview.rotation==90 || preview.rotation==-270 ))))
				return false;
		}
		if(dirmat[x+BASE/2][y+BASE/2][north]!=preview.dir[south])
			return false;
		else if(dirmat[x+BASE/2][y+BASE/2][north]==false)
			falsematch++;
		preview.uptodown=-1;
		if(list[x+BASE/2][y+BASE/2].type==up)
		{
			if(preview.type!=down)
				preview.h=list[x+BASE/2][y+BASE/2].h+RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
		else
		{
			if(preview.type==down)
				preview.h=list[x+BASE/2][y+BASE/2].h-RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
	}
	else
		falsematch++;
	//right check
	x=min(preview.x+1,BASE/2-1);
	y=preview.y;
	if(spot_taken(x,y))
	{
		if(preview.type==up || preview.type==down)
		{
			if(abs(preview.rotation)!=180)
				return false;
		}
		if(dirmat[x+BASE/2][y+BASE/2][west]!=preview.dir[east])
			return false;
		else if(dirmat[x+BASE/2][y+BASE/2][west]==false)
			falsematch++;
		preview.lefttoright=-1;
		if(list[x+BASE/2][y+BASE/2].type==up)
		{
			if(preview.type!=down)
				preview.h=list[x+BASE/2][y+BASE/2].h+RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
		else
		{
			if(preview.type==down)
				preview.h=list[x+BASE/2][y+BASE/2].h-RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
	}
	else
		falsematch++;
	//up check
	x=preview.x;
	y=min(preview.y+1,BASE/2-1);
	if(spot_taken(x,y))
	{
		if(preview.type==up || preview.type==down)
		{
			if(!((preview.type==down && (preview.rotation==-90 || preview.rotation==270 )) || 
			(preview.type==up &&  (preview.rotation==-90 || preview.rotation==270 ))))
				return false;
		}
		if(dirmat[x+BASE/2][y+BASE/2][south]!=preview.dir[north])
			return false;
		else if(dirmat[x+BASE/2][y+BASE/2][south]==false)
			falsematch++;
		preview.uptodown=1;
		if(list[x+BASE/2][y+BASE/2].type==up)
		{
			if(preview.type!=down)
				preview.h=list[x+BASE/2][y+BASE/2].h+RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
		else
		{

			if(preview.type==down)
				preview.h=list[x+BASE/2][y+BASE/2].h-RISE_H;
			else
				preview.h=list[x+BASE/2][y+BASE/2].h;
		}
	}
	else 
		falsematch++;
	if(falsematch==4)
		return false;
	return true;
}

void model::tree_model_draw()
{
	if(preview.type==tree && D2)
		draw(preview);
	for(int i=0;i<BASE;i++)
	{
		for(int j=0;j<BASE;j++)
		{
			if(taken[i][j])
			{
				if(list[i][j].type==tree)
					draw(list[i][j]);
			}
		}
	}
}
void model::draw(block b)
{
	int startx=b.x,starty=b.y;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,b.color);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if(b.type==straight)
	{
		glTranslatef(b.x*blocksize+blocksize/2,b.y*blocksize+blocksize/2,b.h);
		glRotatef(b.rotation,0,0,1);
		glTranslatef(-blocksize/2,0,0);
		drawstraight(startx,starty);
		
	}
	if(b.type==turn)
	{
		float addx,addy;
		if(abs(b.rotation)==0)
		{
			addx=0;addy=0;
		}
		if(b.rotation==90 || b.rotation==-270)
		{
			addx=blocksize;
			addy=0;
		}
		if(b.rotation==180 || b.rotation==-180)
		{
			addx=blocksize;
			addy=blocksize;
		}
		if(b.rotation==270 || b.rotation==-90)
		{
			addx=0;
			addy=blocksize;
		}
		glTranslatef(b.x*blocksize+addx,b.y*blocksize+addy,b.h);
		glRotatef(b.rotation,0,0,1);
		drawturn();
	}
	if(b.type==up)
	{
		glTranslatef(b.x*blocksize+blocksize/2,b.y*blocksize+blocksize/2,b.h);
		glRotatef(b.rotation,0,0,1);
		glTranslatef(-blocksize/2,0,0);
		drawup();
	}
	if(b.type==down)
	{
		glTranslatef(b.x*blocksize+blocksize/2,b.y*blocksize+blocksize/2,b.h);
		glRotatef(b.rotation+180,0,0,1);
		glTranslatef(-blocksize/2,0,0);
		drawup();
	}
	if(b.type==people || b.type==tree)
	{
		glTranslatef(b.x*blocksize,b.y*blocksize,blocksize/2);
		if(D3)
		{
			GLfloat view[16];
			QVector3D up(0,0,1);
			QVector3D look(Camerapos.x - (b.x*blocksize)
						  ,Camerapos.y - (b.y*blocksize) 
						  ,Camerapos.z);
			look.normalize();
			QVector3D right = QVector3D::crossProduct(up,look);
			right.normalize();
			QVector3D finalup = QVector3D::crossProduct(look,right);
			finalup.normalize();
			view[0]=right.x();view[1]=right.y();view[2]=right.z();view[3]=0.0;
			view[4]=finalup.x();view[5]=finalup.y();view[6]=finalup.z();view[7]=0.0;
			view[8]=look.x();view[9]=look.y();view[10]=look.z();view[11]=0.0;
			view[12]=0;view[13]=0;view[14]=0;view[15]=1.0;
			glTranslatef(+blocksize/2,+blocksize/2,0);
			glMultMatrixf(view);
			glTranslatef(-blocksize/2,-blocksize/2,0);
		}
		draw_treeorperson(b.type);
	}
	glPopMatrix();
}
void model::drawstraight(int x, int y)
{
	//GLUquadricObj *obj = gluNewQuadric();
	//gluCylinder(obj,RADIUS,RADIUS,blocksize, 30, 30);
	drawcylinder(blocksize);
}
void model::drawturn()
{
	float r=blocksize/2;
	float width=ceil((PI*r/2)/turnsteps);
	for(float theta=0;theta<PI/2;theta+=0.01)
	{
		float x=r*cos(theta);
		float y=r*sin(theta);
		glPushMatrix();
		glTranslatef(x,y,0.0);
		glRotatef(theta*180/PI,0,0,1);
		glRotatef(90,0,0,1);
		drawcylinder(width);
		glPopMatrix();
	}
}
void model::drawup()
{
	float thetastep=updownangle/(float)edgesteps;
	for(float theta=0,x=0;theta<updownangle;theta+=thetastep,x++)
	{
		glPushMatrix();
		glTranslatef(x,0,0);
		glRotatef(-theta*180/PI,0,1,0);
		drawcylinder(8.0);
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(edgesteps,0,0.0);
	glRotatef(-updownangle*180/PI,0,1,0);
	drawcylinder(updownlength);
	glPopMatrix();
	for(float theta=updownangle,x=blocksize-edgesteps;theta>0;theta-=thetastep,x++)
	{
		glPushMatrix();
		glTranslatef(x,0,RISE_H);
		glRotatef(-theta*180/PI,0,1,0);
		drawcylinder(8.0);
		glPopMatrix();
	}
}
void model::drawcylinder(GLfloat length)
{	
	/*glEnable( GL_TEXTURE_2D );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture( GL_TEXTURE_2D, texture_id[3]);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);*/

	glPushMatrix();
	glTranslatef(0,RADIUS/2,0);
	glRotatef(90,0,1,0);
	gluCylinder(qobj,RADIUS/2,RADIUS/2,length,30,30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,-RADIUS/2,0);
	glRotatef(90,0,1,0);
	gluCylinder(qobj,RADIUS/2,RADIUS/2,length,30,30);
	glPopMatrix();
	/*glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);*/
	return;
	//int nsides=NSIDES;
	//GLfloat w=RADIUS;
	//GLfloat h=length;
	//GLfloat *y= new float[nsides+1];
	//GLfloat *z= new float[nsides+1];
	//float angle;
	//QVector3D q1,q2,q3;
	////get the x and z position on a circle for all the sides
	//for(int i=0; i < nsides+1; i++)
	//{
	//	angle = 2*PI / (nsides) * i;
	//	y[i] = sin(angle) * w;
	//	z[i] = cos(angle) * w;
	//}

	////draw the top of the cylinder
	//glBegin(GL_TRIANGLE_FAN);
	//q1=QVector3D(0,0,0);
	//q2=QVector3D(0,y[nsides/4],z[nsides/4]);
	//q3=QVector3D::normal(q1,q2);
	//glNormal3f(q3.x(),q3.y(),q3.z());

	//glVertex3f(0,0,0);
	//for(int i=0; i < nsides+1; i++)
	//{
	//	glNormal3f(q3.x(),q3.y(),q3.z());
	//	glVertex3f(0,y[i],z[i]);
	//}
	//glEnd();
	////draw the center of the cylinder
	//glBegin(GL_QUAD_STRIP); 
	//for(int i=0; i < nsides+1; i++)
	//{
	//	q1=QVector3D(0,y[i],z[i]);
	//	q2=QVector3D(h,y[i],z[i]);
	//	q3=QVector3D::normal(q1,q2);
	//	glNormal3f(q3.x(),q3.y(),q3.z());
	//	glVertex3f(0,y[i],z[i]);
	//	glNormal3f(q3.x(),q3.y(),q3.z());
	//	glVertex3f(h,y[i],z[i]);
	//}
	//glEnd();

	////draw the bottom of the cylinder
	//glBegin(GL_TRIANGLE_FAN);
	//q1=QVector3D(h,0,0);
	//q2=QVector3D(h,y[nsides/4],z[nsides/4]);
	//q3=QVector3D::normal(q1,q2);
	//glNormal3f(q3.x(),q3.y(),q3.z());
	//glVertex3f(h,0,0);
	//for(int i=0; i < nsides+1; i++)
	//{
	//	glNormal3f(q3.x(),q3.y(),q3.z());
	//	glVertex3f(h,y[i], z[i]);
	//}
	//glEnd();
	//delete y;
	//delete z;
}
void model::draw_skybox()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	float range=(BASE/2)*blocksize+1;
	float X,Y,Z;
	X=Y=Z=1.0*range;
	glBindTexture(GL_TEXTURE_2D,texture_id[0]);
	glBegin(GL_QUADS);
	//left face
	glTexCoord2f(0.0,0.0); glVertex3f(-X,Y,-Z);
	glTexCoord2f(0.0,1.0); glVertex3f(-X,-Y,-Z);
	glTexCoord2f(1.0,1.0); glVertex3f(-X,-Y,Z);
	glTexCoord2f(1.0,0.0); glVertex3f(-X,Y,Z);
	//right face
	glTexCoord2f(0.0,0.0); glVertex3f(X,Y,-Z);
	glTexCoord2f(0.0,1.0); glVertex3f(X,-Y,-Z);
	glTexCoord2f(1.0,1.0); glVertex3f(X,-Y,Z);
	glTexCoord2f(1.0,0.0); glVertex3f(X,Y,Z);
	//front face
	glTexCoord2f(0.0,0.0); glVertex3f(-X,Y,-Z);
	glTexCoord2f(0.0,1.0); glVertex3f(X,Y,-Z);
	glTexCoord2f(1.0,1.0); glVertex3f(X,Y,Z);
	glTexCoord2f(1.0,0.0); glVertex3f(-X,Y,Z);
	//back face
	glTexCoord2f(0.0,0.0); glVertex3f(-X,-Y,-Z);
	glTexCoord2f(0.0,1.0); glVertex3f(X,-Y,-Z);
	glTexCoord2f(1.0,1.0); glVertex3f(X,-Y,Z);
	glTexCoord2f(1.0,0.0); glVertex3f(-X,-Y,Z);
	//top face
	glTexCoord2f(0.0,0.0); glVertex3f(-X,-Y,Z);
	glTexCoord2f(0.0,1.0); glVertex3f(X,-Y,Z);
	glTexCoord2f(1.0,1.0); glVertex3f(X,Y,Z);
	glTexCoord2f(1.0,0.0); glVertex3f(-X,Y,Z);
	//bottom face
	glTexCoord2f(0.0,0.0); glVertex3f(-X,-Y,-Z);
	glTexCoord2f(0.0,1.0); glVertex3f(X,-Y,-Z);
	glTexCoord2f(1.0,1.0); glVertex3f(X,Y,-Z);
	glTexCoord2f(1.0,0.0); glVertex3f(-X,Y,-Z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void model::draw_treeorperson(TYPE t)
{
	glEnable(GL_TEXTURE_2D);
	if(t==tree)
	{
		if(D2 || drawball)
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D,texture_id[1]);
		}
		else
		{
			glEnable(GL_BLEND);
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D,texture_id[1]);
			glBlendFunc(GL_DST_COLOR, GL_SRC_ALPHA);
		}
	}
	if(t==people)
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D,texture_id[2]);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex3f(0,0,0);
	glTexCoord2f(0.0,1.0); glVertex3f(blocksize,0,0);
	glTexCoord2f(1.0,1.0); glVertex3f(blocksize,blocksize,0);
	glTexCoord2f(1.0,0.0); glVertex3f(0,blocksize,0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void model::load_textures()
{
	glGenTextures(5,&texture_id[0]);
	//Sky
	QImage skyimage("Sky.png");
	glBindTexture(GL_TEXTURE_2D,texture_id[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, skyimage.width(), skyimage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,skyimage.bits());
	//TreeS
	QImage treeimage("Tree.png");
	glBindTexture(GL_TEXTURE_2D,texture_id[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, treeimage.width(), treeimage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,treeimage.bits());
	//People
	QImage peopleimage("People.png");
	glBindTexture(GL_TEXTURE_2D,texture_id[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, peopleimage.width(), peopleimage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,peopleimage.bits());
	//Metal
	QImage metalimage("metal-texture.png");
	glBindTexture(GL_TEXTURE_2D,texture_id[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, metalimage.width(), metalimage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,metalimage.bits());
	//Metal
	QImage ballimage("SoftballColor.png");
	glBindTexture(GL_TEXTURE_2D,texture_id[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, ballimage.width(), ballimage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE,ballimage.bits());

}