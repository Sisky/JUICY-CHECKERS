/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: network.h
 *   Description:  This is the Network interface class, this Client version will handle the management of
 *					players connecting, lobby joining/creation, and general Client management
 *
 */

#ifndef __Client_H__
#define __Client_H__

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

	void SendMovement(int x1, int y1, int x2, int y2);
	void GetTurn();
	void GetLobbies();
	void JoinLobby();
	void TakePiece();

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
	
	static const int PORT = 61126;
	static const bool isServer = false;


	// Hold containers of players

	// Hold containers of championships

	// Hold games
};

#endif //__Client_H__