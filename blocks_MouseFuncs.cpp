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


bool block::ddPassingClick(int _x, int _y){
	//*********** this function checks whether or not a dd is open; if so, it returns false, and keeps the click
	bool ret=true;
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ret=ret&&ddGroup[i].passingClick(_x,_y);
	}
	return ret;
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



/*************************** Mouse Functions ***************************
 ***********************************************************************/



void bGroup::motion(double _x, double _y){
	//-------- if we are moving but not clicked, inform the blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		blocks[i].motion(x,y);
	}
}




