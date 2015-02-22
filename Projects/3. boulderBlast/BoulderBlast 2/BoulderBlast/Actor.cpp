#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <iostream>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    int key;
    if (getWorld()->getKey(key))
    {
        int x = getX();
        int y = getY();
        
        // user hit a key this tick!
        switch (key)
        {
            case KEY_PRESS_LEFT:
                //... move player to the left ...;
                setDirection(left);
                if (canMove(x - 1, y)) {
                    moveTo(x - 1, y);
                }
                break;
            case KEY_PRESS_RIGHT:
                //... move player to the right ...;
                setDirection(right);
                if (canMove(x + 1, y)) {
                    moveTo(x + 1, y);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (canMove(x, y + 1)) {
                    moveTo(x, y + 1);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (canMove(x, y - 1)) {
                    moveTo(x, y - 1);
                }
                break;
            case KEY_PRESS_SPACE:
                //... add a Bullet in the square in front of the Player...;
                break;
                // etc
            case KEY_PRESS_ESCAPE:
                break;
        }
    }
}

bool Player::canMove(int x, int y) const {
    if (x > VIEW_WIDTH || x < 0 || y > VIEW_HEIGHT || y < 0) {
        return false;
    }
    
    std::vector<Actor*> actors = getWorld()->getActors();
    for (auto actor : actors) {
        if (actor->getX() == x && actor->getY() == y)
        {
            if (actor->canOccupy()) {
                return true;
            }
            else { return false; }
        }
    }
    
    return true;
}