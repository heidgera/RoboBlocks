/*
 *  blockLoad.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 6/23/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blockLoad.h"

string config(string file){
  string ret;
	ifstream config(ofToDataPath(file).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=");
		if(token.size()){
			if(!token[0].compare("ROOT_DIR")){
				ret = token[1];
			}
		}
		else {
			ret=".";
		}
		
	}
	config.close();
  return ret;
}

blockGroup::blockGroup(string dir)
{
  nLoaded=0;
  load(dir);
}

void blockGroup::load(string dir)
{
  int nDir=0;
  nDir = DIR.listDir(dir);
  //you can now iterate through the files as you like
  for(int i = 0; i < nDir; i++){
    vector<string> spl= ofSplitString(DIR.getPath(i), "/");
    cout << spl[spl.size()-1] << endl;
    if(spl[spl.size()-1]=="blocks.xml") blockXML.loadFile(DIR.getPath(i)), nLoaded++;
    else if(spl[spl.size()-1]=="anim.xml") animXML.loadFile(DIR.getPath(i)), nLoaded++;
    else if(spl[spl.size()-1]=="icon.png") choice.setup(256,256,DIR.getPath(i)), nLoaded++;
    else if(spl[spl.size()-1]=="example.png") example.loadImage(DIR.getPath(i)), bImgLoaded=true;
  }
  blockXML.setCurrentTag(";blocks");
  title=blockXML.getAttribute("name");
  subtitle=blockXML.getAttribute("subtitle");
  cout << blockXML.filename << endl;
}

void rootGroup::load(string dir)
{
  int nDir=0;
  nDir = DIR.listDir(dir);
  //you can now iterate through the files as you like
  for(int i = 0; i < nDir; i++){
    vector<string> spl= ofSplitString(DIR.getPath(i), "/");
    vector<string> spl2= ofSplitString(spl[spl.size()-1], ".");
    if(spl2.size()==1){
      if(spl2[0]!="blocks")
        set.push_back(blockGroup(DIR.getPath(i)));
    }
  }
}

blockGroup & rootGroup::operator[](int i)
{
  return set[i];
}

ofButton & rootGroup::operator()(int i)
{
  return set[i].choice;
}

blockGroup * rootGroup::getSelected()
{
  blockGroup * ret=0;
  for (unsigned int i=0; i<size(); i++) {
    if(set[i].choice.getAvailable()==false) ret=&set[i];
  }
  return ret;
}

bool rootGroup::clickDown(int _x, int _y)
{
  bool ret=0;
  for (unsigned int i=0; i<size(); i++) {
    if(set[i].choice.clickDown(_x,_y)){
      for (unsigned int j=0; j<size(); j++) {
        set[j].choice.setAvailable(true);
      }
      set[i].choice.setAvailable(false);
      ret=1;
    }
  }
  return ret;
}
  
void rootGroup::clickUp()
{
  for (unsigned int i=0; i<size(); i++) {
    set[i].choice.clickUp();
  }
}
void rootGroup::reset()
{
  for (unsigned int j=0; j<size(); j++) {
    set[j].choice.setAvailable(true);
  }
}