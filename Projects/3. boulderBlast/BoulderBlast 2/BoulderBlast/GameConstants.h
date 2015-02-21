#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

// IDs for the game objects

const int IID_PLAYER			= 0;
const int IID_SNARLBOT			= 1;
const int IID_KLEPTOBOT			= 2;
const int IID_ANGRY_KLEPTOBOT	= 3;
const int IID_ROBOT_FACTORY		= 4;
const int IID_BULLET			= 5;
const int IID_WALL				= 6;
const int IID_EXIT				= 7;
const int IID_BOULDER			= 8;
const int IID_HOLE				= 9;
const int IID_JEWEL				= 10;
const int IID_RESTORE_HEALTH	= 11;
const int IID_EXTRA_LIFE		= 12;
const int IID_AMMO				= 13;

// sounds

const int SOUND_THEME			= 0;
const int SOUND_ROBOT_DIE		= 1;
const int SOUND_PLAYER_DIE		= 2;
const int SOUND_ENEMY_FIRE		= 3;
const int SOUND_PLAYER_FIRE		= 4;
const int SOUND_GOT_GOODIE		= 5;
const int SOUND_REVEAL_EXIT		= 6;
const int SOUND_FINISHED_LEVEL	= 7;
const int SOUND_ROBOT_BORN		= 8;
const int SOUND_ROBOT_IMPACT	= 9;
const int SOUND_PLAYER_IMPACT	= 10;
const int SOUND_ROBOT_MUNCH		= 11;

const int SOUND_NONE			= -1;

// keys the user can hit

const int KEY_PRESS_LEFT	= 1000;
const int KEY_PRESS_RIGHT	= 1001;
const int KEY_PRESS_UP		= 1002;
const int KEY_PRESS_DOWN	= 1003;
const int KEY_PRESS_SPACE	= ' ';
const int KEY_PRESS_ESCAPE	= '\x1b';

// board dimensions 

const int VIEW_WIDTH	= 15;
const int VIEW_HEIGHT	= 15;

// status of each tick (did the player die?)

const int GWSTATUS_PLAYER_DIED		= 0;
const int GWSTATUS_CONTINUE_GAME	= 1;
const int GWSTATUS_PLAYER_WON		= 2;
const int GWSTATUS_FINISHED_LEVEL	= 3;
const int GWSTATUS_LEVEL_ERROR		= 4;

#endif // GAMECONSTANTS_H_
