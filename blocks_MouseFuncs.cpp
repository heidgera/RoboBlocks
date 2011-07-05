/*
 *  blocks_MouseFuncs.cpp
 *  robotBlocksVectors
 *
 *  Created by Aaron Heidgerken on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 *	Comments updated 5/1/54
 *
 */

#include "blocks.h"

//------------------Mouse motion/Dragging ----------------------

void block::motion(int _x, int _y)
{
	//-------- check if the mouse is inside an open dropDown; if so, change highlighting
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ddGroup[i].motion(_x,_y);
	}
}


//****************************** Click Functions ************************

bool block::ddClickDown(int _x, int _y,bool & ddopen, bool inHand)
{
	//********* this function checks mouse click against contained dropdowns, only allowing one click per group
	bool ret=false;
	bool sel=false;
	
	//-------- check the dropdowns inside the topblock first
	for (unsigned int j=0; j<ddGroup.size(); j++) {
		//-------- if we are over a any drop down with no drop downs open, or we are over the only open dd,
		if(ret=((ddGroup[j].over(_x,_y)&&!ddopen)||ddGroup[j].open)&&!inHand){
			//-------- we check if we are over the dropdown
			if(ddopen=ddGroup[j].clickDown(_x,_y)){
				//-------- if we are, we set ddOpen to the dropdown number
				ddOpen=j+1;
			}
			else if(!ddopen) ddOpen=0; // if we are not over the dropdown, set ddOpen to false;
			sel=ddSelected=sel||ddGroup[j].selected; //update the selected bool.
			//ret=true;
		}
	}
	if(!bSeq){
		for (unsigned int i=0; i<blocksIn.size(); i++) {
			//-------- if we have not yet selected a dd, check the blocks inside
			if(!sel){
				blocksIn[i].ddClickDown(_x,_y,ddopen,inHand);
				sel=sel||blocksIn[i].ddSelected;
			}
		}
		for (unsigned int i=0; i<blocksOn.size(); i++) {
			//-------- if we still have not selected a dd, check the blocks on
			if(!sel){
				blocksOn[i].ddClickDown(_x,_y,ddopen,inHand);
				sel=sel||blocksOn[i].ddSelected;
			}
		}
	}
	return sel; //-------- return whether or not we have selected anything
}

bool block::ddPassingClick(int _x, int _y){
	//*********** this function checks whether or not a dd is open; if so, it returns false, and keeps the click
	bool ret=true;
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ret=ret&&ddGroup[i].passingClick(_x,_y);
	}
	return ret;
}

bool block::clickDown(int _x, int _y){
	//-------- checks to see if we are over the block, if we are over a guard position, if there are any dropdowns that
	//-------- are taking the click, if there is an open dropdown, and if we are inside of a conditional block
	//-------- if none of these are true except being over the block, then we grab the block
	grabbed=(over(_x,_y)&&!onBlockNum(_x, _y)&&ddPassingClick(_x,_y)&&!ddSelected&&(!cond||(cond&&clickInside(_x,_y)&&bSeq)||(cond&&!clickInside(_x,_y))));
	return grabbed;
}

bool block::clickUp(){
	//-------- click up on everything in the block that needs clicking up
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ddGroup[i].selected=false;
	}
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].clickUp();
	}
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].clickUp();
	}
	grabbed=ddSelected=false;
	return grabbed;
}

int block::onBlockOn(int _x,int _y){
	//-------- if we are over a block below, and not looking for a dd click, returns the block's number in the vector+1.
	//-------- else, returns 0 (false);
	int ret=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].over(_x,_y)&&blocksOn[i].ddPassingClick(_x,_y)) ret=i+1;
	}
	return ret;
}

int block::onBlockNum(int _x,int _y){
	//-------- if we are over a numBlock which is not a placeholder, return it's number+1, else 0;
	int ret=0;
	for (unsigned int i=0; i<numBlocks.size(); i++) {
		if(numBlocks[i].over(_x,_y)&&!numBlocks[i].placeHolder) ret=i+1;
	}
	return ret;
}

int block::onBlockIn(int _x,int _y){
	//-------- if we are over a block inside and not inside of of that block, return it's position+1, else 0;
	int ret=0;
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		if(blocksIn[i].over(_x,_y)&&!blocksIn[i].clickInside(_x,_y)) ret=i+1;
	}
	return ret;
}

