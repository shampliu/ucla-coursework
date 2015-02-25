#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"

// Actor without hitpoints
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
    virtual bool canOccupy() = 0;
    virtual bool hittable() {
        return true;
    }
    virtual void takeHit() { };
    
    
    StudentWorld* getWorld() const {
        return m_world;
    }
    
    void setDead() {
        m_isAlive = false;
    }
    
    bool isAlive() const {
        return m_isAlive;
    }
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
    
    
    
};

class Wall : public Actor {
public:
    Wall(StudentWorld* world, int startX, int startY) : Actor(world, IID_WALL, startX, startY, none) { };
    
    ~Wall() { };
    
    virtual void doSomething() { };
    virtual bool canOccupy() {
        return false;
    }
    
private:
    
};

class Hole : public Actor {
public:
    Hole(StudentWorld* world, int startX, int startY) : Actor(world, IID_HOLE, startX, startY, none) { };
    
    virtual void doSomething();
    
    virtual bool canOccupy() {
        return false;
    }
    
    virtual bool hittable() {
        return false;
    }
};

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

class Exit : public Actor {
public:
    Exit(StudentWorld* world, int startX, int startY) : Actor(world, IID_EXIT, startX, startY, none) {
        
        setVisible(false);
    };
    
    virtual void doSomething() { };
    virtual bool canOccupy() {
        return true;
    };
    virtual bool hittable() {
        return false;
    }
    
};

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

class Ammo : public Actor {
public:
    Ammo(StudentWorld* world, int startX, int startY) : Actor(world, IID_AMMO, startX, startY, none) { };
    virtual void doSomething();
    virtual bool canOccupy() {
        return true;
    };
    virtual bool hittable() {
        return false;
    }
};

// Actor with hitpoints
class LivingActor : public Actor {
public:
    LivingActor(int health, StudentWorld* world, int imageID, int startX, int startY, Direction dir = none) : Actor(world, imageID, startX, startY, dir), m_health(health) {

        
    };
    
    // return value of 1 means the actor has been killed
    virtual int isHit(int damage) {
        m_health -= damage;
        if (m_health <= 0) {
            setDead();
            return 1;
        }
        return 0;
    };
    
    virtual bool canOccupy() {
        return false;
    };
    
    virtual void doSomething() = 0;
    
    
    
    
private:
    int m_health;
};

class Boulder : public LivingActor {
    
public:
    Boulder(StudentWorld* world, int startX, int startY) : LivingActor(10, world, IID_BOULDER, startX, startY, none) { };
    
    virtual void doSomething() { };
    
    virtual bool canOccupy() {
        return false;
    }
    
    virtual void takeHit() {
        isHit(2); 
    }
    
    bool push(Direction dir);
    
};

// LivingActor with gun
class DangerousActor : public LivingActor {
public:
    DangerousActor(int health, StudentWorld* world, int imageID, int startX, int startY, Direction dir) : LivingActor(health, world, imageID, startX, startY, dir) { };
    
    void shoot();
    
};

class Player : public DangerousActor {
    
public:
    Player(StudentWorld* world, int startX, int startY) : DangerousActor(20, world, IID_PLAYER, startX, startY, right), m_ammo(20) { };
    
    virtual void doSomething();
    
    
    int getAmmo() const {
        return m_ammo;
    }
    
    void reload(int amt) {
        m_ammo += amt; 
    }
    
    virtual void takeHit() {
        isHit(2);
    }
    
    bool canMove(int& x, int& y, Direction dir);
    
private:
    int m_ammo;
};

class SnarlBot : public DangerousActor {
public:
    SnarlBot(StudentWorld* world, int startX, int startY, Direction dir) : DangerousActor(10, world, IID_SNARLBOT, startX, startY, dir) {
        m_ticks = 0;
        
        
        int ticks = (28 - getWorld()->getLevel()) / 4;
        ticks < 3 ? m_rest = 3 : m_rest = ticks;
    };
    virtual void doSomething();

    virtual void takeHit();

    
private:
    int m_ticks;
    int m_rest;
    
};

class KleptoBot : public LivingActor {
    
};


#endif // ACTOR_H_
