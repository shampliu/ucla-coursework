//
//  Pit.h
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef __snakePit__Pit__
#define __snakePit__Pit__

#include <stdio.h>
#include "globals.h"
#include <iostream>
#include <string>
#include "History.h"

using namespace std;

class Snake;
class Player;

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display(string msg) const;
    void    displayHistory() const;
    
    // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   addHistory();
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();
    
    History& history();
    
private:
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Snake*   m_snakes[MAXSNAKES];
    int      m_nSnakes;
    History* m_history;

};

#endif /* defined(__snakePit__Pit__) */
