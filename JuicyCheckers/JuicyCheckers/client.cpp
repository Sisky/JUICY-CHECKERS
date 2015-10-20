/*
*   Author:  Jony Hill <xyc8034@aut.ac.nz>
*   Filename: network.cpp
*   Description:  This is the implemetation of the network class, this Client version will handle the management of
*					players connecting, lobby joining/creation, and general Client management
*
*/	

// Includes 
#include "client.h"

#include <tchar.h>
#include <stdio.h>
#include <string.h>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
#include "BitStream.h"

#include "stdafx.h"

#include "datastructures.h"

Client::Client()
	: peer(0)
{
	// Create the reference to the rakpeer interface
	peer = RakNet::RakPeerInterface::GetInstance();

	// Now Initialise RakNet
	Initialize();
}

Client::~Client()
{
	// Destroy the RakPeerInterface instance
	peer->Shutdown(200);

	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void
	Client::Initialize()
{
	// Start the RakNet Client
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);
	peer->Connect("127.0.0.1", PORT, 0, 0);	
}

void 
	Client::Process(float _delta)
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

				// This is where we now send a join request to the Client to be added to the chat

				//// Use a BitStream to write a custom user message
				//// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				//RakNet::BitStream bsOut;
				////bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				//bsOut.Write("Hello world");
				//peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

				// Now send a request to the server to join chat
				RakNet::MessageID typeId; // This will be assigned to a type I've added after ID_USER_PACKET_ENUM, lets say ID_SET_TIMED_MINE				
				
				typeId=ID_USER_JOIN_SERVER;
				RakNet::BitStream myBitStream;
				myBitStream.Write(typeId);
				// Assume we have a Mine* mine object
				myBitStream.Write(5);				

				peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The Client is full.\n");
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
Client::handleUserPacket(RakNet::Packet* packet)
{
	// Handle any packets that are relating to the user, this is to seperate out
	// raknet packets and the users packets.
	bool handled = true;
	switch(packet->data[0])
	{
		case ID_USER_JOIN_SERVER:
			{
				// The server has accepted our request to join the server
				RakNet::MessageID typeId=ID_USER_MASTER_CHAT; // This will be assigned to a type I've added after ID_USER_PACKET_ENUM, lets say ID_SET_TIMED_MINE
				RakNet::RakString rakString("Name: Hello prepare to be checked");
								
				RakNet::BitStream myBitStream;				
				myBitStream.Write(typeId);
				myBitStream.Write(rakString);

				peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_GET_LOBBYS:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_JOIN_LOBBY:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_LOBBY_CHAT:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_MASTER_CHAT:
			{
				// We are receving some master chat add it to the chat buffer
				// Get the bitstream out of the packet and forward it on to all connected peers
				RakNet::BitStream myBitStream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data
				RakNet::RakString rs;
				myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				myBitStream.Read(rs);


				Ogre::LogManager::getSingletonPtr()->logMessage(rs.C_String());
			}
			break;
		case ID_USER_CHAMPIONSHIP_REGISTER:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_GAME_UPDATE:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_MOVE_PIECE:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_KING_PIECE:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_TAKE_PIECE:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_SPEND_UPGRADE:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_GET_UPGRADES:
			{
				// The user has joined the Client 
			}
			break;
		case ID_USER_ERROR:
			{
				// The user has joined the Client 
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