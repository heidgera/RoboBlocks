/*
 *  blocksWriteOutFuncs.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/9/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"
extern string curDir;
extern string ROOT_DIR;

void resetList(block & t, map<string,bool> & used)
{
	//-------- resets the log of what blocks have been used in the last print stage
	used[""]=false;
	used[t.title]=false;
	for (unsigned int i=0; i<t.numInside(); i++) {
		resetList(t.blocksIn[i],used);
	}
	for (unsigned int i=0; i<t.size(); i++) {
		resetList(t.blocksOn[i],used);
	}
}

//-------------------------- block Print functions-------------------------------------

void block::newPrintOut(ofstream* fOut,ifstream * fInput,int t, map<string,bool> * printList){
	//******* okay, let's try this; this may be a fairly complicated way to write out the .pde for compiling
	//******* and uploading, but it seems to work okay. This function reads a block of code from a file handed from the functions
	//******* below, parses it to find references to contained blocks or dropdowns, and properly formats it
	
	bool printed=printList->find(title)->second;
	bool partP=partnerWritten(printList);
	
	//-------- init the buffer, the pos counters and the end flag
	string buffer;
	int strtPos=0,endPos=0;
	bool bEnd=false;
	
	//-------- while we're not at the end of the file and we haven't reached the end of the read section
	while ((*fInput).peek()!=EOF&&!bEnd) {
		
		//-------- get a line and reset the position counters
		getline((*fInput),buffer);
		strtPos=endPos=0;
		
		//-------- write t amount of tabs to fOut
		for (int i=0; i<t&&buffer.compare("}"); i++) {
			*fOut << "\t";
		}
		//-------- if the buffer is not a single '}', then parse the line; otherwise, it indicates the end of the section
		if(buffer.compare("}")){
			//-------- init the foundTab var, and start stepping through the buffer
			int foundTab=0;
			for (unsigned int i=0; i<buffer.length(); i++) {
				//-------- if we find a '$'
				if(buffer[i]=='$'){
					//-------- step through the buffer until you find [ ,\n;")]
					strtPos=endPos=++i;
					while (!isDelim(buffer[endPos]," ,\n;\")")) {
						endPos++;
					}
					i=endPos-1; // move the step counter to the end of the word
					
					//-------- once you have found the segment of code after the $, store it in "temp" and split the string by "[]"
					//-------- doing this allows you to find which dropdown to look at (most often, it's 0)
					string temp(buffer,strtPos,endPos-strtPos);
					vector<string> tempVec=ofSplitString(temp, "[]");
					
					//-------- pos stores the vector position of the dd you are looking for 
					int pos=0;
					if(tempVec.size()>1){
						pos=atoi(tempVec[1].c_str());
					}
					
					//-------- reassemble the string without the []
					if(tempVec.size()>2) temp = tempVec[0] + tempVec[2];
					
					if(temp.compare("blockIn")==0&&buffer[i+1]==';') i++;
					//-------- generate the map for use in the switch
					map<string,int> list;
					list["dd.num"]=0;
					list["dd.str"]=1;
          list["dd.ind"]=2;
					list["blockOn"]=3;
					list["blockIn"]=4;
					list["blockIf"]=5;
					switch (list.find(temp)->second) {
						case 0:
							//-------- if temp=="dd.num" write the value of the "pos" dropdown
							*fOut << ddGroup[pos].getValue();
							break;
						case 1:
							//-------- if temp=="dd.str" write the string of the value stored in the dd[pos]
							*fOut << ddGroup[pos].getString();
							break;
            case 2:
							//-------- if temp=="dd.ind" write the index of the value stored in the dd[pos]
							*fOut << ddGroup[pos].getIndex();
							break;
						case 3:
							//-------- if temp==blockOn, printout the loop function for each of the blocks on
							for(unsigned int i=0; i<blocksOn.size(); i++){
								blocksOn[i].printData("loop(){",fOut,t,printList);
							}
							break;
						case 4:{
							//-------- if temp==blockIn, printout the loop for each of the blocks inside
							int outP=fOut->tellp();
							fOut->seekp(outP-2);
							for(unsigned int i=0; i<blocksIn.size(); i++){
								blocksIn[i].printData("loop(){",fOut,t+1,printList,false);
							}
							resetList(*this, *printList);
							for(unsigned int i=0; i<blocksIn.size(); i++){
								//blocksIn[i].printData("end(){",fOut,t+1,printList);
							}
							outP=fOut->tellp();
							fOut->seekp(outP-1);
							break;
						}
						case 5:
							//-------- if temp==blockIf, print the numBlocks
							//-------- just realized this won't work with more than one block, need to do it like the
							//-------- dd blocks above
							//-------- TODO: don't be a dumbshit
							for(unsigned int i=0; i<numBlocks.size(); i++){
								if(!numBlocks[i].placeHolder)
									numBlocks[i].printData("loop(){",fOut,0,printList);
								else {
									*fOut << "0";
								}

							}
							break;
						default:
							break;
					}
				}
				//-------- if the buff line is preceded by a '@', print the line only if the block has not already appeared
				else if(buffer[i]=='@'){
					if (printed) i=buffer.length();
				}
				//-------- if the buffer at the current pos is '~', print only if the block or a complement block
				//-------- has not been printed before
				else if(buffer[i]=='~'){
					if (partP) i=buffer.length();
				}
				//-------- increment the foundtab count if we find a '\t'
				else if(buffer[i]=='\t'){
					foundTab++;
					if (foundTab>1) *fOut << buffer[i];
				}
				//-------- if nothing else, printout the character
				else if(buffer[i]!='\n'&&buffer[i]!='\r') *fOut << buffer[i];
			}
			//-------- newline after buffer if it was not a numblock
			//-------- TODO: figure out why no new line if printed already
			if(!numBlock) *fOut << '\n';
		}
		else {
			//-------- if we found a '}' by itself, end.
			(*fInput).seekg (0, ios::end);
			bEnd=true;
		}
	}
}

bool block::partnerWritten(map<string,bool> * printed)
{
	//-------- checks to see if a complement block has been printed
	bool ret=false;
	map<string,bool>::iterator it;
	for (unsigned int i=0; i<part.size(); i++) {
		it=printed->find(part[i]);
		if(it!=printed->end()){
			ret=ret||it->second;
		}
	}
	//-------- checks to see if the block itself has been printed
	ret=ret||printed->find(title)->second;
	return ret;
}

void block::printData(string part,ofstream* k,int t,map<string,bool> * printed, bool printIn){
	string buffer;
	cout << filename << " is the base filename" << endl;
	if(filename.compare("null")){
		//-------- open the file where the end routines are located
		ifstream f(ofToDataPath(ROOT_DIR+"/blocks/"+filename).c_str());
		//-------- burn the buffer until we find the start of the end
		bool found=false;
		while(f.peek()!=EOF&&!found){
			if (buffer.compare(part)) {
				getline(f,buffer);
			}
			else {
				found=true;
			}
		}
		if(found){
			//-------- printout the code for the start routines
			newPrintOut(k,&f,t,printed);
			//-------- set the printed flag for the block and close the input 
			(*printed)[title]=true;
			f.close();
			
			//-------- print the start routines for the blocks on and in
			if(printIn)
				for (unsigned int i=0; i<blocksIn.size(); i++) {
					blocksIn[i].printData(part,k,t,printed,printIn);
				}
			for (unsigned int i=0; i<blocksOn.size(); i++) {
				blocksOn[i].printData(part,k,t,printed,printIn);
			}
		}
	}
	else {
		for (unsigned int i=0; i<blocksOn.size(); i++) {
			blocksOn[i].printData(part,k,t,printed,printIn);
		}
	}

}

//------------------ bGroup Writeout Functions ----------------

void bGroup::resetUsed(block & t)
{
	//-------- resets the log of what blocks have been used in the last print stage
	used[""]=false;
	used[t.title]=false;
	for (unsigned int i=0; i<t.numInside(); i++) {
		resetUsed(t.blocksIn[i]);
	}
	for (unsigned int i=0; i<t.size(); i++) {
		resetUsed(t.blocksOn[i]);
	}
}

void bGroup::resetUsed()
{
	resetUsed(base);
	for (unsigned int i=0; i<blocks.size(); i++) {
		resetUsed(blocks[i]);
	}
}

void bGroup::writeFile(string filename)
{
  ofstream out(ofToDataPath(filename).c_str());
	writeFile(&out);
	out.close();
}

void bGroup::writeFile(ofstream *k){
	//blocks[lastBlock].printOut(k);
	if(blocks.size()||1){
		//-------- Updated to allow for greater flexability (ie we can change basic behavior, without recompile
		
		//-------- open the path to the wrapper file, which contains general program structure.
		string buffer;
		ifstream f(ofToDataPath("arduino_make/onceWrapper.wrp").c_str());
		while ((f).peek()!=EOF) {
			//-------- read the next line into the buffer and init the position keepers and the tab count
			getline((f),buffer);
			int strtPos=0,endPos=0;
			int tabs=0;
			for (unsigned int i=0; i<buffer.length(); i++) {
				//-------- if we find a tab, increment; if the next char is not a '$' print out a tab.
				if(buffer[i]=='\t'){
					tabs++;
					if(buffer[i+1]!='$') *k << '\t';
				}
				//-------- if we find a '$'
				else if(buffer[i]=='$'){
					//-------- find the next word after the $
					strtPos=endPos=++i;
					while (buffer[endPos]!='.'&&buffer[endPos]) {
						endPos++;
					}
					i=endPos;
					string temp(buffer,strtPos,endPos-strtPos);
					//-------- if it was "blocks"
					if(!temp.compare("blocks")){
						//-------- find the word after the '.'
						strtPos=endPos=++i;
						while (buffer[endPos]!='('&&buffer[endPos]) {
							endPos++;
						}
						string temp2(buffer,strtPos,endPos-strtPos);
						
						//-------- reset the used block map
						resetUsed();
						
						//-------- print out the appropriate routines, according to the name that followed the '.'
						if(!temp2.compare("global")){
							//blocks[lastBlock].printGlobal(k,0,&used);
							base.printData("global(){",k,0,&used);
						}
						else if(!temp2.compare("setup")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("setup(){",k,tabs,&used);
						}
            else if(!temp2.compare("start")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("start(){",k,tabs,&used);
						}
						else if(!temp2.compare("loop")){
							base.printData("loop(){",k,tabs,&used,false);
						}
            else if(!temp2.compare("end")){
							//blocks[lastBlock].printSetup(k,tabs,&used);
							base.printData("end(){",k,tabs,&used);
						}
						i=endPos+1;
					}
				}
				//-------- if we didn't find either of those things, write the i^th character to k. 
				else {
					*k << buffer[i];
				}
			}
			//-------- end each line with a newline.
			*k << endl;
		}
	}
}
