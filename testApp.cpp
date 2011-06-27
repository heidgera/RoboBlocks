#include "testApp.h"

string curDir="basic";
string ROOT_NAME;
string ROOT_DIR;

//--------------------------------------------------------------
void testApp::setup(){
	ROOT_DIR=config("config.cfg");
  
  sets.load(ROOT_DIR);
	
	//--------- Initialize the valid working space for the blocks
	blocks.setup(250, 0, ofGetWidth(), ofGetHeight());
	
	objects = 0;
	
	//--------- Load the background images (wood panel and aluminum
	background.loadImage("images/background.jpg");
	sideBarBack.loadImage("images/woodPanel.jpg");
	
	//--------- Load the images for the buttons
	clearBut.setup(64,64,"images/deleteBlocks2.png");
	upBut.setup( 128,128,"images/upload2.png");
	redoBut.setup(64, 64, "images/redo.png");
	undoBut.setup(64, 64, "images/undo.png");
  
  skipBut.setup(300, 100, "images/skipBut.png");
	
  demo.setup("View demo", "fonts/Arial.ttf", 20);
	//--------- Load font for drawing on screen
	arial.loadFont("fonts/DinC.ttf");
	arial.setSize(30);
	arial.setMode(OF_FONT_CENTER);
  
  label.loadFont("fonts/Arial.ttf");
  label.setSize(20);
  label.setMode(OF_FONT_TOP);
	
	
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	
	//********** for some reason, vc++ hangs on loading rasapi32.dll when trying to load videos
	//******* hence this being only for OSX and linux
	
	crabMovie.loadMovie("movies/crab.mov");
	crabMovie.setVolume(0);
	crabMovie.play();
	
#endif
	
	//screenCast.loadMovie("movies/screenCast.mp4");
	//screenCast.setLoopState(false);
	anim.setup();
  animStep=0;
  
	ofHideCursor();
	pointer.loadImage("images/pointer.png");
}

//--------------------------------------------------------------
void testApp::update(){
	
	//---------- blocks.update confirms that all the blocks are in the proper positions
  //if((ofGetElapsedTimeMillis()/500)%2)
    blocks.update();
  if(timeOut.running())
    objects->update();
	
	//---------- Set which blocks are available to be pressed
	redoBut.setAvailable(blocks.redoAvailable());
	undoBut.setAvailable(blocks.undoAvailable());
	upBut.setAvailable(!systemCall.isRunning());
  demo.setAvailable(!anim.isPlaying());
	
	//--------- if the exhibit just timed out, clear the blocks
	if(timeOut.justExpired()){
		blocks.clear();
    sets.reset();
	}
	
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	
	//--------- Again, only working for osx, TODO: figure out why it doesn't work on the windows build
	if(timeOut.expired()){
		crabMovie.idleMovie();
		if(!crabMovie.isPlaying())
			crabMovie.play();
	}
	
#endif
	
	//--------- if we are running the upload script, spin the spinner
	if(systemCall.isRunning()){
		spinner.spin();
	}
	
  if(anim.isPlaying())
    anim.updateNextEvent();
}


