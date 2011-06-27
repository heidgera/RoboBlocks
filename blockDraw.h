/*
 *  blockDraw.h
 *  robotBlocks_redux
 *
 *  Created by Exhibits on 1/31/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#ifndef _blockDraw
#define _blockDraw

#include "ofMain.h"
#include "ofExtended.h"

void glBezier3f(double Ax,double Bx, double Cx,double Ay,double By, double Cy,double r=1, double g=1, double bl=1, double aug=.2);

void glWideCurve(double Ax,double Bx, double Cx,double Ay,double By, double Cy,double sp=1,double r=1, double g=1, double bl=1, double aug=.2,bool top=false);

void drawBlock(double x, double y, double w, double h);

void drawBaseBlock(double x, double y, double w, double h);

void drawBlockShadow(double x, double y, double w, double h);

void shadeCircleSlice(double x, double y, double ir, double oR,int beg,int end, double strt, double r,double g, double b,double shBot,double shTop);

void drawCircleSeg(double x, double y, double ir, double oR,int beg,int end, double r,double g, double b);

void drawBigBlockShadow(double x, double y, double w, double h,double oH);

void drawBigBlock(double x, double y, double w, double h, double oh);

#endif
