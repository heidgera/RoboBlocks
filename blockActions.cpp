/*
 *  blockActions.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/13/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

extern int pixPerInch;

#include "blocks.h"

struct evalData {
  bool isNumber;
  double number;
  string varName;
  int usrData;
  char nextOperator;
  evalData(double num){
    number=num;
    isNumber=true;
    usrData=nextOperator=0;
  }
  evalData(){
    isNumber=false;
    usrData=nextOperator=0;
  }
  evalData(string var){
    isNumber=false;
    varName=var;
    usrData=nextOperator=0;
  }
};

evalData operate(evalData & first, evalData & second)
{
  evalData ret;
  switch (first.nextOperator) {
    case '-':
      ret.number=first.number-second.number;
      break;
    case '+':
      ret.number=first.number+second.number;
      break;
    case '/':
      ret.number=first.number/second.number;
      break;
    case '*':
      ret.number=first.number*second.number;
      break;
    default:
      break;
  }
  ret.isNumber=true;
  ret.nextOperator=second.nextOperator;
  
  ret.varName=ofToString(ret.number,0);

  return ret;
}

int getNumDelimited(string exp, string delims)
{
  int ret=0;
  for (unsigned int i=0; i<exp.length(); i++) {
    if (isDelim(exp[i],delims)) {
      ret++;
    }
  }
  return ret+1;
}

evalData getSubExpression(string exp, int j)
{
  evalData ret;
  string delims="+-/*";
  string expr;
  int strt=0, end=0;
  int expNum=0;
  int numDeltd=getNumDelimited(exp, delims);
  if(j>numDeltd) j=numDeltd;
  for (unsigned int i=0; i<exp.length(); i++) {
    if (isDelim(exp[i],delims)) {
      strt=((expNum==0)?end:end+1);
      end=i;
      if(expNum==j){
        expr=string(exp,strt,end-strt);
        if(i<exp.length()-1&&i) ret.nextOperator=exp[i];
        else if(i==0) ret.nextOperator=exp[i];
      }
      expNum++;
    }
  }
  if(j==expNum){
    strt=((j>0)?end+1:end);
    end=exp.length();
    expr=string(exp,strt,end-strt);
  }
  ret.varName=expr;
  if(expr.length()&&expr[0]!='$') ret.number=ofToInt(ret.varName), ret.isNumber=true;
  if(!expr.length()&&ret.nextOperator=='-'){
    ret.number=-1;
    ret.isNumber=true;
    ret.nextOperator='*';
    ret.varName="-1";
  }

  return ret;
}

vector<evalData> breakIntoEvaluable(string exp)
{
  vector<evalData> ret;
  for (int i=0; i<getNumDelimited(exp, "+-/*"); i++) {
    ret.push_back(getSubExpression(exp, i));
  }
  return ret;
}

vector<evalData> evaluateNumbers(string exp)
{
  vector<evalData> ret;
  vector<evalData> eval=breakIntoEvaluable(exp);
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      if((eval[i+1].nextOperator!='*'&&eval[i+1].nextOperator!='/')||(eval[i].nextOperator=='*'||eval[i].nextOperator=='/')){
        evalData newData=operate(eval[i], eval[i+1]);
        eval[i]=newData;
        eval.erase(eval.begin()+i+1);
      }
    }
  }
  for (unsigned int i=0; i<eval.size(); i++) {
    if(i<eval.size()-1&&eval[i].isNumber&&eval[i+1].isNumber){
      evalData newData=operate(eval[i], eval[i+1]);
      eval[i]=newData;
      eval.erase(eval.begin()+i+1);
    }
  }
  ret=eval;
  return ret;
}

void block::registerAction(string str)
{
  vector<string> splt=ofSplitString(str, ":");
  if(splt.size()>1){
    if(splt[0]=="move") action.act=OF_BLOCK_MOVE;
    else if(splt[0]=="turn") action.act=OF_BLOCK_TURN;
    else if(splt[0]=="while") action.act=OF_BLOCK_WHILE;
    else if(splt[0]=="if") action.act=OF_BLOCK_IF;
    else if(splt[0]=="repeat") action.act=OF_BLOCK_REPEAT;
    action.dataStr=splt[1];
  }
}

double block::evalVar(string str)
{
  double ret=0;
  vector<string> spl=ofSplitString(str, "$[]()");
  for (unsigned int i=0; i<spl.size(); i++) {
    if(spl[i]=="dd") ret=ddGroup[ofToInt(spl[i+1])].getValue();
    else if(spl[i]=="ppi") ret=pixPerInch;
  }
  return ret;
}

double block::parseNumber(string str)
{
  vector<evalData> eval=evaluateNumbers(action.dataStr);
  string t;
  for (unsigned int i=0; i<eval.size(); i++) {
    if(!eval[i].isNumber) eval[i].number=evalVar(eval[i].varName),eval[i].varName=ofToString(eval[i].number,0), eval[i].isNumber=true;
    t+=eval[i].varName+eval[i].nextOperator;
  }
  eval=evaluateNumbers(t);
  return eval[0].number;
}

void block::parseAction()
{
  if(!action.bParsed){
    switch (action.act) {
      case OF_BLOCK_MOVE:
        action.data[0]=parseNumber(action.dataStr);
        break;
      case OF_BLOCK_TURN:
        action.data[0]=parseNumber(action.dataStr);
        break;
      /*case OF_BLOCK_WHILE:
        action.data[0]=parseNumber(action.dataStr);
        break;
      case OF_BLOCK_IF:
        action.data[0]=parseNumber(action.dataStr);
        break;*/
      case OF_BLOCK_REPEAT:
        if(action.dataStr!="$forever")
          action.data[0]=parseNumber(action.dataStr);
        break;
      default:
        break;
    }
    action.bParsed=true;
  }
  for (unsigned int i=0; i<blocksIn.size(); i++) {
    blocksIn[i].parseAction();
  }
  for (unsigned int i=0; i<blocksOn.size(); i++) {
    blocksOn[i].parseAction();
  }
}

