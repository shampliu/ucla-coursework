#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    int key;
    if (getWorld()->getKey(key))
    {
        // user hit a key this tick!
        switch (key)
        {
            case KEY_PRESS_LEFT:
                //... move player to the left ...;
                setDirection(left);
                if (canMove(getX() - 1, getY())) {
                    moveTo(getX() - 1, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                //... move player to the right ...;
                setDirection(right);
                if (canMove(getX() + 1, getY())) {
                    moveTo(getX() + 1, getY());
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (canMove(getX(), getY()) + 1) {
                    moveTo(getX(), getY() + 1);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (canMove(getX(), getY()) - 1) {
                    moveTo(getX(), getY() - 1);
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
    if (x > 15 || x < 0 || y > 15 || y < 0) {
        return false;
    }
    
    std::vector<Actor*> actors = getWorld()->getActors();
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getX() == x && actors[i]->getY() == y)
        {
            if (actors[i]->canOccupy()) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    
    return true;
    
    
    
    
    return false;
}