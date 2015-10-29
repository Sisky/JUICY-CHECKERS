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

#include <deque>

MenuSystem::MenuSystem()
	: currentTray(0)
	, isReady(false)
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
		//if(hitFlag){break;}
	}

	//hitFlag = currentTray->injectMouseMove(me);
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
		//if(hitFlag){break;}
	}

	//hitFlag = currentTray->injectMouseDown(me, id);
	
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
		//if(hitFlag){break;}
	}

	hitFlag = currentTray->injectMouseUp(me,id);

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
		(*it)->hideCursor();
	}
	currentTray = mTrays[menu];
	mTrays[menu]->showTrays();
	mTrays[menu]->showCursor();
	currentMenu = menu;
	currentText.clear();
}

void MenuSystem::buttonHit(OgreBites::Button* button)
{
	if(button == startButton)
	{
		ipStr = ip->getText();
		name = pName->getText();


		clientPtr->Initialize(ipStr.c_str(), name.c_str());
		//SetMenu(LISTLOBBYMENU);
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

		SetMenu(LISTLOBBYMENU);
	}
	else if(button == createLobbyButton)
	{
		// We should transition to the lobby creation menu
		SetMenu(CREATELOBBYMENU);
	}
	else if(button == joinLobby)
	{
		// Send a message to the server to add us to the lobby we selected
		// We will wait until we get the reply from the server to transisition into
		// the lobby screen though
		// Pass the index of the selected item since the clients
		// lobby vector is in the same order.
		clientPtr->JoinLobby(selectLobby->getSelectionIndex());
		SetMenu(LOBBYMENU);
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
	else if(button == lobbyReadyButton)
	{
		if(isReady)
		{
			lobbyReadyButton->setCaption("NOT READY");
			isReady = !isReady;
			clientPtr->sendReady();
		}
		else
		{
			lobbyReadyButton->setCaption("READY");
			isReady = !isReady;
			clientPtr->sendReady();
		}
	}
	else if(button == matchUpgrade)
	{
		SetMenu(UPGRADEMENU);
	}
	else if(button == powerupBack)
	{
		SetMenu(MATCHMENU);
	}
	else if(button == powerupOne)
	{
		
	}
	else if(button == powerupTwo)
	{
		
	}
	else if(button == powerupThree)
	{
		
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
	// Process all the sdk tray managers
	for(std::vector<OgreBites::SdkTrayManager*>::iterator it = mTrays.begin();
		it != mTrays.end();
		++it)
	{
		(*it)->frameRenderingQueued(evt);
	}

	// Update the chat windows
	updateChats();

	// Check for when to change state when we are connected
	if(currentMenu == STARTMENU)
	{
		// Check if the network connects
		if(clientPtr->getIsConnected())
		{
			SetMenu(LISTLOBBYMENU);
		}
	}


	// Update the players that are in the lobby
	if(currentMenu == LOBBYMENU)
	{
		std::vector<RakNet::RakString>* vec = clientPtr->GetLobbyUsers();
		lobbyPlayersTextBox->clearText();

		int index = 1;
		for(std::vector<RakNet::RakString>::iterator it = vec->begin();
			it != vec->end();
			++it)
		{
			Ogre::String str = Ogre::String((*it).C_String());
			if(index % 3 == 0)
			{ 
				str.append("\n");
			}
			else
			{
				str.append(" ");
			}

			lobbyPlayersTextBox->appendText(str);


			++index;
		}
	}


	// Process for when we get the signal to transition into the match menu state
	if(currentMenu == LOBBYMENU)
	{
		// If we are to transition set the flag back to flase
		if(clientPtr->getTransitionMatch())
		{
			clientPtr->setTransitionMatch(false);

			SetMenu(MATCHMENU);
		}
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

	// Update the lobby chat box

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
	case OIS::KC_RETURN:
		{
			// Send the current text over the network to either lobby or master
			// depending on which we are in.
			switch(currentMenu)
			{
			case LISTLOBBYMENU:
				{
					// Send the chat to the master
					Ogre::String builder = Ogre::String(name + ": " + currentText.c_str()); 
					clientPtr->SendMasterChat(builder.c_str());
					currentText.clear();
				}
				break;
			case LOBBYMENU:
				{
					// Send the chat the lobby
					Ogre::String builder = Ogre::String(name + ": " + currentText.c_str()); 
					clientPtr->SendLobbyChat(builder.c_str());
					currentText.clear();
				}
				break;
			case MATCHMENU:
				{
					// Send the chat to the match
					clientPtr->SendMatchChat(currentText.c_str());
				}
				break;
			}
		}
		break;
	default:
		{
			if(ke.text >= ' ' && ke.text <= 'z')
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
			ip->setText(currentText);
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
			lobbyChatTextField->setText(currentText); 
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

void MenuSystem::updateChats()
{
	switch(currentMenu)
	{
	case LISTLOBBYMENU:
		{
			// Send the chat to the master
			std::deque<RakNet::RakString>* vec = clientPtr->GetMasterChatLog();			
			//while(!(vec->empty()))
			//{
				//masterChatBox->appendText(vec->begin()->C_String());
			//}			
		}
		break;
	case LOBBYMENU:
		{
			// Send the chat to the master
			std::deque<RakNet::RakString>* vec = clientPtr->GetLobbyChatLog();			
			while(!(vec->empty()))
			{
				lobbyChatBox->appendText(Ogre::DisplayString(vec->begin()->C_String()) + "\n");		
				Ogre::Real scroll = lobbyChatTextField->getScrollPercentage();
				vec->pop_front();
			}		
		}
		break;
	case MATCHMENU:
		{
			// Send the chat to the match
			clientPtr->SendMatchChat(currentText.c_str());
		}
		break;
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
			pName = currentTray->createTextBox(OgreBites::TL_CENTER, "pName", "Enter Your Name",300,75);
			int randomNumbers = rand() % 1000;
			char bfr[256];
			char* iden = itoa(randomNumbers, bfr, 10);
			pName->setText(Ogre::String("Player")+iden);
			ip = currentTray->createTextBox(OgreBites::TL_CENTER, "ip", "Enter the Server IP",300,75);
			ip->setText("127.0.0.1");
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
			joinLobby = currentTray->createButton(OgreBites::TL_CENTER, "joinLobby", "Join Lobby");
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
			currentTray->createLabel(OgreBites::TL_TOP, "lobbyMenuTitle", "Lobby Menu", 500);
									
			// List the current players
			currentTray->createLabel(OgreBites::TL_CENTER, "lobbyMenuPlayers", "Players", 300);
			lobbyPlayersTextBox = currentTray->createTextBox(OgreBites::TL_CENTER, "lobbyPlayers", "Players",400,150);

			// Have a (ready) toggle button
			if(isReady)
			{
				lobbyReadyButton = currentTray->createButton(OgreBites::TL_CENTER, "readyButton", "READY");				
				//isReady = !isReady;
			}
			else
			{
				lobbyReadyButton = currentTray->createButton(OgreBites::TL_CENTER, "readyButton", "NOT READY");	
				//isReady = !isReady;
			}

			// Have a chat box 
			lobbyChatBox = currentTray->createTextBox(OgreBites::TL_CENTER, "chatBox", "Chat Window",500,250);
			lobbyChatTextField = currentTray->createTextBox(OgreBites::TL_CENTER, "chatTextField", "Enter Chat Here",500,80);


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
			matchTurn = currentTray->createLabel(OgreBites::TL_TOPLEFT, "turnLabel", "Your Turn");

			// Indicate with a label the current powerup credit level
			matchCredits = currentTray->createLabel(OgreBites::TL_TOPLEFT, "powerupLabel", "Powerup Credits:0");
			// Have a button that opens up the upgrade menu

			matchUpgrade = currentTray->createButton(OgreBites::TL_TOPLEFT, "upgradeButton", "Open Upgrades");

			

			// Add the tray into the tray container.  NOTE: This function is designed to be run from 0 -> MENU.max
			mTrays.push_back(currentTray);
		}
		break;
	case UPGRADEMENU:
		{
			// Create the Tray Manager
			currentTray = new OgreBites::SdkTrayManager("UpgradeMenu", mWindow, mInputContext, this);

			matchTurn = currentTray->createLabel(OgreBites::TL_CENTER, "nameLabel", "Powerup Menu");
			matchCredits = currentTray->createLabel(OgreBites::TL_CENTER, "credits", "Credits: 0");
			powerupOne = currentTray->createButton(OgreBites::TL_CENTER, "buyPU1", "Powerup One");
			powerupTwo = currentTray->createButton(OgreBites::TL_CENTER, "buyPU2", "Powerup Two");
			powerupThree = currentTray->createButton(OgreBites::TL_CENTER, "buyPU3", "Powerup Three");
			powerupBack = currentTray->createButton(OgreBites::TL_CENTER, "powerupBack", "Back");

			

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