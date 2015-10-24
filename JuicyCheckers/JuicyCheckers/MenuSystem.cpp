/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *            Ken Par
 *   Filename: MenuSystem.h
 *   Description:  This is the MenuSystem class implementation.  This class handles the state showing of the 
 *					different menus ingame, it also ties in with the Main and Client class for 
 *					sending messages to the server on actions
 *
 */	
#include "stdafx.h"
#include "MenuSystem.h"

MenuSystem::MenuSystem()
{

}

MenuSystem::~MenuSystem()
{

}

bool MenuSystem::InitialiseOverlaySystem(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;

	mOverlaySystem = new Ogre::OverlaySystem();
	sceneMgr->addRenderQueueListener(mOverlaySystem);

	return(mOverlaySystem != 0);
}

bool MenuSystem::Initialise(Ogre::RenderWindow* window, OgreBites::InputContext inputContext,
			OIS::InputManager* inputManager, OIS::Mouse* mouse, OIS::Keyboard* keyboard)
{
	mWindow = window;
	mInputContext = inputContext;
	mInputManager = inputManager;
	mMouse = mouse;
	mKeyboard = keyboard;



	inputContext.mKeyboard = mKeyboard;
    inputContext.mMouse = mMouse;

	// Create all the menus in the order the enum has them
	//		STARTMENU,		LISTLOBBYMENU,		CREATELOBBYMENU,		
	//      LOBBYMENU,		MATCHMENU,			RESULTMENU
	OgreBites::SdkTrayManager* currentTray = 0;

	// Create all the menus from the enum in order
	for(int i = STARTMENU; i < MENU_MAX; ++i)
	{
		createMenu(static_cast<MENUS>(i));
	}


	return true;
}

bool MenuSystem::MouseMoved(const OIS::MouseEvent& me)
{
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->injectMouseMove(me);
	}

	return true;
}

bool MenuSystem::MousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->injectMouseDown(me,id);
	}

	return true;
}

bool MenuSystem::MouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->injectMouseUp(me,id);
	}

	return true;
}

void MenuSystem::SetMenu(MENUS menu)
{
	// Hide any menu that might be showing and then display the new menu
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->hideTrays();
	}

	mTrays[menu]->showTrays();
}

void MenuSystem::buttonHit(OgreBites::Button* button)
{
	if(button == startButton)
	{
		SetMenu(MATCHMENU);
	}
	else if(button == exitButton)
	{
		//mMain->setShutdown();
		// Having Main.h included in here caused all matter of problems
		// Might be better to have a bool flag for shutdown in this
		// class and have the main look check to see if we want to quit.
		// frameRenderingQueued can return a false and quit
	}
}

//void 
//MenuSystem::SetMainRef(TutorialApplication* main)
//{
//	mMain = main;
//}

void MenuSystem::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->frameRenderingQueued(evt);
	}
}

void 
MenuSystem::createMenu(MENUS menu)
{
	OgreBites::SdkTrayManager* currentTray = 0;
	switch(menu)
	{
	case STARTMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("StartMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed
			currentTray->createLabel(OgreBites::TL_TOP, "GameTitle", "Juicy Checkers", 500);
			startButton = currentTray->createButton(OgreBites::TL_CENTER, "startBtn", "Start Multiplayer");
			exitButton = currentTray->createButton(OgreBites::TL_CENTER, "extBtn", "Exit Game");

			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case LISTLOBBYMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("ListLobbyMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case CREATELOBBYMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("CreateLobbyMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;
	case LOBBYMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("LobbyMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case MATCHMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("MatchMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case RESULTMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("ResultMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	}
}