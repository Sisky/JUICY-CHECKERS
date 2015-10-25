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

#include <OISKeyboard.h>

MenuSystem::MenuSystem()
	: currentTray(0)
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
	bool hitFlag = false;
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		hitFlag = ((*it)->injectMouseMove(me)) || (hitFlag); 
	}

	return hitFlag;
}

bool MenuSystem::MousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	bool hitFlag = false;
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		hitFlag = ((*it)->injectMouseDown(me,id)) || (hitFlag);
	}

	return hitFlag;
}

bool MenuSystem::MouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	bool hitFlag = false;
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		hitFlag = ((*it)->injectMouseUp(me,id)) || (hitFlag);

		
	}

	return hitFlag;
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
	currentTray = mTrays[menu];
	mTrays[menu]->showTrays();
	currentMenu = menu;
	currentText.clear();
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

		Ogre::String str = lobbyName->getText();
		const char* data = str.c_str();

		RakNet::RakString name = data;
		clientPtr->CreateLobby(name);

		MenuSystem::updateLobbies();
	}
	else if(button == createLobbyButton)
	{
		// We should transition to the lobby creation menu
		SetMenu(CREATELOBBYMENU);
	}
	else if(button == refreshLobbyButton)
	{		
		updateLobbies();

		// Also instruct the Client interface to request a new listing of lobbies
		clientPtr->RefreshLobbies();
	}
	else if(button == lobbyBack)
	{
		SetMenu(STARTMENU);
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
//MenuSystem::SetMainRef(JuicyCheckers* main)
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

	selectLobby->clearItems();
	lobbyVector.clear();

	// Add the lobbies to the SelectLobby object
	for(std::vector<LobbyMsg>::iterator lobby = (*lobbyListMsg).begin();
		lobby != (*lobbyListMsg).end(); 
		++lobby)
	{   
		char* str = new char[64];
		itoa(lobby->networkID, str, 10);
		selectLobby->addItem(Ogre::String(lobby->name.C_String()));
		lobbyVector.push_back(Ogre::String(lobby->name.C_String()));		
	}

	// Make sure the items are set and shown
	selectLobby->setItems(lobbyVector);
	selectLobby->show();
	adjustTrays();

}

void MenuSystem::setClientPtr(Client* ptr)
{
	clientPtr = ptr;
}
Client* MenuSystem::getClientPtr()
{
	return clientPtr;
}

void MenuSystem::processTextEvent(const OIS::KeyEvent& ke)
{
	switch(ke.key)
	{
	case OIS::KC_BACK:
		{
			if(currentText.size() > 0)
			{
				currentText.pop_back();
			}
		}
		break;
	default:
		{
			if(ke.text >= 'A' && ke.text <= 'z')
			{
				currentText.push_back(ke.text);
			}
		}
	}

	// Depending on which tray is active send the text to different text boxes
	switch(currentMenu)
	{
	case STARTMENU:
		{

		}
		break;
	case LISTLOBBYMENU:
		{

		}
		break;
	case CREATELOBBYMENU:
		{
			lobbyName->setText(currentText);
		}
		break;
	case LOBBYMENU:
		{

		}
		break;
	case MATCHMENU:
		{

		}
		break;
	case RESULTMENU:		
		{

		}
		break;
	}
}

bool 
MenuSystem::isShown()
{
	return currentTray->areTraysVisible();
}

void 
MenuSystem::adjustTrays()
{
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->adjustTrays();
		
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
			// Create the label
			currentTray->createLabel(OgreBites::TL_TOP, "JoinCreateLobby", "Join/Create Lobby", 500);
			createLobbyButton = currentTray->createButton(OgreBites::TL_CENTER, "createLobby", "Create Lobby");
			// Have the option to create a new lobby

			// Below this we will have a listing of lobbys that the user can click on
			selectLobby = currentTray->createLongSelectMenu(OgreBites::TL_CENTER, "selectLobby", Ogre::DisplayString("Select a Lobby"), Ogre::Real(300.0), 12); 
			refreshLobbyButton = currentTray->createButton(OgreBites::TL_CENTER, "refreshLobby", "Refresh Lobbies");
			lobbyBack = currentTray->createButton(OgreBites::TL_CENTER, "lobbyBack", "Back");
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
			// Have a label that indicates what is currently set ready or unready

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

			// Indicate whos turn it currently is with a label

			// Indicate with a label the current powerup credit level

			// Have a button that opens up the upgrade menu

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