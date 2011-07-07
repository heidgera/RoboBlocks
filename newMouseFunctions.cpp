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

void block::newUpdateHeight(block * held)
{
  
}

void block::newUpdatePositions(block * held)
{
  
}

int block::newBelow(block & t)
{
  int ret=0;
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
  ofSetColor(255,255,255,64);
  if(t.inBounds(x, midLine, w, cH))
    ret=true,ofSetColor(255,255,255,128);
  ofRect(x, midLine, w, cH);
  return ret;
}

int block::newInside(block & drop)
{
  int ret=0;
  if(bConditional){
    int inLine=y+yIn0/2;
    int bottomSpace=h0-(yIn0+hIn0);
    int inH=yIn0/2+h-(yIn0+bottomSpace);
    ofSetColor(0,0,0,64);
    if(drop.inBounds(x+xIn0, inLine, w-xIn0, inH))
      ret=true,ofSetColor(0,0,0,128);
    ofRect(x+xIn0, inLine, w-xIn0, inH);
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

bool block::newClickUp(int _x, int _y)
{
  for (unsigned int j=0; j<ddGroup.size(); j++) {
    ddGroup[j].selected=false;
  }
  bGrabbed=ddSelected=false;
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].newClickUp(_x,_y);
  }
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].newClickUp(_x,_y);
  }
}

//******************************** Individual functions **********************

bool searchUnderBlock(dropBlock & foundBlock,block & strt, block & drpd, ofBlockType t)
{
  bool tmp=0;
  vector<block> bV;
  
  //------- select which vector to examine
  if(t==OF_BLOCK_IN) bV=strt.blocksIn;
  else bV=strt.blocksOn;
  
  //------- iterate through the vector
  int spc=0;
  if(bV.size()) spc=((bV[0].bConditional)?bV[0].yIn0/2:bV[0].h/2);
  else spc=((strt.bConditional)?strt.yIn0/2:strt.h/2);

  if(!strt.bConditional){
    if (strt.beneath(drpd,spc)) {
      tmp=true;
      foundBlock.set(strt,t,0);
    }
  }
  else {
    if (drpd.inBounds(strt.x+strt.xIn0,strt.y+strt.yIn0/2, strt.w-strt.xIn0, strt.yIn0/2+spc)) {
      ofSetColor(255, 0, 0,128);
      ofRect(strt.x+strt.xIn0,strt.y+strt.yIn0/2, strt.w-strt.xIn0, strt.yIn0/2+spc);
      tmp=true;
      foundBlock.set(strt,t,0);
    }
  }
    
  
  for (unsigned int i=0; i<bV.size()&&!tmp; i++) {
    block * nxt=0;
    spc=bV[i].h/2;
    if(bV[i].bConditional) spc=bV[i].yIn0/2;
    //------- find the halfway point on the next block
    if(i<bV.size()-1) nxt=&(bV[i+1]);
    if(nxt) spc=((nxt->bConditional)?nxt->yIn0/2:nxt->h/2);
    if (bV[i].beneath(drpd,spc)&&!bV[i].newInside(drpd)) {
      tmp=true;
      foundBlock.set(strt,t,i+1);
    }
    else if(bV[i].newInside(drpd)){
      tmp=searchUnderBlock(foundBlock,bV[i],drpd,OF_BLOCK_IN);
    }
  }
  return tmp;
}

dropBlock underWhich(block & strt, block & drpd)
{
  dropBlock ret;
  bool tmp=0;
  if(strt.newInside(drpd)){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_IN);
  }
  else if(!tmp&&strt.newBelow(drpd)){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_ON);
  }
  return ret;
}

//******************************** bGroup ************************************

bool bGroup::newClickUp(int _x, int _y)
{
  for (unsigned int i=0; i<blocks.size(); i++) {
    blocks[i].newClickUp(x,y);
  }
  base.newClickUp(x, y);
  bool ret=0;
  if(held.bGrabbed){
    held.clickUp();
    if(!(held.x<x||held.x>x+w||held.y<y||held.y>y+h)){
      if(processBlockDrop(held, base)) ret=true;
      for (unsigned int i=0; i<blocks.size()&&!ret; i++){
        ret=processBlockDrop(held, blocks[i]);
      }
      if(!ret) pushBlocks(held, blocks, blocks.size(),true);
    }
  }
  inHand=false;
  return ret;
}

bool bGroup::processBlockDrop(block & drop,block & t)
{
  bool ret=0;
  dropBlock fnd;
  fnd=underWhich(t, drop);
  if(fnd.found()){
    ret=true;
    pushBlocks(drop, *fnd.inThis, fnd.index);
  }
  return ret;
}

bool bGroup::pushBlocks(block & dropped, vector<block> & into, int i, bool top)
{
  dropped.bGrabbed=false;
  if(top) into.insert(into.begin()+i,dropped);
  else{
    vector<block> k=dropped.blocksOn;
    dropped.blocksOn.clear();
    into.insert(into.begin()+i,dropped);
    into.insert(into.begin()+i+1,k.begin(),k.end());
  }
}

bool bGroup::newHandleClickUp(block & grab, block & chk)
{

}
