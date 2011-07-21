/*
 *  blocks.cpp
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"
#include "blockDraw.h"

/*
 *  RobotGUI.h
 *  
 *
 *  Created by Exhibits on 9/13/10.
 *  Copyright 2010 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _block_
#define _block_
#include <fstream>
#include <vector>
#include <algorithm>

int pixPerInch=25;

string defaultFont="ArialNarrow.ttf";

/*****************************************************************
 * block(ofTag & cur,ofColor col, int _y):ofInterObj(-200,-200,150,45) :: constructor for block, a subclass of ofInterObj
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    -200 :
 *    -200 :
 *    150 :
 *    45 :
 *
 *  Output________
 *
 *    New instance of block :
 *
 */

block::block(ofTag & cur,ofColor col, int _y):ofInterObj(-200,-200,150,45) {
	//********* This is the method by which all of the blocks are first generated from the xml files in the data root.
	//-------- TODO: get rid of the garbage with the color triples. blech.
	//-------- load the font for the arialHeader, at 10 pt.
	
	arialHeader.loadFont(defaultFont);
	arialHeader.setSize(14);
	insertSpace=0;
	//-------- color initialization
	if(cur.getAttribute("color").length())
		color=ofColor(strtol(cur.getAttribute("color").c_str(),NULL,0));
	else color=col;
	
	//-------- load name from the name of the xmlNode
	title=cur.getAttribute("name");
	//cout << title << endl;
	
	//-------- init some variables, to prevent garbage from happening
	bConditional=0;
	numBlock=ddOpen=false;
	titleDisp=10;
	y=_y;
	numHolder=false;
	placeHolder=false;
	bBase=false;
	bSeq=false;
	//-------- declare the vector for splitting the title string, and the map used for the switch
	vector<string> titleSplit;
	map<string,int> list;
	list["seq"]=0;
	list["cond"]=1;
	list["wid"]=2;
	list["hgt"]=3;
	list["action"]=4;
	list["file"]=5;
	list["part"]=6;
	list["num"]=7;
	list["dropdown"]=8;
	list["blocksIn"]=9;
  list["blocksOn"]=10;
	for(int i=0;i<cur.size();i++){
		string node[2]={cur[i].getLabel(),cur[i].getValue()};
		//-- node[0] is the label, node[1] is the value
    if(list.find(node[0])!=list.end()){
      switch (list.find(node[0])->second) {
        case 0: // indicates whether or not a block is a sequence of other blocks
          bSeq=true;
          break;
        case 1: //-- cond
          //-------- set cond=true, and change size
          bConditional=atoi(node[1].c_str());
          h=105;
          w=200;
          titleDisp=30;
          break;
        case 2: //wid, deprecated, handled with routines below switch
          w=atoi(node[1].c_str());
          break;
        case 3: // hgt, deprecated, same as above
          h=atoi(node[1].c_str());
          break;
        case 4: //actions
          registerAction(node[1]);
          break;
        case 5: // file
          //-- definitely not deprecated, used to store value of which file to write from
          filename=node[1];
          break;
        case 6: // part
          //-- stores the name of the complement blocks
          part.push_back(node[1]);
          break;
        case 7: // num
          //-- set the statement block flag
          numBlock=true;
          titleDisp=0;
          h=20;
          w=90;
          break;
        case 8: // dropdown
          //-- add a new dropdown menu to the block
          ddGroup.push_back(ofDropDown(cur[i]));
          break;
        case 9:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksIn.push_back(block(cur[i][j],color,0));
            }
          }
          break;
        case 10:
          for (unsigned int j=0; j<cur[i].size(); j++) {
            if (cur[i][j].getLabel()=="block") {
              blocksOn.push_back(block(cur[i][j],color,0));
            }
          }
          break;
        default:
          break;
      }
    }
	}
  h0=h;
  w0=w;
  if(bConditional){
    xIn0=20;
    yIn0=40;
    hIn0=45;
  }
  else xIn0=yIn0=hIn0=0;
	int ddNum=0;
	
	//-------- assign a default value to the xdis of each dd
	for (unsigned int i=0; i<ddGroup.size(); i++) {
		ddGroup[i].xdis=0;
	}
	
	//-------- change the font size if it is a statement block
	if(numBlock){
    arialHeader.setSize(7);
  }
	
	//-------- split the title into words by looking for " ", and establish a baseline for the width with "."
	titleSplit = ofSplitString(title, " ");
	int sp=1;
	int spSize=arialHeader.stringWidth(".");
	int totalwidth=titleDisp;
	
	//-------- set the displacement for each object in the title, statement blocks and dropdowns
	for (unsigned int i=0; i<titleSplit.size(); i++) {
		if(!titleSplit[i].compare("%d")){
			if(ddNum<ddGroup.size()){
				//-------- augment the orginal positon with the current total width
				ddGroup[ddNum].xdis+=totalwidth+spSize*1.5;
				//-------- update total width
				totalwidth+=ddGroup[ddNum].w+spSize*2;
				//-------- if you have two dropdowns in a row, make sure they don't overlap
				if(i==titleSplit.size()-1||(i==titleSplit.size()-2&&!titleSplit[i+1].compare("%d"))){
					ddGroup[ddNum].xdis+=spSize;
				}
				ddNum++;
			}
		}
		else if(!titleSplit[i].compare("%b")){
			//-------- if we find a statement block, init it as a placeholder
			numHolder=true;
			int cur=numBlocks.size();
			numBlocks.push_back(block());
			numBlocks[cur].x=totalwidth+titleDisp;
			numBlocks[cur].xo=numBlocks[cur].x;
			numBlocks[cur].w=50;
			numBlocks[cur].h=20;
			if(h<=5+numBlocks[cur].h) h=5+numBlocks[cur].h;
			totalwidth+=50+spSize;
		}
		else {
			totalwidth+=arialHeader.stringWidth(titleSplit[i]);
		}
	}
	
	title="";
	ddNum=0;
	for (unsigned int i=0; i<titleSplit.size(); i++) {
		if(!titleSplit[i].compare("%d")){
			if(ddNum<ddGroup.size()){
				sp=0;
				int origWid = arialHeader.stringWidth(title);
				while (arialHeader.stringWidth(title)+spSize*sp-origWid<ddGroup[ddNum].w) {
					sp++;
					title.append(" ");
				}
				ddNum++;
			}
		}
		else if(!titleSplit[i].compare("%b")){
			sp=0;
			int origWid = arialHeader.stringWidth(title);
			while (arialHeader.stringWidth(title)+spSize*sp-origWid<50) {
				sp++;
				title.append(" ");
			}
		}
		else {
			title.append(titleSplit[i].c_str());
			for (int k=0; k<sp; k++) {
				title.append(" ");
			}
		}
	}
	double newWid=totalwidth+titleDisp;
	if(!numBlock) w=max(newWid,w);
	else {
		w=newWid-10;
	}

	for(int i=0; i<ddGroup.size(); i++)
		ddGroup[i].changeSize(ddGroup[i].w, (ddGroup[i].arial.stringHeight("1")+4));

	h0=h;
}

