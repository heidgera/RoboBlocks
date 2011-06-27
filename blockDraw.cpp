/*
 *  blockDraw.cpp
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockDraw.h"

void glBezier3f(double Ax,double Bx, double Cx,double Ay,double By, double Cy,double r, double g, double bl, double aug){
	double a=1.0;
	double b=1.0-a;
	double x,y;
	double steps=10;
	for (int i=0; i<steps+1; i++) {
		x=Ax*a*a+Bx*2*a*b+Cx*b*b;
		y=Ay*a*a+By*2*a*b+Cy*b*b;
		//double dis = (Cy<Ay)? (y-Cy)/(Ay-Cy):(y-Ay)/(Cy-Ay);
		//glColor3f(r+aug*dis,g+aug*dis,bl+aug*dis);
		glVertex2d(x,y);
		a-=1/steps;
		b=1.0-a;
	}
}

void glWideCurve(double Ax,double Bx, double Cx,double Ay,double By, double Cy,double sp,double r, double g, double bl, double aug,bool top){
	//bool up=true;
	double strt=.8;
	double a=1.0;
	double b=1.0-a;
	double x,y;
	double steps=10;
	for (int i=0; i<steps+1; i++) {
		x=Ax*a*a+Bx*2*a*b+Cx*b*b;
		y=Ay*a*a+By*2*a*b+Cy*b*b;
		if(!top) glColor4f(r+aug,g+aug,bl+aug,strt); else glColor4f(r,g,bl,0);
		glVertex2d(x,y+sp);
		if(!top) glColor4f(r,g,bl,0); else glColor4f(r+aug,g+aug,bl+aug,strt);
		glVertex2d(x,y);
		a-=1/steps;
		b=1.0-a;
	}
}
void drawBlock(double x, double y, double w, double h){
	float r=ofGetStyle().color.r/255.;
	float g=ofGetStyle().color.g/255.;
	float b=ofGetStyle().color.b/255.;
	if(h>5) h-=5;
	double v=10;
	double s=10;
	double shBot=-.3;
	double shTop=.3;
	double botDis=2*v;
	double topDis=2*v;
	double strt=.8;
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+2*s,y+h);
	glBezier3f(x+s,x,x,y+h,y+h,y+h-v);
	glBezier3f(x,x,x+s,y+v,y,y);
	glBezier3f(x+2*s,x+2*s,x+3*s,y,y+v/2,y+v/2);
	glVertex2f(x+s*3,y+h-v);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(r,g,b);
	glVertex2f(x+s*3,y+h-v);
	glBezier3f(x+4*s,x+4*s,x+3*s,y+h,y+h+v/2,y+h+v/2);
	glBezier3f(x+3*s,x+2*s,x+2*s,y+h+v/2,y+h+v/2,y+h);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(r,g,b);
	glVertex2f(x+4*s,y+h);
	glVertex2f(x+s*3,y+h-v);
	glBezier3f(x+3*s,x+4*s,x+4*s,y+v/2,y+v/2,y);
	glBezier3f(x+w-s,x+w,x+w,y,y,y+v);
	glBezier3f(x+w,x+w,x+w-s,y+h-v,y+h,y+h);
	glEnd();
	
	glColor3f(0,0,0);
	
	///****************** Shading starting at the top right corner *****
	glBegin(GL_QUAD_STRIP);
	for ( int i = 270; i <= 360; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+v);
	}
	//**Bottom bottom**
	for ( int i = 0; i <= 90; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+h-v);
	}
	glWideCurve(x+4*s,x+4*s,x+3*s,y+h-botDis,y+h-botDis+v/2,y+h-botDis+v/2,botDis,r,g,b,shBot-.1);
	glWideCurve(x+3*s,x+2*s,x+2*s,y+h-botDis+v/2,y+h-botDis+v/2,y+h-botDis,botDis,r,g,b,shBot-.1);
	for ( int i = 90; i <= 180; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+s,y+h-v);
	}
	for ( int i = 180; i <= 270; ++i ){
		float perc2 = shTop;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+s+s*cos(i*M_PI/(180)),y+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+s,y+v);
	}
	glColor4f(r,g,b,0);
	glVertex2f(x+s,y+v);
	glColor4f(r+shTop,g+shTop,b+shTop,strt);
	glVertex2f(x+s,y);
	glWideCurve(x+2*s,x+2*s,x+3*s,y,y+v/2,y+v/2,topDis,r,g,b,shTop,true);
	glWideCurve(x+3*s,x+4*s,x+4*s,y+v/2,y+v/2,y,topDis,r,g,b,shTop,true);
	glColor4f(r,g,b,0);
	glVertex2d(x+w-s,y+v);
	glColor4f(r+shTop,g+shTop,b+shTop,strt);
	glVertex2d(x+w-s,y);
	glEnd();
}

void drawBaseBlock(double x, double y, double w, double h){
	float r=ofGetStyle().color.r/255.;
	float g=ofGetStyle().color.g/255.;
	float b=ofGetStyle().color.b/255.;
	if(h>5) h-=5;
	double v=10;
	double s=10;
	double shBot=-.3;
	double shTop=.3;
	double botDis=2*v;
	double topDis=2*v;
	double strt=.8;
	ofSetColor(ofGetStyle().color);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*3,y+h-v);
	glVertex2f(x+w, y);
	glBezier3f(x+w,x+w,x+w-s,y+h-v,y+h,y+h);
	glBezier3f(x+4*s,x+4*s,x+3*s,y+h,y+h+v/2,y+h+v/2);
	glBezier3f(x+3*s,x+2*s,x+2*s,y+h+v/2,y+h+v/2,y+h);
	glBezier3f(x+s,x,x,y+h,y+h,y+h-v);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+w,y+h);
	glBezier3f(x+w,x+w,x+w-s,y+h+v,y+h,y+h);
	glVertex2f(x+w-s,y+h-v);
	glVertex2f(x+w,y+h-v);
	glEnd();
	double rad=8*s-5;
	ofCircle(x+w+rad, y+h, rad);
	ofRect(x+w, y, rad*2, h);
	glColor3f(0,0,0);
	
	///****************** Shading starting at the top right corner *****
	glBegin(GL_QUAD_STRIP);
	/*for ( int i = 270; i <= 360; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+v);
	}*/
	//**Bottom bottom**
	float perc2 = shBot;
	glColor4f(r+perc2,g+perc2,b+perc2,strt);
	/*glVertex2d(x+w,y);
	glColor4f(r,g,b,0);
	glVertex2f(x+w-s,y);
	for ( int i = 0; i <= 90; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+h-v);
	}*/
	//------- with upload on top
	glVertex2d(x+w+rad*2,y);
	glColor4f(r,g,b,0);
	glVertex2f(x+w+rad*1.5,y);
	for ( int i = 0; i <= 90; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w+rad+(rad)*cos(i*M_PI/(180)),y+h+rad*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2d(x+w+rad+4*s*cos(i*M_PI/(180)),y+h+4*s*sin(i*M_PI/(180)));
	}
	for ( int i = 90; i <= 165; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w+rad+rad*cos(i*M_PI/(180)),y+h+rad*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2d(x+w+rad+4*s*cos(i*M_PI/(180)),y+h+4*s*sin(i*M_PI/(180)));
	}
	for ( int i = 360; i >= 270; i-- ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+h+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2d(x+w-s+5*s*cos(i*M_PI/(180)),y+h+v+5*v*sin(i*M_PI/(180)));
	}
	//--------
	glWideCurve(x+4*s,x+4*s,x+3*s,y+h-botDis,y+h-botDis+v/2,y+h-botDis+v/2,botDis,r,g,b,shBot-.1);
	glWideCurve(x+3*s,x+2*s,x+2*s,y+h-botDis+v/2,y+h-botDis+v/2,y+h-botDis,botDis,r,g,b,shBot-.1);
	for ( int i = 90; i <= 180; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+s,y+h-v);
	}
	perc2 = shTop;
	glColor4f(r+perc2,g+perc2,b+perc2,strt);
	glVertex2d(x,y);
	glColor4f(r,g,b,0);
	glVertex2f(x+s,y);
	glEnd();
}

