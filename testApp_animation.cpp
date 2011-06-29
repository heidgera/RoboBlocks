/*
 *  testApp_animation.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 6/23/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "testApp.h"

/**
 *  animationStepRequested(ofXML & animXML)- handles next step of an animation 
 *
 *  this function takes input from an xml file and, based on the animStep
 * pushes the approriate step to the anim class.
 *
 *  Input_________
 *    ofXML & animXML-- parsed xml file to be used in determining the next step;
 *
 *  Return________
 *    NONE: function is a void.
 */

void testApp::animationStepRequested(ofXML & animXML)
{
  //------- try to set the current tag for the current animation XML file
  if(animXML.setCurrentTag(";animation")){
    //------- set the default event type to "end"
    vMouseType vType=OF_VMOUSE_END;
    //------- variable declarations
    ofInterObj * object=0;
    double duration=0;
    double speed=0;
    int xint=0, yint=0;
    //------- if the current animation step is less than the total number of steps
    if(animStep<animXML.getNumTag("step")){
      //------- push into the tag and fetch the event Type
      animXML.pushTag("step",animStep);
      string type=animXML.getAttribute("type");
      if(type.length()){
        if(type=="move") vType=OF_VMOUSE_MOVE_TO;
        else if(type=="clickDown") vType=OF_VMOUSE_CLICK_DOWN,xint=anim.x,yint=anim.y;
        else if(type=="clickUp") vType=OF_VMOUSE_CLICK_UP,xint=anim.x,yint=anim.y;
      }
      
      //------- if we have a time attribute, set the duration var to that attribute
      string time=animXML.getAttribute("time");
      if(time.length()){
        duration=ofToFloat(time);
      }
      //-------otherwise, we need to look for the speed at which we want to move, to be used later to determine time
      else {
        string spd=animXML.getAttribute("speed");
        if(spd.length()){
          speed=ofToFloat(spd);
        }
      }
      //------- if there is a pos tag inside the current tag, push into it
      if(animXML.getNumTag("pos")){
        animXML.pushTag("pos");
        //------- and look at the type of position tag it is 
        string typ=animXML.getAttribute("type");
        if(typ=="obj") //-- if it is an object tag, search for the specified object using the searchForObject function
          object = searchForObject(animXML.getCurrentTag(),xint,yint);
        else if(typ=="coord"){
          //-------if it is a coordinate, set xint and yint to the x and y attributes.
          xint=ofToInt(animXML.getAttribute("x"));
          yint=ofToInt(animXML.getAttribute("y"));
        }
        //-------if it is a mouse tag, set the xint and yint to the mouse coordinates
        else if(typ=="mouse") xint=mouseX,yint=mouseY;
        
        if(!duration){ //-- if we haven't yet set the duration, use the speed to determine duration
          int xtmp=((object)?object->x+xint:xint);
          int ytmp=((object)?object->y+yint:yint);
          double dist= sqrt((xtmp-anim.x)*(xtmp-anim.x)+(ytmp-anim.y)*(ytmp-anim.y));
          duration=dist/speed;
        }
        
        //-- if we are following an object, the next event is called, referencing that object
        if(object) anim.nextEvent(vType,(*object),xint, yint,duration);
        else anim.nextEvent(vType,xint, yint,duration);  //-- otherwise, it is called using only x and y
        animXML.popTag();
      }
      else { //-- if there is not a position tag inside, the next event takes place where the vMouse is currently located
        anim.nextEvent(vType,xint, yint,duration);
      }
    }
    //-- if the animStep is greater than the current number of steps in the xml, end the animation
    else anim.nextEvent(OF_VMOUSE_END,mouseX,mouseY,0);
  }
}

/**
 *  animationStepRequested(ofXML & animXML)- handles next step of an animation 
 *
 *  this function takes input from an xml file and, based on the animStep
 * pushes the approriate step to the anim class.
 *
 *  Input_________
 *    ofXML & animXML-- parsed xml file to be used in determining the next step;
 *
 *  Return________
 *    NONE: function is a void.
 */


ofInterObj * testApp::searchForObject(ofTag & tag, int & _x, int & _y)
{
  ofInterObj * ret=0;
  string where=tag.getAttribute("where");
  string xTemp=tag.getAttribute("x");
  string yTemp=tag.getAttribute("y");
  vector<string> whSplit = ofSplitString(where, "[]");
  cout << whSplit.size() << " is the size" << endl;
  if(whSplit[0]=="sidebar"){
    if(whSplit.size()==2) ret=&((*objects)[ofToInt(whSplit[1])]);
    if(whSplit.size()==3) ret=&((*objects)[ofToInt(whSplit[1])][ofToInt(whSplit[2])]);
  }
  else if(whSplit[0]=="base"){
    if(whSplit.size()==1) ret=&blocks.base;
    else if(whSplit.size()>=2) ret=searchBlock(whSplit,blocks.base.blocksOn[ofToInt(whSplit[1])],2);
  }
  else if(whSplit[0]=="upload"){
    ret=&upBut;
  }
  vector<string> xSpl = ofSplitString(xTemp, "+-/*");
  if(xSpl.size()==1) _x=ofToInt(xSpl[0]);
  else if(xSpl.size()==2) _x=operaterByChar(ret->w,xTemp[xSpl[0].length()],ofToFloat(xSpl[1]));
  vector<string> ySpl = ofSplitString(yTemp, "+-/*");
  if(ySpl.size()==1) _y=ofToInt(ySpl[0]);
  else if(ySpl.size()==2) _y=operaterByChar(ret->h,yTemp[ySpl[0].length()],ofToFloat(ySpl[1]));
  
  return ret;
}

ofInterObj * testApp::searchBlock(vector<string> spl, block & b, int off)
{
  ofInterObj * ret=0;
  if(off>=spl.size()) ret=&b;
  else {
    char which=spl[off][0];
    int num=ofToInt(string(spl[off],1,spl[off].length()));
    if(which=='i') ret=searchBlock(spl,b.blocksIn[num],off+1);
    else if(which=='d') ret=&(b.ddGroup[num]);
  }
  return ret;
}