/*****************************************************************
 * block(const block &t) :: constructor for block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const block &t :
 *
 *  Output________
 *
 *    New instance of block :
 *
 */

block::block(const block &t){
	*this=t;
}

/*****************************************************************
 * ~block() :: deconstructor for block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    New instance of block :
 *
 */

block::~block() {
	blocksOn.clear();
	blocksIn.clear();
	ddGroup.clear();
}

/*****************************************************************
 * operator=(const block &t) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    const block &t :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void block::operator=(const block &t) {
	x=t.x;
	xo=t.xo;
	y=t.y;
	w=t.w;
	h=t.h;
  w0=t.w0;
  h0=t.h0;
  yIn0=t.yIn0;
  xIn0=t.xIn0;
  hIn0=t.hIn0;
	arialHeader=t.arialHeader;
	numBlock=t.numBlock;
	bConditional=t.bConditional;
	bGrabbed=t.bGrabbed;
	title =t.title;
	filename=t.filename;
	numBlocks=t.numBlocks;
	titleDisp=t.titleDisp;
	ddGroup=t.ddGroup;
	blocksOn=t.blocksOn;
	blocksIn=t.blocksIn;
	ddOpen=t.ddOpen;
	numHolder=t.numHolder;
	placeHolder=t.placeHolder;
	color=t.color;
	bBase=t.bBase;
	bSeq=t.bSeq;
  insertSpace=t.insertSpace;
  action=t.action;
}

/*****************************************************************
 * setup(double _w, double _h) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    double _w :
 *     double _h :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void block::setup(double _w, double _h)
{
	w=_w, h=_h;
  w0=w;
  h0=h;
  hIn0=0;
  yIn0=0;
  xIn0=0;
	arialHeader.loadFont("DinC.ttf");
	arialHeader.setSize(20);
	//title="to program, connect blocks here";
	//title="connect blocks here and press button";
  title="";
	bGrabbed=bConditional=0;
	w=max(w,double(arialHeader.stringWidth(title)+20));
	numBlock=ddOpen=false;
	titleDisp=10;
	numHolder=false;
	color.set(0xdbb700);
	filename="null";
	placeHolder=false;
	bBase=true;
}

//--------------- Maybe unnecessary functions? --------------

int block::size(int j){
	return blocksOn.size();
}

int block::numInside(){
	return blocksIn.size();
}

block & block::lastBlock(){
	if (blocksOn.size()) return blocksOn[blocksOn.size()-1];
	else return *this;
}

block & block::operator[](int i){
	return blocksOn[i];
}


/*****************************************************************
 * fullWidth() :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    double :
 *
 */