void drawBlockShadow(double x, double y, double w, double h){
	if(h>5) h-=5;
	double v=10;
	double s=10;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+2*s,y+h);
	glBezier3f(x+s,x,x,y+h,y+h,y+h-v);
	glBezier3f(x,x,x+s,y+v,y,y);
	glBezier3f(x+2*s,x+2*s,x+3*s,y,y+v/2,y+v/2);
	glVertex2f(x+s*3,y+h-v);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*3,y+h-v);
	glBezier3f(x+4*s,x+4*s,x+3*s,y+h,y+h+v/2,y+h+v/2);
	glBezier3f(x+3*s,x+2*s,x+2*s,y+h+v/2,y+h+v/2,y+h);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+4*s,y+h);
	glVertex2f(x+s*3,y+h-v);
	glBezier3f(x+3*s,x+4*s,x+4*s,y+v/2,y+v/2,y);
	glBezier3f(x+w-s,x+w,x+w,y,y,y+v);
	glBezier3f(x+w,x+w,x+w-s,y+h-v,y+h,y+h);
	glEnd();
}

void shadeCircleSlice(double x, double y, double ir, double oR,int beg,int end, double strt, double r,double g, double b,double shBot,double shTop){
	for ( int i = end+1; i >= beg-1; i-- ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+ir*cos(i*M_PI/(180)),y+ir*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2d(x+oR*cos(i*M_PI/(180)),y+oR*sin(i*M_PI/(180)));
	}
}