//--------------------------------------------------------------
void testApp::draw(){
	
	//--------- If we have selected one of the groups of blocks, draw the composition screen
	if(timeOut.running()){
		
		//--------- Init the size variables for the side and bottom bar
		int sideWidth=objects->w-15;
		int menuBarH=75;
		
		//--------- Slightly dim the background image, and draw it on the bottom layer
		//ofSetColor(178, 178, 178);
		//background.draw(0, 0,ofGetWidth(),ofGetHeight());
    
    //--------- Draw background with slightly yellow grid over it.
		ofBackground(0x33, 0x33, 0x33);
    
		ofSetColor(0x444400);
		for (int i=0; i*10<ofGetHeight(); i++) {
			ofRect(sideWidth, i*10, ofGetWidth()-(sideWidth), 1);
		}
		for (int i=0; i*10<ofGetWidth(); i++) {
			ofRect(sideWidth+i*10, 0, 1, ofGetHeight());
		}
		
		//--------- Draw any blocks that are in the composition area and are not clicked.
		blocks.draw();
		
		
		//--------- if we are running the upload script, dim the background
		if(systemCall.isRunning()){
			
		}
		
    drawSidebar(sideWidth,menuBarH);
		
		//******************** Menu Bar **************************
		//--------- Draw the programming base block at the top of the screen
		int titleBarH=100;
    int subTitleH=50;
    
    arial.setSize(70);
		arial.setMode(OF_FONT_TOP);
		string title="Program the "+ROOT_NAME+" behaviors";
    double scaleTitle=1;
    if(arial.stringWidth(title)>ofGetWidth()){
      scaleTitle=(3*ofGetWidth()/4)/arial.stringWidth(title);
    }
    
    titleBarH=arial.stringHeight(title)*scaleTitle+10;
    
    int menuBot=titleBarH+menuBarH;
    int subtitleBot=menuBot+subTitleH;
		
    drawMenuBar(titleBarH,menuBarH);
    
    drawBase_and_Demo(subtitleBot);
    
    
    drawSubtitleBar(menuBot,subTitleH,menuBarH);
    
    
    //--------- Draw the bar on the top of the screen
		ofSetColor(0x555555);
		ofShadeBox(0, 0, ofGetWidth(), titleBarH, OF_DOWN, .3);
		ofShade(0, titleBarH, 10, ofGetWidth(), OF_DOWN);
    ofSetColor(0);
    
    //****************** Title text, scaled
		
    arial.setSize(70);
		arial.setMode(OF_FONT_TOP);
		ofSetColor(0xDFF500);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, 0, 0);
    ofScale(scaleTitle, scaleTitle, 0);
		arial.drawString(title,0, 0);
    ofPopMatrix();
    arial.setMode(OF_FONT_CENTER);
    arial.setMode(OF_FONT_BOT);
    arial.setSize(30);

		
		//********************** Draw the blocks which are being held by the mouse ********
		
		blocks.drawForeground();
		
	}
	//************************** if the exhibit has timed out, draw the welcome screen ************** 
	else {
		drawAttract();
	}
	
	//************************ if we're uploading, draw a fancy "uploading..." on screen

	if (systemCall.isRunning()) {
    ofSetColor(0, 0, 0,128);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255);
		spinner.draw((ofGetWidth())/2,ofGetHeight()/2, 300);
		arial.setMode(OF_FONT_LEFT);
		arial.setSize(70);
		string printOut="uploading";
		for (int i=0; i<(spinner.count()/3)%4; i++) {
			printOut.append(".");
		}
		ofSetColor(255, 255, 255);
		arial.drawString(printOut, ofGetWidth()/4, ofGetHeight()/4);
		arial.setMode(OF_FONT_CENTER);
		arial.setSize(30);
	}
	
  //*************** Draw the cursor on the screen
  
	if(anim.isPlaying()){
    ofSetColor(0, 0, 0,128);
    ofRoundShape(skipBut.x-10, skipBut.y-10, skipBut.w+20, skipBut.h+20, 20, true);
    skipBut.draw((ofGetWidth()-skipBut.w)/2, ofGetHeight()*3./4);
    if(anim.isClicked()) ofSetColor(128, 128, 128);	
    else ofSetColor(255, 255, 255);
    pointer.draw(anim.x-10, anim.y, pointer.width*2,pointer.height*2);
    ofSetColor(128,128,128,128);
    pointer.draw(mouseX-10, mouseY, pointer.width*2,pointer.height*2);
    
  }
	else {
    ofSetColor(255, 255, 255);
    pointer.draw(mouseX-10, mouseY, pointer.width*2,pointer.height*2);
  }
}

