//
//  Player.h
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef __snakePit__Player__
#define __snakePit__Player__

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "globals.h"

using namespace std;

class Pit;

class Player
{
public:
    // Constructor
    Player(Pit *pp, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
    
    // Mutators
    void   stand();
    void   move(int dir);
    void   setDead();
    
private:
    Pit*  m_pit;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

#endif /* defined(__snakePit__Player__) */
