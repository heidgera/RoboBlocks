/*
 *  blocks.h
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

/*
 *  RobotGUI.h
 *  
 *
 *  Created by Exhibits on 9/13/10.
 *  Copyright 2010 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _blocks_
#define _blocks_
#include "ofMain.h"
#include "ofExtended.h"
#include <fstream>
#include <algorithm>
#include <deque>
#include "turtle.h"



//************ type for different kinds of blocks *********
enum ofBlockType {
	OF_BLOCK_NULL,OF_BLOCK_ON,OF_BLOCK_IN,OF_BLOCK_NUM
};

enum ofBlockActions {
  OF_BLOCK_NULL_ACT,OF_BLOCK_MOVE, OF_BLOCK_TURN, OF_BLOCK_WHILE, OF_BLOCK_IF,OF_BLOCK_REPEAT
};

struct blockAction {
  ofBlockActions act;
  double data[2];
  double nDone;
  string dataStr;
  bool bExecuted;
  bool bData;
  bool bParsed;
  blockAction(){
    act=OF_BLOCK_NULL_ACT;
    data[0]=data[1]=0;
    bData=bParsed=bExecuted=false;
  }
  blockAction(ofBlockActions a, double d1, double d2=0){
    act=a,data[0]=d1,data[1]=d2;
  }
};

/*****************************************************************
 * class block: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *   ofColor color::_ _ _ _ _ _ _ _ 
 *   bool grabbed:: _ _ _ _ _ _ _ _ 
 *   bool grabbedOn:: _ _ _ _ _ _ _ 
 *   bool placeHolder:: _ _ _ _ _ _ 
 *   int ddOpen:: _ _ _ _ _ _ _ _ _ 
 *   bool cond::_ _ _ _ _ _ _ _ _ _ 
 *   bool numBlock::_ _ _ _ _ _ _ _ 
 *   bool  numHolder::_ _ _ _ _ _ _ 
 *   bool ddSelected::_ _ _ _ _ _ _ 
 *   bool deleteMe::_ _ _ _ _ _ _ _ 
 *   bool  bBase::_ _ _ _ _ _ _ _ _ 
 *   bool bSeq::_ _ _ _ _ _ _ _ _ _ 
 *   vector<block> numBlocks::_ _ _ 
 *   vector<block> blocksIn:: _ _ _ 
 *   vector<block> blocksOn:: _ _ _ 
 *   ofFontContainer arialHeader::_ 
 *   vector<string> part::_ _ _ _ _ 
 *   unsigned char typ::_ _ _ _ _ _ 
 *   vector<ofDropDown> ddGroup:: _ 
 *   int oH:: _ _ _ _ _ _ _ _ _ _ _ 
 *   int titleDisp::_ _ _ _ _ _ _ _ 
 *   int xo:: _ _ _ _ _ _ _ _ _ _ _ 
 *   string filename::_ _ _ _ _ _ _ 
 *   string title:: _ _ _ _ _ _ _ _ 
 *   placeHolder=true:: _ _ _ _ _ _ 
 */

class block: public ofInterObj {
public:
	ofColor color;
	bool placeHolder;
	int ddOpen;
	bool cond,numBlock, numHolder,ddSelected, bBase,bSeq;
	vector<block> numBlocks;
	vector<block> blocksIn;
	vector<block> blocksOn;
	ofFontContainer arialHeader;
	vector<string> part;
	unsigned char typ;
	vector<ofDropDown> ddGroup;
	int titleDisp,xo;
  
  blockAction action;
  
  int h0,w0,xIn0,yIn0,hIn0;
  
	string filename;
	string title;
	
	block(ofTag & xml,ofColor col, int y);
	
	block():ofInterObj(){
		placeHolder=true;
	}
	
	block(const block &t);
	
	~block();
	
	void setup(double _w, double _h);
	
	void operator=(const block &t);
  
  //*********** action functions
  
  void registerAction(string str);
  
  double evalVar(string str);
  
  void parseAction();
  
  double parseNumber(string str);
  
  void executeAction();
  
  //*********** draw functions
	
	void drawShadow();
	
	void drawSelected();
	
	void drawDD(double vert);
	
	void drawOpenDD();
	
	void draw(bool fade=false);
	
	void draw(int _x, int _y);
	
	int size(int j=0);
	
	int numInside();
	
	block * topBlock();
	
	block & lastBlock();
	
	block & operator[](int i);
  
  double fullWidth();
	
	int onBlockOn(int x,int y);
	
	bool ddPassingClick(int x, int y);
	
	void motion(int x, int y);
	
