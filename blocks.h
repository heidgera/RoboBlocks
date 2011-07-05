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

//************ type for different kinds of blocks *********
enum ofBlockType {
	OF_BLOCK_ON,OF_BLOCK_IN,OF_BLOCK_NUM
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
	bool grabbed,grabbedOn,placeHolder;
	int ddOpen;
	bool cond,numBlock, numHolder,ddSelected,deleteMe, bBase,bSeq;
	vector<block> numBlocks;
	vector<block> blocksIn;
	vector<block> blocksOn;
	ofFontContainer arialHeader;
	vector<string> part;
	unsigned char typ;
	vector<ofDropDown> ddGroup;
	int oH,titleDisp,xo;
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
	
	int heightOnlyOn(bool moving=false);
	
	int heightInside();
  
  double fullWidth();
	
	void updateIn();
	
	void updateOn();
	
	int blockIsOverNum(block & t);
	
	int onBlockOn(int x,int y);
	
	int onBlockIn(int x,int y);
	
	int inBlockIn(int x,int y);
	
	int inBlockOn(int x,int y);
	
	int onBlockNum(int _x,int _y);
	
	void updatePositions();
	
	void updatePositions(block & t);
	
	bool updateSize(int k=0);
	
	bool onInside(int x,int y,int dispx=0, int dispy=0);
	
	bool overNum(block & t);
	
	int blockIsInside(block t);
	
	int overHolderBelow(block t);
	
	int overHolderInside(block t);
	
	void addInside(int i, block t);
	
	void addInside(block & check, block grab, int i);
	
	void addOn(block t,int pos);
	
	void addBelow(block *,block *);
	
	void addIn(block t, int pos=0);
	
	bool below(int x,int y,int dispx=0, int dispy=0);
	
	bool ddPassingClick(int x, int y);
	
	bool clickInside(int x,int y);
	
	void motion(int x, int y);
	
	bool partnerWritten(map<string,bool> * printed);
	
	void newPrintOut(ofstream* k,ifstream * f,int t=0, map<string,bool> * printed=0);
	
	void printData(string part,ofstream* k,int t=0,map<string,bool> * printed=0, bool printIn=true);
	
	bool clickDown(int _x, int _y);
	
	bool clickUp();
	
	bool ddClickDown(int _x, int _y, bool & ddopen, bool inHand);
	
	vector<block> passBlocks(ofBlockType d,int start);
	
	int blockIsBelow(block t);
};

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
public:
	block base;
	clock_t dblClick;
	int lastBlock;
	bool grabbed,inHand,ddopen;
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