void drawCircleSeg(double x, double y, double ir, double oR,int beg,int end, double r,double g, double b){
	glBegin(GL_QUAD_STRIP);
	for ( int i = end+1; i >= beg-1; i-- ){
		glColor4f(r,g,b,1);
		glVertex2d(x+ir*cos(i*M_PI/(180)),y+ir*sin(i*M_PI/(180)));
		glVertex2d(x+oR*cos(i*M_PI/(180)),y+oR*sin(i*M_PI/(180)));
	}
	glEnd();
}

void drawBigBlockShadow(double x, double y, double w, double h,double oH){
	if(h>5) h-=5;
	double v=(h>oH) ? 2*oH/21 : 2*h/21;
	double s=w/20;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y+v);
	glBezier3f(x,x,x+s,y+v,y,y);
	glBezier3f(x+s*2,x+s*2,x+s*3,y,y+v/2,y+v/2);
	glVertex2f(x+s*4,y+v*4);
	glBezier3f(x+3*s,x+2*s,x+2*s,y+4*v,y+4*v,y+5*v);
	glVertex2f(x+s*2,y+h);
	glBezier3f(x+s,x,x,y+h,y+h,y+h-v);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*6,y+v*4);
	glBezier3f(x+s*6,x+s*6,x+s*5,y+v*4,y+v*4.5,y+v*4.5);
	glBezier3f(x+s*5,x+s*4,x+s*4,y+v*4.5,y+v*4.5,y+v*4);
	glBezier3f(x+s*3,x+s*4,x+s*4,y+v/2,y+v/2,y);
	glBezier3f(x+w-s,x+w,x+w,y,y,y+v);
	glBezier3f(x+w,x+w,x+w-s,y+v*3,y+v*4,y+v*4);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*2,y+h);
	glBezier3f(x+s*2,x+s*2,x+s*3,y+h-v*3,y+h-v*2,y+h-v*2);
	glBezier3f(x+s*4,x+s*4,x+s*5,y+h-v*2,y+h-v*1.5,y+h-v*1.5);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*4,y+h);
	glBezier3f(x+s*4,x+s*4,x+s*3,y+h,y+h+v/2,y+h+v/2);
	glBezier3f(x+s*3,x+s*2,x+s*2,y+h+v/2,y+h+v/2,y+h);
	glVertex2f(x+s*5,y+h-v*1.5);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*6,y+h);
	glVertex2f(x+s*4,y+h);
	glBezier3f(x+s*5,x+s*6,x+s*6,y+h-v*1.5,y+h-v*1.5,y+h-v*2);
	glBezier3f(x+w-s,x+w,x+w,y+h-v*2,y+h-v*2,y+h-v);
	glBezier3f(x+w,x+w,x+w-s,y+h-v,y+h,y+h);
	glEnd();
}

