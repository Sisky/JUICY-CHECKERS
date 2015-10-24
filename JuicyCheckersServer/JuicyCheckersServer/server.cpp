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

#include "errors.h"

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

				// Write all the lobby network ids and lobby names to the stream so the client can pick from one
				for(std::vector<Lobby*>::iterator lobby = mLobbies.begin();
					lobby != mLobbies.end(); 
					++lobby)
				{   
					lobbyBitStream.Write((*lobby)->GetNetworkID());
					lobbyBitStream.Write((*lobby)->GetName());
				}

				peer->Send(&lobbyBitStream, LOW_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_JOIN_LOBBY:
			{
				printf("New User Joining Lobby\n");
				// The user is attmepting to join a lobby.  First get the NetworkID of the lobby the player
				// is wishing to join
				RakNet::BitStream lobbyStream(packet->data, packet->length, false);
				lobbyStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::NetworkID lobbyID = RakNet::UNASSIGNED_NETWORK_ID;
				lobbyStream.Read(lobbyID);

				// Make sure this lobby exists and if so add the user, if the lobby does not exist return the user
				// an error
				bool doesLobbyExist = false;
				for(std::vector<Lobby*>::iterator lobby = mLobbies.begin();
					lobby != mLobbies.end(); 
					++lobby)
				{   
					if((*lobby)->GetNetworkID() == lobbyID)
					{
						// This lobby does exist
						doesLobbyExist = true;
						break;
					}
				}

				// Send the result back to the user
				RakNet::BitStream myBitStream;	
				if(doesLobbyExist)
				{
					RakNet::MessageID typeId = ID_USER_JOIN_LOBBY;
					myBitStream.Write(typeId);
				}
				else
				{
					RakNet::MessageID typeId = ID_USER_ERROR;
					myBitStream.Write(typeId);
					myBitStream.Write(ERROR_ID_NO_LOBBY);
				}
																																		
				peer->Send(&myBitStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_CREATE_LOBBY:
			{
				// A user wants to create their own lobby
				printf("A user is creating their own lobby\n");

				// Get the data out from the packet
				RakNet::BitStream createLobby(packet->data, packet->length, false);
				createLobby.IgnoreBytes(sizeof(RakNet::MessageID));

				RakNet::RakString name;
				createLobby.Read(name);

				// Create a new lobby object
				Lobby* lob = new Lobby();
				lob->SetHostingPlayer(packet->guid);
				lob->GetNetworkID();

				// Add the lobby to our lobby container
				mLobbies.push_back(lob);

				// Send the NetworkID of this lobby back to the user
				RakNet::BitStream replyLobby;
				replyLobby.Write((RakNet::MessageID)ID_USER_CREATE_LOBBY);
				replyLobby.Write(lob->GetNetworkID());

				// Send the reply to the client		
				peer->Send(&replyLobby, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
			}
			break;
		case ID_USER_LOBBY_CHAT:
			{
				// If we get a message for the lobby chat relay the message to the correct lobby as indicated by the LobbyID
				RakNet::BitStream lobbyChat(packet->data, packet->length, false);
				lobbyChat.IgnoreBytes(sizeof(RakNet::MessageID));

				// Get the NetworkID of the lobby that this chat is for
				RakNet::NetworkID lobbyID = RakNet::UNASSIGNED_NETWORK_ID;
				lobbyChat.Read(lobbyID);

				// Check to see that this lobby exists, if the lobby exists forward the message onto the lobby object
				// if the lobby does not exist return the user an error message
				bool doesLobbyExist = false;
				Lobby* curLobby = 0;
				for(std::vector<Lobby*>::iterator lobby = mLobbies.begin();
					lobby != mLobbies.end(); 
					++lobby)
				{   
					if((*lobby)->GetNetworkID() == lobbyID)
					{
						// This lobby does exist
						doesLobbyExist = true;
						break;
					}
				}

				if(doesLobbyExist && curLobby != 0)
				{
					curLobby->RelayChat(peer, packet);
				}
				else
				{
					RakNet::BitStream replyLobby;
					replyLobby.Write((RakNet::MessageID)ID_USER_ERROR);
					replyLobby.Write(ERROR_ID_LOBBYCHAT);
				}
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

			// For any of the below cases we forward the packet onto the lobby which will forward the packet
			// to the appropriate match
		case ID_USER_CHAMPIONSHIP_REGISTER:
		case ID_USER_GAME_UPDATE:
		case ID_USER_MOVE_PIECE:
		case ID_USER_KING_PIECE:
		case ID_USER_TAKE_PIECE:
		case ID_USER_SPEND_UPGRADE:
		case ID_USER_GET_UPGRADES:
			{
				// The user is attempting to move a piece filter the packet up through the lobby to the
				// match that is resonsible for the move
				RakNet::BitStream myBitStream(packet->data, packet->length, false);
				myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::NetworkID lobbyID = RakNet::UNASSIGNED_NETWORK_ID;
				myBitStream.Read(lobbyID);

				bool doesLobbyExist = false;
				Lobby* curLobby = 0;
				for(std::vector<Lobby*>::iterator lobby = mLobbies.begin();
					lobby != mLobbies.end(); 
					++lobby)
				{   
					if((*lobby)->GetNetworkID() == lobbyID)
					{
						// This lobby does exist
						doesLobbyExist = true;
						break;
					}
				}

				if(doesLobbyExist && curLobby != 0)
				{
					curLobby->ProcessNetworkMessage(peer, packet);
				}
				else
				{
					RakNet::BitStream replyLobby;
					replyLobby.Write((RakNet::MessageID)ID_USER_ERROR);
					replyLobby.Write(ERROR_ID_LOBBYCHAT);
				}
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