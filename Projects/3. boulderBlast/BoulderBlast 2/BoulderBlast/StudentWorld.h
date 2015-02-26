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
        m_bonus = 1000;
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
    void decBonus(); 
    unsigned int getBonus() const {
        return m_bonus;
    }
    Actor* checkSpace(int x, int y, std::string search);
    
    void createBullet(int x, int y, GraphObject::Direction dir);
    
    std::vector<Actor*> getActors() {
        return m_actors;
    }
    
    bool canShoot(int x, int y, int dest, GraphObject::Direction dir, std::string check);
    
    void spawnKlepto(int x, int y);
    
    Player* getPlayer() {
        return m_player;
    }
    

    int	loadLevel();

private:
    std::vector<Actor*> m_actors;
    Player* m_player;
    unsigned int m_bonus;
    
    
};

#endif // STUDENTWORLD_H_
