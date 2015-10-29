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

#include <RakPeerInterface.h>
#include <BitStream.h>

#include "MessageIdentifiers.h"

#include "errors.h"

#include "match.h"

// Local Includes

Lobby::Lobby(RakNet::RakPeerInterface *_peer)
	: isRunning(false)
{
	peer = _peer;
}

Lobby::Lobby()
	: isRunning(false)
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
Lobby::SetName(RakNet::RakString _name)
{
	name = _name;
}

RakNet::RakString 
Lobby::GetName()
{
	return name;
}

void 
	Lobby::RelayChat(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	// Loop over all the clients connected to this lobby
	// If we get a message from other user pass the message onto all connected clients
	// Iterate over the RakNet GUID's and send a message to all of them.

	for(std::vector<player>::iterator connectedPeer = mPlayerContainer.begin();
		connectedPeer != mPlayerContainer.end(); 
		++connectedPeer)
	{   
		//if((packet->guid) == (*connectedPeer)) { continue; } // Don't forward the message back to the sending peer			
		RakNet::SystemAddress sa = peer->GetSystemAddressFromGuid((*connectedPeer).guid);

		// Get the bitstream out of the packet and forward it on to all connected peers
		RakNet::BitStream myBitStream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data		

		peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa,false);

	}

	// Display the lobby message 
	RakNet::BitStream myBitStream(packet->data, packet->length, false);
	printf("User Sent Master Chat Message\n");
	myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	myBitStream.IgnoreBytes(sizeof(RakNet::NetworkID));
	RakNet::RakString msg;
	myBitStream.Read(msg);

	printf("Message: "); printf(msg.C_String()); printf("\n");				
}

void 
	Lobby::SetHostingPlayer(RakNet::RakNetGUID hostingPlayer)
{
	mHostingPlayer = hostingPlayer;
	//AddClient(hostingPlayer);
}

void 
	Lobby::AddClient(RakNet::RakNetGUID guid, RakNet::RakString& name)
{
	player p;
	p.guid = guid;
	p.name = name;
	mPlayerContainer.push_back(p);
}

bool 
	Lobby::RemoveClient(RakNet::RakNetGUID guid)
{
	bool didRemove = false;
	//mPlayerContainer.erase(std::remove(mPlayerContainer.begin(), mPlayerContainer.end(), guid), mPlayerContainer.end());
	for(std::vector<player>::iterator players = mPlayerContainer.begin();
		players != mPlayerContainer.end(); 
		++players)
	{   
		if((*players).guid == guid)
		{
			// This lobby does exist
			players = mPlayerContainer.erase(players);
			didRemove = true;
			break;
		}
	}

	return didRemove;
}

void 
	Lobby::ProcessNetworkMessage(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	// This function processes network messages that are directed at this lobby
	// if then gets the network id of the match and forwards it on
	RakNet::BitStream myBitStream(packet->data, packet->length, false);
	myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));	// Ignore the message identifer 
	myBitStream.IgnoreBytes(sizeof(RakNet::NetworkID)); // Ignore the ID of the Lobby

	// Get the ID of the match that this message belongs to
	RakNet::NetworkID matchID = RakNet::UNASSIGNED_NETWORK_ID;
	myBitStream.Read(matchID);

	// Check to see that we have a match with this ID
	// if we do forward the message onto the match
	bool doesMatchExist = false;
	Match* curMatch = 0;
	for(std::vector<Match*>::iterator match = mMatches.begin();
		match != mMatches.end(); 
		++match)
	{   
		if((*match)->GetNetworkID() == matchID)
		{
			// This lobby does exist
			doesMatchExist = true;
			curMatch = (*match);
			break;
		}
	}

	if(doesMatchExist && curMatch != 0)
	{
		curMatch->ProcessPacket(peer, packet);
	}
	else
	{
		RakNet::BitStream replyLobby;
		replyLobby.Write((RakNet::MessageID)ID_USER_ERROR);
		replyLobby.Write(ERROR_ID_MATCHID);
	}

}