int block::inBlockIn(int _x,int _y){
	//-------- if we are inside of a block inside, return it's pos+1, else 0;
	int ret=0;
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		if(blocksIn[i].onInside(_x,_y)) ret=i+1;
	}
	return ret;
}

int block::inBlockOn(int _x,int _y){
	//-------- if we are inside of a block below, return it's position+1, else 0;
	int ret=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].onInside(_x,_y)) ret=i+1;
	}
	return ret;
}


/*************************** Mouse Functions ***************************
 ***********************************************************************/

void bGroup::blockDown(block & t,int _x,int _y){
	//*********** clickdown function for blocks in the blockgroup
	int ret=0;
	int pos=0;
  if (!inHand&&!ddopen&&t.clickDown(_x,_y)) {
    inHand=true;
    
    //-------- and change the anchor point
    dispx = t.x-_x;
    dispy = t.y-_y;
  }
	else if(pos=t.onBlockOn(_x,_y)){
		if(t.blocksOn[pos-1].onBlockIn(_x, _y)&&!t.blocksOn[pos-1].bSeq) blockDown(t.blocksOn[pos-1], _x, _y);//addFromClick(t.blocksOn[pos-1], _x, _y);
		//-------- if we are on a block below
	
		else for (unsigned int j=0; j<t.blocksOn.size(); j++) {
			//-------- click the block we are over,
			if (!inHand&&!ddopen&&t.blocksOn[j].clickDown(_x,_y)) {
				//-------- add it and the blocks below it to the vector of blocks in bGroup,
				int cur=add(t.passBlocks(OF_BLOCK_ON,j));
				inHand=true;
				
				//-------- and change the anchor point for the mouse
				dispx = blocks[cur].x-_x;
				dispy = blocks[cur].y-_y;
			}
		}
	}
	//-------- if we are on a block inside, add the block inside to the main group
	else if(t.onBlockIn(_x,_y)&&!t.bSeq) addFromClick(t, _x, _y);
	else if(ret=t.onBlockNum(_x, _y)){
		//-------- if we are on a cond statement block, add it to the main group,
		//-------- and turn the previous one back to placeholder
		int cur=blocks.size();
		blocks.push_back(t.numBlocks[ret-1]);
		t.numBlocks[ret-1].w=50;
		t.numBlocks[ret-1].xo-=5;
		t.numBlocks[ret-1].placeHolder=true;
		used[blocks[cur].title]=false;
		blocks[cur].grabbed=grabbed=inHand=true;
		
		//-------- also, change the anchor point again
		dispx = blocks[cur].x-_x;
		dispy = blocks[cur].y-_y;
	}
	
	else {
		//-------- check all connected blocks
		for (unsigned int i=0; i<t.numInside(); i++) {
			if(!t.bSeq)
				blockDown(t.blocksIn[i], _x, _y);
		}
		for (unsigned int i=0; i<t.size(); i++) {
			blockDown(t.blocksOn[i], _x, _y);
		}
	}
}

void bGroup::doubleClick(double _x, double _y, void func()){
	
}

void bGroup::clickDown(double _x, double _y){
	for (int i=0; i<blocks.size(); i++) {
		//-------- if we are over a block, pick it up
		/*if (!inHand&&!ddopen&&blocks[i].clickDown(_x,_y)) {
			inHand=true;
			
			//-------- and change the anchor point
			dispx = blocks[i].x-_x;
			dispy = blocks[i].y-_y;
		}*/
		/*//-------- check to see if we are doing things with drop downs
		if(blocks[i].ddClickDown(_x,_y,ddopen,inHand)||blocks[i].ddSelected);
		
		//-------- finally, check to see if we are interacting with connected blocks
		else blockDown(blocks[i],_x,_y);*/
    if(blocks[i].ddClickDown(_x,_y,ddopen,inHand)||blocks[i].ddSelected);
    else blockDown(blocks[i],_x,_y);
	}
	if(base.ddClickDown(_x,_y,ddopen,inHand)||base.ddSelected);
	else blockDown(base, _x, _y);
	//
}

void bGroup::drag(double _x, double _y){
	//-------- if we're holding a block, update the position
	if(inHand){
		for (unsigned int i=0; i<blocks.size(); i++) {
			if (blocks[i].grabbed) {
				blocks[i].move(_x+dispx,_y+dispy);
				blocks[i].updatePositions();
			}
		}
	}
}

void bGroup::motion(double _x, double _y){
	//-------- if we are moving but not clicked, inform the blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		blocks[i].motion(x,y);
	}
}

