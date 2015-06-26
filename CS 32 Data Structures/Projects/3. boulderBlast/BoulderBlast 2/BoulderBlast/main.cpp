#include "glut.h"
#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "Z:/CS32/BoulderBlast/Assets" or "/Users/fred/cs32/BoulderBlast/Assets"

//const string assetDirectory = "/Users/changliu/Desktop/CS\ 32/Projects/3.\ boulderBlast/BoulderBlast\ 2/DerivedData/BoulderBlast/Build/Products/Debug/Assets";
const string assetDirectory = "Assets";

class GameWorld;

GameWorld* createStudentWorld(string assetDir = "");

int main(int argc, char* argv[])
{
	{
		string path = assetDirectory;
		if (!path.empty())
			path += '/';
		ifstream ifs(path + "level00.dat");
		if (!ifs)
		{
			cout << "Cannot find level00.dat in ";
			cout << (assetDirectory.empty() ? "current directory"
											: assetDirectory) << endl;
			return 1;
		}
	}

    glutInit(&argc, argv);

    srand(static_cast<unsigned int>(time(nullptr)));

    GameWorld* gw = createStudentWorld(assetDirectory);
    Game().run(gw, "Boulder Blast");
}
