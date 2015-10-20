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
	// Destroy the RakPeerInterface instance
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void
	Server::Initialize()
{
	// Start the RakNet server
	RakNet::SocketDescriptor sd(PORT,0);
	peer->Startup(MAX_CLIENTS, &sd, 1);
	printf("Starting up Server");

	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	mIsRunning = true;
}

void 
	Server::Process(float _delta)
{
	// Every Frame we check to see if there have been new network messages
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
				printf("Our connection request has been accepted.\n");

				// This is where we now send a join request to the server to be added to the chat

				//// Use a BitStream to write a custom user message
				//// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				//RakNet::BitStream bsOut;
				////bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				//bsOut.Write("Hello world");
				//peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
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
			}
			break;
		case ID_USER_GET_LOBBYS:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_JOIN_LOBBY:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_LOBBY_CHAT:
			{
				// The user has joined the server 
			}
			break;
		case ID_USER_MASTER_CHAT:
			{
				// The user has joined the server 
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