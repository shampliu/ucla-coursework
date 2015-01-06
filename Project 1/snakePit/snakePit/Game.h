//
//  Game.h
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef __snakePit__Game__
#define __snakePit__Game__

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "Pit.h"
#include "globals.h"
#include "Player.h"

using namespace std;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nSnakes);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Pit* m_pit;
};

#endif /* defined(__snakePit__Game__) */
