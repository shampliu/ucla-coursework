#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

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
    
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = getPlayer()->getHealth();
    int ammo = getPlayer()->getAmmo();
    unsigned int bonus = m_bonus;
    // Next, create a string from your statistics, of the form:
    // Score: 0000100 Level: 03 Lives: 3 Health: 70% Ammo: 216 Bonus: 34
    
    string s = formatDisplay(score, level, lives, health, ammo, bonus);
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatDisplay(int score, int level, int lives, int health, int ammo, unsigned int bonus) {
    ostringstream l;
    l.fill('0');
    l << setw(2) << level;
    string lvl = l.str();
    
    ostringstream s;
    s.fill('0');
    s << setw(7) << score;
    string scr = s.str();
    
    return "Score: " + scr + " Level: " + lvl + " Lives: " + to_string(lives) + " Health: " + to_string(health*5) + "% Ammo: " + to_string(ammo) + " Bonus " + to_string(bonus);
    
}

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
        }
        if (! m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_completed)
        {
            playSound(SOUND_FINISHED_LEVEL);
            increaseScore(2000 + m_bonus);
            m_completed = false;
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    // Remove newly-dead actors after each tick
    removeDeadGameObjects(); // delete dead game objects
    // Reduce the current bonus for the Level by one
    if (m_bonus > 0) {
        m_bonus--;
    }
    
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
    
    Level lev(assetDirectory());
    
    // format level
    int level = getLevel();
    ostringstream l;
    l.fill('0');
    l << setw(2) << level;
    string formattedLevel = "level" + l.str() + ".dat";
    
    cout << level;
    
    
    Level::LoadResult result = lev.loadLevel(formattedLevel);
    if (result	==	Level::load_fail_file_not_found	||
        result	==	Level:: load_fail_bad_format)
        return -1; //	something	bad	happened!
    
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            Level::MazeEntry item =	lev.getContentsOf(x,y);
            switch (item) {
                case Level::player:
                    m_player = new Player(this, x, y);
                    break;
                case Level::wall: {
                    m_actors.push_back(new Wall(this, x, y));
                    break;
                }
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
                    m_jewels++; 
                    break;
                case Level::restore_health:
                    m_actors.push_back(new Health(this, x, y));
                    break;
                case Level::extra_life:
                    m_actors.push_back(new Life(this, x, y));
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
                case Level::kleptobot_factory:
                    m_actors.push_back(new Factory(this, x, y, false));
                    break;
                case Level::angry_kleptobot_factory:
                    m_actors.push_back(new Factory(this, x, y, true));
                    break;
                default:
                    break;
            }
        }
    }
    
    return 1;
}

void StudentWorld::insert(Actor* a) {
    m_actors.push_back(a);
}


// free all actors when player completes current level or dies
void StudentWorld::cleanUp() {
    delete m_player;
    
    auto ap = m_actors.begin();
    while (ap != m_actors.end())
    {
        delete *ap;
        ap = m_actors.erase(ap);
    }
    
//    std::vector<Actor*>::iterator it;
//    for (it = m_actors.begin(); it != m_actors.end(); ) {
//        std::vector<Actor*>::iterator it2 = it;
//        it = m_actors.erase(it);
//        delete *it2;
//    }
    
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

Actor* StudentWorld::checkSpace(int x, int y, string search) {
    
//    if (x > VIEW_WIDTH || x < 0 || y > VIEW_HEIGHT || y < 0) {
//        status = "out of range";
//        return nullptr;
//    }
    
    std::vector<Actor*> actors = getActors();
    for (auto actor : actors) {
        if (actor->getX() == x && actor->getY() == y)
        {
            if (search == "kleptobot") {
                KleptoBot* k = dynamic_cast<KleptoBot*>(actor);
                if (k != nullptr) {
                    return k;
                }
            }
            else if (search == "goodie") {
                Goodie* g = dynamic_cast<Goodie*>(actor);
                if (g != nullptr) {
                    return g; 
                }
            }
            else if (search == "living") {
                LivingActor* l = dynamic_cast<LivingActor*>(actor);
                if (l != nullptr) {
                    return l; 
                }
            }
            else {
                return actor;
            }
        }
    }
    if (m_player->getX() == x && m_player->getY() == y) {
        return m_player;
    }
    
//    status = "empty"; 
    return nullptr;
}

bool StudentWorld::canShoot(int x, int y, int dest, GraphObject::Direction dir, string check) {
    switch (dir) {
        case GraphObject::up:
            if (check != "v") { return false; }
            
            // facing wrong way
            if (dest < y) { return false; }
            // start one square ahead of initial because the initial parameter is the actor's position
            for (int i = y+1; i < dest; i++) {
                
                Actor* ap = checkSpace(x, i, "");
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::right:
            if (check != "h") { return false; }
            
            if (dest < x) { return false; }
            for (int i = x+1; i < dest; i++) {
                
                Actor* ap = checkSpace(i, y, "");
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::down:
            if (check != "v") { return false; }
            
            if (dest > y) { return false; }
            for (int i = y-1; i > dest; i--) {
                
                Actor* ap = checkSpace(x, i, "");
                if (ap == nullptr || (ap != nullptr && !ap->hittable())) {
                    continue;
                }
                else {
                    return false;
                }
            }
            break;
        case GraphObject::left:
            if (check != "h") { return false; }
            
            if (dest > x) { return false; }
            for (int i = x-1; i > dest; i--) {
                
                Actor* ap = checkSpace(i, y, "");
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

