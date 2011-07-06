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
  int bottomSpace=h0-(yIn0+hIn0);
  int midLine=y+((bConditional)?yIn0+h-bottomSpace:h/2);
  int cH=h+y-midLine+newHeightOn()+((bConditional)?yIn0/2:h/2);
  if(t.inBounds(x, midLine, w, cH))
    ret=true;
  ofSetColor(255,255,255,128);
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
      ret=true,ofSetColor(0,0,0,196);
    ofRect(x+xIn0, inLine, w-xIn0, inH);
  }
  return ret;
}

bool block::beneath(block & chk, int blw)
{
  bool ret=0;
  int bottomSpace=h0-(yIn0+hIn0);
  int midLine=y+((bConditional)?yIn0+h-bottomSpace:h/2);
  int cH=h+y-midLine+newHeightOn()+blw;
  if(chk.inBounds(x, midLine, w, cH)) ret=true;
  
  return ret;
}
       
bool block::inBounds(int xX, int yX, int wX, int hX)
{
  bool ret=0;
  if(((x>xX && x<xX+wX)||(x+w>xX && x+w<xX+wX))&&((y>yX && y<yX+hX)||(y+h>yX && y+h<yX+hX))) ret=true;
  return ret;
}

//******************************** Individual functions **********************

bool searchUnderBlock(dropBlock & foundBlock,block & strt, block & drpd, ofBlockType t)
{
  bool tmp=0;
  vector<block> bV;
  if(t==OF_BLOCK_IN) bV=strt.blocksIn;
  else bV=strt.blocksOn;
  for (unsigned int i=0; i<bV.size()&&!tmp; i++) {
    block * nxt=0;
    int spc=0;
    if(i<bV.size()-1) nxt=&(bV[i+1]);
    if(nxt) spc=((nxt->bConditional)?nxt->yIn0/2:nxt->h/2);
    if (bV[i].beneath(drpd,spc)&&!bV[i].newInside(drpd)) {
      tmp=true;
      foundBlock.set(strt,t,i);
    }
    else if(bV[i].newInside(drpd)){
      cout << "oops" << endl;
      tmp=true;
      foundBlock=underWhich(bV[i],drpd);
    }
  }
}

dropBlock underWhich(block & strt, block & drpd)
{
  dropBlock ret;
  bool tmp=0;
  if(strt.newInside(drpd)){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_IN);
    if(!tmp) ret.set(strt,OF_BLOCK_IN,0);
  }
  if(strt.newBelow(drpd)){
    tmp=searchUnderBlock(ret,strt,drpd,OF_BLOCK_ON);
    if(!tmp) ret.set(strt,OF_BLOCK_ON,strt.blocksOn.size());
  }
  return ret;
}

//******************************** bGroup ************************************

bool bGroup::newClickUp(int _x, int _y)
{
  bool ret=0;
  if(held.bGrabbed){
    if(processBlockDrop(held, base)) ret=true;
    for (unsigned int i=0; i<blocks.size()&&!ret; i++){
      ret=processBlockDrop(held, blocks[i]);
    }
    if(!ret) pushBlocks(held, blocks, blocks.size(),true);
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
