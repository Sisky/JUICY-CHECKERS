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

//#include "stdafx.h"
//
//#include <vector>
//
// #include <OISMouse.h>
// #include <Ogre.h>
// #include <OgreRoot.h>
// #include <OISEvents.h>
// #include <OISInputManager.h>
// #include <OISKeyboard.h>
// #include <OISMouse.h>
// #include <OgreWindowEventUtilities.h>
// #include <SdkCameraMan.h>
// #include <SdkTrays.h>
// #include <OgreFontManager.h>
// #include <OgreBorderPanelOverlayElement.h>
// #include <OgreTextAreaOverlayElement.h>

#include <string>
class Client;


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
	void itemSelected(OgreBites::SelectMenu* selectMenu);

	void SetMenu(MENUS menu);

	//void SetMainRef(JuicyCheckers* main);

	void setClientPtr(Client* ptr);
	Client* getClientPtr();

	void frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Methods to update the current lobbies
	void updateLobbies();

	void processTextEvent(const OIS::KeyEvent& ke);
	
	bool isShown();

	void adjustTrays(); 

	void updateChats();

protected:
	void createMenu(MENUS menu);

private:

public:


protected:

private:
	

		// Menu Stuff
    // OgreBites
    Ogre::OverlaySystem*        mOverlaySystem;

	// We hold these pointers to SDKTrayWidgets so we can
	// check which widget triggered the listener event

	// Widgets for the Start Game Menu
	OgreBites::Button*			startButton;
	OgreBites::TextBox* pName;
	OgreBites::TextBox* ip;
	OgreBites::Button*			exitButton;

	Ogre::String name;
	Ogre::String ipStr;
	

	// Widgets for the Lobby Selection Menu
	OgreBites::SelectMenu*			selectLobby;
	Ogre::StringVector lobbyVector;
	OgreBites::Button*			createLobbyButton;
	OgreBites::Button*			refreshLobbyButton;
	OgreBites::Button*			joinLobby;
	OgreBites::Button*			lobbyBack;

	// Widgets for the create lobby menu
	OgreBites::TextBox* lobbyName;
	OgreBites::Button* createLobby;
	OgreBites::Button* createLobbyBack;

	// Widgets for the lobby menu
	OgreBites::TextBox* lobbyPlayersTextBox;
	OgreBites::Button* lobbyReadyButton;
	OgreBites::TextBox* lobbyChatBox;
	OgreBites::TextBox* lobbyChatTextField;
    
	// OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
	OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel

	Ogre::SceneManager* mSceneMgr; 
	Ogre::RenderWindow* mWindow;
	OgreBites::InputContext mInputContext;
	OIS::InputManager* mInputManager; 
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;	

		// The array of SdkMenus
	std::vector<OgreBites::SdkTrayManager*> mTrays;
	OgreBites::SdkTrayManager* currentTray;
	MENUS currentMenu;

	//TutorialApplication* mMain;

	Client* clientPtr;
	std::string currentText;

};

#endif __MENUSYSTEM_H__