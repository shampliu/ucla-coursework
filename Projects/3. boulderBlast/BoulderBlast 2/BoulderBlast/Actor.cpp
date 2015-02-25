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
                    m_ammo--;
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

/* Jewel
 ------------------------------ */
void Jewel::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if Player is on the same square as the Jewel
    if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) {
        getWorld()->increaseScore(50);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
    }
}

/* SnarlBot
 ------------------------------ */
void SnarlBot::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if SnarlBot is supposed to rest
    if (m_ticks < m_rest) {
        m_ticks++;
    }
    // decide whether to shoot or not
    else {
        m_ticks = 0;
        int px = getWorld()->getPlayer()->getX();
        int py = getWorld()->getPlayer()->getY();
        Direction dir = getDirection();
        
        // player is in the same column as the SnarlBot and SnarlBot
        if (px == getX()) {
            if (getWorld()->canShoot(getX(), getY(), py, dir)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }
        }
        // player is in the same row as the SnarlBot
        if (py == getY()) {
            if (getWorld()->canShoot(getX(), getY(), px, dir)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }

        }
    }
    
    // move or change direction if can't shoot
    
    
}

void SnarlBot::takeHit() {
    // killed
    if (isHit(2) == 1) {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_ROBOT_DIE);
    }
    // hurt but not dead
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

