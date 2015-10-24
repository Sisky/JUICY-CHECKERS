/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *            Ken Par
 *   Filename: MenuSystem.h
 *   Description:  This is the MenuSystem class.  This class handles the state showing of the 
 *					different menus ingame, it also ties in with the Main and Client class for 
 *					sending messages to the server on actions
 *
 */

#ifndef __MENUSYSTEM_H__
#define __MENUSYSTEM_H__

#include <vector>
#include "stdafx.h"
#include <OISMouse.h>
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


class MenuSystem : public OgreBites::SdkTrayListener
{
public:
	enum MENUS
	{
		STARTMENU,
		LISTLOBBYMENU,
		CREATELOBBYMENU,
		LOBBYMENU,
		MATCHMENU,
		RESULTMENU,
		MENU_MAX
	};

	MenuSystem();
	~MenuSystem();

	bool Initialise(Ogre::RenderWindow* window, OgreBites::InputContext mInputContext,
			OIS::InputManager* mInputManager, OIS::Mouse* mMouse, OIS::Keyboard* mKeyboard);
	bool InitialiseOverlaySystem(Ogre::SceneManager* mSceneMgr);

	bool MouseMoved(const OIS::MouseEvent& me);
	bool MousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	bool MouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

	void buttonHit(OgreBites::Button* button);

	void SetMenu(MENUS menu);

	//void SetMainRef(TutorialApplication* main);

	void frameRenderingQueued(const Ogre::FrameEvent& evt);

protected:
	void createMenu(MENUS menu);

private:

public:


protected:

private:
	

		// Menu Stuff
    // OgreBites
    Ogre::OverlaySystem*        mOverlaySystem;
	OgreBites::Button*			startButton;
	OgreBites::Button*			exitButton;
	OgreBites::TextBox*			textBox;
    
	OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
	OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel

	Ogre::SceneManager* mSceneMgr; 
	Ogre::RenderWindow* mWindow;
	OgreBites::InputContext mInputContext;
	OIS::InputManager* mInputManager; 
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

		// The array of SdkMenus
	std::vector<OgreBites::SdkTrayManager*> mTrays;

	//TutorialApplication* mMain;
};

#endif //__MENUSYSTEM_H__