double block::fullWidth()
{
  double ret=0;
  if(blocksIn.size()){
    ret+=blocksIn[0].x-x;
    double maximum=0;
    for (unsigned int i=0; i<blocksIn.size(); i++) {
      maximum=max(maximum,blocksIn[i].w);
    }
    ret+=maximum;
  }
  ret=max(w,ret);
  return ret;
}





/****************************************************************
 *********************BlockGroup*********************************/

bGroup::bGroup(double _x, double _y,double wid,double hgt):ofInterObj(_x,_y,wid,hgt){
	//blocks.reserve(100);
	x=_x,y=_y,w=wid,h=hgt;
	used[""]=false;
}

/*****************************************************************
 * bGroup():ofInterObj() :: constructor for bGroup, a subclass of ofInterObj()
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    NONE :
 *
 *  Output________
 *
 *    New instance of bGroup :
 *
 */

bGroup::bGroup():ofInterObj(){
	bGrabbed=inHand=ddopen=0;
}

bGroup::~bGroup(){
	blocks.clear();
}

//------------------------ Utilities -------------------

void bGroup::setup(double _x, double _y,double wid,double hgt){
	//blocks.reserve(100);
  bSequencePlay=bTesting=false;
	bGrabbed=inHand=ddopen=false;
	cSetup(_x,_y,wid,hgt);
	used[""]=false;
  base.setup(530, 90);
	base.blocksOn.reserve(100);
	states.recordState(storageState(blocks,base));
  held.setup(0,0);
  
  mapp.loadImage("maps/map_2.jpg");
  turtle.setup(2.5*(mapp.width/12.),mapp.width-1.5*(mapp.width/12.), 25*4.25,4.5*25);
  actionTime.set(0.01);
  pixPerInch=mapp.width/48;
  currentTest=0;
}

int bGroup::size(){
	return blocks.size();
}

void bGroup::clear(){
	base.blocksOn.clear();
	blocks.clear();
}

block bGroup::operator[](int i){
	return blocks[i];
}

void bGroup::recordState()
{
	states.recordState(storageState(blocks,base));
}

void bGroup::undoState()
{
	if(states.undoAvailable()){
    storageState * t;
    if((t=states.undoState())){
      blocks=t->blocks;
      base=t->base;
    }
  }
}

void bGroup::redoState()
{
	if(states.redoAvailable()){
    storageState * t;
    if((t=states.redoState())){
      blocks=t->blocks;
      base=t->base;
    }
  }
}

bool bGroup::undoAvailable()
{
	return states.undoAvailable();
}

bool bGroup::redoAvailable()
{
	return states.redoAvailable();
}


//----------------------- Add block Functions -------------------

void bGroup::addFromSB(block t,int _x,int _y){
	if(t.over(_x,_y)||t.onBlockOn(_x, _y)&&!inHand){
		int numBlocks=size();
		if(numBlocks<99){
			used[t.title]=false;
      held=t;
      held.bGrabbed=bGrabbed=inHand=true;
			dispx = held.x-_x;
			dispy = held.y-_y;
		}
		else {
			printf("Remove some blocks, doncha know.");
		}
	}
}

void bGroup::update()
{
  if(!bTesting){
    for (unsigned int i=0; i<blocks.size(); i++) {
      blocks[i].newUpdateHeight();
      blocks[i].newUpdatePositions();
    }
  }
  base.newUpdateHeight();
  base.newUpdatePositions();
	
  if(bTesting){
    bSequencePlay=idleSequence(&base);
  }
}



#endif