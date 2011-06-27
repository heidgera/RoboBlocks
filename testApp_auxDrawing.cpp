/*
 *  testApp_auxDrawing.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 6/23/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "testApp.h"

//********************* Sidebar *************************
void testApp::drawSidebar(int sideWidth, int menuBarH)
{
  //--------- Dim the brushed steel panel image slightly, and draw it on the left of the screen
  ofSetColor(190, 190, 190);
  double sideBarImgAdj=ofGetHeight()/double(background.width);
  background.draw(sideWidth-background.width*sideBarImgAdj, 0,background.width*sideBarImgAdj,ofGetHeight());
  
  //--------- Shade the bar, and put drop shadows to the right of the bar
  ofShade(sideWidth, 0, 15, ofGetHeight(), OF_LEFT,.35);
  ofShade(sideWidth, 0, 10, ofGetHeight()-menuBarH, OF_RIGHT,.5);
  ofShade(sideWidth, ofGetHeight()-menuBarH, 5, menuBarH, OF_RIGHT,.5);
  
  //--------- Draw the bars into the sidebar, and then draw the blocks for the open bar
  objects->draw(0,blocks.base.y+80); 
  
  if(sets.getSelected()){
    ofImage & img=sets.getSelected()->example;
    ofSetColor(200,200,200,196);
    img.draw((objects->w-img.width)/2, objects->y+objects->h+40);
  }
}

//********************* Menu bar holding the clear and undo-redo buttons *********************
void testApp::drawMenuBar(int & yPos, int & hgt)
{
  
  //--------- Draw a light gray bar on the bottom of the screen, and shade it gently
  ofSetColor(0xCFCFCF);
  ofShadeBox(0, yPos, ofGetWidth(), hgt, OF_DOWN,(0xCF-0xA8)/255.);
  ofShade(0, yPos, 10, ofGetWidth(), OF_UP,.15);
  
  //--------- Draw the relevent buttons onto the bottom bar, in appropriate positions
  //--------- right justified
  clearBut.draw(ofGetWidth()-100,yPos+hgt-(hgt+clearBut.h)/2);
  
  //--------- Centered in the bar
  int center=(sets(sets.size()-1).x+sets(sets.size()-1).w+clearBut.x)/2;
  redoBut.draw(center+50,yPos+hgt-(hgt+redoBut.h)/2);
  undoBut.draw(center-50,yPos+hgt-(hgt+undoBut.h)/2);
  
  if(sets.size()){
    ofButton & last=sets(sets.size()-1);
    ofShade((undoBut.x+last.x+last.w)/2, yPos, 10, hgt, OF_LEFT,.25);
    ofShade((undoBut.x+last.x+last.w)/2, yPos, 10, hgt, OF_RIGHT,.25,false);
  }
  
  ofShade((clearBut.x+redoBut.x+redoBut.w)/2, yPos, 10, hgt, OF_LEFT,.25);
  ofShade((clearBut.x+redoBut.x+redoBut.w)/2, yPos, 10, hgt, OF_RIGHT,.25,false);
}

///********************* Base block ***************
void testApp::drawBase_and_Demo(int yPos)
{
  
  blocks.drawBase((ofGetWidth()-300)/2, yPos);
  upBut.draw(blocks.base.x+blocks.base.w+10,blocks.base.y+blocks.base.h-50);
  
  
  //******************** Draw the text onto the base block
  ofSetColor(0);
  string baseLabel="1. Connect instruction blocks\n   here";
  ofRectangle one(blocks.base.x+25, blocks.base.y+10+label.stringHeight("Co")+4,label.stringWidth(baseLabel)-5,2);
  label.drawString(baseLabel, blocks.base.x+20, blocks.base.y+10);
  label.setMode(OF_FONT_RIGHT);
  baseLabel="2. Upload instructions";
  ofRectangle two(blocks.base.x+blocks.base.w+upBut.w, blocks.base.y+10+label.stringHeight("K")+4,label.stringWidth(baseLabel)-5,2);
  label.drawString(baseLabel, blocks.base.x+blocks.base.w+upBut.w, blocks.base.y+10);
  label.setMode(OF_FONT_LEFT);
  
  //***************** Underline the text on the block
  ofSetColor(0);
  ofSetLineWidth(1);
  ofLine(one.x+one.width, one.y, one.x, one.y);
  ofLine(one.x, one.y, one.x, one.y+40);
  ofLine(two.x-two.width, two.y, two.x, two.y);
  ofLine(two.x-2*two.width/3, two.y, two.x-2*two.width/3, upBut.y+upBut.h/4);
  ofLine(upBut.x, upBut.y+upBut.h/4, two.x-2*two.width/3, upBut.y+upBut.h/4);
  
  //********************** Demo button
  
  ofRoundShadow(-30, yPos-30, objects->w-10, 105, 20, .5);
  ofSetColor(0x555555);
  ofRoundBox(-20, yPos-20, objects->w-30, 80, 20, .2);
  
  demo.draw(20, yPos+10);
}

//********************** Subtitle bar
void testApp::drawSubtitleBar(int yPos, int hgt, int tabHgt)
{
  //-------- Draw the black line 
  ofRect(0, yPos-2, ofGetWidth(), 2);
  ofShade(0, yPos, 10, ofGetWidth(), OF_UP);
  
  string subtitle=(sets.getSelected())?sets.getSelected()->subtitle:"Default";
    
  ofSetColor(0x555555);
  ofShadeBox(0, yPos, ofGetWidth(), hgt, OF_DOWN,(0xCF-0xA8)/255.);
  ofShade(0, yPos+hgt, 10, ofGetWidth(), OF_UP,.25);
  ofShade(0, yPos+hgt, 10, ofGetWidth(), OF_DOWN);
  
  
  
  if(sets.getSelected()){
    ofButton & sel=(sets.getSelected()->choice);
    ofSetColor(0);
    ofRect(sel.x-7, sel.y-2, sel.w+14, (yPos)-sel.y);
    ofSetColor(0x555555);
    ofRect(sel.x-5, sel.y, sel.w+10, yPos-sel.y);
    for (unsigned int i=0; i<sets.size(); i++) {
      sets(i).draw(50+i*100,yPos-(tabHgt+sets(i).h)/2,64,64);
    }
  }
  
  ofSetColor(0xDFF500);
  arial.setSize(24);
  arial.setMode(OF_FONT_TOP);
  arial.setMode(OF_FONT_LEFT);
  arial.drawString(subtitle, 50, yPos);
  arial.setMode(OF_FONT_CENTER);
  arial.setMode(OF_FONT_BOT);
  arial.setSize(30);
}

void testApp::drawAttract()
{
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
  ofSetColor(255, 255, 255);
  crabMovie.draw(0,0,ofGetWidth(),ofGetHeight());
#endif
  
  
  //--------- Get the size of the text so we can draw a nice box on screen
  string printOut="Select a programmer level to begin";
  ofRectangle k=arial.getBoundingBox(printOut, ofGetWidth()/2, ofGetHeight()/2);
  
  //--------- Draw a nice box on screen
  ofSetColor(0,0,0,128);
  if(sets.size())
    ofRoundShape(ofGetWidth()/(sets.size()+1)-sets(0).w/2-20, k.y-20, ((sets(sets.size()-1).x+sets(sets.size()-1).w)-sets(0).x)+40, k.height+sets(0).h+40, 10, 1);
  else{
    timeOut.set(180);
  }
  
  //--------- Draw the text in the box
  ofSetColor(255, 255, 255);
  arial.drawString(printOut, ofGetWidth()/2, ofGetHeight()/2);
  
  for (unsigned int i=0; i<sets.size(); i++) {
    sets(i).draw((i+1)*ofGetWidth()/(sets.size()+1)-sets(0).w/2,k.y+k.height,240,240);
  }
}