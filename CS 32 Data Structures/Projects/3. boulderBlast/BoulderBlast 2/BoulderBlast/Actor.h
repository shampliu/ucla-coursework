#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <cstdlib>

/* Actor

 character with no hitpoints
 
 ------------------------------ */
class Actor: public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = none) : GraphObject(imageID, startX, startY, dir) {
        setVisible(true);
        m_isAlive = true;
        m_world = world;
    };
    
    void convertDir(int& x, int& y, Direction dir);
    
    virtual ~Actor() { };
    
    virtual void doSomething() = 0;
    virtual bool canOccupy(){
        return false;
    }
    virtual bool hittable() {
        return true;
    }
    // do nothing if is hit
    virtual void isHit(int damage) { };
    
    
    StudentWorld* getWorld() const { return m_world; }
    
    void setDead() { m_isAlive = false; }
    
    bool isAlive() const { return m_isAlive; }
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
    
};

/* Wall
 ------------------------------ */
class Wall : public Actor {
public:
    Wall(StudentWorld* world, int startX, int startY) : Actor(world, IID_WALL, startX, startY, none) { };
    
    ~Wall() { };
    
    virtual void doSomething() { };
    
private:
    
};

/* Hole
 ------------------------------ */
class Hole : public Actor {
public:
    Hole(StudentWorld* world, int startX, int startY) : Actor(world, IID_HOLE, startX, startY, none) { };
    
    virtual void doSomething() { };
    
    virtual bool hittable() {
        return false;
    }
};

/* Bullet
 ------------------------------ */
class Bullet : public Actor {
public:
    Bullet(StudentWorld* world, int startX, int startY, Direction dir) : Actor(world, IID_BULLET, startX, startY, dir) { };
    
    virtual void doSomething();
    virtual bool canOccupy() {
        return true;
    }
    
    virtual bool hittable() {
        return false;
    }
};

/* Exit
 ------------------------------ */
class Exit : public Actor {
public:
    Exit(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXIT, startX, startY, none) {
        m_visible = false;
        setVisible(false);
    };
    
    virtual void doSomething();
    virtual bool canOccupy() {
        return true;
    };
    virtual bool hittable() {
        return false;
    }
    
private:
    bool m_visible;
};

/* Jewel
 ------------------------------ */
class Jewel : public Actor {
public:
    Jewel(StudentWorld* world, int startX, int startY) : Actor(world, IID_JEWEL, startX, startY, none) { };
    
    virtual void doSomething();
    virtual bool canOccupy() {
        return true;
    };
    virtual bool hittable() {
        return false;
    }
};

/* Factory
 ------------------------------ */
class Factory : public Actor {
public:
    Factory(StudentWorld* world, int startX, int startY, bool angry) : Actor(world, IID_ROBOT_FACTORY, startX, startY, none) {
        m_angry = angry;
    };
    
    virtual void doSomething();
    
    bool countRegion();
private:
    
    
private:
    bool m_angry;
    std::vector<Actor*> m_robots;
    
};

/* Goodie
 
 can be taken by a KleptoBot
 
 ------------------------------ */
class Goodie : public Actor {
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, none) {
        m_visible = true;
    };
    
    virtual void doSomething() = 0;
    virtual bool canOccupy() {
        return true;
    };
    virtual bool hittable() {
        return false;
    }
    
    void setV(bool b) {
        m_visible = b;
    }
    bool isVisible() const { return m_visible; };
    
private:
    bool m_visible;
    
};

/* Health
 ------------------------------ */
class Health : public Goodie {
public:
    Health(StudentWorld* world, int startX, int startY) : Goodie(world, IID_RESTORE_HEALTH, startX, startY) { };
    virtual void doSomething();
};

/* Life
 ------------------------------ */
class Life : public Goodie {
public:
    Life(StudentWorld* world, int startX, int startY) : Goodie(world, IID_EXTRA_LIFE, startX, startY) { };
    virtual void doSomething();
};

/* Ammo
 ------------------------------ */
class Ammo : public Goodie {
public:
    Ammo(StudentWorld* world, int startX, int startY) : Goodie(world, IID_AMMO, startX, startY) { };
    virtual void doSomething();
};

/* LivingActor
 
 actor with hitpoints
 
 ------------------------------ */
