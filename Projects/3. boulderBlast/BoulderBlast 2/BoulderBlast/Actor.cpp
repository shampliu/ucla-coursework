#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

/* Player 
 ------------------------------ */

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
            // canMove function will automatically change x & y if possible
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (canMove(x, y, left)) {
                    moveTo(x, y);
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (canMove(x, y, right)) {
                    moveTo(x, y);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (canMove(x, y, up)) {
                    moveTo(x, y);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (canMove(x, y, down)) {
                    moveTo(x, y);
                }
                break;
            case KEY_PRESS_SPACE:
                if (getAmmo() > 0) {
                    shoot();
                    
                }
                break;
            case KEY_PRESS_ESCAPE:
                break;
        }
    }
}

bool Player::canMove(int& x, int& y, Direction dir) {
    int dx = x;
    int dy = y;
    
    switch (dir) {
        case up:
            dy += 1;
            break;
        case right:
            dx += 1;
            break;
        case down:
            dy -= 1;
            break;
        case left:
            dx -= 1;
            break;
        case none:
            break;
    }
    
    string status = "";
    Actor* ap = getWorld()->checkSpace(dx, dy, status);
    
    // empty square
    if (ap == nullptr && status == "") {
        x = dx;
        y = dy;
        return true;
    }
    
    // boulder
    if (status == "boulder") {
        Boulder* b = dynamic_cast<Boulder*>(ap);
        
        // can push
        if (b->push(dir)) {
            x = dx;
            y = dy;
            return true;
        }
        
    }
    
    return false;
}

//bool Player::canMove(int x, int y) const {
//    if (x > VIEW_WIDTH || x < 0 || y > VIEW_HEIGHT || y < 0) {
//        return false;
//    }
//    
//    
//    std::vector<Actor*> actors = getWorld()->getActors();
//    for (auto actor : actors) {
//        if (actor->getX() == x && actor->getY() == y)
//        {
//            if (actor->canOccupy()) {
//                return true;
//            }
//            else {
//                Boulder* b = dynamic_cast<Boulder*>(actor);
//                if (b != nullptr) { // is a boulder
//                    
//                    
//                }
//                return false;
//            }
//        }
//    }
//    
//    return true;
//}


/* Dangerous Actor
 ------------------------------ */
void DangerousActor::shoot() {
    getWorld()->createBullet(getX(), getY(), getDirection());
    
}


/* Boulder
 ------------------------------ */
bool Boulder::push(Direction dir) {
    int x = getX();
    int y = getY();
    
    switch (dir) {
        case up:
            y += 1;
            break;
        case right:
            x += 1;
            break;
        case down:
            y -= 1;
            break;
        case left:
            x -= 1;
            break;
        case none:
            break;
    }
    
    string status = "";
    Actor* ap = getWorld()->checkSpace(x, y, status);
    
    // empty square
    if (ap == nullptr && status == "") {
        moveTo(x, y);
        return true;
        
    }
    
    if (ap != nullptr && status == "hole") {
        moveTo(x, y);
        ap->setDead();
        setDead();
        return true;
    }
    
    
    ////////
    return false; // take this out
    
}

/* Hole
 ------------------------------ */
void Hole::doSomething() {
    if (! isAlive()) {
        return;
    }

};


/* Bullet
 ------------------------------ */
void Bullet::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    int x = getX();
    int y = getY();
    
    string status = "";
    Actor* ap = getWorld()->checkSpace(x, y, status);
    
    if (ap != nullptr && ap->hittable() && status != "out of range") {
        ap->takeHit();
        setDead();
        return;
    }
    
    Direction dir = getDirection();
    switch (dir) {
        case up:
            y += 1;
            break;
        case right:
            x += 1;
            break;
        case down:
            y -= 1;
            break;
        case left:
            x -= 1;
            break;
        case none:
            break;
    }
    
    moveTo(x, y); 
}



