/*
 *  blocks_DrawingFuncs.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/9/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"
#include "blockDraw.h"

//-------------------------block Draw Functions ---------------

void block::drawShadow(){
	//-------- if a block is a bracket block, draw a big shadow
	
	ofSetColor(0, 0, 0,75);
	if (bConditional) {
		drawBigBlockShadow(x+5,y+5,w,h,h0);
	}
	//-------- if a block is a conditional statement block, draw the rounded block shadow
	else if(numBlock) {
		if(bGrabbed) roundedShadow(x+5,y+5,w,h,h/2,.4);
	}
	//-------- if it's a normal block, draw a regular block shadow
	else {
		drawBlockShadow(x+5,y+5,w,h);
	}
	//-------- draw shadows for blocks on
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawShadow();
	}
	//-------- draw shadows for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawShadow();
	}
}

void block::drawSelected(){
	//-------- if a block is a bracket block, draw a big shadow
	
	ofSetColor(0xF4ED47);
	if (bConditional) {
		drawBigBlockShadow(x-5,y-5,w+10,h+10,h0+10);
	}
	//-------- if a block is a conditional statement block, draw the rounded block shadow
	else if(numBlock) {
		if(bGrabbed) roundedShadow(x-5,y-5,w+10,h+10,(h+10)/2,.4);
	}
	//-------- if it's a normal block, draw a regular block shadow
	else {
		drawBlockShadow(x-5,y-5,w+10,h+10);
	}
	//-------- draw shadows for blocks on
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawSelected();
	}
	//-------- draw shadows for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawSelected();
	}
}

void block::drawDD(double vert){
	//-------- draw the dropdowns for blocks beneath
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawDD(vert);
	}
	
	//-------- draw the dropdowns for blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawDD(vert);
	}
	
	//-------- draw the dd for the current block, if it is not open
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		if(!ddGroup[i].open){
			ddGroup[i].move(x+ddGroup[i].xdis,y+10*vert);
			ddGroup[i].changeSize(ddGroup[i].w, (ddGroup[i].arial.stringHeight("1")+4)*vert);
			if(h>=h0*.90) ddGroup[i].draw();
			else if(vert>.01) ddGroup[i].drawShape();
		}
	}
}

void block::drawOpenDD(){
	//-------- draw dds for below and in
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].drawOpenDD();
	}
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].drawOpenDD();
	}
	
	//-------- draw open dd for current block
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		if(ddGroup[i].open){
			ddGroup[i].move(x+ddGroup[i].xdis,y+10);
			if(h>=h0*.90) ddGroup[i].draw();
		}
	}
}

void block::draw(int _x, int _y)
{
	x=_x;
	y=_y;
	draw();
}

void block::draw(bool fade){
	//******** TODO: this really should be updated to matrix transformations of the shapes
	//-------- as it stands we divide the height by the original height, to get a scaling factor
	double vert=(h/h0>0.1) ? h/h0 : 0;
	if(vert>1)vert=1;
	//-------- if h/h0>.01 then
	if(vert){
		//-------- we set the color of the block and draw the shape depending on type
		ofSetColor(color);
		if (bConditional) drawBigBlock(x,y,w,h,h0,w0);
		else if(numBlock) ofRoundBox(x,y,w,h,h/4*vert);
		else if(bBase) drawBaseBlock(x, y, w, h);
		else drawBlock(x,y,w,h);
		
    if(!bBase){
      //-------- This part draws the text on the shape, and is already functioning with matrix transforms
      glColor3f(1,1,1);
      ofPushMatrix();
      
      //-------- TODO: remove magic numbers, replace with named variable in class definition
      //-------- if the height of block is greater than the original height, adjust the draw pos of the text
      int Ho=(((h>35&&!bBase)?35:h)-(arialHeader.stringHeight("Kjg")))/2+arialHeader.stringHeight("K");
      ofTranslate(x+titleDisp,y+Ho*vert,0);
      ofScale(1, vert, 1);
      if(vert) arialHeader.drawString(title,0,0);
      ofPopMatrix();
    }
	}
	//-------- for each of the statement positions,draw a placeholder or the block in that position
	for (unsigned int i=0; i<numBlocks.size(); i++) {
		if(numBlocks[i].placeHolder){
			ofSetColor(255, 255, 255);
			int Ho=((h>35)?35:h)-numBlocks[i].h;
			ofRoundBox(x+5+numBlocks[i].xo,y+Ho/2*vert,numBlocks[i].w,numBlocks[i].h*vert,(20/4)*vert);
		}
		else {
			numBlocks[i].draw();
		}
	}
	//-------- Draw the blocks below
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].draw();
	}
	//-------- draw the blocks inside
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].draw(bSeq||fade);
	}
	
	//-------- draw all of the drop downs
	drawDD(vert);
	
	drawOpenDD();
	if(fade){
		ofSetColor(255, 255, 255,75);
		if (bConditional) {
			drawBigBlockShadow(x,y,w,h,h0);
		}
		//-------- if a block is a conditional statement block, draw the rounded block shadow
		else if(numBlock) {
			if(bGrabbed) roundedShadow(x,y,w,h,h/2,.4);
		}
		//-------- if it's a normal block, draw a regular block shadow
		else {
			drawBlockShadow(x,y,w,h);
		}
	}
}


//---------------Group Draw Functions ------------------

void indicate(dropBlock & db)
{
  ofSetColor(255, 255, 255);
  if(db.index){
    block & t=(*db.inThis)[db.index-1];
    ofRect(t.x+5, t.y+t.h, t.w-10, 10);
  }
  else {
    block & t=*(db.belowThis);
    if(db.whichVector==OF_BLOCK_IN)
      ofRect(t.x+t.xIn0+5, t.y+t.yIn0, t.w-t.xIn0-10, 10);
    else
      ofRect(t.x+5, t.y+t.h, t.w-10, 10);
  }
}

void bGroup::drawIndicators(block & grab, block & k)
{
  dropBlock db=underWhich(k, grab);
  if(db.found()){
    indicate(db);
  }
  db=underWhich(grab, k);
  if(db.found()&&db.index==db.inThis->size()){
    indicate(db);
  }
}

void bGroup::drawBase(int _x, int _y)
{
	//base.drawSelected();
  y=_y;
	base.draw(_x,_y);
}

void bGroup::draw(){
	//-------- if the blocks are not grabbed, and the dds are not open, draw the blocks
	for (unsigned int i=0; i<size(); i++) {
		if (!blocks[i].bGrabbed&&!blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
}

void bGroup::drawForeground(){
	//******** if a block is currently being held, draw it on different layers, depending on Circumstances
	//-------- draw all the shadows
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].bGrabbed) {
			blocks[i].drawShadow();
		}
	}
	//-------- draw all the grabbed blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- draw ungrabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].numBlock&&!blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	//-------- draw grabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].numBlock&&blocks[i].bGrabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- Not deprecated, displays blocks when dd clicked.
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
  
  if(held.bGrabbed){
    held.drawShadow();
    held.draw();
  }
  
  if(inHand&&held.bGrabbed){
    drawIndicators(held, base);
    for (unsigned int i=0; i<size(); i++) {
      drawIndicators(held,blocks[i]);
    }
  }
  
  
}