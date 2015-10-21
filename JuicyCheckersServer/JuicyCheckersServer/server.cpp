/*
*   Author:  Jony Hill <xyc8034@aut.ac.nz>
*   Filename: network.cpp
*   Description:  This is the implemetation of the network class, this server version will handle the management of
*					players connecting, lobby joining/creation, and general server management
*
*/	

// Includes 
#include "server.h"

#include <tchar.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <iterator>


#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID

#include <RakString.h>

#include "datastructures.h"

#include "lobby.h"

Server::Server()
	: peer(0)
	, mIsRunning(false)
{
	// Create the reference to the rakpeer interface
	peer = RakNet::RakPeerInterface::GetInstance();

	// Now Initialise RakNet
	Initialize();
}

Server::~Server()
{
	peer->Shutdown(300);

	// Destroy the RakPeerInterface instance
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void
	Server::Initialize()
{
	// Start the RakNet server
	RakNet::SocketDescriptor sd(PORT,0);
	peer->Startup(MAX_CLIENTS, &sd, 1);
	printf("Starting up Server\n");

	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	mIsRunning = true;

	// BELOW HERE IS FOR DEBUG PURPOSES AND SHOULD NOT EXIST
	// create a new lobby

	
	
}

void 
	Server::Process(float _delta)
{
	// This sleep aparently keeps raknet responsive
	Sleep(30);

//	 Every Frame we check to see if there have been new network messages
	RakNet::Packet *packet; // The current packet

	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				
			}
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (isServer){
				printf("A client has disconnected.\n");
			} else {
				printf("We have been disconnected.\n");
			}
			break;
		case ID_CONNECTION_LOST:
			if (isServer){
				printf("A client lost the connection.\n");
			} else {
				printf("Connection lost.\n");
			}
			break;

		default:			
			if(!handleUserPacket(packet))
			{
				// Unhandled Packet
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
			}
			break;
		}
	}
}

bool 
Server::handleUserPacket(RakNet::Packet* packet)
{
	// Handle any packets that are relating to the user, this is to seperate out
	// raknet packets and the users packets.
	bool handled = true;
	switch(packet->data[0])
	{
		case ID_USER_JOIN_SERVER:
			{
				// The user has joined the server 
				printf("New Player Requesting to join the server\n");
				printf("System Address: "); printf(packet->systemAddress.ToString());; printf("\n");
				printf("RakNet GUID: "); printf(packet->guid.ToString());; printf("\n");
				mConnectedUsers.push_back(packet->guid);

				// Pass the user a success message back
				RakNet::MessageID typeId = ID_USER_JOIN_SERVER;
				RakNet::RakString rakString("Name: Hello prepare to be checked");	

				RakNet::BitStream myBitStream;			
				myBitStream.Write(typeId);
				myBitStream.Write(rakString);

				peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				
			}
			break;
		case ID_USER_GET_LOBBYS:
			{
				printf("User Getting Lobbies\n");
				// The user is attempting to request a listing of all the lobbys
				RakNet::MessageID typeID = ID_USER_GET_LOBBYS;

				// Create the BitStream for the lobby
				RakNet::BitStream lobbyBitStream;
				lobbyBitStream.Write(typeID); // Write the type of the network address

				// Write the number of lobbys, and then after that send the NUID of this lobby.
				int numLobbies = mLobbies.size();
				lobbyBitStream.Write(numLobbies);

				// Write all the lobby network ids to the stream so the client can pick from one
				for(std::vector<Lobby*>::iterator lobby = mLobbies.begin();
					lobby != mLobbies.end(); 
					++lobby)
				{   
					lobbyBitStream.Write((*lobby)->GetNetworkID());
				}

				peer->Send(&lobbyBitStream, LOW_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_JOIN_LOBBY:
			{
				printf("New User Joining Lobby\n");
				// The user is attmepting to join a lobby. Note:  If the lobby the player has chosen does not exist
				// create it for the user
				RakNet::NetworkID nID = CreateLobby(packet->guid);

				// Pass the user a success message back
				RakNet::MessageID typeId = ID_USER_JOIN_LOBBY;
				
				RakNet::BitStream myBitStream;			
				myBitStream.Write(typeId);
				
				peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_LOBBY_CHAT:
			{
				// If we get a message for the lobby chat relay the message to the correct lobby as indicated by the LobbyID

			}
			break;
		case ID_USER_MASTER_CHAT:
			{
				// If we get a message from other user pass the message onto all connected clients
				// Iterate over the RakNet GUID's and send a message to all of them.
				

				for(std::vector<RakNet::RakNetGUID>::iterator connectedPeer = mConnectedUsers.begin();
					connectedPeer != mConnectedUsers.end(); 
					++connectedPeer)
				{   
					//if((packet->guid) == (*connectedPeer)) { continue; } // Don't forward the message back to the sending peer			
					RakNet::SystemAddress sa = peer->GetSystemAddressFromGuid(*connectedPeer);

					// Get the bitstream out of the packet and forward it on to all connected peers
					RakNet::BitStream myBitStream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data		

					peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa,false);

				}
					RakNet::BitStream myBitStream(packet->data, packet->length, false);
					printf("User Sent Master Chat Message\n");
					myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));
					RakNet::RakString msg;
					myBitStream.Read(msg);

					printf("Message: "); printf(msg.C_String()); printf("\n");				
			}
			break;
		case ID_USER_CHAMPIONSHIP_REGISTER:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_GAME_UPDATE:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_MOVE_PIECE:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_KING_PIECE:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_TAKE_PIECE:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_SPEND_UPGRADE:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_GET_UPGRADES:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_ERROR:
			{
				// The user has joined the server 
			}
			break;
		default:			
			{
				handled = false;
			}
			break;
	}

	return handled;
}

bool
Server::isRunning()
{
	return (mIsRunning);
}

// This function creates a new lobby
RakNet::NetworkID 
Server::CreateLobby(RakNet::RakNetGUID playerGUID)
{
	Lobby* newLobby = new Lobby();
	newLobby->SetNetworkIDManager(&NUIDManager);

	newLobby->SetHostingPlayer(playerGUID);

	mLobbies.push_back(newLobby);

	return newLobby->GetNetworkID();
}