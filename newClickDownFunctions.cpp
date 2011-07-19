/*
 *  newClickDownFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/5/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"

bool block::newClickInside(int _x, int _y)
{
  bool ret=false;
  if(bConditional&&_x>x+xIn0&&_x<x+fullWidth()-xIn0&&_y>y+yIn0&&_y<y+yIn0+h) ret=true;
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

bool block::newClickDD(int _x, int _y, bool & ddopen)
{
  bool ret=0;
  for (unsigned int j=0; j<ddGroup.size(); j++) {
		//-------- if we are over a any drop down with no drop downs open, or we are over the only open dd,
		if(((ddGroup[j].over(_x,_y)&&!ddopen)||ddGroup[j].open)){
			//-------- we check if we are over the dropdown
			if(ddopen=ddGroup[j].clickDown(_x,_y)){
				//-------- if we are, we set ddOpen to the dropdown number
				ddOpen=j+1;
			}
			else if(!ddopen) ddOpen=0; // if we are not over the dropdown, set ddOpen to false;
			ret=ddSelected=ret||ddGroup[j].selected; //update the selected bool.
		}
	}
  return ret;
}

//******************************* bGroup ************************************

bool bGroup::newClickDown(int _x, int _y)
{
  bool ret=false;
  for (unsigned int i=0; i<blocks.size()&&!ret; i++) {
    if(!ret) ret=newHandleClick(blocks,i,_x,_y,true);
  }
  for (unsigned int i=0; i<base.blocksOn.size()&&!ret; i++) {
    if(!ret) ret=newHandleClick(base.blocksOn,i,_x,_y);
  }
  return ret;
}

bool bGroup::newHandleClick(vector<block> & chk, int i, int _x, int _y, bool top)
{
  bool ret=false;
  if(!chk[i].ddPassingClick(_x,_y)){
    if(chk[i].newClickDD(_x,_y,ddopen))
      ret=true;
  }
  else if(!ret&&chk[i].newClickDown(_x,_y)&&!ddopen){
    inHand=ret=true;
    dispx = chk[i].x-_x;
    dispy = chk[i].y-_y;
    if(!top) pullBlocks(chk, i);
    else held = chk[i],chk.erase(chk.begin()+i);
  }
  else {
    for (unsigned int j=0; j<chk[i].blocksIn.size()&&!ret; j++) {
      if(!ret) ret=newHandleClick(chk[i].blocksIn,j,_x,_y,false);
    }
    for (unsigned int j=0; j<chk[i].blocksOn.size()&&!ret; j++) {
      if(!ret)ret=newHandleClick(chk[i].blocksOn,j,_x,_y,false);
    }
  }
  return ret;
}

void bGroup::pullBlocks(vector<block> & chk, int i)
{
  held=chk[i];
  held.blocksOn.assign(chk.begin()+(i+1),chk.end());
  chk.erase(chk.begin()+i,chk.end());
}