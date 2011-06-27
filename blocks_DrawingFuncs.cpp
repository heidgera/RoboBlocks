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
	if (cond) {
		drawBigBlockShadow(x+5,y+5,w,h,oH);
	}
	//-------- if a block is a conditional statement block, draw the rounded block shadow
	else if(numBlock) {
		if(grabbed) roundedShadow(x+5,y+5,w,h,h/2,.4);
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
	if (cond) {
		drawBigBlockShadow(x-5,y-5,w+10,h+10,oH+10);
	}
	//-------- if a block is a conditional statement block, draw the rounded block shadow
	else if(numBlock) {
		if(grabbed) roundedShadow(x-5,y-5,w+10,h+10,(h+10)/2,.4);
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
			if(h>=oH*.90) ddGroup[i].draw();
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
			if(h>=oH*.90) ddGroup[i].draw();
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
	double vert=(h/oH>0.1) ? h/oH : 0;
	if(vert>1)vert=1;
	//-------- if h/oH>.01 then
	if(vert){
		//-------- we set the color of the block and draw the shape depending on type
		ofSetColor(color);
		if (cond) drawBigBlock(x,y,w,h,oH);
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
		if (cond) {
			drawBigBlockShadow(x,y,w,h,oH);
		}
		//-------- if a block is a conditional statement block, draw the rounded block shadow
		else if(numBlock) {
			if(grabbed) roundedShadow(x,y,w,h,h/2,.4);
		}
		//-------- if it's a normal block, draw a regular block shadow
		else {
			drawBlockShadow(x,y,w,h);
		}
	}
}


//---------------Group Draw Functions ------------------

void bGroup::drawIndicators(block & grab, block & k)
{
	//-------- Display the linking indicators when the blocks are hovering, drawn is to keep track if an indicator has been drawn
	bool bDrawn=false;
	int num=0;
	ofSetColor(0x00ccff);
	
	//-------- if the grabbed block is immediately below the block being checked against (k), draw the white indicator
	if((k.blockIsBelow(grab)==1)&&!grab.numBlock&&!k.numBlock){
		ofRect(k.x, k.y+k.h, grab.w, 10);
		bDrawn=true;
	}
	//-------- if the grabbed block is immediately inside of k, draw the indicator inside the block
	else if(k.blockIsInside(grab)==1&&!k.bSeq&&!k.inBlockIn(grab.x, grab.y)&&!grab.numBlock&&!k.numBlock){
		ofRect(k.x+25,k.y+45,grab.w,10);
		bDrawn=true;
	}
	//-------- if we're holding a statement block, and over a holder, change color of indicator
	else if(num=k.blockIsOverNum(grab)){
		ofSetColor(128, 128, 200);
		ofRoundBox(k.x+5+k.numBlocks[num-1].xo,k.y+7,k.numBlocks[num-1].w,k.numBlocks[num-1].h,(20/4)); //TODO: real number for y pos
	}
	
	//-------- if we have not drawn an indicator, check blocks in and on
	if(!bDrawn){
		for (unsigned int i=0; i<k.numInside(); i++) {
			if(!k.bSeq) drawIndicators(grab, k.blocksIn[i]);
		}
		for (unsigned int i=0; i<k.size(); i++) {
			drawIndicators(grab, k.blocksOn[i]);
		}
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
		if (!blocks[i].grabbed&&!blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
	//-------- if there is a block which is grabbed, draw some indicators
}

void bGroup::drawForeground(){
	//******** if a block is currently being held, draw it on different layers, depending on Circumstances
	//-------- draw all the shadows
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].grabbed) {
			blocks[i].drawShadow();
		}
	}
	//-------- draw all the grabbed blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].grabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- draw ungrabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].numBlock&&!blocks[i].grabbed) {
			blocks[i].draw();
		}
	}
	//-------- draw grabbed statement blocks
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].numBlock&&blocks[i].grabbed) {
			blocks[i].draw();
		}
	}
	
	//-------- Not deprecated, displays blocks when dd clicked.
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].ddOpen) {
			blocks[i].draw();
		}
	}
  for (unsigned int i=0; i<size(); i++) {
		if(inHand&&blocks[i].grabbed){
			for (unsigned int j=0; j<blocks.size(); j++){
				drawIndicators(blocks[i],blocks[j]);
				int pos=0;
				if((pos=blocks[i].blockIsBelow(blocks[j]))&&!blocks[i].numBlock){
					if(pos==1) drawBox(blocks[i].x,blocks[i].y+blocks[i].h,blocks[j].w,10,1,1,1);
					else if(pos-2<blocks[i].size()) drawBox(blocks[i].blocksOn[pos-2].x,blocks[i].blocksOn[pos-2].y\
                                                  +blocks[i].blocksOn[pos-2].h,blocks[j].w,10,1,1,1);
				}
			}
		}
	}
  for (unsigned int i=0; i<size(); i++) {
		if(inHand&&blocks[i].grabbed){
			drawIndicators(blocks[i], base);
		}
	}
}