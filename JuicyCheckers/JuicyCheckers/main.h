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



#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <SdkCameraMan.h>

#include "ParticleUniverseSystemManager.h"

class BoardSquare;
class Board;

//---------------------------------------------------------------------------

class TutorialApplication : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);
	bool go();

	// << refers to bitshifting the value.... as in 00000001 << 0 = 00000001
	// 00000001 << 1 = 00000010 = 2 in binary etc
	enum QueryFlags
	{
	  ROBOT_MASK = 1 << 0,
	  NINJA_MASK = 1 << 1,
	  BOARD_MASK = 1 << 2,
	  PARTICLE_MASK = 1 << 3
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
	// init the ninjas
	void addNinjas();
	// init particle system
	void addParticleSystems();
	// actually create the scene elements
	void createScene();



	// Process BufferedInput
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);
	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

private:
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

	// the player boards
	Board* pBoard;

};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------

