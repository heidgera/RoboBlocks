/*
 *  blockLoad.h
 *  robotBlocks
 *
 *  Created by Exhibits on 6/23/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "blocks.h"
#include "ofxDirList.h"

string config(string file);

struct blockGroup {
  int nLoaded;
  bool bImgLoaded;
  ofxDirList DIR;
  ofXML blockXML;
  ofXML animXML;
  ofButton choice;
  ofImage example;
  string title;
  string subtitle;
  blockGroup(string dir);
  void load(string dir);
};

struct rootGroup {
  ofxDirList DIR;
  vector<blockGroup> set;
  int size(){ return set.size(); }
  blockGroup & operator[](int i);
  ofButton & operator()(int i);
  blockGroup * getSelected();
  void load(string rootDir);
  bool clickDown(int _x, int _y);
  void clickUp();
  void reset();
};