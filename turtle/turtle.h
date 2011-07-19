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

class ofTurtle {
public:
  ofPoint start,pos;
  int w,h;
  ofVector bearing;
  bool bMoving;
  ofTurtle(){
    start.x=start.y=0;
    pos.x=pos.y=0;
    w=h=0;
  }
  void setup(int _x, int _y, int _w, int _h);
};