	bool partnerWritten(map<string,bool> * printed);
	
	void printOut(ofstream* k,ifstream * f,int t=0, map<string,bool> * printed=0);
	
	void printData(string part,ofstream* k,int t=0,map<string,bool> * printed=0, bool printIn=true);
	
	int blockIsBelow(block t);
  
  //--------------------------- New functions --------------------------
  
  bool bGrabbed, bConditional, null;
  
  int insertSpace;
  
  int newHeightOn();
  
  int newHeightIn();
  
  int effectiveHeight();
  
  int newUpdateHeight();
  
  void newUpdatePositions();
  
  bool newBelow(block & t);
  
  bool beneath(block & blockToCheck, int spaceBelow=0);
  
  bool newInside(block & t);
  
  bool inBounds(int xX, int yX, int wX, int hX);
  
  bool newClickInside(int _x, int _y);
  
  bool newClickDD(int _x, int _y, bool & ddopen);
  
  bool newClickDown(int x, int y);
  
  bool newClickUp(int _x, int _y);
  
  block separateBlock(ofBlockType bType, int index);
  
};
                 
struct dropBlock {
  int index;
  bool bFound;
  block * belowThis;
  vector<block> * inThis;
  ofBlockType whichVector;
  dropBlock(){ inThis=0,index=bFound=belowThis=0,whichVector=OF_BLOCK_NULL; }
  dropBlock(block & thisBlock, ofBlockType type, int atThisPos){ set(thisBlock,type,atThisPos); }
  void set(block & thisBlock, ofBlockType type, int atThisPos){
    bFound=true;
    index=atThisPos;
    belowThis=&thisBlock;
    whichVector=type;
    if(type==OF_BLOCK_ON) inThis=&(thisBlock.blocksOn);
    else if(type==OF_BLOCK_IN) inThis=&(thisBlock.blocksIn);
  }
  bool found() { return bFound; }
};

dropBlock underWhich(block & startBlock, block & dropped);

/*****************************************************************
 * struct storageState 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    vector<block> blocks:: _ _ _ _ 
 *    block base:: _ _ _ _ _ _ _ _ _ 
 */

struct storageState {
  vector<block> blocks;
  block base;
  storageState(){}
  storageState(vector<block> & k, block b){
    blocks=k;
    base=b;
  }
};

/*****************************************************************
 * class bGroup: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    vector<block> blocks:: _ _ _ _ _ _ 
 *    map<string,bool> used::_ _ _ _ _ _
 *    deque<storageState> storedState::_
 *    int stateCount:: _ _ _ _ _ _ _ _ _
 *    ofImage rTop:: _ _ _ _ _ _ _ _ _ _
 *    ofImage rBot:: _ _ _ _ _ _ _ _ _ _
 *    ofImage rSide::_ _ _ _ _ _ _ _ _ _
 *    public: block base:: _ _ _ _ _ _ _
 *    clock_t dblClick:: _ _ _ _ _ _ _ _
 *    int lastBlock::_ _ _ _ _ _ _ _ _ _
 *    bool grabbed,inHand,ddopen:: _ _ _
 *    double dispx, dispy::_ _ _ _ _ _ _
 *    double placex, placey::_ _ _ _ _ _
 */

class bGroup: public ofInterObj {
	vector<block> blocks;
	map<string,bool> used;
	//deque<storageState> storedState;
	//int stateCount;
  stateSave<storageState> states;
	ofImage rTop;
	ofImage	rBot;
	ofImage rSide;
  
  ofTimer actionTime;
  bool bSequencePlay;
  bool bTesting;
public:
  ofImage mapp;
  ofTurtle turtle;
  
	block base;
	clock_t dblClick;
	int lastBlock;
	bool bGrabbed,inHand,ddopen;
	double dispx, dispy;
	double placex, placey;
	bGroup(double x, double y,double wid,double hgt);
	
	bGroup();
	
	~bGroup();
	
	void recordState();
	
	void undoState();
	
	void redoState();
	
	bool undoAvailable();
	
	bool redoAvailable();
	
	void setup(double x, double y,double wid,double hgt);
	
	int size();
	
	block operator[](int i);
  
  
  
  void parseActions();
  
  void startTesting();
  
  void stopTesting();
  
  void startSequence();
  
  bool idleSequence(block * search);
  
  bool checkAgainstImage();
  
  void pauseSequence();
  
  bool isTesting();
  
  block * currentTest;
  
  void drawCurrentBlock();
  
  bool interpretDataStr(string str);
  
  block * nextActionBlock(block & b,bool skip=false);
  
  
	
