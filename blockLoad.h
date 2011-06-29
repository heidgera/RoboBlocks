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

/*****************************************************************
 * struct blockGroup 
 *
 *  Description:: stores the information for a single block group; all are public members
 *
 *  Vars:
 *
 *    nLoaded::         indicates the number of files which were loaded
 *    bImgLoaded::      indicates if an example image was loaded
 *    DIR::             ofxDirList used to rummage through the directory it is handed
 *    blockXML::        xmlParser to hold data from the 'blocks.xml'
 *    animXML::         xmlParser to hold data from the 'anim.xml'
 *    ofButton choice ::button to be shown to the public to select group
 *    ofImage example ::if we have an example image, it is stoored here
 *    string title ::   Title to be displayed on page
 *    string subtitle ::Subtitle to be display on page
 */


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

/*****************************************************************
 * struct rootGroup 
 *
 *  Description:: Holds all of the block information for the program
 *
 *
 *  Vars:
 *
 *   ofxDirList DIR::           Directory lister to iterate through the root dir
 *    vector<blockGroup> set::  vector for holding each of the groups of blocks
 */

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