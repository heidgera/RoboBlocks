#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofExtended.h"
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
#include "ofxSystemCall.h"
#endif
#include "ofxShapes.h"
#include "blocks.h"
#include "ofVMouse.h"
#include "blockLoad.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
  void drawBase_and_Demo(int yPosition);
  void drawSubtitleBar(int yPos, int hgt, int tabHgt);
  void drawTitleBar(int & height);
  void drawMenuBar(int & yPos, int & hgt);
  void drawSidebar(int sideWidth, int menuBarH);
  void drawAttract();
  
  //--------- Animation functions
  void animationStepRequested(ofXML & animXML);
  ofInterObj * searchForObject(ofTag & tag, int & _x, int & _y);
  ofInterObj * searchBlock(vector<string> spl, block & b, int offset);
  int animStep;
  ofXML animXML;

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	void upload();
	void loadBlocks(string dir);
  void loadBlocks(blockGroup & bg);
	
	bGroup blocks;
	sbGroup * objects;
	ofXML parse;
  

	ofImage background;
	ofImage sideBarBack;
	
	ofButton upBut;
	ofButton clearBut;
	ofButton redoBut;
	ofButton undoBut;
	ofButton diyBut;
	ofButton homeBut;
	
	ofButton skipBut;
	
	ofTimer timeOut;
	ofFont	arial;
  ofFont  label;
	ofSystemCall systemCall;
	ofProgressSpinner spinner;
	
	ofVideoPlayer crabMovie;
	
	ofVMouse anim;
	ofImage pointer;
  
  ofButton  demo;
  
  rootGroup sets;
};

#endif