void block::executeAction()
{
  
}

block * bGroup::nextActionBlock(block & b, bool skip)
{
  block * ret=0;
  for (unsigned int i=0; i<b.blocksIn.size()&&!ret; i++) {
    if(!b.blocksIn[i].action.bExecuted){
      ret=&b.blocksIn[i];
    }
  }
  for (unsigned int i=0; i<b.blocksOn.size()&&!ret; i++) {
    if(!b.blocksOn[i].action.bExecuted)
      ret=&b.blocksOn[i];
  }
  return ret;
}

void bGroup::parseActions()
{
  for (unsigned int i=0; i<base.blocksOn.size(); i++) {
    base.blocksOn[i].parseAction();
  }
}

void resetActions(block & t, bool skip=false)
{
  if(!skip){
    t.action.bParsed=t.action.bExecuted=false;
    t.action.bData=t.action.data[1]=0;
  }
  for (unsigned int i=0; i<t.blocksIn.size(); i++) {
    resetActions(t.blocksIn[i]);
  }
  for (unsigned int i=0; i<t.blocksOn.size(); i++) {
    resetActions(t.blocksOn[i]);
  }
}

void bGroup::startSequence()
{
  parseActions();
  resetActions(base);
  turtle.pos.x=turtle.start.x;
  turtle.pos.y=turtle.start.y;
  turtle.bearing.x=0;
  turtle.bearing.y=-1;
  turtle.clear();
  bSequencePlay=true;
}

bool bGroup::checkAgainstImage(){
  
}

bool bGroup::interpretDataStr(string str)
{
  bool ret=false;
  vector<string> spl=ofSplitString(str, "$[]()");
  bool negate=false;
  for (unsigned int i=0; i<spl.size(); i++) {
    cout << spl[i] << endl;
    if(spl[i]=="!") negate=true;
    else if(spl[i]=="front") ret=turtle.frontIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="frontLeft") ret=turtle.frontLeftIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="frontRight") ret=turtle.frontRightIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="leftPath") ret=turtle.leftIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="rightPath") ret=turtle.rightIsClear(ofToFloat(spl[i+1])*pixPerInch, mapp);
    else if(spl[i]=="forever") ret=1;
  }
  if(negate) ret=!ret;
  return ret;
}

bool bGroup::idleSequence(block * search)
{
  bool ret=false;
  if(bSequencePlay){
    if(search==&base) ret=true;
    if(actionTime.expired()){
      block * t;
      if(!currentTest||currentTest->action.bExecuted||currentTest==search)
        currentTest=t=nextActionBlock(*search);
      else t=currentTest;
      if(t){
        block & nxt=*t;
        blockAction & bA=nxt.action;
        ret=true;
        switch (bA.act) {
          case OF_BLOCK_MOVE:
            if(bA.data[0]<0){
              if(bA.data[0]<bA.data[1]){
                turtle.move(-2);
                bA.data[1]-=2;
              }
              else bA.bExecuted=true;
            }
            else {
              if(bA.data[0]>bA.data[1]){
                turtle.move(2);
                bA.data[1]+=2;
              }
              else bA.bExecuted=true;
            }
            actionTime.set(0.001);
            break;
            
            
          case OF_BLOCK_TURN:
            if(bA.data[0]<0){
              if(bA.data[0]<bA.data[1]){
                turtle.turn(-2);
                bA.data[1]-=2;
              }
              else bA.bExecuted=true;
            }
            else{
              if(bA.data[0]>bA.data[1]){
                turtle.turn(2);
                bA.data[1]+=2;
              }
              else bA.bExecuted=true;
            }
            actionTime.set(0.001);
            break;
            
            
          case OF_BLOCK_WHILE:
            if(interpretDataStr(bA.dataStr)){
              if(!idleSequence(&nxt)){
                resetActions(nxt,true);
              }
            }
            else bA.bExecuted=true;
            break;
            
            
          case OF_BLOCK_IF:
            if(interpretDataStr(bA.dataStr)||bA.bData){
              bA.bData=true;
              if(!idleSequence(&nxt))
                bA.bExecuted=true,bA.bData;
            }
            else bA.bExecuted=true;
            break;
            
            
          case OF_BLOCK_REPEAT:
            if(!bA.dataStr.compare("$forever")){
              if(interpretDataStr(bA.dataStr)){
                if(!idleSequence(&nxt))
                  resetActions(nxt,true);
                ret=true;
              }
              else resetActions(nxt);
            }
            else{
              if(bA.data[0]>bA.data[1]){
                if(!idleSequence(&nxt)){
                  bA.data[1]++;
                  resetActions(nxt,true);
                }
              }
              else bA.bExecuted=true,resetActions(nxt,true);
            }
            break;
            
            
          default:
            ret=false;
            break;
        }
      }
      else {
        ret=false;
        //resetActions(base);
      }
    }
  }
  return ret;
}

void bGroup::drawCurrentBlock()
{
  if(currentTest){
    ofSetColor(255, 255, 255);
    currentTest->drawSelected();
    currentTest->draw();
  }
}

void bGroup::startTesting()
{
  bTesting=true;
  currentTest=0;
}

void bGroup::stopTesting()
{
  bTesting=false;
}

bool bGroup::isTesting()
{
  return bTesting;
}

void bGroup::pauseSequence()
{
  
}
