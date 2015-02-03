////
////  queue.cpp
////  2. stacks&queues
////
////  Created by Chang Liu on 2/1/15.
////  Copyright (c) 2015 Chang Liu. All rights reserved.
////
//
//#include <iostream>
//#include <string>
//#include <queue>
//
//using namespace std;
//
//class Coord
//{
//public:
//    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
//    int r() const { return m_r; }
//    int c() const { return m_c; }
//private:
//    int m_r;
//    int m_c;
//};
//
//bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
//    // Return true if there is a path from (sr,sc) to (er,ec)
//    // through the maze; return false otherwise
//    
//    // sr ranges from 0 - rows-1, sc ranges from 0 to cols-1
//    
//    queue<Coord> coordStack;     // declare a stack of Coords
//    coordStack.push(Coord(sr, sc));
//    maze[sr][sc] = 'O';
//    while (! coordStack.empty()) {
//        Coord a = coordStack.front();
//        
//        coordStack.pop();
//        if (a.r() == er && a.c() == ec) {
//            return true;
//        }
//        
//        // NORTH
//        if (maze[a.r() - 1][a.c()] == '.') {
//            maze[a.r() - 1][a.c()] = 'O';
//            coordStack.push(Coord(a.r() - 1, a.c()));
//        }
//        // EAST
//        if (maze[a.r()][a.c() + 1] == '.') {
//            maze[a.r()][a.c() + 1] = 'O';
//            coordStack.push(Coord(a.r(), a.c() + 1));
//        }
//        // SOUTH
//        if (maze[a.r() + 1][a.c()] == '.') {
//            maze[a.r() + 1][a.c()] = 'O';
//            coordStack.push(Coord(a.r() + 1, a.c()));
//        }
//        // WEST
//        if (maze[a.r()][a.c() - 1] == '.') {
//            maze[a.r()][a.c() - 1] = 'O';
//            coordStack.push(Coord(a.r(), a.c() - 1));
//        }
//    }
//    
//    
//    
//    return false;
//}
//
//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X........X",
//        "XX.X.XXXXX",
//        "X..X.X...X",
//        "X..X...X.X",
//        "XXXX.XXX.X",
//        "X.X....XXX",
//        "X..XX.XX.X",
//        "X...X....X",
//        "XXXXXXXXXX"
//    };
//    
//    if (pathExists(maze, 10,10, 6,4, 1,1))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}
