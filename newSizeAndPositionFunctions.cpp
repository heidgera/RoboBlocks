/*
 *  newSizeAndPositionFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blocks.h"

int block::newHeightOn()
{
  int ret=0;
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newUpdateHeight();
    ret+=blocksOn[i].h;
  }
  return ret;
}

int block::newHeightIn()
{
  int ret=0;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].newUpdateHeight();
  }
  
}

int block::effectiveHeight()
{
  return h+insertSpace;
}

int block::newUpdateHeight()
{
  int ret=0;
  int topHeight=yIn0;
  int botHeight=h0-(yIn0+hIn0);
  if(bConditional){
    for (unsigned int i=0; i<blocksIn.size(); i++) {
      ret+=blocksIn[i].newUpdateHeight()-5;
    }
    ret+=5;
  }
  if(ret<hIn0) ret=hIn0; 
  h=ret+topHeight+botHeight;
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newUpdateHeight();
  }
  return h+insertSpace;
}

void alignBlocks(vector<block> & t)
{
  for (unsigned i=1; i<t.size(); i++) {
    t[i].move(t[i-1].x,t[i-1].y+t[i-1].h+t[i-1].insertSpace-5);
    t[i].newUpdatePositions();
  }
}

void block::newUpdatePositions()
{
  if(blocksIn.size()){
    blocksIn[0].move(x+xIn0,y+yIn0+insertSpace);
    blocksIn[0].newUpdatePositions();
  }
  alignBlocks(blocksIn);
  if(blocksOn.size()){
    blocksOn[0].move(x+xIn0,y+h+insertSpace-5);
    blocksOn[0].newUpdatePositions();
  }
  alignBlocks(blocksOn);
}

//********************* position boolean functions ********************

bool block::newBelow(block & t)
{
  bool ret=0;
  int midLine=y;
  int cH=h+y+newHeightOn();
  if(bConditional){
    int bottomSpace=h0-(yIn0+hIn0);
    midLine=y+h-bottomSpace;
    cH+=yIn0/2-midLine;
  }
  else {
    midLine=y+h/2;
    cH+=h/2-midLine;
  }
  if(t.inBounds(x, midLine, w, cH))
    ret=true;
  return ret;
}

bool block::newInside(block & drop)
{
  bool ret=0;
  if(bConditional){
    int inLine=y+yIn0/2;
    int bottomSpace=h0-(yIn0+hIn0);
    int inH=yIn0/2+h-(yIn0+bottomSpace);
    if(drop.inBounds(x+xIn0, inLine, w-xIn0, inH))
      ret=true;
  }
  return ret;
}

bool block::beneath(block & chk, int blw)
{
  bool ret=0;
  int bottomSpace=h0-(yIn0+hIn0);
  int midLine=y+h/2;
  
  if(bConditional) midLine=y+h-bottomSpace;
  int cH=h+y-midLine+blw;
  
  if(chk.inBounds(x, midLine, w, cH)) ret=true;  
  return ret;
}

bool block::inBounds(int xX, int yX, int wX, int hX)
{
  bool ret=0;
  int pH=h;
  if(bConditional) pH=yIn0;
  if(((x>xX && x<xX+wX)||(x+w>xX && x+w<xX+wX)||(x<=xX && x+w >= xX + wX))&&((y>yX && y<yX+hX)||(y+pH>yX && y+pH<yX+hX))) ret=true;
  return ret;
}
