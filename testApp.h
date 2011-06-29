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

/*****************************************************************
 * class testApp : public ofBaseApp
 *
 *  Description:: 
 *
 *
 *  Vars:
 *
 *    int animStep:: _ _ _ _ _ _ _ _ the current animation step, if playing.
 *    ofXML animXML::_ _ _ _ _ _ _ _ the xmlParser to hold the data for the current animation
 *    bGroup blocks::_ _ _ _ _ _ _ _ the block group, containing the base block and all the blocks which have been drag onto the stage
 *    sbGroup * objects::_ _ _ _ _ _ the sidebar bars and blocks
 *    ofImage background:: _ _ _ _ _ the background image, now used as sidebar image
 *    ofButton upBut:: _ _ _ _ _ _ _ the upload button, possibly should be migrated to the 'bGroup'
 *    ofButton clearBut::_ _ _ _ _ _ button to clear the workspace blocks
 *    ofButton redoBut:: _ _ _ _ _ _ button to redo any block action previously undone
 *    ofButton undoBut:: _ _ _ _ _ _ button to undo any block action
 *    ofButton skipBut:: _ _ _ _ _ _ button that is displayed during an animation to skip the animation
 *    ofTimer timeOut::_ _ _ _ _ _ _ exhibit timer
 *    ofFont titleFont:: _ _ _ _ _ _ font used for large labels
 *    ofFont label:: _ _ _ _ _ _ _ _ font used to write on the base block. Should be migrated.
 *    ofSystemCall systemCall::_ _ _ system call thread, to run the upload script
 *    ofProgressSpinner spinner::_ _ spinner to indicate when the program is uploading
 *    ofVideoPlayer crabMovie::_ _ _ video player used to display the attract movie
 *    ofVMouse anim::_ _ _ _ _ _ _ _ virtual mouse class used to demonstrate
 *    ofImage pointer::_ _ _ _ _ _ _ pointer used by the mouse and by the virtualMouse
 *    ofButton demo::_ _ _ _ _ _ _ _ button to start the demo
 *    rootGroup sets:: _ _ _ _ _ _ _ storage container for the block data
 */




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
	
	ofButton upBut;
	ofButton clearBut;
	ofButton redoBut;
	ofButton undoBut;
	
	ofButton skipBut;
	
	ofTimer timeOut;
	ofFont	titleFont;
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
