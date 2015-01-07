//
//  History.cpp
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "History.h"
#include "Pit.h"

History::History(int nRows, int nCols) {
    m_pit = new Pit(nRows, nCols);
    for (int i=0; i < nRows; i++)
    {
        for (int j=0; j < nCols; j++)
        {
            m_array[i][j] = 0;
        }
    }
    
}

bool History::record(int r, int c) {
    if (r > m_pit->rows() || c > m_pit->cols())
        return false;
    m_array[r-1][c-1]++;
    return true;
}

void History::display() const {
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    for (r = 0; r < m_pit->rows(); r++)
        for (c = 0; c < m_pit->cols(); c++)
        {
            if (m_array[r][c] == 0)
                grid[r][c] = '.';
            else if (m_array[r][c] >= 26)
                grid[r][c] = 'Z';
            else
            {
                char count = m_array[r][c]+100;
                grid[r][c] = count;
            }
        }
   
    clearScreen();
    for (r = 0; r < m_pit->rows(); r++)
    {
        for (c = 0; c < m_pit->cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
}

