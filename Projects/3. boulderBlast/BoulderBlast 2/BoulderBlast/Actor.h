#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <cstdlib>

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
    // do nothing if is hit
    virtual void isHit(int damage) { };
    
    
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

class Factory : public Actor {
public:
    Factory(StudentWorld* world, int startX, int startY, bool angry) : Actor(world, IID_ROBOT_FACTORY, startX, startY, none) {
        m_angry = angry;
    };
    
    virtual void doSomething();
    virtual bool canOccupy() {
        return false;
    };
    
    bool countRegion();
    
    // either implement one function to check or loop thru robots array
private:
    
    
private:
    bool m_angry;
    std::vector<Actor*> m_robots;
    
};

// can be taken by KleptoBot
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
    
    void toggleVisible() {
        m_visible == true ? m_visible = false : m_visible = true;
    }
    bool isVisible() const { return m_visible; };
    
private:
    bool m_visible;
    
};

class Health : public Goodie {
public:
    Health(StudentWorld* world, int startX, int startY) : Goodie(world, IID_RESTORE_HEALTH, startX, startY) { };
    virtual void doSomething();
};

class Life : public Goodie {
public:
    Life(StudentWorld* world, int startX, int startY) : Goodie(world, IID_EXTRA_LIFE, startX, startY) { };
    virtual void doSomething();
};

class Ammo : public Goodie {
public:
    Ammo(StudentWorld* world, int startX, int startY) : Goodie(world, IID_AMMO, startX, startY) { };
    virtual void doSomething();
};

// Actor with hitpoints
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
    
    int getHealth() {
        return m_health;
    }
    
    void changeHealth(int amt) {
        m_health += amt;
    }
    
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
    
    bool push(Direction dir);
    
};

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
    
    bool canOccupy() {
        return false; 
    }
    
    int getJewels() {
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

// these guys hurt
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
    
    void isHit(int damage); 
    
    virtual void doSomething() = 0;

    
private:
    int m_ticks;
    int m_rest;
    
};


class SnarlBot : public Enemy {
public:
    SnarlBot(StudentWorld* world, int startX, int startY, Direction dir) : Enemy(10, world, IID_SNARLBOT, startX, startY, dir) { };
    
    void shoot() {
        getWorld()->createBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
    }
    virtual void doSomething();
    
};

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
    
    void toggleBlocked() {
        m_blocked == true ? m_blocked = false : m_blocked = true;
    }
    
    void resetDist() {
        m_maxDist = rand() % 6 + 1;
    }
    
    virtual void doSomething();
    
    virtual void isHit(int damage);
    
    
private:
    int m_maxDist;
    bool m_blocked;
    Goodie* m_item;
};

class AngryKleptoBot : public KleptoBot {
public:
    AngryKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(8, world, startX, startY) { };
    
    virtual void doSomething();
    
    void shoot() {
        getWorld()->createBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
    }
};


#endif // ACTOR_H_
