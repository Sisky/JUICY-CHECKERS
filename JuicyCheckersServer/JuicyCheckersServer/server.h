/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: network.h
 *   Description:  This is the Network interface class, this server version will handle the management of
 *					players connecting, lobby joining/creation, and general server management
 *
 */

#ifndef __SERVER_H__
#define __SERVER_H__

//#include "RakPeerInterface.h"
//#include "MessageIdentifiers.h"
//#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
#include "lobby.h"

#include <vector>

class Server
{
	// Member Functions
public:
	Server();
	~Server();

	void Process(float _delta);

	bool isRunning();

protected:
	void Initialize();
	bool handleUserPacket(RakNet::Packet* packet);

	// This function creates a new lobby
	RakNet::NetworkID CreateLobby(RakNet::RakNetGUID playerGUID);

	void ProcessDisconnect(RakNet::Packet* packet);


	
private:

	// Member Variables
public:

protected:

private:
	// Hold RakNet Variables
	RakNet::RakPeerInterface *peer;
	static const int MAX_CLIENTS = 16;
	static const int PORT = 61126;
	static const bool isServer = true;

	// This holds the Network ID Manager for lookup of NetworkID's
	RakNet::NetworkIDManager NUIDManager;

	// We use this boolean to stop the server
	bool mIsRunning;


	// Hold containers connected users
	std::vector<RakNet::RakNetGUID> mConnectedUsers;
	std::vector<Lobby*> mLobbies;

	// Hold containers of championships

	// Hold games
};

#endif //__SERVER_H__