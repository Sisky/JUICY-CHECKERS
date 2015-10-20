/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: network.h
 *   Description:  This is the Network interface class, this server version will handle the management of
 *					players connecting, lobby joining/creation, and general server management
 *
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

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

	bool mIsRunning;


	// Hold containers of players

	// Hold containers of championships

	// Hold games
};

#endif //__SERVER_H__