//
//  Pit.cpp
//  snakePit
//
//  Created by Chang Liu on 1/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include <iostream>
#include "Pit.h"
#include "globals.h"
#include "Snake.h"
#include "Player.h"

using namespace std;

Pit::Pit(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Pit created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nSnakes = 0;
    m_history = new History(nRows, nCols);
}

Pit::~Pit()
{
    for (int k = 0; k < m_nSnakes; k++)
        delete m_snakes[k];
    delete m_player;
}

int Pit::rows() const
{
    return m_rows;
}

int Pit::cols() const
{
    return m_cols;
}

Player* Pit::player() const
{
    return m_player;
}

int Pit::snakeCount() const
{
    return m_nSnakes;
}

int Pit::numberOfSnakesAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nSnakes; k++)
    {
        const Snake* sp = m_snakes[k];
        if (sp->row() == r  &&  sp->col() == c)
            count++;
    }
    return count;
}

void Pit::display(string msg) const
{
    // Position (row,col) in the pit coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';
    
    // Indicate each snake's position
    for (int k = 0; k < m_nSnakes; k++)
    {
        const Snake* sp = m_snakes[k];
        char& gridChar = grid[sp->row()-1][sp->col()-1];
        switch (gridChar)
        {
            case '.':  gridChar = 'S'; break;
            case 'S':  gridChar = '2'; break;
            case '9':  break;
            default:   gridChar++; break;  // '2' through '8'
        }
    }
    
    // Indicate player's position
    if (m_player != nullptr)
    {
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (m_player->isDead())
            gridChar = '*';
        else
            gridChar = '@';
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
    // Write message, snake, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << snakeCount() << " snakes remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Pit::addSnake(int r, int c)
{
    // Dynamically allocate a new Snake and add it to the pit
    if (m_nSnakes == MAXSNAKES)
        return false;
    m_snakes[m_nSnakes] = new Snake(this, r, c);
    m_nSnakes++;
    return true;
}

bool Pit::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Dynamically allocate a new Player and add it to the pit
    m_player = new Player(this, r, c);
    return true;
}

bool Pit::destroyOneSnake(int r, int c)
{
    for (int k = 0; k < m_nSnakes; k++)
    {
        if (m_snakes[k]->row() == r  &&  m_snakes[k]->col() == c)
        {
            delete m_snakes[k];
            m_snakes[k] = m_snakes[m_nSnakes-1];
            m_nSnakes--;
            return true;
        }
    }
    return false;
}

bool Pit::moveSnakes()
{
    for (int k = 0; k < m_nSnakes; k++)
    {
        Snake* sp = m_snakes[k];
        sp->move();
        if (sp->row() == m_player->row()  &&  sp->col() == m_player->col())
            m_player->setDead();
    }
    
    // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

void Pit::displayHistory() const {
    m_history->display(); 
}

History& Pit::history() {
    return *m_history;
}