class LivingActor : public Actor {
public:
    LivingActor(int health, StudentWorld* world, int imageID, int startX, int startY, Direction dir = none) : Actor(world, imageID, startX, startY, dir), m_health(health) {

        
    };
    
    virtual void isHit(int damage) {
        changeHealth(damage * -1);
        if (m_health <= 0) {
            setDead();
        }
    };
    
    int getHealth() const { return m_health; }
    
    void changeHealth(int amt) { m_health += amt; }
    
    virtual void doSomething() = 0;
    
private:
    int m_health;
};

/* Boulder
 ------------------------------ */
class Boulder : public LivingActor {
    
public:
    Boulder(StudentWorld* world, int startX, int startY) : LivingActor(10, world, IID_BOULDER, startX, startY, none) { };
    
    virtual void doSomething() { };
    
    bool push(Direction dir);
    
};

/* Player
 ------------------------------ */
class Player : public LivingActor {
    
public:
    Player(StudentWorld* world, int startX, int startY) : LivingActor(20, world, IID_PLAYER, startX, startY, right), m_ammo(20), m_jewels(0) { };
    
    virtual void doSomething();
    
    
    int getAmmo() const {
        return m_ammo;
    }
    
    void reload(int amt) {
        m_ammo += amt; 
    }
    
    int getJewels() const {
        return m_jewels;
    }
    void incJewels() {
        m_jewels++;
    }
    
    void isHit(int damage); 
    
    void shoot() {
        getWorld()->createBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_PLAYER_FIRE);
    }
    
    bool canMove(int& x, int& y, Direction dir);
    
private:
    int m_ammo;
    int m_jewels;
};

/* Enemy
 
 the bad guys
 
 ------------------------------ */
class Enemy : public LivingActor {
public:
    Enemy(int health, StudentWorld* world, int imageID, int startX, int startY, Direction dir) : LivingActor(health, world, imageID, startX, startY, dir) {
        m_ticks = 0;
        int ticks = (28 - getWorld()->getLevel()) / 4;
        ticks < 3 ? m_rest = 3 : m_rest = ticks;
    };
    
    int getRest() const { return m_rest; }
    int getTicks() const { return m_ticks; }
    void setTicks() {
        (m_ticks < m_rest) ? m_ticks++ : m_ticks = 0;
    }
    
    void isHit(int damage) = 0;
    
    virtual void doSomething() = 0;

    
private:
    int m_ticks;
    int m_rest;
    
};

/* SnarlBot
 ------------------------------ */
class SnarlBot : public Enemy {
public:
    SnarlBot(StudentWorld* world, int startX, int startY, Direction dir) : Enemy(10, world, IID_SNARLBOT, startX, startY, dir) { };
    
    
    virtual void isHit(int damage);
    void shoot() {
        getWorld()->createBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
    }
    virtual void doSomething();
    
};

/* KleptoBot
 ------------------------------ */
class KleptoBot : public Enemy {
public:
    KleptoBot(int health, StudentWorld* world, int startX, int startY) : Enemy(health, world, IID_KLEPTOBOT, startX, startY, right) {
        m_maxDist = rand() % 6 + 1;
        m_item = nullptr;
        m_blocked = false;
    };
    
    Goodie* getItem() {
        return m_item;
    }
    
    void addItem(Goodie* g) {
        m_item = g; 
    }
    
    int getDist() {
        return m_maxDist;
    }
    
    void changeDist(int change) {
        m_maxDist += change; 
    }
    
    bool isBlocked() {
        return m_blocked;
    }
    
    void setBlocked(bool b) {
        m_blocked = b;
    }
    
    void resetDist() {
        m_maxDist = rand() % 6 + 1;
    }
    
    virtual void shoot() { };
    
    virtual bool isAngry() { return false; }
    
    
    
    virtual void doSomething();
    
    virtual void isHit(int damage);
    
    
private:
    int m_maxDist;
    bool m_blocked;
    Goodie* m_item;
};

/* AngryKleptoBot
 ------------------------------ */
class AngryKleptoBot : public KleptoBot {
public:
    AngryKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(8, world, startX, startY) { };
    
    virtual void isHit(int damage);
    virtual bool isAngry() { return true; }
    virtual void shoot() {
        getWorld()->createBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
    }
};


#endif // ACTOR_H_
