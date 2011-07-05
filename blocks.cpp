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
	
	//-------- color initialization
	if(cur.getAttribute("color").length())
		color=ofColor(strtol(cur.getAttribute("color").c_str(),NULL,0));
	else color=col;
	
	//-------- load name from the name of the xmlNode
	title=cur.getAttribute("name");
	//cout << title << endl;
	
	//-------- init some variables, to prevent garbage from happening
	cond=0;
	numBlock=ddOpen=deleteMe=false;
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
	list["ifblock"]=4;
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
          cond=atoi(node[1].c_str());
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
        case 4: //ifblock
          //-- sets the flag for being a numHolder
          //-- deprecated, i think, by %n in the title
          numHolder=true;
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
				ddGroup[ddNum].xdis+=totalwidth+spSize;
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
	double newWid=totalwidth+10;
	if(!numBlock) w=max(w,newWid);
	else {
		w=newWid-10;
	}

	for(int i=0; i<ddGroup.size(); i++)
		ddGroup[i].changeSize(ddGroup[i].w, (ddGroup[i].arial.stringHeight("1")+4));

	oH=h;
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
	arialHeader=t.arialHeader;
	oH=t.oH;
	numBlock=t.numBlock;
	typ=t.typ;
	cond=t.cond;
	grabbed=t.grabbed;
	title =t.title;
	filename=t.filename;
	numBlocks=t.numBlocks;
	titleDisp=t.titleDisp;
	ddGroup=t.ddGroup;
	blocksOn=t.blocksOn;
	blocksIn=t.blocksIn;
	ddOpen=t.ddOpen;
	deleteMe=t.deleteMe;
	numHolder=t.numHolder;
	placeHolder=t.placeHolder;
	color=t.color;
	bBase=t.bBase;
	bSeq=t.bSeq;
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
	w=_w, h=oH=_h;
	arialHeader.loadFont("DinC.ttf");
	arialHeader.setSize(20);
	//title="to program, connect blocks here";
	//title="connect blocks here and press button";
  title="";
	cond=0;
	w=max(w,double(arialHeader.stringWidth(title)+20));
	numBlock=ddOpen=deleteMe=false;
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

//-------------------------- Updating Functions --------------------------------------------------------

/*****************************************************************
 * updatePositions() :: function of block
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
 *    NONE :
 *
 */

void block::updatePositions(){
	if(blocksIn.size()) blocksIn[0].move(x+20,y+40),blocksIn[0].updatePositions();
	for (unsigned int i=1; i<blocksIn.size()&&blocksIn.size()>1; i++) {
		blocksIn[i].move(blocksIn[i-1].x,blocksIn[i-1].y+blocksIn[i-1].h-5);
		blocksIn[i].updatePositions();
	}
	if(blocksOn.size()) blocksOn[0].move(x,y+h-5),blocksOn[0].updatePositions();
	for (unsigned int i=1; i<blocksOn.size()&&blocksOn.size()>1; i++) {
		blocksOn[i].move(blocksOn[i-1].x,blocksOn[i-1].y+blocksOn[i-1].h-5);
		blocksOn[i].updatePositions();
	}
	for (unsigned int i=0; i<numBlocks.size(); i++) {
			numBlocks[i].move(x+numBlocks[i].xo+1,y+(40-numBlocks[i].h)/2);
	}
}

/*****************************************************************
 * updatePositions(block & t) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    block & t :
 *
 *  Output________
 *
 *    NONE :
 *
 */

