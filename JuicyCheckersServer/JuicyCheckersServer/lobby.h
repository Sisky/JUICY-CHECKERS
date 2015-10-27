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
#include "RakString.h"

#include "match.h"

class Lobby : public RakNet::NetworkIDObject
{
	// Member Functions
public:
	Lobby();
	~Lobby();

	void Process(float _delta);
	void RelayChat(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);

	void SetHostingPlayer(RakNet::RakNetGUID hostingPlayer);

	void AddClient(RakNet::RakNetGUID guid, RakNet::RakString& name);
	bool RemoveClient(RakNet::RakNetGUID guid);

	// This method will create all the games and assign two players to each game.
	void StartGames();

	void SetName(RakNet::RakString _name);
	RakNet::RakString GetName();

	void ProcessNetworkMessage(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);

	void SendPlayerUpdate(RakNet::RakPeerInterface* peer);

	void ProcessDisconnect(RakNet::RakPeerInterface* peer,RakNet::Packet* packet);

	bool SetReady(RakNet::Packet* packet);
	

protected:

private:

	// Member Variables
public:

protected:

private:
	// Store what the name of this lobby is
	RakNet::RakString name;
	
	// Hold a container of RakNetGUID's that are part of this
	// lobby
	struct player
	{
		RakNet::RakNetGUID guid;
		RakNet::RakString name;
		bool ready;
		player()
		{			
			ready = false;
		}
	};
	std::vector<player> mPlayerContainer;

	// Hold the GUID of the player that created this lobby
	// as only this player should have authorative control
	RakNet::RakNetGUID mHostingPlayer;

	// Hold a container of Games that are part of this lobby/championship
	std::vector<Match*> mMatches;
	
};

#endif // __LOBBY_H__