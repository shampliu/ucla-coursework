//
//  History.h
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef __snakePit__History__
#define __snakePit__History__

#include <stdio.h>
#include "globals.h"


class Pit;

class History
{
public:
    History(int nRows, int nCols);
    ~History();
    bool record(int r, int c);
    void display() const;
    
private:
    Pit* m_pit;
    int  m_array[20][40];

};

#endif /* defined(__snakePit__History__) */