void block::updatePositions(block & t){
	
	if(blocksIn.size()&&!bSeq&&blockIsInside(t)==1&&!blocksIn[0].onInside(t.x, t.y)) blocksIn[0].move(x+20,y+40+t.h+t.heightOnlyOn()),blocksIn[0].updatePositions(t);
	else if(blocksIn.size()) blocksIn[0].move(x+20,y+40),blocksIn[0].updatePositions(t);
	for (unsigned int i=1; i<blocksIn.size()&&blocksIn.size()>1; i++) {
		if(blockIsInside(t)-2==i-1&&!bSeq&&!blocksIn[i-1].onInside(t.x, t.y)&&!blocksIn[i].cond) 
			blocksIn[i].move(blocksIn[i-1].x,blocksIn[i-1].y+blocksIn[i-1].h+t.h+t.heightOnlyOn()-5);
		else blocksIn[i].move(blocksIn[i-1].x,blocksIn[i-1].y+blocksIn[i-1].h-5);
		blocksIn[i].updatePositions(t);
	}
	if(blocksOn.size()&&!bSeq&&blockIsBelow(t)==1) blocksOn[0].move(x,y+h-5),blocksOn[0].updatePositions(t);
	else if(blocksOn.size()) blocksOn[0].move(x,y+h-5),blocksOn[0].updatePositions(t);
	for (unsigned int i=1; i<blocksOn.size()&&blocksOn.size()>1; i++) {
		if(blocksOn[i-1].blockIsBelow(t)==1&&!bSeq&&!blocksOn[i-1].onInside(t.x, t.y)&&!blocksOn[i].cond) {
			blocksOn[i].move(blocksOn[i-1].x,blocksOn[i-1].y+blocksOn[i-1].h+t.h+t.heightOnlyOn()-5);
		}
		else blocksOn[i].move(blocksOn[i-1].x,blocksOn[i-1].y+blocksOn[i-1].h-5);
		blocksOn[i].updatePositions(t);
	}
	for (unsigned int i=0; i<numBlocks.size(); i++) {
			numBlocks[i].move(x+numBlocks[i].xo+1,y+(40-numBlocks[i].h)/2);
	}
}

/*****************************************************************
 * heightOnlyOn(bool moving) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    bool moving :
 *
 *  Output________
 *
 *    int :
 *
 */

int block::heightOnlyOn(bool moving){
	int ret=0;
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].cond&&!moving) blocksOn[i].updateSize();
		ret+=blocksOn[i].h-5;
	}
	return ret;
}

/*****************************************************************
 * heightInside() :: function of block
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
 *    int :
 *
 */

