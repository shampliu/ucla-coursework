#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething() {
    
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                //... move player to the left ...;
                break;
            case KEY_PRESS_RIGHT:
                //... move player to the right ...;
                break;
            case KEY_PRESS_SPACE:
                //... add a Bullet in the square in front of the Player...;
                break;
                // etc  
        }
    }
}