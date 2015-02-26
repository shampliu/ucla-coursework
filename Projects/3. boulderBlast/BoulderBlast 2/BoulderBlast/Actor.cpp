#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <iostream>
#include <string>
#include "GraphObject.h"
#include <cstdlib>
#include <map>

using namespace std;

/* Actor
 ------------------------------ */
void Actor::convertDir(int& x, int& y, GraphObject::Direction dir) {
    switch (dir) {
        case GraphObject::up:
            y += 1;
            break;
        case GraphObject::right:
            x += 1;
            break;
        case GraphObject::down:
            y -= 1;
            break;
        case GraphObject::left:
            x -= 1;
            break;
        case GraphObject::none:
            break;
    }
}

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
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (canMove(x, y, left)) {
                    moveTo(x-1, y);
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (canMove(x, y, right)) {
                    moveTo(x+1, y);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (canMove(x, y, up)) {
                    moveTo(x, y+1);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (canMove(x, y, down)) {
                    moveTo(x, y-1);
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
    
    convertDir(dx, dy, dir);
    
    Actor* ap = getWorld()->checkSpace(dx, dy, "");
    
    // empty square or object that can be occupied
    if (ap == nullptr || ap->canOccupy()) {
        return true;
    }
    
    // boulder
    Boulder* b = dynamic_cast<Boulder*>(ap);
    if (b != nullptr) {
        // can push
        if (b->push(dir)) {
            return true;
        }
    }
    
    return false;
}

/* Boulder
 ------------------------------ */
bool Boulder::push(Direction dir) {
    int dx = getX();
    int dy = getY();
    
    convertDir(dx, dy, dir);
    
    Actor* ap = getWorld()->checkSpace(dx, dy, "");
    
    // empty square
    if (ap == nullptr) {
        moveTo(dx, dy);
        return true;
        
    }
    
    // hole
    Hole* h = dynamic_cast<Hole*>(ap);
    if (h != nullptr) {
        moveTo(dx, dy);
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
    
    // removal of Hole is managed by Boulder class
};


/* Bullet
 ------------------------------ */
void Bullet::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    int dx = getX();
    int dy = getY();
    
    Actor* ap = getWorld()->checkSpace(dx, dy, "living");
    if (ap != nullptr) {
        ap->isHit(2);
        setDead();
        return;
    }
    
    ap = getWorld()->checkSpace(dx, dy, "");
    if (ap != nullptr && ap->hittable()) {
        ap->isHit(2);
        setDead();
        return;
    }
    
    convertDir(dx, dy, getDirection());
    
    moveTo(dx, dy);
}

/* Exit
 ------------------------------ */
void Exit::doSomething() {
    if (getWorld()->getJewels() == getWorld()->getPlayer()->getJewels()) {
        m_visible = true; 
        setVisible(true);
    }
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && m_visible) {
        getWorld()->completed(); 
    }
}

/* Jewel
 ------------------------------ */
void Jewel::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if Player is on the same square as the Jewel and Jewel is visible
    if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) {
        getWorld()->increaseScore(50);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->incJewels();
        setDead();
    }
}

/* Ammo
 ------------------------------ */
void Ammo::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if Player is on the same square as the Ammo
    if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible()) {
        getWorld()->increaseScore(100);
        getWorld()->getPlayer()->reload(20);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
    }
}

/* Health
 ------------------------------ */
void Health::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if Player is on the same square as the Health
    if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible()) {
        getWorld()->increaseScore(500);
        int inc = 20 - getWorld()->getPlayer()->getHealth();
        getWorld()->getPlayer()->changeHealth(inc);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
    }
}

/* Life
 ------------------------------ */
void Life::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    // if Player is on the same square as the Health
    if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible()) {
        getWorld()->incLives();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
    }
}

/* Enemy
 ------------------------------ */
void Enemy::isHit(int damage) {
    // killed
    changeHealth(damage * -1);
    
    if (getHealth() <= 0) {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
    }
    // hurt but not dead
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

/* SnarlBot
 ------------------------------ */
void SnarlBot::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    int dx = getX();
    int dy = getY();
    Direction dir = getDirection();
    
    // if SnarlBot is supposed to rest
    if (getTicks() < getRest()) {
        setTicks();
    }
    // decide whether to shoot or not
    else {
        setTicks();
        int px = getWorld()->getPlayer()->getX();
        int py = getWorld()->getPlayer()->getY();
        
        string check;
        
        // player is in the same column as the SnarlBot and SnarlBot
        if (px == getX()) {
            check = "v";
            if (getWorld()->canShoot(dx, dy, py, dir, check)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }
        }
        // player is in the same row as the SnarlBot
        if (py == getY()) {
            check = "h"; 
            if (getWorld()->canShoot(dx, dy, px, dir, check)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }

        }
        
        // move or change direction if can't shoot
        convertDir(dx, dy, dir);
        
        Actor* ap = getWorld()->checkSpace(dx, dy, "");
        
        // empty square or object that can be occupied
        if (ap == nullptr || ap->canOccupy()) {
            moveTo(dx, dy);
        }
        // change direction
        else {
            switch (dir) {
                case up:
                    setDirection(down);
                    break;
                case right:
                    setDirection(left);
                    break;
                case down:
                    setDirection(up);
                    break;
                case left:
                    setDirection(right);
                    break;
            }
        }
    }
}

/* Factory
 ------------------------------ */
bool Factory::countRegion() {
    int count = 0;
    int x = getX();
    int y = getY();
    
    for (auto robot : m_robots) {
        int rx = robot->getX();
        int ry = robot->getY();
        
        if (rx == x && ry == y) {
            return false;
        }

        if ((rx <= x+3 || rx >= x-3) && (ry <= y+3 || ry >= y-3)) {
            count++;
        }
    }
    
    if (count < 3) { return true; }
    return false;
}

