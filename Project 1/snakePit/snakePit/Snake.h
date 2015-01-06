
#ifndef __snakePit__Snake__
#define __snakePit__Snake__

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "Pit.h"
#include "globals.h"

using namespace std;

class Snake
{
public:
    // Constructor
    Snake(Pit* pp, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    
private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};


#endif /* defined(__snakePit__Snake__) */
