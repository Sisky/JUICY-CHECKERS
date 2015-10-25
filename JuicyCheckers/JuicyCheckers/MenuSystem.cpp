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

#include "client.h"

#include "datastructures.h"

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
		SetMenu(LISTLOBBYMENU);
	}
	else if(button == exitButton)
	{
		//mMain->setShutdown();
		// Having Main.h included in here caused all matter of problems
		// Might be better to have a bool flag for shutdown in this
		// class and have the main look check to see if we want to quit.
		// frameRenderingQueued can return a false and quit
	}
	else if(button == createLobby)
	{
		// Send the server a lobby creation message.
		// Get the name of the lobby
		RakNet::RakString name = RakNet::RakString(reinterpret_cast<const char*>((lobbyName->getText().c_str())));
		clientPtr->CreateLobby(name);
	}
	else if(button == createLobbyButton)
	{
		// We should transition to the lobby creation menu
		SetMenu(CREATELOBBYMENU);
	}
	else if(button == createLobbyBack)
	{
		// We should transition to the lobby creation menu
		SetMenu(LISTLOBBYMENU);
	}
}

void 
MenuSystem::itemSelected(OgreBites::SelectMenu* selectMenu)
{
	if(selectMenu == selectLobby)
	{
		//selectMenu>
	}
	//else if(()
	//{
		// Just incase we have more selectmenus
	//}
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
	MenuSystem::updateLobbies()
{
	std::vector<LobbyMsg>* lobbyListMsg = clientPtr->GetLobbies();

	lobbyVector.clear();

	for(std::vector<LobbyMsg>::iterator lobby = (*lobbyListMsg).begin();
		lobby != (*lobbyListMsg).end(); 
		++lobby)
	{   
		lobbyVector.push_back(Ogre::String(lobby->name +"|"+ lobby->networkID));
	}
}

void MenuSystem::setClientPtr(Client* ptr)
{
	clientPtr = ptr;
}
Client* MenuSystem::getClientPtr()
{
	return clientPtr;
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
			// Create the label
			currentTray->createLabel(OgreBites::TL_TOP, "JoinCreateLobby", "Join/Create Lobby", 500);
			createLobbyButton = currentTray->createButton(OgreBites::TL_CENTER, "createLobby", "Create Lobby");
			// Have the option to create a new lobby


			//lobbyVector.push_back(Ogre::String("Jonys Lobby"));
			//lobbyVector.push_back(Ogre::String("Daves Lobby"));
			//lobbyVector.push_back(Ogre::String("Kens Lobby"));
			// Below this we will have a listing of lobbys that the user can click on
			selectLobby = currentTray->createThickSelectMenu(OgreBites::TL_CENTER, "selectLobby", Ogre::DisplayString("Select a Lobby"), Ogre::Real(300.0), 12, lobbyVector); 
			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case CREATELOBBYMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("CreateLobbyMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed
			// Label
			currentTray->createLabel(OgreBites::TL_TOP, "CreateLobby", "Create Lobby", 500);
			lobbyName = currentTray->createTextBox(OgreBites::TL_CENTER, "lobbyName", "Lobby Name", 500 , 300);
			createLobby = currentTray->createButton(OgreBites::TL_CENTER, "createLobbyButton", "Create Lobby");
			createLobbyBack = currentTray->createButton(OgreBites::TL_CENTER, "createLobbyButtonBack", "Back");
			

			// Text area for the name

			// Button to create


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;
	case LOBBYMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("LobbyMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed
			// Label
			// List the current players

			// Have a (ready) button

			// Have a chat box 


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case MATCHMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("MatchMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed
			// Here we will have some elements such as labels in certain positions to indicate 
			// the current turn,  timers upgrades etc etc.


			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	case RESULTMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("ResultMenu", mWindow, mInputContext, this);

			// Design the tray, Save the buttons if you want to check if they are pressed
			// Label

			// Label results




			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;

	}
}