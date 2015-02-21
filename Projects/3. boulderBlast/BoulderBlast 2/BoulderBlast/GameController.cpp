#include "glut.h"
#include "GameController.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "SoundFX.h"
#include "SpriteManager.h"
#include <string>
#include <map>
#include <utility>
#include <cstdlib>
#include <cmath>
using namespace std;

#if defined(_MSC_VER)
#pragma comment(lib, "glut32")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#endif

static const int WINDOW_WIDTH = 768; //1024;
static const int WINDOW_HEIGHT = 768;

static const int PERSPECTIVE_NEAR_PLANE = 4;
static const int PERSPECTIVE_FAR_PLANE  = 22;

static const double VISIBLE_MIN_X = -2.4375; //-3.25;
static const double VISIBLE_MAX_X = 2.4375; //3.25; 
static const double VISIBLE_MIN_Y = -2;
static const double VISIBLE_MAX_Y = 2;
static const double VISIBLE_MIN_Z = -20;
//static const double VISIBLE_MAX_Z = -6;

static const double FONT_SCALEDOWN = 760.0;

static const int MS_PER_FRAME = 10;

static const double SCORE_Y = 3.8;
static const double SCORE_Z = -10;

static const double PI = 4 * atan(1.0);

struct SpriteInfo
{
	unsigned int imageID;
	unsigned int frameNum;
	std::string  tgaFileName;
};

static void convertToGlutCoords(double x, double y, double& gx, double& gy, double& gz);
static void drawPrompt(string mainMessage, string secondMessage);
static void drawScoreAndLives(string);

void GameController::initDrawersAndSounds()
{
	SpriteInfo drawers[] = {
		{ IID_PLAYER			, 0, "dude_1.tga" },
		{ IID_PLAYER			, 1, "dude_2.tga" },
		{ IID_PLAYER			, 2, "dude_3.tga" },
		{ IID_KLEPTOBOT			, 0, "kleptobot-1.tga" },
		{ IID_KLEPTOBOT			, 1, "kleptobot-2.tga" },
		{ IID_KLEPTOBOT			, 2, "kleptobot-3.tga" },
		{ IID_ANGRY_KLEPTOBOT	, 0, "kleptobot-1.tga" },
		{ IID_ANGRY_KLEPTOBOT	, 1, "kleptobot-2.tga" },
		{ IID_ANGRY_KLEPTOBOT	, 2, "kleptobot-3.tga" },
		{ IID_SNARLBOT			, 0, "snarlbot-1.tga"  },
		{ IID_SNARLBOT			, 1, "snarlbot-2.tga" },
		{ IID_SNARLBOT			, 2, "snarlbot-3.tga" },
		{ IID_SNARLBOT			, 3, "snarlbot-4.tga" },
		{ IID_BULLET			, 0, "bullet.tga" },
		{ IID_ROBOT_FACTORY		, 0, "factory.tga" },
		{ IID_JEWEL				, 0, "jewel.tga" }, 
		{ IID_RESTORE_HEALTH	, 0, "medkit.tga" },
		{ IID_EXTRA_LIFE		, 0, "extralife.tga" },
		{ IID_AMMO				, 0, "ammo.tga" },
		{ IID_EXIT				, 0, "exit.tga" },
		{ IID_WALL				, 0, "wall.tga" },
		{ IID_BOULDER			, 0, "boulder.tga" },
		{ IID_HOLE				, 0, "hole.tga" }
	};

	SoundMapType::value_type sounds[] = {
		make_pair(SOUND_THEME			, "theme.wav"),
		make_pair(SOUND_PLAYER_FIRE		, "torpedo.wav"),
		make_pair(SOUND_ENEMY_FIRE		, "pop.wav"),
		make_pair(SOUND_ROBOT_DIE		, "explode.wav"),
		make_pair(SOUND_PLAYER_DIE		, "die.wav"),
		make_pair(SOUND_GOT_GOODIE		, "goodie.wav"),
		make_pair(SOUND_REVEAL_EXIT		, "revealexit.wav"),
		make_pair(SOUND_FINISHED_LEVEL	, "finished.wav"),
		make_pair(SOUND_ROBOT_IMPACT	, "clank.wav"),
		make_pair(SOUND_PLAYER_IMPACT	, "ouch.wav"),
		make_pair(SOUND_ROBOT_MUNCH		, "munch.wav"),
		make_pair(SOUND_ROBOT_BORN		, "materialize.wav"),
	};
	
	for (int k = 0; k < sizeof(drawers)/sizeof(drawers[0]); k++)
	{
		string path = m_gw->assetDirectory();
		if (!path.empty())
			path += '/';
	    const SpriteInfo& d = drawers[k];
		if (!m_spriteManager.loadSprite(path + d.tgaFileName, d.imageID, d.frameNum))
			exit(0);
	}
	for (int k = 0; k < sizeof(sounds)/sizeof(sounds[0]); k++)
		m_soundMap[sounds[k].first] = sounds[k].second;
}

