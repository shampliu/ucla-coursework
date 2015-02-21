#ifndef LEVEL_H_
#define LEVEL_H_

#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

class Level
{
public:

	enum MazeEntry {
		empty, exit, player, horiz_snarlbot, vert_snarlbot,
		kleptobot_factory, angry_kleptobot_factory, wall, boulder, hole,
		jewel, restore_health, extra_life, ammo
	};
	enum LoadResult {
		load_success, load_fail_file_not_found, load_fail_bad_format};

	Level(std::string assetDir)
	 : m_pathPrefix(assetDir)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
			for (int x = 0; x < VIEW_WIDTH; x++)
				m_maze[y][x] = empty;

		if (!m_pathPrefix.empty())
			m_pathPrefix += '/';
	}

	LoadResult loadLevel(std::string filename)
	{
		std::ifstream levelFile((m_pathPrefix + filename).c_str());
		if (!levelFile)
			return load_fail_file_not_found;

		  // get the maze

		std::string line;
		bool foundExit = false;
		bool foundPlayer = false;

		for (int y = VIEW_HEIGHT-1; std::getline(levelFile, line); y--)
		{
			if (y < 0)	// too many maze lines?
			{
				if (line.find_first_not_of(" \t\r") != std::string::npos)
					return load_fail_bad_format;  // non-blank line
				char dummy;
				if (levelFile >> dummy)	 // non-blank rest of file
				return load_fail_bad_format;
				break;
			}

			if (line.size() < VIEW_WIDTH  ||  line.find_first_not_of(" \t\r", VIEW_WIDTH) != std::string::npos)
				return load_fail_bad_format;
				
			for (int x = 0; x < VIEW_WIDTH; x++)
			{
				MazeEntry me;
				switch (tolower(line[x]))
				{
					default:   return load_fail_bad_format;
					case ' ':  me = empty;						break;
					case 'x':  me = exit; foundExit = true;		break;
					case '@':  me = player; foundPlayer = true; break;
					case 'h':  me = horiz_snarlbot;				break;
					case 'v':  me = vert_snarlbot;				break;
					case '1':  me = kleptobot_factory;			break;
					case '2':  me = angry_kleptobot_factory;	break;
					case '#':  me = wall;						break;
					case 'b':  me = boulder;					break;
					case 'o':  me = hole;						break;
					case '*':  me = jewel;						break;
					case 'r':  me = restore_health;				break;
					case 'e':  me = extra_life;					break;
					case 'a':  me = ammo;						break;
				}
				m_maze[y][x] = me;
			}
		}

		if (!foundExit || !foundPlayer || !edgesValid())
			return load_fail_bad_format;

		return load_success;
	}

	MazeEntry getContentsOf(unsigned int x, unsigned int y) const
	{
		return (x < VIEW_WIDTH && y < VIEW_HEIGHT) ? m_maze[y][x] : empty;
	}

private:

	MazeEntry	m_maze[VIEW_HEIGHT][VIEW_WIDTH];
	std::string m_pathPrefix;

	bool edgesValid() const
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
			if (m_maze[y][0] != wall || m_maze[y][VIEW_WIDTH-1] != wall)
				return false;
		for (int x = 0; x < VIEW_WIDTH; x++)
			if (m_maze[0][x] != wall || m_maze[VIEW_HEIGHT-1][x] != wall)
				return false;

		return true;
	}
};

#endif // LEVEL_H_
