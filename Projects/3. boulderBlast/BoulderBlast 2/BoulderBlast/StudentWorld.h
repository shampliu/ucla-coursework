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
    
    ~StudentWorld() {
        cleanUp(); 
    }
    
    void updateDisplay();
    

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    void removeDeadGameObjects();
    
    Actor* checkSpace(int x, int y);
    
    void createBullet(int x, int y, GraphObject::Direction dir);

    
    std::vector<Actor*> getActors() {
        return m_actors;
    }
    
    bool canShoot(int x, int y, int dest, GraphObject::Direction dir);
    
    
    Player* getPlayer() {
        return m_player;
    }
    

    int	loadLevel();

private:
    std::vector<Actor*> m_actors;
    Player* m_player;
    
    
};

#endif // STUDENTWORLD_H_