static void doSomethingCallback()
{
	Game().doSomething();
}

static void reshapeCallback(int w, int h)
{
	Game().reshape(w, h);
}

static void keyboardEventCallback(unsigned char key, int x, int y)
{
	Game().keyboardEvent(key, x, y);
}

static void specialKeyboardEventCallback(int key, int x, int y)
{
	Game().specialKeyboardEvent(key, x, y);
}

static void timerFuncCallback(int val)
{
	Game().doSomething();
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);
}

void GameController::run(GameWorld* gw, string windowTitle)
{
	gw->setController(this);
	m_gw = gw;
	m_gameState = welcome;
	m_lastKeyHit = INVALID_KEY;
	m_singleStep = false;
	m_curIntraFrameTick = 0;
	m_playerWon = false;

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow(windowTitle.c_str()); 

	initDrawersAndSounds();

	glutKeyboardFunc(keyboardEventCallback);
	glutSpecialFunc(specialKeyboardEventCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(doSomethingCallback);
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);

	glutMainLoop(); 
}

void GameController::keyboardEvent(unsigned char key, int /* x */, int /* y */)
{
	switch (key)
	{
		case 'a': case '4': m_lastKeyHit = KEY_PRESS_LEFT;  break;
		case 'd': case '6': m_lastKeyHit = KEY_PRESS_RIGHT; break;
		case 'w': case '8': m_lastKeyHit = KEY_PRESS_UP;	break;
		case 's': case '2': m_lastKeyHit = KEY_PRESS_DOWN;  break;
		case 'f':           m_singleStep = true;			break;
		case 'r':           m_singleStep = false;			break;
		case 'q': case 'Q': m_gameState = quit;				break;
		default:            m_lastKeyHit = key;				break;
	}
}

void GameController::specialKeyboardEvent(int key, int /* x */, int /* y */)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:  m_lastKeyHit = KEY_PRESS_LEFT;		break;
		case GLUT_KEY_RIGHT: m_lastKeyHit = KEY_PRESS_RIGHT;	break;
		case GLUT_KEY_UP:    m_lastKeyHit = KEY_PRESS_UP;		break;
		case GLUT_KEY_DOWN:  m_lastKeyHit = KEY_PRESS_DOWN;		break;
		default:             m_lastKeyHit = INVALID_KEY;		break;
	}
}

void GameController::playSound(int soundID)
{
	if (soundID == SOUND_NONE)
		return;

	SoundMapType::const_iterator p = m_soundMap.find(soundID);
	if (p != m_soundMap.end())
	{
		string path = m_gw->assetDirectory();
		if (!path.empty())
			path += '/';
		SoundFX().playClip(path + p->second);
	}
}

void GameController::doSomething()
{
	switch (m_gameState)
	{
		case not_applicable:
			break;
		case welcome:
			playSound(SOUND_THEME);
			m_mainMessage = "Welcome to Boulder Blast!";
			m_secondMessage = "Press Enter to begin play...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = init;
			break;
		case contgame:
			m_mainMessage = "You lost a life!";
			m_secondMessage = "Press Enter to continue playing...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = cleanup;
			break;
		case finishedlevel:
			m_mainMessage = "Woot! You finished the level!";
			m_secondMessage = "Press Enter to continue playing...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = cleanup;
			break;
		case makemove:
			m_curIntraFrameTick = ANIMATION_POSITIONS_PER_TICK;
			m_nextStateAfterAnimate = not_applicable;
			{
				int status = m_gw->move();
				if (status == GWSTATUS_PLAYER_DIED)
				{
					  // animate one last frame so the player can see what happened
					m_nextStateAfterAnimate = (m_gw->isGameOver() ? gameover : contgame);
				}
				else if (status == GWSTATUS_FINISHED_LEVEL)
				{
					m_gw->advanceToNextLevel();
					  // animate one last frame so the player can see what happened
					m_nextStateAfterAnimate = finishedlevel;
				}
			}
			m_gameState = animate;
			break;
		case animate:
			displayGamePlay();
			if (m_curIntraFrameTick-- <= 0)
			{
				if (m_nextStateAfterAnimate != not_applicable)
					m_gameState = m_nextStateAfterAnimate;
				else
				{
					int key;
					if (!m_singleStep  ||  getLastKey(key))
						m_gameState = makemove;
				}
			}
			break;
		case cleanup:
			m_gw->cleanUp();
			m_gameState = init;
			break;
		case gameover:
			{
				ostringstream oss;
				oss << (m_playerWon ? "You won the game!" : "Game Over!")
					<< " Final score: " << m_gw->getScore() << '!';
				m_mainMessage = oss.str();
			}
			m_secondMessage = "Press Enter to quit...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = quit;
			break;
		case prompt:
			drawPrompt(m_mainMessage, m_secondMessage);
			{
				int key;
				if (getLastKey(key) && key == '\r')
					m_gameState = m_nextStateAfterPrompt;
			}
			break;
		case init:
			{
				int status = m_gw->init();
				SoundFX().abortClip();
				if (status == GWSTATUS_PLAYER_WON)
				{
					m_playerWon = true;
					m_gameState = gameover;
				}
				else if (status == GWSTATUS_LEVEL_ERROR)
				{
					m_mainMessage = "Error in level data file encoding!";
					m_secondMessage = "Press Enter to quit...";
					m_gameState = prompt;
					m_nextStateAfterPrompt = quit;
				}
				else
					m_gameState = makemove;
			}
			break;
		case quit:
			exit(0);
	}
}