	void addFromSB(block t,int x,int y);
	
	void writeFile(ofstream *k);
  
  void writeFile(string filname);
	
	void resetUsed(block & t);
	
	void resetUsed();
	
	void draw();
	
	void drawBase(int _x, int _y);
	
	void drawIndicators(block & grab,block & k);
	
	void update();
	
	int heightUpdate(block& grab, block & comp);
	
	int add(vector<block> t, int j=0);
	
	void addNum(block & holder, block & held);
	
	void addFromClick(block & t, int x, int y);
	
	void blockDown(block & t,int x,int y);
	
	void doubleClick(double x, double y, void func());
	
	void clickDown(double x, double y);
	
	void clear();
	
	bool onCond(int x,int y,block t);
	
	bool aboveCond(int x,int y,block * t, block * grabbedBlock);
	
	//bool isInsideBlockBelow(block &t);
	
	int onIf(int x,int y,block t);
	
	void drawForeground();
	
	void drag(double x, double y);
	
	void motion(double x, double y);
	
	void deleteBlock(block * delBlk);
	
	block * deleteLast(block * del);
	
	void deleteGroup(block * del);
	
	int handleClickUps(block & first, block & second, bool checkOn=true);
	
	void clickUp(double x, double y);
	
	int grabbedBlockBelow(int i, int j);
	
	int above(int i, int j);
  
  //--------------------- New functions ------------------------
  
  block held;
  
  bool newClickDown(int _x, int _y);
  
  bool newClickUp(int _x, int _y);
  
  bool newHandleClickUp(block & grab, block & chk);
  
  bool newHandleClick(vector<block> & chk, int indx, int _x, int _y, bool top=false);
  
  bool processBlockDrop(block & drop,block & target, bool reverse=false);
  
  bool pushBlocks(block & dropped, vector<block> & into, int i, bool top=false);
  
  void pullBlocks(vector<block> & chk, int i);
  
};

/*****************************************************************
 * class sideBar: public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    public: ofFont arialLabel::_ _ 
 *    ofFont arialHeader:: _ _ _ _ _ 
 *    bool Open::_ _ _ _ _ _ _ _ _ _ 
 *    float r,g,b::_ _ _ _ _ _ _ _ _ 
 *    ofColor color::_ _ _ _ _ _ _ _ 
 *    int xo,yo::_ _ _ _ _ _ _ _ _ _ 
 *    //bool grabbed:: _ _ _ _ _ _ _ 
 *    string filename::_ _ _ _ _ _ _ 
 *    vector<block> blocks:: _ _ _ _ 
 *    double space:: _ _ _ _ _ _ _ _ 
 *    //int textNum::_ _ _ _ _ _ _ _ 
 *    return blocks[i]:: _ _ _ _ _ _ 
 */

class sideBar: public ofInterObj {
public:
	ofFont arialLabel;
	ofFont arialHeader;
	bool Open;
	float r,g,b;
	ofColor color;
	int xo,yo;
	//bool grabbed;
	string filename;
	vector<block> blocks;
	double space;
	//int textNum;
	sideBar();
	sideBar(int x,int y,int w,int h,string file="",ofColor col=ofColor(255,255,255));
	~sideBar();
	void update(double spc);
	
	int size(){ return blocks.size();}
	
	block & operator[](int i){ return blocks[i];}
	
	double updateSize();
	
	void operator=(const sideBar t);
	
	void draw();
	
	void draw(int _x, int _y);
	
	//void draw(double k,double space);
};

/*****************************************************************
 * class sbGroup : public ofInterObj 
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    bGroup * dest::_ _ _ _ _ _ _ _
 *    int sideBarSpace:: _ _ _ _ _ _ 
 *    public: vector<sideBar> bars:: 
 *    return bars[i]:: _ _ _ _ _ _ _ 
 *    friend class bGroup::_ _ _ _ _ 
 */


class sbGroup : public ofInterObj {
	bGroup * dest;
	int sideBarSpace;
public:
	vector<sideBar> bars;
	void updateBlocks(int i);
	
	sbGroup(ofXML & xml,bGroup * destin=0);
	
	~sbGroup(){
		bars.clear();
	}
	
	int size(){ return bars.size();}
	
	sideBar & operator[](int i){ return bars[i];}
	
	void setDest(bGroup * destin);
	
	void unfold();
	
	void update();
	
	void updateHeight();
	
	void draw();
	
	void draw(int _x, int _y);
	
	bool clickDown(double x, double y);
	
	//int size();
	
	friend class bGroup;
};

#endif