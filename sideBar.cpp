/*
 *  sideBar.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"

extern string defaultFont;

/****************************** Sidebar *******************************
 **********************************************************************/

/*****************************************************************
 * sideBar(int _x,int _y,int w,int h,string file,ofColor col):ofInterObj(_x,_y,w,h) :: constructor for sideBar, a subclass of ofInterObj(_x,_y,w,h)
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    _x :
 *    _y :
 *    w :
 *    h :
 *
 *  Output________
 *
 *    New instance of sideBar :
 *
 */

sideBar::sideBar(int _x,int _y,int w,int h,string file,ofColor col):ofInterObj(_x,_y,w,h){
	arialHeader.loadFont("DinC.ttf");
	arialHeader.setSize(20);
	bOver=bPressed=false;
	Open = false;
	xo=_x;
	yo=_y;
	color=col;
	filename=file;
}

/*****************************************************************
 * sideBar():ofInterObj() :: constructor for sideBar, a subclass of ofInterObj()
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    New instance of sideBar :
 *
 */

sideBar::sideBar():ofInterObj(){
	arialHeader.loadFont(defaultFont);
	arialHeader.setSize(20);
	bOver=bPressed=false;
	Open = false;
}

/*****************************************************************
 * ~sideBar() :: deconstructor for sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    New instance of sideBar :
 *
 */

sideBar::~sideBar() {
}

/*****************************************************************
 * operator=(const sideBar t) :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const sideBar t :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sideBar::operator=(const sideBar t) {
	x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
	xo=t.xo;
	yo=t.yo;
}

/*****************************************************************
 * draw() :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sideBar::draw(){
	if(Open){
		roundedShadow(x+20,y,w,h,h/2,.5);
		roundedBox(x,y,w,h,h/2,.5,.5,.52);
		roundedBox(x+(yo-y),y,w,h,h/2,r,g,b);
		ofSetColor(255, 255, 255);
		arialHeader.drawString(filename,x+w/4,y+3*h/4);
	}
	else {
		roundedBox(x,y,w,h,h/4,.5,.5,.52);
		roundedBox(x-200,y,w,h,h/4,r,g,b);
		ofSetColor(255, 255, 255);
		arialHeader.drawString(filename,x+w/4,y+3*h/4);
	}
}

/*****************************************************************
 * draw(int _x, int _y) :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sideBar::draw(int _x, int _y){
	x=_x, y=_y;
	ofRoundShadow(x+5,y,w,h,h/4, .5);
	ofSetColor(0x555555);
	ofRoundBox(x,y,w,h,h/4, .2);
	ofSetColor(color);
	if(!Open) ofRoundBox(x-(w-60),y,w,h,h/4, .2);
	else ofRoundBox(x,y,w,h,h/4, .2);
	glColor3f(1,1,1);
	arialHeader.drawString(filename,x+w/4,y+3*h/4);
	if(Open){
		int temp=y+h+20;
		for (unsigned int j=0; j<blocks.size(); j++) {
			blocks[j].draw(20,temp);
			temp+=blocks[j].h+blocks[j].heightOnlyOn()+20;
		}
	}
}

/*****************************************************************
 * updateSize() :: function of sideBar
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    double :
 *
 */

double sideBar::updateSize()
{
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].w>3*w/4) {
			w=max(w,4*blocks[i].w/3);
		}
	}
	return w;
}

/****************************** Sidebar Group *************************
 **********************************************************************/

/*****************************************************************
 * updateBlocks(int i) :: function of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int i :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::updateBlocks(int i){
	int divs=bars[i].blocks.size();
	for (unsigned int j=0; j<divs; j++) {
		int w=bars[i].blocks[j].w;
		bars[i].blocks[j].x=(bars[i].w-30-w)/2;
		bars[i].blocks[j].y=bars[i].y+5*bars[i].h/4+sideBarSpace*j*\
		((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace)/divs;
		bars[i].blocks[j].h=bars[i].blocks[j].oH*((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace);
	}
}

/*****************************************************************
 * sbGroup(ofXML & xml,bGroup * destin):ofInterObj() :: constructor for sbGroup, a subclass of ofInterObj()
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    New instance of sbGroup :
 *
 */


sbGroup::sbGroup(ofXML & xml,bGroup * destin):ofInterObj(){
	dest=destin;
  xml.setCurrentTag(";blocks");
	string font=xml.getCurrentTag().getAttribute("font");
	ofTag & tag=xml.getCurrentTag();
	for (unsigned int i=0; i<tag.size(); i++) {
		if (tag[i].getLabel()=="bar") {
			string col=tag[i].getAttribute("color");
			unsigned long colTrip=strtol(col.c_str(),NULL,0);
			ofColor color(colTrip);
			//cout << tag[i].getAttribute("name") + " " + tag[i].getLabel() <<endl;
			unsigned int curBar=bars.size();
			bars.push_back( sideBar(-20,50+40*i,260,40,tag[i].getAttribute("name"),color));
			for (unsigned int j=0; j<tag[i].size(); j++) {
				if (tag[i][j].getLabel()=="block") {
          int curBlock=bars[curBar].blocks.size();
					bars[curBar].blocks.push_back(block(tag[i][j],color,j*45));
          bars[curBar].w=max(bars[curBar].w,bars[curBar].blocks[curBlock].fullWidth());
          w=max(bars[curBar].w+40,w);
				}
			}
		}
	}
	bars.push_back( sideBar(-20,50+40*bars.size(),260,40,"Filler",ofColor(0,0,0)));
	if (bars.size()) {
		y=bars[0].y;
		x=0;
	}
  for (unsigned int i=0; i<bars.size(); i++) {
    bars[i].w=w;
  }
	updateHeight();
	if(bars.size()>=2) bars[0].Open=true;
	for (unsigned int i=0; i<bars.size(); i++) {
		//updateBlocks(i);
	}
}

