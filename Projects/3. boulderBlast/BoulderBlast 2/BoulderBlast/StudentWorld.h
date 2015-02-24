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

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

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
    Actor* checkSpace(int x, int y, std::string& status);

    
    std::vector<Actor*> getActors() {
        return m_actors;
    }
    

    int	loadLevel();

private:
    std::vector<Actor*> m_actors;
    Player* m_player;
    
    
};

#endif // STUDENTWORLD_H_