void GameController::displayGamePlay()
{
	glEnable(GL_DEPTH_TEST); // must be done each time before displaying graphics or gets disabled for some reason
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	
	std::set<GraphObject*>& graphObjects = GraphObject::getGraphObjects();
	for (auto it = graphObjects.begin(); it != graphObjects.end(); it++)
	{
		GraphObject* cur = *it;
		if (cur->isVisible())
		{
			cur->animate();

			double x, y, gx, gy, gz;
			cur->getAnimationLocation(x,y);
			convertToGlutCoords(x,y, gx, gy, gz);
			
			SpriteManager::Angles angle;
			switch (cur->getDirection())
			{
				case GraphObject::up:
					angle = SpriteManager::face_up;
					break;
				case GraphObject::down:
					angle = SpriteManager::face_down;
					break;
				case GraphObject::left:
					angle = SpriteManager::face_left;
					break;
				default:
				case GraphObject::right:
				case GraphObject::none:
					angle = SpriteManager::face_right;
					break;
			}

			int imageID = cur->getID();
			int frame = cur->getAnimationNumber() % m_spriteManager.getNumFrames(imageID);
			m_spriteManager.plotSprite(imageID, frame, gx, gy, gz, angle);
		}
	}
	
	drawScoreAndLives(m_gameStatText);
	
	glutSwapBuffers();
}

void GameController::reshape (int w, int h) 
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ();
	gluPerspective(45.0, double(WINDOW_WIDTH) / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	glMatrixMode (GL_MODELVIEW); 
} 

static void convertToGlutCoords(double x, double y, double& gx, double& gy, double& gz)
{
	x /= VIEW_WIDTH;
	y /= VIEW_HEIGHT;
	gx = 2 * VISIBLE_MIN_X + .3 + x * 2 * (VISIBLE_MAX_X - VISIBLE_MIN_X);
	gy = 2 * VISIBLE_MIN_Y +      y * 2 * (VISIBLE_MAX_Y - VISIBLE_MIN_Y);
	gz = .6 * VISIBLE_MIN_Z;
}

static void doOutputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str, bool centered)
{
	if (centered)
	{
		double len = glutStrokeLength(GLUT_STROKE_ROMAN, reinterpret_cast<const unsigned char*>(str)) / FONT_SCALEDOWN;
		x = -len / 2;
		size = 1;
	}
	GLfloat scaledSize = size / FONT_SCALEDOWN;
	glPushMatrix();
	glLineWidth(1);
	glLoadIdentity();
	glTranslatef(x, y, z);
	glScalef(scaledSize, scaledSize, scaledSize);
	for ( ; *str != '\0'; str++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
	glPopMatrix();
}

//static void outputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str)
//{
//	doOutputStroke(x, y, z, size, str, false);
//}

static void outputStrokeCentered(GLfloat y, GLfloat z, const char* str)
{
	doOutputStroke(0, y, z, 1, str, true);
}

static void drawPrompt(string mainMessage, string secondMessage)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	outputStrokeCentered(1, -5, mainMessage.c_str());
	outputStrokeCentered(-1, -5, secondMessage.c_str());
	glutSwapBuffers();
}

static void drawScoreAndLives(string gameStatText)
{
	static int RATE = 1;
	static GLfloat rgb[3] = { .6, .6, .6 };
	for (int k = 0; k < 3; k++)
	{
		rgb[k] += (-RATE + rand() % (2*RATE+1)) / 100.0;
		if (rgb[k] < .6)
			rgb[k] = .6;
		else if (rgb[k] > 1.0)
			rgb[k] = 1.0;
	}
	glColor3f(rgb[0], rgb[1], rgb[2]);
	outputStrokeCentered(SCORE_Y, SCORE_Z, gameStatText.c_str());
}