int block::heightInside(){
	int ret=0;
	if(blocksIn.size()&&cond)
		for (unsigned int i=0; i<blocksIn.size(); i++) {
			ret+=blocksIn[i].h-5;
		}
	else if (cond&&blocksIn.size()==0) {
		ret=oH-65;
	}
	return ret;
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

/*****************************************************************
 * updateSize(int k) :: function of block
 *
 *  Description::
 *
 *
 *  Input_________
 *
 *    int k :
 *
 *  Output________
 *
 *    bool :
 *
 */

bool block::updateSize(int k)
{
	bool ret=false;
	for (unsigned int i=0; i<blocksIn.size(); i++) {
		blocksIn[i].updateSize();
	}
	for (unsigned int i=0; i<blocksOn.size(); i++) {
		blocksOn[i].updateSize();
	}
	if(cond&&heightInside()+65!=h) h=heightInside()+65,ret=true;
	return ret;
}


//---------------------------------- Block Connect Functions ----------------------

vector<block> block::passBlocks(ofBlockType d,int start)
{
	vector<block> ret;
	if(d==OF_BLOCK_ON){
		ret.assign(blocksOn.begin()+start,blocksOn.end());
		blocksOn.erase(blocksOn.begin()+start,blocksOn.end());
	}
	else if(d==OF_BLOCK_IN){
		ret.assign(blocksIn.begin()+start,blocksIn.end());
		blocksIn.erase(blocksIn.begin()+start,blocksIn.end());
	}
	return ret;
}

void block::addOn(block t,int pos=0){
	vector<block> k=t.passBlocks(OF_BLOCK_ON, 0);
	blocksOn.insert(blocksOn.begin()+pos,block(t));
	blocksOn.insert(blocksOn.begin()+pos+1,k.begin(),k.end());
	blocksOn[pos].grabbed=false;
}

bool block::below(int _x,int _y,int dispx, int dispy){
	return ((_x+dispx>x&&_x+dispx<x+w)||\
			(_x+dispx+75>x&&_x+dispx+75<x+w))&& \
	(_y+dispy)>y+h-10&&(_y+dispy)<y+h+30;
}

int block::blockIsBelow(block t)
{
	int ret=0;
	if(below(t.x,t.y+5)) ret=1;
	for (int i=0; i<blocksOn.size(); i++) {
		if(blocksOn[i].below(t.x,t.y+5)) ret=i+2;
	}
	return ret;
}

bool block::onInside(int _x,int _y,int dispx, int dispy){
	return cond&&((_x>x&&_x<x+w)||((_x+75)>x&&(_x+75)<x+w))&&(_y>y+20&&_y<y+h-10);
}

bool block::clickInside(int _x,int _y){
	return cond&&((_x>x+20&&_x<x+w))&&(_y>y+40&&_y<y+h-20);
}

int block::blockIsInside(block t)
{
	int ret=0;
	if(!blocksIn.size()) 
		if(onInside(t.x,t.y+5)) ret=1;
	for (int i=0; i<blocksIn.size(); i++) {
		if(onInside(t.x,t.y+5)&&t.y+5<blocksIn[i].y+blocksIn[i].h){
			ret=i+1;
			break;
		}
		if(onInside(t.x,t.y+5)&&blocksIn[i].below(t.x,t.y+5)){
			ret=i+2;
			break;
		}
	}
	return ret;
}

bool block::overNum(block & t)
{
	return (placeHolder&&((t.x>x&&t.x<x+w)||((t.x+t.w/2)>x&&(t.x+t.w/2)<x+w))&&((t.y>y&&t.y<y+h)||(t.y+t.h/2>y&&t.y+t.h/2<y+h)));
}

int block::blockIsOverNum(block & t){
	int ret=0;
	for (unsigned int i=0; i<numBlocks.size(); i++) {
		if (numBlocks[i].overNum(t)) {
			ret=i+1;
		}
	}
	return ret;
}

void block::addInside(int i, block t)
{
	if(i<blocksIn.size()){
		int pos=0;
		if(blocksIn[i].cond&&(pos=blocksIn[i].blockIsInside(t))){
			blocksIn[i].addInside(pos-1,t);
		}
		else {
			addIn(t, i);
		}
	}
	else addIn(t, i);
}

void block::addInside(block & check, block grab, int i)
{
	int pos=0;
	if(check.cond&&(pos=check.blockIsInside(grab))){
		check.addInside(check.blocksIn[pos-1],grab,pos-1);
	}
	else {
		addIn(grab, i);
	}
}


void block::addIn(block t,int pos){
	vector<block> k=t.passBlocks(OF_BLOCK_ON, 0);
	blocksIn.insert(blocksIn.begin()+pos,block(t));
	blocksIn.insert(blocksIn.begin()+pos+1,k.begin(),k.end());
	blocksIn[pos].grabbed=false;
}



/****************************************************************
 *********************BlockGroup*********************************/

bGroup::bGroup(double _x, double _y,double wid,double hgt):ofInterObj(_x,_y,wid,hgt){
	//blocks.reserve(100);
	x=_x,y=_y,w=wid,h=hgt;
	used[""]=false;
}

bGroup::bGroup():ofInterObj(){
	grabbed=inHand=ddopen=0;
}

bGroup::~bGroup(){
	blocks.clear();
}

//------------------------ Utilities -------------------

void bGroup::setup(double _x, double _y,double wid,double hgt){
	//blocks.reserve(100);
	grabbed=inHand=ddopen=false;
	cSetup(_x,_y,wid,hgt);
	used[""]=false;
  base.setup(530, 90);
	base.blocksOn.reserve(100);
	states.recordState(storageState(blocks,base));
	rBot.loadImage("images/rBot.png");
	rTop.loadImage("images/rTop.png");
	rSide.loadImage("images/rSide.png");
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

int bGroup::add(vector<block> t, int j){
	int cur=blocks.size();
	blocks.push_back(t[j]);
	if(t.size()>j+1) blocks[cur].blocksOn.assign(t.begin()+(j+1),t.end());
	t.erase(t.begin()+j,t.end());
	return cur;
}

void bGroup::addFromSB(block t,int _x,int _y){
	if(t.over(_x,_y)||t.onBlockOn(_x, _y)&&!inHand){
		int numBlocks=size();
		if(numBlocks<99){
			blocks.push_back(t);
			used[t.title]=false;
			blocks[numBlocks].grabbed=grabbed=inHand=true;
			dispx = blocks[numBlocks].x-_x;
			dispy = blocks[numBlocks].y-_y;
		}
		else {
			printf("Remove some blocks, doncha know.");
		}
	}
}

void bGroup::addFromClick(block & t, int _x, int _y)
{
	for (unsigned int j=0; j<t.blocksIn.size(); j++) {
		if (!inHand&&!ddopen&&!t.ddSelected){
			int blk=0;
			if(blk=t.blocksIn[j].onBlockIn(_x, _y)){
				addFromClick(t.blocksIn[j].blocksIn[blk-1], _x, _y);
			}
			else if(t.blocksIn[j].clickDown(_x,_y)) {
				int cur=add(t.passBlocks(OF_BLOCK_IN,j));
				inHand=true;
				dispx = blocks[cur].x-_x;
				dispy = blocks[cur].y-_y;
			}
		}
	}
	for (unsigned int j=0; j<t.blocksOn.size(); j++) {
		if (!inHand&&!ddopen&&!t.ddSelected){
			int blk=0;
			if(blk=t.blocksOn[j].onBlockIn(_x, _y)){
				addFromClick(t.blocksOn[j].blocksIn[blk-1], _x, _y);
			}
		}
	}
}



int bGroup::heightUpdate(block & grab, block & comp)
{
	int ret=0;
	for (unsigned int i=0; i<comp.numInside(); i++) {
		heightUpdate(grab, comp.blocksIn[i]);
	}
	for (unsigned int i=0; i<comp.size(); i++) {
		heightUpdate(grab, comp.blocksOn[i]);
	}
	if(comp.blockIsInside(grab)&&!comp.bSeq&&!grab.numBlock){
		if(!comp.inBlockIn(grab.x,grab.y)) comp.h=grab.heightOnlyOn(true)+comp.heightInside()+grab.h+65-((!comp.numInside())?40:0);
		else comp.h=comp.heightInside()+65-((!comp.numInside())?40:0);
		comp.updatePositions(grab);
	}
	return ret;
}

void bGroup::update()
{
	for (unsigned int i=0; i<blocks.size(); i++) {
		blocks[i].updateSize();
		blocks[i].updatePositions();
	}
	base.updateSize();
	base.updatePositions();
	
	for (unsigned int i=0; i<blocks.size(); i++) {
		if(inHand&&blocks[i].grabbed){
			for (unsigned int j=0; j<blocks.size(); j++) {
				if(i!=j) heightUpdate(blocks[i], blocks[j]);
				if(blocks[i].onInside(blocks[j].x,blocks[j].y)&&!blocks[j].numBlock&&!blocks[i].inBlockIn(blocks[j].x, blocks[j].y)){
					blocks[i].h=blocks[j].heightOnlyOn(true)+blocks[i].heightInside()+blocks[j].h+65-((!blocks[i].numInside())?40:0);
					blocks[i].updatePositions(blocks[i]);
				}
			}
			heightUpdate(blocks[i], base);
		}
	}
}



#endif