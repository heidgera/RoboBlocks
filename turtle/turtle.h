/*
 *  turtle.h
 *  robotBlocks
 *
 *  Created by Exhibits on 7/19/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofExtended.h"

class wall {
public:
  ofPoint pos;
  int w,h;
  wall(int x, int y, int _w, int _h);
  void setup(int x, int y, int _w, int _h);
  bool inside(ofPoint p);
};

vector<wall> & walls();

void addWall(int x, int y, int w, int h);

class ofTurtle {
  int whlWid;
  int whlHgt;
  deque<ofPoint> lines;
  int numLineStored;
  bool bTurnSinceRecord;
public:
  ofPoint start,pos;
  int w,h;
  ofVector bearing;
  bool bMoving;
  ofTurtle();
  void setup(int _x, int _y, int _w, int _h);
  void move(int pixels);
  void turn(int degrees);
  void clear(){ lines.clear();}
  bool front(int pixels, vector<wall> & walls);
  bool sensorIsClear(ofPoint strtPnt,int pixels, ofImage & walls, int direction=0);
  bool frontIsClear(int pixels, ofImage & walls);
  bool frontLeftIsClear(int pixels, ofImage & walls);
  bool frontRightIsClear(int pixels, ofImage & walls);
  bool rightIsClear(int pixels, ofImage & walls);
  bool leftIsClear(int pixels, ofImage & walls);
  ofPoint pointAlongBearing(int pix);
  void draw(int _x, int _y);
};