/*****************************************************************
 * setDest(bGroup * destin) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    bGroup Desktop Documents Downloads Java Library Mail Movies Music Pictures Public Sites bin doc example gmon.out include libtool modkit-community-edition-read-only python tmp xstc destin :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::setDest(bGroup * destin){
	dest=destin;
}

/*****************************************************************
 * updateHeight() :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::updateHeight(){
	double maxWid=0;
	double maxHeight=0;
	for (unsigned int i=0; i<bars.size(); i++) {
		double hgt=20;
		for (unsigned int j=0; j<bars[i].size(); j++) {
			hgt+=bars[i][j].h+bars[i][j].heightOnlyOn()+20;
		}
		maxHeight=max(maxHeight,hgt);
		maxWid=max(maxWid,bars[i].updateSize());
	}
	sideBarSpace=maxHeight;
	h=sideBarSpace-y+40*(bars.size());
	w=max(w,maxWid);
	for (unsigned int i=0; i<bars.size(); i++) {
		bars[i].w=maxWid;
	}
}

/*****************************************************************
 * update() :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::update()
{
	for (unsigned int i=0; i<bars.size(); i++) {
		for (unsigned int j=0; j<bars[i].size(); j++) {
			if(bars[i][j].updateSize())
				updateHeight();
			bars[i][j].updatePositions();
		}
	}
}

/*****************************************************************
 * unfold() :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::unfold(){
	for (unsigned int i=0; i<bars.size()-1; i++) {
		int dir;
		if (bars[i].Open&&bars[i+1].y<bars[i+1].yo+sideBarSpace) {
			dir = 1;
		}
		else if(!bars[i].Open&&bars[i+1].y>bars[i].y+bars[i].h){
			dir = -1;}
		else {
			dir=0;
		}
		if(dir){
			while((bars[i].Open&&bars[i+1].y<bars[i+1].yo+sideBarSpace)||\
				  (!bars[i].Open&&bars[i+1].y>bars[i].y+bars[i].h))
				for (unsigned int j=0; j<bars.size(); j++) {
					if (j>i) {
						bars[j].y+=dir;
					}
				}
		}
	}
}

/*****************************************************************
 * draw(int _x, int _y) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int _x :
 *     int _y :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void sbGroup::draw(int _x, int _y)
{
	x=_x, y=_y;
	draw();
}

/*****************************************************************
 * draw() :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    NONE :
 *
 */


void sbGroup::draw(){
	
	//--------- Draw a brown box onto the sidebar to hold the blocks
	//ofSetColor(0x80633B);
	ofSetColor(0x777777);
	ofShadeBox(0,y,w-15-25,h,OF_DOWN,.1);
	
	int pos=0;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		ofSetColor(0, 0, 0,60);
		ofRect(bars[i].x+bars[i].h/2, bars[i].y+bars[i].h, bars[i].w-bars[i].h, bars[i+1].y-(bars[i].y+bars[i].h));
		shadeHor(0,bars[i].y+bars[i].h,5,bars[i+1].y-(bars[i].y+bars[i].h),.4);
		shadeHor(bars[i].w-bars[i].h,bars[i].y+bars[i].h,-5,bars[i+1].y-(bars[i].y+bars[i].h),.4);
		if(bars[i].Open) shadeVer(0,bars[i].y+bars[i].h,bars[i].w-bars[i].h,5,.4);
		if(bars[i+1].y>bars[i].y+bars[i].h) shadeVer(0,bars[i+1].y,bars[i].w-bars[i].h,-5,.4);
		bars[i].draw(-20,pos+y);
		pos+=bars[i].h;
		if(bars[i].Open) pos+=sideBarSpace;
	}
	bars[bars.size()-1].y=y+h;
}

/*****************************************************************
 * clickDown(double _x, double _y) :: member of sbGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    double _x :
 *     double _y :
 *
 *  Output________
 *
 *    bool :
 *
 */

bool sbGroup::clickDown(double _x, double _y){
	bool ret=false;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		if(bars[i].over(_x,_y)){ 
			for (unsigned int j=0; j<bars.size()-1; j++) {
				if (j!=i) {
					bars[j].Open=false;
				}
			}
			ret=bars[i].Open=true;
		}
		for (unsigned int j=0; j<bars[i].blocks.size(); j++) {
			if(dest&&bars[i].Open) dest->addFromSB(bars[i].blocks[j],_x,_y);
		}
	}
	return ret;
}
