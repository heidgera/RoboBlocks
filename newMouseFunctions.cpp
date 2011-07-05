/*
 *  newMouseFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blocks.h"

int block::newHeightOn()
{
  
}

int block::newHeightIn()
{
  int ret=0;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].newUpdateHeight();
  }
  
}

void block::newUpdateHeight(block * held)
{
  
}

void block::newUpdatePositions(block * held)
{
  
}

bool block::newBelow(block & t)
{
  
}

bool block::newInside(block & t)
{
  
}

bool block::newClickInside(int _x, int _y)
{
  bool ret=false;
  if(bConditional&&_x>x+xIn0&&_x<x+w-xIn0&&_y>y+yIn0&&_y<y+yIn0+hIn0) ret=true;
  return ret;
}

bool block::newClickDown(int _x, int _y)
{
  bool ret=false;
  if(over(_x,_y)&&!newClickInside(_x, _y)&&ddPassingClick(_x, _y)&&!ddSelected){
    ret=bGrabbed=true;
  }
  return ret;
}

block block::separateBlock(ofBlockType bType, int index)
{
  
}


//******************************* bGroup ************************************
  
bool bGroup::newClickDown(int _x, int _y)
{
  
}


