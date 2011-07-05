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

//******************************** bGroup ************************************

bool bGroup::newClickUp(int _x, int _y)
{
  
}

bool bGroup::newHandleClickUp(block & grab, block & chk)
{
  int ret=0;
	int pos=0;
	//-------- if the i-block is grabbed, and both the i- and j-blocks are not numBlocks check some more
	//-------- the i-block is the block being released, int this case
	if(grab.grabbed&&!grab.numBlock&&!comp.numBlock){
		
		//-------- if the block we are releasing is below the j- block, and it is not inside the j-block
		if ((pos=comp.blockIsBelow(grab))&&!comp.inBlockOn(grab.x,grab.y)) {
			//-------- add the i-block in the (pos-1) position below the j-block,
			//-------- set the last block to j, and set delete flag on i.
			comp.addOn(grab,pos-1);
			grab.deleteMe=true;
			//lastBlock=j;
			ret=2;
		}
		
		//-------- if the block we're releasing has multiple blocks beneath, and the last one is just above the j-block,
		else if ((pos=grab.blockIsBelow(comp))==grab.size()+1&&!comp.inBlockOn(grab.x,grab.y)&&checkOn) {
			//-------- add the j-block to the end of the stack of the i-block, and set the j-block delete flag
			grab.addOn(comp,pos-1);
			comp.deleteMe=true;
			ret=1;
		}
		
		//-------- if we release the block into a block on one of the main blocks
		else if ((pos=comp.inBlockOn(grab.x,grab.y))) {
			int inPos=0;
			if((inPos=comp.blocksOn[pos-1].blockIsInside(grab))&&!comp.blocksOn[pos-1].bSeq){
				//-------- add the block into the proper position in the bracket block
				comp.blocksOn[pos-1].addInside(inPos-1,grab);
				grab.deleteMe=true;
				ret=2;
			}
		}
		
		//-------- if the i-block is inside the j-block,
		else if(pos=comp.blockIsInside(grab)&&!comp.bSeq) {
			//-------- add it at the proper location, and set the delete flag
			comp.addInside(pos-1,grab);
			grab.deleteMe=true;
			ret=2;
		}
		
		//-------- if the j-block is inside the i-block when we release, add j into i, and flag for deletion
		else if(grab.onInside(comp.x,comp.y)&&!grab.bSeq&&checkOn){
			grab.addIn(comp);
			comp.deleteMe=true;
			ret=1;
		}
	}
	
	//******** TODO: expand this section to include guards for catching all problems, actually, it might do that.
	//-------- if the i-block is a numBlock add to block in or on
	else if(grab.grabbed&&grab.numBlock&&(pos=comp.overHolderBelow(grab))){
		if(pos==1) addNum(comp, grab);
		else if(pos>1) addNum(comp.blocksOn[pos-2], grab);
		ret=2;
	}
	else if(grab.grabbed&&grab.numBlock&&(pos=comp.overHolderInside(grab))){
		if(pos==1) addNum(comp, grab);
		else if(pos>1) addNum(comp.blocksIn[pos-2], grab);
		ret=2;
	}
}