void testApp::upload()
{
	//--------- Generate the file which we will compile and upload to the arduino
	blocks.writeFile("arduino_make/arduino_make.pde");
	
	//--------- once we've generated the file, compile and upload with one of teh following scripts
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	systemCall.run(ofToDataPath("arduino_make/upload.sh"));
#else
	systemCall.run(ofToDataPath("\"data\\arduino_make\\build.bat\""));
#endif
	
	//--------- unpress the upload button 
	upBut.setPressed(false);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//************** only the key=='n' is used, to signal when the animation has finished a step.
	if(key=='w'){
		upload();
	}
	if(key=='p'){
		if(!anim.isPlaying()){
			
		}
		anim.play();
	}
	if(key=='r'){
		if(!anim.isPlaying()) anim.play();
		else anim.pause();
	}
  if(key=='n'){
    animationStepRequested(animXML);
    animStep++;
  }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//--------- if the mouse is moving and not clicked, inform the blocks
	blocks.motion(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//--------- if we are dragging the mouse, tell blocks so it can update the positions of any of the held blocks
  if(!anim.isPlaying()||(anim.isPlaying()&&button==VMOUSE_BUTTON))
     blocks.drag(x, y);
}

//****************** Function for changing the selected set of blocks ***********

void testApp::loadBlocks(blockGroup & bg){
	
  if(bg.nLoaded>=3){
    //--------- Delete the old parsing of the xml file and the current selection of blocks
    if(objects) delete objects;
    
    //--------- load the new blocks with the blockGroup data
    ROOT_NAME=bg.title;
    objects = new sbGroup(bg.blockXML,&blocks);
    
    animXML=bg.animXML;
    //--------- set the timeout timer to three minutes
    timeOut.set(180);
    
  #if defined( TARGET_OSX ) || defined( TARGET_LINUX )
    //--------- Pause the movie, if we are using the movie
    crabMovie.setPaused(true);
  #endif
    windowResized(0, 0);
  }
}

//***************** Mouse Click down function ***************
void testApp::mousePressed(int x, int y, int button){
  
  if(sets.clickDown(x,y)&&!anim.isPlaying()){
    if(sets.getSelected())
      loadBlocks((*sets.getSelected()));
  }
  
	if(anim.isPlaying()&&button!=VMOUSE_BUTTON){
    if(skipBut.clickDown(x, y)){
      blocks.clear();
      anim.pause(); 
    }
  }
	//--------- If we aren't timed out, check all of the click for the composition screen
	if(timeOut.running()&&(!anim.isPlaying()||(anim.isPlaying()&&button==VMOUSE_BUTTON))){
		
		//--------- reset the timeOut timer with each click
		timeOut.set(180);
		
		//--------- Check the blocks in the sidebar, to see if they have been clicked
		objects->clickDown(x, y);
		
		//--------- Check the blocks in the comp area
		blocks.clickDown(x, y);
		
		//--------- Run upload function if the upload button is pressed
		if(upBut.clickDown(x, y)){
			upload();
		}
		
		//--------- If we press the clear button, clear the blocks in the comp area
		if (clearBut.clickDown(x, y)) {
			blocks.clear();
		}
    
    if(demo.clickDown(x, y)){
      blocks.clear();
      animStep=0;
      anim.play();
    }
		
		//--------- if we press the undo button, roll back the state of the blockGroup
		if (undoBut.clickDown(x, y)) {
			blocks.undoState();
		}
		
		//--------- if we press the redo button, push the state forward
		if (redoBut.clickDown(x, y)) {
			blocks.redoState();
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(timeOut.running()&&(!anim.isPlaying()||(anim.isPlaying()&&button==VMOUSE_BUTTON))){
    //--------- do a bunch of clickups
    blocks.clickUp(x, y);
    upBut.clickUp();
    clearBut.clickUp();
    redoBut.clickUp();
    undoBut.clickUp();
    demo.clickUp();
    sets.clickUp();
  }
  skipBut.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	//--------- if the window changes size, refresh the size of the objects group
  if(timeOut.running()){
    objects->updateHeight();
    blocks.move(objects->w, 0);
  }
	blocks.changeSize(ofGetWidth()-blocks.x, ofGetHeight());
}

