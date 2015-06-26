//
//  History.cpp
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "History.h"
#include "Pit.h"

using namespace std;

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            m_grid[i][j] = '.';
        }
    }
    
    
}

bool History::record(int r, int c) {
    if ((r <= m_rows && r >= 1 ) && (c <= m_cols && c >= 1))
    {
        if (m_grid[r-1][c-1] == '.')
        {
            m_grid[r-1][c-1] = 'A';
            return true;
        }
        else if (m_grid[r-1][c-1] == 'Z')
        {
            return true;
        }
        else
        {
            m_grid[r-1][c-1]++;
            return true;
        }
    }
    else
        return false;
}

void History::display() const {
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
    
}

