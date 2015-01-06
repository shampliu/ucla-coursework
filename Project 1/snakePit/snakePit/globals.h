//
//  globals.h
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef __snakePit__globals__
#define __snakePit__globals__

#include <stdio.h>

const int MAXROWS = 20;             // max number of rows in the pit
const int MAXCOLS = 40;             // max number of columns in the pit
const int MAXSNAKES = 180;          // max number of snakes allowed

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();


#endif /* defined(__snakePit__globals__) */