void Factory::doSomething() {

    if (countRegion()) {
        // 1/50 chance of it being 1, then spawn the kleptobot
        if (rand() % 50 == 1) {
            if (m_angry) {
                AngryKleptoBot* k = new AngryKleptoBot(getWorld(), getX(), getY());
                getWorld()->insert(k);
                m_robots.push_back(k);
            }
            else {
                KleptoBot* k = new KleptoBot(5, getWorld(), getX(), getY());
                getWorld()->insert(k);
                m_robots.push_back(k);
            }
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}

/* KleptoBot
 ------------------------------ */
void KleptoBot::isHit(int damage) {
    changeHealth(damage * -1);
    if (getHealth() <= 0) {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        setDead();
        if (m_item != nullptr) {
            m_item->toggleVisible();
            m_item->setVisible(true);
        }
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
};

void KleptoBot::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    int dx = getX();
    int dy = getY();
    Direction dir = getDirection();
    
    // if KleptoBot is supposed to rest
    if (getTicks() < getRest()) {
        setTicks();
    }
    
    else {
        setTicks();
        
        // check for Goodie on the same square
        Actor* ap = getWorld()->checkSpace(dx, dy, "goodie");
        if (ap != nullptr) {
            if (rand() % 10 == 0) {
                m_item = dynamic_cast<Goodie*>(ap);
                m_item->setVisible(false);
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
            }
        }
        
        // check if it has moved the max distance
        if (m_maxDist > 0 && m_blocked == false) {
            m_maxDist--;
            
            // move or change direction if can't shoot
            convertDir(dx, dy, dir);
            
            Actor* ap = getWorld()->checkSpace(dx, dy, "");
            
            // empty square or object that can be occupied
            if (ap == nullptr || ap->canOccupy()) {
                moveTo(dx, dy);
                if (m_item != nullptr) {
                    m_item->moveTo(dx, dy);
                }
                return;
            }
            // encountered obstruction
            else {
                m_blocked = true;
            }
        }
        // change direction
        else {
            m_blocked = false;
            
            // reset max distance
            resetDist();
            map<int, bool> check;
            int count = 0;
            
            // count will check all four directions
            while (count < 4) {
                int rng = rand() % 4;
                
                if (check.find(rng)->second == true) {
                    continue;
                }
                else {
                    check[rng] = true;
                    Direction dir;
                    switch (rng) {
                        case 0:
                            dir = up;
                            break;
                        case 1:
                            dir = right;
                            break;
                        case 2:
                            dir = down;
                            break;
                        case 3:
                            dir = left;
                            break;
                    }
                    convertDir(dx, dy, dir);
                    Actor* ap = getWorld()->checkSpace(dx, dy, "");
                    
                    if (ap == nullptr || ap->canOccupy()) {
                        setDirection(dir);
                        return;
                    }
                    count++;
                }
            }
        }
    }
}

/* Angry KleptoBot
 ------------------------------ */
void AngryKleptoBot::doSomething() {
    if (! isAlive()) {
        return;
    }
    
    int dx = getX();
    int dy = getY();
    Direction dir = getDirection();
    
    // if KleptoBot is supposed to rest
    if (getTicks() < getRest()) {
        setTicks();
    }
    
    else {
        setTicks();
        
        int px = getWorld()->getPlayer()->getX();
        int py = getWorld()->getPlayer()->getY();
        
        string check;
        
        // player is in the same column as the SnarlBot and SnarlBot
        if (px == getX()) {
            check = "v";
            if (getWorld()->canShoot(dx, dy, py, dir, check)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }
        }
        // player is in the same row as the SnarlBot
        if (py == getY()) {
            check = "h";
            if (getWorld()->canShoot(dx, dy, px, dir, check)) {
                shoot();
                getWorld()->playSound(SOUND_ENEMY_FIRE);
                return;
            }
            
        }
        
        // check for Goodie on the same square
        Actor* ap = getWorld()->checkSpace(dx, dy, "goodie");
        if (ap != nullptr) {
            if (rand() % 10 == 0) {
                addItem(dynamic_cast<Goodie*>(ap));
                getItem()->setVisible(false);
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
            }
        }
        
        // check if it has moved the max distance
        if (getDist() > 0 && !isBlocked()) {
            changeDist(-1);
            
            // move or change direction if can't shoot
            convertDir(dx, dy, dir);
            
            Actor* ap = getWorld()->checkSpace(dx, dy, "");
            
            // empty square or object that can be occupied
            if (ap == nullptr || ap->canOccupy()) {
                moveTo(dx, dy);
                if (getItem() != nullptr) {
                    getItem()->moveTo(dx, dy);
                }
                return;
            }
            // encountered obstruction
            else {
                toggleBlocked();
            }
        }
        // change direction
        else {
            toggleBlocked();
            
            // reset max distance
            resetDist();
            map<int, bool> check;
            int count = 0;
            
            // count will check all four directions
            while (count < 4) {
                int rng = rand() % 4;
                
                if (check.find(rng)->second == true) {
                    continue;
                }
                else {
                    check[rng] = true;
                    Direction dir;
                    switch (rng) {
                        case 0:
                            dir = up;
                            break;
                        case 1:
                            dir = right;
                            break;
                        case 2:
                            dir = down;
                            break;
                        case 3:
                            dir = left;
                            break;
                    }
                    convertDir(dx, dy, dir);
                    Actor* ap = getWorld()->checkSpace(dx, dy, "");
                    
                    if (ap == nullptr || ap->canOccupy()) {
                        setDirection(dir);
                        return;
                    }
                    count++;
                }
            }
        }
    }
}






