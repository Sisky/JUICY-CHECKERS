/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: network.h
 *   Description:  This is the Network interface class, this Client version will handle the management of
 *					players connecting, lobby joining/creation, and general Client management
 *
 */

#ifndef __Client_H__
#define __Client_H__

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

protected:
	void Initialize();
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


	
	static const int PORT = 61126;
	static const bool isServer = false;

	std::vector<LobbyMsg> lobbyNetworkID;


	// Hold containers of players

	// Hold containers of championships

	// Hold games
};

#endif //__Client_H__