void drawBigBlock(double x, double y, double w, double h, double oh){
	float r=ofGetStyle().color.r/255.;
	float g=ofGetStyle().color.g/255.;
	float b=ofGetStyle().color.b/255.;
	if(h>5) h-=5;
	double v=(h>5) ? 2*h/21 : 0;
	if(v) v=(h<oh) ? 2*h/21 : 2*oh/21;
	double s=w/20;
	double shBot=-.5;
	double shTop=.6;
	double botDis=v;
	double topDis=v;
	double strt=.8;
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y+v);
	glBezier3f(x,x,x+s,y+v,y,y);
	glBezier3f(x+s*2,x+s*2,x+s*3,y,y+v/2,y+v/2);
	glVertex2f(x+s*4,y+v*4);
	glBezier3f(x+3*s,x+2*s,x+2*s,y+4*v,y+4*v,y+5*v);
	glVertex2f(x+s*2,y+h);
	glBezier3f(x+s,x,x,y+h,y+h,y+h-v);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*6,y+v*4);
	glBezier3f(x+s*6,x+s*6,x+s*5,y+v*4,y+v*4.5,y+v*4.5);
	glBezier3f(x+s*5,x+s*4,x+s*4,y+v*4.5,y+v*4.5,y+v*4);
	glBezier3f(x+s*3,x+s*4,x+s*4,y+v/2,y+v/2,y);
	glBezier3f(x+w-s,x+w,x+w,y,y,y+v);
	glBezier3f(x+w,x+w,x+w-s,y+v*3,y+v*4,y+v*4);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*2,y+h);
	glBezier3f(x+s*2,x+s*2,x+s*3,y+h-v*3,y+h-v*2,y+h-v*2);
	glBezier3f(x+s*4,x+s*4,x+s*5,y+h-v*2,y+h-v*1.5,y+h-v*1.5);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*4,y+h);
	glBezier3f(x+s*4,x+s*4,x+s*3,y+h,y+h+v/2,y+h+v/2);
	glBezier3f(x+s*3,x+s*2,x+s*2,y+h+v/2,y+h+v/2,y+h);
	glVertex2f(x+s*5,y+h-v*1.5);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x+s*6,y+h);
	glVertex2f(x+s*4,y+h);
	glBezier3f(x+s*5,x+s*6,x+s*6,y+h-v*1.5,y+h-v*1.5,y+h-v*2);
	glBezier3f(x+w-s,x+w,x+w,y+h-v*2,y+h-v*2,y+h-v);
	glBezier3f(x+w,x+w,x+w-s,y+h-v,y+h,y+h);
	glEnd();
	
	/******************* Shading on the bottom of the top*********/
	glBegin(GL_QUAD_STRIP);
	for ( int i = 270; i <= 361; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+v);
	}
	for ( int i = -2; i <= 90; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+v*3+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+v*3);
	}
	glWideCurve(x+6*s,x+6*s,x+5*s,y+v*4-botDis,y+v*4-botDis+v/2,y+v*4-botDis+v/2,botDis,r,g,b,shBot-.1);
	glWideCurve(x+5*s,x+4*s,x+4*s,y+v*4-botDis+v/2,y+v*4-botDis+v/2,y+v*4-botDis,botDis,r,g,b,shBot-.1);
	shadeCircleSlice(x+s*3,y+v*5,v,2*v,180,270,strt,r,g,b,-shBot,shBot);
	shadeCircleSlice(x+s*3,y+h-v*3,v,2*v,90,180,strt,r,g,b,-shBot,-shTop);
	glWideCurve(x+4*s,x+4*s,x+5*s,y+h-v*2,y+h-2*v+v/2,y+h-v*2+v/2,topDis,r,g,b,shTop,true);
	glWideCurve(x+5*s,x+6*s,x+6*s,y+h-v*2+v/2,y+h-v*2+v/2,y+h-v*2,topDis,r,g,b,shTop,true);
	glColor4f(r+shTop,g+shTop,b+shTop,strt);
	glVertex2d(x+v*6,y+h-v*2);
	glColor4f(r,g,b,0);
	glVertex2d(x+v*6,y+h-v*2+topDis);
	/**Bottom bottom**/
	for ( int i = 270; i <= 360; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+h-v);
	}
	for ( int i = 0; i <= 90; ++i ){
		float perc2 = shBot;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+w-s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+w-s,y+h-v);
	}
	glWideCurve(x+4*s,x+4*s,x+3*s,y+h-botDis,y+h-botDis+v/2,y+h-botDis+v/2,botDis,r,g,b,shBot-.1);
	glWideCurve(x+3*s,x+2*s,x+2*s,y+h-botDis+v/2,y+h-botDis+v/2,y+h-botDis,botDis,r,g,b,shBot-.1);
	for ( int i = 90; i <= 180; ++i ){
		float perc2 = shBot*cos(i*M_PI/(180))-shTop*sin(i*M_PI/(180));
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+s+s*cos(i*M_PI/(180)),y+h-v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+s,y+h-v);
	}
	for ( int i = 180; i <= 270; ++i ){
		float perc2 = shTop;
		glColor4f(r+perc2,g+perc2,b+perc2,strt);
		glVertex2d(x+s+s*cos(i*M_PI/(180)),y+v+v*sin(i*M_PI/(180)));
		glColor4f(r,g,b,0);
		glVertex2f(x+s,y+v);
	}
	glWideCurve(x+2*s,x+2*s,x+3*s,y,y+v/2,y+v/2,topDis,r,g,b,shTop,true);
	glWideCurve(x+3*s,x+4*s,x+4*s,y+v/2,y+v/2,y,topDis,r,g,b,shTop,true);
	glColor4f(r,g,b,0);
	glVertex2d(x+w-s,y+v);
	glColor4f(r+shTop,g+shTop,b+shTop,strt);
	glVertex2d(x+w-s,y);
	glEnd();
}
