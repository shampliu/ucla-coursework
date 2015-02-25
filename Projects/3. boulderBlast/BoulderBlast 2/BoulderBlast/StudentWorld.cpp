#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
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

void StudentWorld::updateDisplay() {
    
    
    
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::move()
{
    // Update the Game Status Line
    updateDisplay(); // update the score/lives/level text at screen top
    // The term "actors" refers to all robots, the Player, Goodies,
    // Boulders, Jewels, Holes, Bullets, the Exit, etc.
    // Give each actor a chance to do something
    
    for (auto actor : m_actors)
    {
        if (m_player->isAlive()) {
            m_player->doSomething();
        }
        if (actor->isAlive())
        {
            // ask each actor to do something (e.g. move)
            actor->doSomething();
            if (! m_player->isAlive())
                return GWSTATUS_PLAYER_DIED;
//            if (thePlayerCompletedTheCurrentLevel())
//            {
//                increaseScoreAppropriately();
//                return GWSTATUS_FINISHED_LEVEL;
//            }
        }
    }
    // Remove newly-dead actors after each tick
    removeDeadGameObjects(); // delete dead game objects
    // Reduce the current bonus for the Level by one
//    reduceLevelBonusByOne();
//    // If the player has collected all of the Jewels on the level, then we
//    // must expose the Exit so the player can advance to the next level
//    if (thePlayerHasCollectedAllOfTheJewelsOnTheLevel())
//        exposeTheExitInTheMaze(); // make the exit Active
//    // return the proper result
//    if (! m_player->isAlive())
//        return GWSTATUS_PLAYER_DIED;
//    if (thePlayerCompletedTheCurrentLevel())
//    {
//        increaseScoreAppropriately();
//        return GWSTATUS_FINISHED_LEVEL;
//    }
    
    
    // the player hasn’t completed the current level and hasn’t died, so
    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
    return 0;
}

void StudentWorld::removeDeadGameObjects() {
    
    for (auto ap = m_actors.begin(); ap != m_actors.end(); )
    {
        if (!(*ap)->isAlive()) {
            delete (*ap);
            ap = m_actors.erase(ap);
        }
        else {
            ap++;
        }
    }
    
}

int StudentWorld::loadLevel() {
    
    string	curLevel = "level00.dat";
    Level lev(assetDirectory());
    
    // format level
    int level = getLevel();
    string formattedLevel;
    if (level % 10 > 0) {
        formattedLevel = "level" + to_string(level) + ".dat";
    }
    else {
        formattedLevel = "level0" + to_string(level) + ".dat";
    }
    
    Level::LoadResult result = lev.loadLevel(formattedLevel);
    if (result	==	Level::load_fail_file_not_found	||
        result	==	Level:: load_fail_bad_format)
        return -1; //	something	bad	happened!
    
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            Level::MazeEntry item =	lev.getContentsOf(x,y);
            switch (item) {
                case Level::wall: {
                    m_actors.push_back(new Wall(this, x, y));
                    break;
                }
                case Level::player:
                    m_player = new Player(this, x, y);
                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(this, x, y));
                    break;
                case Level::hole:
                    m_actors.push_back(new Hole(this, x, y));
                    break;
                case Level::exit:
                    m_actors.push_back(new Exit(this, x, y));
                    break;
                case Level::jewel:
                    m_actors.push_back(new Jewel(this, x, y));
                    break;
                case Level::ammo:
                    m_actors.push_back(new Ammo(this, x, y));
                    break;
                case Level::horiz_snarlbot:
                    m_actors.push_back(new SnarlBot(this, x, y, GraphObject::right));
                    break;
                case Level::vert_snarlbot:
                    m_actors.push_back(new SnarlBot(this, x, y, GraphObject::down));
                    break;
                default:
                    break;
            }
        }
    }
    
    return 1;
}

// free all actors when player completes current level or dies
void StudentWorld::cleanUp() {
    delete m_player;
    
    std::vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); ) {
        std::vector<Actor*>::iterator it2 = it;
        it = m_actors.erase(it);
        delete *it2;
    }
    
}

void StudentWorld::createBullet(int x, int y, GraphObject::Direction dir) {
    
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
    m_actors.push_back(new Bullet(this, x, y, dir));
    
}

Actor* StudentWorld::checkSpace(int x, int y) {
    
//    if (x > VIEW_WIDTH || x < 0 || y > VIEW_HEIGHT || y < 0) {
//        status = "out of range";
//        return nullptr;
//    }
    
    std::vector<Actor*> actors = getActors();
    for (auto actor : actors) {
        if (actor->getX() == x && actor->getY() == y)
        {
//            // check if actor is a Boulder
//            Boulder* b = dynamic_cast<Boulder*>(actor);
//            if (b != nullptr) {
//                status = "boulder";
//                return actor;
//            }
//            
//            // check if actor is a Hole
//            Hole* h = dynamic_cast<Hole*>(actor);
//            if (h != nullptr) {
//                status = "hole";
//                return actor;
//            }
            
            return actor;
        }
    }
    
//    status = "empty"; 
    return nullptr;
}

bool StudentWorld::canShoot(int x, int y, int dest, GraphObject::Direction dir) {
    switch (dir) {
        case GraphObject::up:
            // facing wrong way
            if (dest < y) { return false; }
            // start one square ahead of initial because the initial parameter is the actor's position
            for (int i = y+1; i < dest; i++) {
                
                Actor* ap = checkSpace(x, i);
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::right:
            if (dest < x) { return false; }
            for (int i = x+1; i < dest; i++) {
                
                Actor* ap = checkSpace(i, y);
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::down:
            if (dest > y) { return false; }
            for (int i = y-1; i > dest; i--) {
                
                Actor* ap = checkSpace(x, i);
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::left:
            if (dest > x) { return false; }
            for (int i = x-1; i > dest; i--) {
                
                Actor* ap = checkSpace(i, y);
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
    }
    
    return true; 
    
}

