/*
 *  newMouseFunctions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/1/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */


#include "blocks.h"

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
  vector<block> * blck;
  
  //------- select which vector to examine
  if(t==OF_BLOCK_IN) blck=&strt.blocksIn;
  else blck=&strt.blocksOn;
  
  vector<block> & bV=*blck;
  
  //------- iterate through the vector
  int spc=0;
  if(bV.size()) spc=((bV[0].bConditional)?bV[0].yIn0/2:bV[0].h/2);
  else spc=((strt.bConditional)?strt.yIn0/2:strt.h/2);

  if(!foundBlock.found()){
    if(!strt.bConditional||t==OF_BLOCK_ON){
      if (strt.beneath(drpd,spc)) {
        tmp=true;
        foundBlock.set(strt,t,0);
      }
    }
    else {
      if (drpd.inBounds(strt.x+strt.xIn0,strt.y+strt.yIn0/2, strt.w-strt.xIn0, strt.yIn0/2+spc)) {
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

void resetInsertSpace(block & t){
  t.insertSpace=0;
  for (unsigned int i=0; i<t.blocksIn.size(); i++) {
    resetInsertSpace(t.blocksIn[i]);
  }
  for (unsigned int i=0; i<t.blocksOn.size(); i++) {
    resetInsertSpace(t.blocksOn[i]);
  }
}

//******************************** bGroup ************************************

void bGroup::drag(double _x, double _y){
	//-------- if we're holding a block, update the position
	if(inHand){
    if(held.bGrabbed){
      held.move(_x+dispx, _y+dispy);
      held.newUpdatePositions();
      /*for (unsigned int i=0; i<blocks.size(); i++) {
        dropBlock db=underWhich(blocks[i], held);
        if(db.found()){
          if((db.index)&&(db.index-1)<db.inThis->size()){
            db.inThis->at(db.index-1).insertSpace=held.newHeightOn()+held.h;
            //cout << db.inThis->at(db.index-1).insertSpace << endl;
            for (unsigned int i=0; i<db.inThis->size(); i++) {
              if(i!=db.index-1) resetInsertSpace(db.inThis->at(i));
            }
          }
          else if((db.index==0)){
            db.belowThis->insertSpace=held.newHeightOn();
            for (unsigned int i=0; i<db.inThis->size(); i++) {
              resetInsertSpace(db.inThis->at(i));
            }
          }
        }
        else resetInsertSpace(blocks[i]);
      }*/
    }
	}
}

bool bGroup::newClickUp(int _x, int _y)
{
  for (unsigned int i=0; i<blocks.size(); i++) {
    blocks[i].newClickUp(x,y);
    resetInsertSpace(blocks[i]);
  }
  base.newClickUp(x, y);
  bool ret=0;
  if(held.bGrabbed){
    held.newClickUp(_x,_y);
    if(!(held.x<x||held.x>x+w||held.y<y||held.y>y+h)){
      if(processBlockDrop(held, base)) ret=true;
      for (unsigned int i=0; i<blocks.size()&&!ret; i++){
        ret=processBlockDrop(held, blocks[i]);
        if(!ret && (ret=processBlockDrop(blocks[i],held,true)))
          blocks.erase(blocks.begin()+i),i--;
      }
      if(!ret) pushBlocks(held, blocks, blocks.size(),true);
    }
    recordState();
  }
  bGrabbed=inHand=false;
  return ret;
}

bool bGroup::processBlockDrop(block & drop,block & target,bool reverse)
{
  bool ret=0;
  dropBlock fnd;
  fnd=underWhich(target, drop);
  if(fnd.found()){
    if(!reverse){
      ret=true;
      pushBlocks(drop, *fnd.inThis, fnd.index);
    }
    else if(fnd.index==fnd.inThis->size()){
      pushBlocks(drop, *fnd.inThis, fnd.index);
      int pos=blocks.size();
      pushBlocks(target, blocks, pos,true);
      ret=true;
    }
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
    if(k.size()) into.insert(into.begin()+i+1,k.begin(),k.end());
  }
}

