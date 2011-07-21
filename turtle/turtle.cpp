/*
 *  turtle.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/19/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "turtle.h"

extern int pixPerInch;

vector<wall> Walls;

vector<wall> & walls(){
  return Walls;
}

void addWall(int x, int y, int w, int h)
{
  Walls.push_back(wall(x,y,w,h));
}

wall::wall(int _x, int _y, int _w, int _h){
  setup(_x, _y, _w, _h);
}

void wall::setup(int _x, int _y, int _w, int _h){
  pos.x=_x,pos.y=_y,w=_w, h=_h;
}

bool wall::inside(ofPoint p)
{
  return p.x>pos.x&&p.x<pos.x+w&&p.y>pos.y&&p.y<pos.y+h;
}

ofTurtle::ofTurtle()
{
  start.x=start.y=0;
  pos.x=pos.y=0;
  w=h=0;
}

void ofTurtle::setup(int _x, int _y, int _w, int _h)
{
  numLineStored=pixPerInch*16;
  start.x=pos.x=_x,start.y=pos.y=_y,w=_w,h=_h;
  whlHgt=h/2;
  whlWid=w/8;
  bearing=ofVector(0,-1);
}

void ofTurtle::move(int pixels)
{
  pos=pos+bearing.unit()*pixels;
  if(!bTurnSinceRecord&&lines.size()>1) lines.pop_front();
  else bTurnSinceRecord=false;
  lines.push_front(pos+bearing.unit()*pixels);
  if(lines.size()>numLineStored) lines.pop_back();
}

void ofTurtle::turn(int degrees)
{
  bTurnSinceRecord=true;
  bearing.rotate(degrees);
}

bool ofTurtle::front(int pixels, vector<wall> & walls)
{
  bool ret=false;
  for(unsigned int i=0; i<walls.size(); i++){
    if(walls[i].inside(pos+bearing.unit()*pixels)) ret=true;
  }
  return ret;
}

bool ofTurtle::frontIsClear(int pixels, ofImage & walls)
{
  bool ret=false;
  unsigned char * k=walls.getPixels();
  ofPoint ps=pos+bearing.unit()*pixels;
  int wid=walls.width;
  int hgt=walls.height;
  if(ps.x>0&&ps.x<wid&&ps.y>0&&ps.y<hgt){
    ret=true;
    if(k[int(ps.y)*wid*3+int(ps.x)*3+1]>200){
      ret=false;
    }
  }
  return ret;
}

ofPoint ofTurtle::pointAlongBearing(int pix)
{
   return pos+bearing.unit()*pix;
}

void ofTurtle::draw(int _x, int _y)
{
  int body=w-whlWid*2;
  int leng=h;
  for (unsigned int i=0; i<lines.size()&&lines.size()>1; i++) {
    ofSetLineWidth(3);
    ofLine(lines[i].x, lines[i].y, lines[i+1].x, lines[i+1].y);
  }
  ofPushMatrix();
  ofTranslate(pos.x, pos.y, 0);
  ofRotate(360-bearing.absoluteAngle());
  //
  //ofTranslate(-pos.x, -pos.y, 0);
  //
  ofSetColor(192,192,192,128);
  ofRect(-body/2, -whlHgt/2, body, leng);
  ofNoFill();
  ofSetColor(192,192,192);
  ofRect(-body/2, -whlHgt/2, body, leng);
  ofFill();
  ofSetColor(96,96,96);
  ofRect(-(whlWid+body/2), -whlHgt/2, whlWid, whlHgt);
  ofRect(body/2, -whlHgt/2, whlWid, whlHgt);
  ofRect(-whlWid/2, leng-whlHgt, whlWid, whlHgt);
  ofSetColor(255, 0, 0);
  ofTriangle(0, -(whlHgt/2+whlHgt/3), -whlHgt/3, -(whlHgt/2-whlHgt/3), whlHgt/3, -(whlHgt/2-whlHgt/3));
  ofPopMatrix();
  
  /*if(front(4*pixPerInch,walls())) ofSetColor(255, 0, 0);
  else ofSetColor(0, 255, 0);
  ofPoint ps = pos+bearing.unit()*4*pixPerInch;
  ofCircle(ps.x, ps.y, 5);*/
}