int block::overHolderBelow(block t)
{
	//-------- check to see if a num block we're holding is over a holder below another block
	int ret=0;
	if (blockIsOverNum(t)) {
		ret=1;
	}
	else for (unsigned int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].blockIsOverNum(t)) ret=i+2;
	}
	return ret;
}

int block::overHolderInside(block t)
{
	//-------- check if numblock is over a holder inside another block
	int ret=0;
	if (blockIsOverNum(t)) {
		ret=1;
	}
	else for (unsigned int i=0; i<blocksIn.size(); i++) {
		if(blocksIn[i].blockIsOverNum(t)) ret=i+2;
	}
	return ret;
}


void bGroup::addNum(block & holder, block & held)
{
	//-------- check for releasing a num block over a holder
	int overWhich=0;
	if((overWhich=holder.blockIsOverNum(held))&&held.numBlock) {
		int prex=holder.numBlocks[overWhich-1].xo;
		int prey=holder.numBlocks[overWhich-1].y;
		holder.numBlocks[overWhich-1]=held;
		holder.numBlocks[overWhich-1].xo=prex+5;
		holder.numBlocks[overWhich-1].y=prey;
		held.deleteMe=true;
	}
}

/********************************************************************************************
 * handleClickUps(block & grab, block & comp, bool checkOn) :: function of bGroup
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    block & grab :
 *     block & comp :
 *     bool checkOn :
 *
 *  Output________
 *
 *    int :
 *
 */

int bGroup::handleClickUps(block & grab, block & comp, bool checkOn){
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

/*****************************************************************
 * clickUp(double _x, double _y) :: function of bGroup
 *
 *  Description:: this is one of the primary functions in this whole program
 *    it checks where we release each block, and tries to connect it where it should go
 *    it's pretty dense; let's try explaining it
 *
 *  Input_________
 *
 *    double _x :
 *    double _y :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void bGroup::clickUp(double _x, double _y){
	for(int i=0; i<blocks.size(); i++){
		//-------- if a block is grabbed, set the lastBlock parameter to it's position in the vector
		if(blocks[i].grabbed){
			lastBlock=i;
			handleClickUps(blocks[i], base,false);
		}
		for (unsigned int j=0; j<blocks.size(); j++) {
			//-------- if the i-block is not also the j-block, do some checking
			if (j!=i) {
				int ret=handleClickUps(blocks[i], blocks[j]);
				if (ret==1) lastBlock=i;
				else if(ret==2) lastBlock=j;
			}
		}
		
		//-------- if we dropped the block outside the checking area, flag for deleting
		if (blocks[i].grabbed&&(blocks[i].x<x-15||blocks[i].x>x+w||blocks[i].y<y||blocks[i].y>y+h)) {
			blocks[i].grabbed=inHand=ddopen=false;
			blocks[i].deleteMe=true;
		}
		
		//-------- if a block was grabbed, let it go and set the grabbed flag, for state saving
		if(blocks[i].grabbed){
			blocks[i].grabbed=inHand=false;
			grabbed=true;
		}
	}
	
	//-------- once we have checked all the states, go through and manage the delete flags and clickup everything
	for (unsigned int i=0; i<blocks.size(); i++) {
		blocks[i].clickUp();
		inHand=false;
		//-------- if the delete flag is set,
		if (blocks[i].deleteMe) {
			//-------- delete it from the block vector
			blocks.erase(blocks.begin()+i);
			//-------- decrement all the postion counters to account for deletion
			if (lastBlock>i) {
				lastBlock--;
			}
			else if(lastBlock==i)
				lastBlock=0;
			i--;
		}
	}
	//-------- if we just were grabbing a block, delete that shit
	if (grabbed) {
		grabbed=false;
		recordState();
	}
	base.clickUp();
}

/*****************************************************************
 * onIf(int _x,int _y,block t) :: function of bGroup
 *
 *  Description:: checks to see if you are over a block holder
 *
 *  Input_________
 *
 *    int _x :
 *    int _y :
 *    block t :
 *
 *  Output________
 *
 *    int :
 *
 */

int bGroup::onIf(int _x,int _y,block t){
	int ret=0;
	for (unsigned int i=0; i<t.numBlocks.size(); i++) {
		if((t.numHolder)&&_x>t.x+t.numBlocks[i].x&&_x<t.x+t.numBlocks[i].x+50&&_y>t.y+5&&_y<t.y+35&&t.numBlocks[i].placeHolder)
			ret=i+1;
	}
	
	return ret;
}