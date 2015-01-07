// snakepit.cpp

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "globals.h"
#include "Game.h"
#include "Pit.h"
#include "Player.h"
#include "Snake.h"

using namespace std;

int main()
{
    // Initialize the random number generator.  (You don't need to
    // understand how this works.)
    srand(static_cast<unsigned int>(time(0)));
    
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(9, 10, 10);
    
    // Play the game
    g.play();
}

//#include "Pit.h"
//#include "History.h"
//#include "Player.h"
//#include "globals.h"
//int main()
//{
//    Pit pt(3, 3);
//    pt.addPlayer(1, 1);
//    pt.addSnake(1, 2);
//    pt.addSnake(2, 2);
//    pt.addSnake(2, 3);
////    pt.player()->move(RIGHT);
////    pt.player()->move(LEFT);
////    pt.player()->move(DOWN);
////    pt.player()->move(RIGHT);
////    pt.player()->move(UP);
//    pt.history().display();
//}
