#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        
	}

	virtual int init()
	{
        int status = loadLevel();
        // if level data exists but isn't in the proper format
        if (status == -1) {
            return GWSTATUS_LEVEL_ERROR;
        }
        
//        
//        // if no level data or last level completed was lvl 99
//        if (1) {
//            return GWSTATUS_PLAYER_WON;
//        }
        
        
        
        
		return GWSTATUS_CONTINUE_GAME;
	}

    virtual int move();

	virtual void cleanUp() // free all actors when player completes current level or dies
	{
	}
    
    // defined functions
    int	loadLevel()
    {
        std::string	curLevel = "level03.dat";
        Level	lev(assetDirectory());
        Level::LoadResult result	=	lev.loadLevel(curLevel);
        if (result	==	Level::load_fail_file_not_found	||
            result	==	Level:: load_fail_bad_format)
            return -1; //	something	bad	happened!
        
        //	otherwise	the	load	was	successful	and	you	can	access	the
        //	contents
        int	x	=	0;
        int	y	=	5;
        Level::MazeEntry item =	lev.getContentsOf(x,	y);
        if	(item	==	Level::player)
            std::cout	<<	"The	player	should	be	placed	at	0,5	in	the	maze\n";
        x =	10;
        y =	7;
        item	=	lev.getContentsOf(x,	y);
        if	(item	==	Level::wall)
            std::cout	<<	"There	should	be	a	wall at	10,7	in	the	maze\n";
        
        
        return 1;
    }

private:
    std::vector<Actor*> m_actors;
    Player* m_player;
    
    
};

#endif // STUDENTWORLD_H_
