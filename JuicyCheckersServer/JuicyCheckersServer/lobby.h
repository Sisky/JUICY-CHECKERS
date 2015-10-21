/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: lobby.h
 *   Description:  This is the Lobby class,  This class handles holding players in a lobby for chat.
 *					This class also handles the starting of a championship from the lobby	
 *	
 */

#ifndef __LOBBY_H__
#define __LOBBY_H__

#include <vector>
#include "RakNetTypes.h"  // MessageID
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"

class Lobby : public RakNet::NetworkIDObject
{
	// Member Functions
public:
	Lobby();
	~Lobby();

	void Process(float _delta);
	void RelayChat(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);

	void SetHostingPlayer(RakNet::RakNetGUID hostingPlayer);

	void AddClient(RakNet::RakNetGUID guid);
	void RemoveClient(RakNet::RakNetGUID guid);


	

protected:

private:

	// Member Variables
public:

protected:

private:
	// Hold a container of RakNetGUID's that are part of this
	// lobby
	std::vector<RakNet::RakNetGUID> mPlayerContainer;

	// Hold the GUID of the player that created this lobby
	// as only this player should have authorative control
	RakNet::RakNetGUID mHostingPlayer;

	// Hold a container of Games that are part of this lobby/championship
	//std::vector<Match*> mMatches;
	
};

#endif // __LOBBY_H__