void 
Lobby::SendPlayerUpdate(RakNet::RakPeerInterface* peer)
{
	// Prepare the packet
	RakNet::BitStream playerUpdate;
	playerUpdate.Write((RakNet::MessageID)ID_USER_JOIN_LOBBY);
	playerUpdate.Write((int)(mPlayerContainer.size()));

	for(std::vector<player>::iterator connectedPeer = mPlayerContainer.begin();
		connectedPeer != mPlayerContainer.end(); 
		++connectedPeer)
	{   
		//if((packet->guid) == (*connectedPeer)) { continue; } // Don't forward the message back to the sending peer			
		if(connectedPeer->ready)
		{
			playerUpdate.Write(RakNet::RakString(connectedPeer->name + ":READY"));
		}
		else
		{
			playerUpdate.Write(RakNet::RakString(connectedPeer->name + ":UNREADY"));
		}
	}

	// Send the packet to all peers
	for(std::vector<player>::iterator connectedPeer = mPlayerContainer.begin();
		connectedPeer != mPlayerContainer.end(); 
		++connectedPeer)
	{   
		//if((packet->guid) == (*connectedPeer)) { continue; } // Don't forward the message back to the sending peer			
		RakNet::SystemAddress sa = peer->GetSystemAddressFromGuid((*connectedPeer).guid);

		peer->Send(&playerUpdate, LOW_PRIORITY,RELIABLE_ORDERED,0,sa,false);

	}
}

void Lobby::ProcessDisconnect(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	if(RemoveClient(packet->guid))
	{
		SendPlayerUpdate(peer);
	}
}

bool 
Lobby::SetReady(RakNet::Packet* packet)
{
	bool changed = false;
	for(std::vector<player>::iterator players = mPlayerContainer.begin();
		players != mPlayerContainer.end(); 
		++players)
	{   
		if((*players).guid == packet->guid)
		{
			(*players).ready = !((*players).ready);
			changed = true;
			break;
		}
	}

	// Check to make sure how many players have set their ready
	// if all players in the lobby are ready.
	int numReady = 0;
	for(std::vector<player>::iterator players = mPlayerContainer.begin();
		players != mPlayerContainer.end(); 
		++players)
	{   
		if((*players).ready == true)
		{
			++numReady;
		}
	}

	if(numReady == mPlayerContainer.size() && mPlayerContainer.size() >= 2)
	{
		// All the players are ready launch the game(s)
		StartChamps();
	}


	return changed;
}

void 
Lobby::StartChamps()
{
	// Should should notify the server to not allow new connections

	isRunning = true;

	// Start the championship by splitting every two players into match objects
	int numPlayers = mPlayerContainer.size();
	bool playersEven = (numPlayers % 2) == 0;
	int numMatches = numPlayers/2;

	// We should have atleast two players ready
	for(int i = 0; i < numMatches; ++i)
	{
		Match* currentMatch = 0;
		currentMatch = new Match(peer);
		RakNet::NetworkID matchID = currentMatch->GetNetworkID();
		currentMatch->SetPlayers(mPlayerContainer[(i*2)].guid,mPlayerContainer[(i*2)+1].guid);

		// We need to send a packet to these players indicating the match network id
		RakNet::BitStream noticationStream;
		RakNet::MessageID typeID = ID_USER_JOIN_MATCH;
		noticationStream.Write(typeID);

		// Send the NetworkID of the match
		noticationStream.Write(matchID);
				

		RakNet::SystemAddress sa1 = peer->GetSystemAddressFromGuid((mPlayerContainer[(i*2)].guid));
		RakNet::SystemAddress sa2 = peer->GetSystemAddressFromGuid((mPlayerContainer[(i*2)+1].guid));
		peer->Send(&noticationStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa1,false);
		peer->Send(&noticationStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa2,false);
		printf("Sending A match notification to two players\n");

		// Push the match onto the container for matches
		mMatches.push_back(currentMatch);
	}

	if(!playersEven)
	{
		// There is one player that is left over in the championship forward them to the next round
		mWinnerPlayers.push_back(*(mPlayerContainer.end()));
	}	
}

bool 
Lobby::getIsRunning()
{
	return isRunning;
}