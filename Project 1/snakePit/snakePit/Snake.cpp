//
//  Snake.cpp
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Snake.h"

Snake::Snake(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** A snake must be in some Pit!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > pp->rows()  ||  c < 1  ||  c > pp->cols())
    {
        cout << "***** Snake created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
}

int Snake::row() const
{
    return m_row;
}

int Snake::col() const
{
    return m_col;
}

void Snake::move()
{
    // Attempt to move in a random direction; if we can't move, don't move
    switch (rand() % 4)
    {
        case UP:     if (m_row > 1)             m_row--; break;
        case DOWN:   if (m_row < m_pit->rows()) m_row++; break;
        case LEFT:   if (m_col > 1)             m_col--; break;
        case RIGHT:  if (m_col < m_pit->cols()) m_col++; break;
    }
}