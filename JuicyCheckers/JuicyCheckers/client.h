/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: network.h
 *   Description:  This is the Network interface class, this Client version will handle the management of
 *					players connecting, lobby joining/creation, and general Client management
 *
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "NetworkIDObject.h"
#include "NetworkIDManager.h"


#include "datastructures.h"
//#include "RakPeerInterface.h"
//#include "MessageIdentifiers.h"
//#include "BitStream.h"
//#include "RakNetTypes.h"  // MessageID

class Client
{
	// Member Functions
public:
	Client();
	~Client();

	void Process(float _delta);

	void SendMovement(int source, int dest);
	void GetTurn();
	std::vector<LobbyMsg>* GetLobbies();
	void JoinLobby(int lobbyIndex);
	void TakePiece();

	void CreateLobby(RakNet::RakString& name);

	void RefreshLobbies();

	void SendMasterChat(const char* _chat);
	void SendLobbyChat(const char* _chat);
	void SendMatchChat(const char* _chat);

	std::deque<RakNet::RakString>* GetMasterChatLog();
	std::deque<RakNet::RakString>* GetLobbyChatLog();
	std::deque<RakNet::RakString>* GetMatchChatLog();

	void Initialize(const char* ip, const char* name);

	bool getIsConnected();

	void sendReady();

	std::vector<RakNet::RakString>* GetLobbyUsers();

	bool getTransitionMatch();
	void setTransitionMatch(bool doTransition);
protected:
	
	bool handleUserPacket(RakNet::Packet* packet);

	// Will need some kind of method to call to update the current gamestate
	//void UpdateGameState(Game& game);


	
private:

	// Member Variables
public:


protected:

private:
	// Hold RakNet Variables
	RakNet::RakPeerInterface *peer;

	RakNet::RakNetGUID serverGUID;

	RakNet::NetworkID lobbyID;

	RakNet::NetworkID currentMatchID;
	bool transitionMatch;

	bool isConnected;

	RakNet::RakString name;


	
	static const int PORT = 61126;
	static const bool isServer = false;

	std::vector<LobbyMsg> lobbyNetworkID;

	std::deque<RakNet::RakString> raknetMasterChat;
	std::deque<RakNet::RakString> raknetLobbyChat;
	std::deque<RakNet::RakString> raknetMatchChat;

	std::vector<RakNet::RakString> LobbyUsers;


	// Hold containers of players

	// Hold containers of championships

	// Hold games
};

#endif __CLIENT_H__

