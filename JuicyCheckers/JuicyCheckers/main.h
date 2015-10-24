/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
___                 __    __ _ _    _
/___\__ _ _ __ ___  / / /\ \ (_) | _(_)
//  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
|___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "client.h"

#include <Ogre.h>
#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <SdkCameraMan.h>
#include <SdkTrays.h>
#include <OgreFontManager.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#include "ParticleUniverseSystemManager.h"
#include "client.h"
#include "MenuSystem.h"




// forward declarations
class BoardSquare;
class Board;
class Piece;
class PieceController;

//---------------------------------------------------------------------------

class TutorialApplication : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);
	bool go();

	void setShutdown();

	// << refers to bitshifting the value.... as in 00000001 << 0 = 00000001
	// 00000001 << 1 = 00000010 = 2 in binary etc
	enum QueryFlags
	{
	  ROBOT_MASK = 1 << 0,
	  NINJA_MASK = 1 << 1,
	  BOARD_BLACK = 1 << 2,
	  BOARD_WHITE = 1 << 3,
	  PARTICLE_MASK = 1 << 4,
	  PIECE_MASK = 1 << 5
	};

	enum Player
	{
		PLAYER_ONE = 1,
		PLAYER_TWO = 2
	};

protected:
	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Methods for initialising the game
	void initScene();
	// init the input
	void initInput();

	// add the pieces to the array
	void addPieces();
	// draw the pieces on the board
	void drawPieces();

	// init particle system
	void addParticleSystems();
	// actually create the scene elements
	void createScene();

	// Initialise Networking
	void initNetworking();

	// Initialise the Menu System
	void initMenu();




	// Process BufferedInput
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

private:
	bool shutdown;

	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Ogre::RenderWindow* mWindow;

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	// Member variables for the OIS input
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	// Variables relating to the ingame camera object
	// float currentDegree;
	int maxDegree;
	int minDegree;

 
	Ogre::RaySceneQuery* mRayScnQuery;

	// Particle system manager
	ParticleUniverse::ParticleSystemManager* pManager;


	Client* client;

	// the player boards
	Board* pBoard;

	// vector array of pieces.. total of 12 per side (24 in total)
	std::vector<Piece*> pPieces;

	// Piece Controller
	PieceController* pController;

	// MenuSystem
	MenuSystem* mMenuSystem;


};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------

