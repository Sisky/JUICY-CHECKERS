/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: lobby.h
 *   Description:  This is the implementation of the Lobby class,  This class handles holding players in a lobby for chat.
 *					This class also handles the starting of a championship from the lobby	
 *	
 */

// This include
#include "lobby.h"

// Library includes
#include <algorithm>

// Local Includes

Lobby::Lobby()
{
	
}

Lobby::~Lobby()
{

}

void 
Lobby::Process(float _delta)
{

}

void 
Lobby::RelayChat(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	// Loop over all the clients connected to this lobby

}

void 
Lobby::SetHostingPlayer(RakNet::RakNetGUID hostingPlayer)
{
	mHostingPlayer = hostingPlayer;
	AddClient(hostingPlayer);
}

void 
Lobby::AddClient(RakNet::RakNetGUID guid)
{
	mPlayerContainer.push_back(guid);
}

void 
Lobby::RemoveClient(RakNet::RakNetGUID guid)
{
	mPlayerContainer.erase(std::remove(mPlayerContainer.begin(), mPlayerContainer.end(), guid), mPlayerContainer.end());
}