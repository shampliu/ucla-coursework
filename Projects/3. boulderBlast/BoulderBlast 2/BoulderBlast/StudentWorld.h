#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>

class Actor;
class Player;
class Level;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}
    
    std::string formatDisplay(int score, int level, int lives, int health, int ammo, unsigned int bonus);
    
    ~StudentWorld() {
        cleanUp(); 
    }
    
    void updateDisplay();
    
    void insert(Actor* a);

    virtual int init();

    virtual int move();
    
    virtual void cleanUp();
    
    void removeDeadGameObjects();
    
    void completed() {
        m_completed = true;
    }
    
    int getJewels() {
        return m_jewels; 
    }
    
    Actor* checkSpace(int x, int y, std::string search);
    
    void createBullet(int x, int y, GraphObject::Direction dir);
    
    std::vector<Actor*> getActors() {
        return m_actors;
    }
    
    bool canShoot(int x, int y, int dest, GraphObject::Direction dir, std::string check);
    
    Player* getPlayer() {
        return m_player;
    }
    

    int	loadLevel();

private:
    std::vector<Actor*> m_actors;
    Player* m_player;
    unsigned int m_bonus;
    int m_jewels;
    bool m_completed;
    
    
};

#endif // STUDENTWORLD_H_
