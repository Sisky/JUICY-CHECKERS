/*
*   Author:  Jony Hill <xyc8034@aut.ac.nz>
*   Filename: network.cpp
*   Description:  This is the implemetation of the network class, this Client version will handle the management of
*					players connecting, lobby joining/creation, and general Client management
*
*/	
#include "stdafx.h"

// Includes 
#include "client.h"

#include <tchar.h>
#include <stdio.h>
#include <string.h>


#include "client.h"

#include "Board.h"
#include "PieceController.h"

#include "Player.h"
#include "Piece.h"

Client::Client()
	: peer(0)
	, lobbyID(RakNet::UNASSIGNED_NETWORK_ID)
	, currentMatchID(RakNet::UNASSIGNED_NETWORK_ID)
	, isConnected(false)
	, transitionMatch(false)
{
	// Create the reference to the rakpeer interface
	peer = RakNet::RakPeerInterface::GetInstance();

	// Now Initialise RakNet
	//Initialize();
}

Client::~Client()
{
	// Destroy the RakPeerInterface instance
	peer->Shutdown(200);

	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void
	Client::Initialize(const char* ip, const char* _name)
{
	// Start the RakNet Client
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);
	peer->Connect(ip, PORT, 0, 0);	

	name = _name;
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
				// We should probably transition to the lobby listing state now
				// and update the listing of lobbies from the server
				RakNet::BitStream lobbyStream;
				lobbyStream.Write((RakNet::MessageID)ID_USER_GET_LOBBYS);

				peer->Send(&lobbyStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				
				// Keep a copy of the serversGUID
				serverGUID = packet->guid;
				isConnected = true;


				// Transistion the Menu system

			}
			break;
		case ID_USER_GET_LOBBYS:
			{
				// The server has sent a response with all the current game lobbies
				// Get the BitStream the server sent
				RakNet::BitStream lobbyStream(packet->data, packet->length, false);

				// Ignore the bytes that state the message id
				lobbyStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Get the number of lobbies
				int numLobbies;
				lobbyStream.Read(numLobbies);
								
				// Get the network ID's of the lobbies the server sent
				// The vector will hold the current network ids of the lobbies
				// the client can then request join a lobby
				lobbyNetworkID.clear();
				for(int i = 0; i < numLobbies; ++i)
				{
					// Read the lobby network ID and read the lobbyName
					RakNet::NetworkID currentID; lobbyStream.Read(currentID);
					RakNet::RakString lobbyName; lobbyStream.Read(lobbyName);
					LobbyMsg msg;
					msg.networkID = currentID;
					msg.name = lobbyName;

					lobbyNetworkID.push_back(msg);
				}				
			}
			break;
		case ID_USER_JOIN_LOBBY:
			{
				// The server uses this enum constant whenever a new user joins to update all
				// players
				// Get the bitstream
				RakNet::BitStream playersStream(packet->data, packet->length, false);

				// Ignore the message header
				playersStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Get the number of players in this message
				int numPlayers = 0;
				playersStream.Read(numPlayers);

				// Clear the players vector
				LobbyUsers.clear();

				// Copy all the players into the vector
				for(int i = 0; i < numPlayers; ++i)
				{
					RakNet::RakString cur;
					playersStream.Read(cur);

					LobbyUsers.push_back(cur);
				}												
			}
			break;
		case ID_USER_JOIN_MATCH:
			{
				// The server has started the championship and has sent us the matchID
				// of the game we are playing
				RakNet::BitStream matchStream(packet->data, packet->length, false);

				// Ignore the message header
				matchStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Get the network ID
				RakNet::NetworkID matchID; 
				matchStream.Read(matchID);

				// Save the network ID of the match we are in
				currentMatchID = matchID;

				// Set a flag so the menu system can transition to the match menu
				transitionMatch = true;

				// Ask the server to send us an update of all the game objects
				RakNet::BitStream request;
				request.Write((RakNet::MessageID)ID_USER_GAME_UPDATE);
				peer->Send(&request, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
			}
			break;
		case ID_USER_CREATE_LOBBY:
			{
				// The Server has responded to our request to join a lobby
				// The server will have put us in the lobby so we should procress to the lobby screen

				
			}
			break;
		case ID_USER_LOBBY_CHAT:
			{
				// We have received some forwarded chat that is destined for the lobby we are currently in
				RakNet::BitStream myBitStream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data
				RakNet::RakString rs;
				myBitStream.IgnoreBytes(sizeof(RakNet::MessageID));	// Ignore the Message ID
				myBitStream.IgnoreBytes(sizeof(RakNet::NetworkID)); // Ignore the lobby ID
				myBitStream.Read(rs);

				// The RakString contains the message being sent to us
				Ogre::LogManager::getSingletonPtr()->logMessage(rs.C_String());
				raknetLobbyChat.push_back(rs);

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
				raknetMasterChat.push_back(rs);

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
				// The server has sent data that contains the entire current state of the game
				// recompile this information from the bitstream
				RakNet::BitStream updateBitstream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data

				// Ignore the message id
				updateBitstream.IgnoreBytes(sizeof(RakNet::MessageID));

				// CURRENT PLAYER
				RakNet::RakNetGUID currentPlayer; updateBitstream.Read(currentPlayer);
				currentPlayerTurn = currentPlayer;
				// PLAYER ONE
				RakNet::NetworkID p1; updateBitstream.Read(p1);

				// PLAYER TWO
				RakNet::NetworkID p2; updateBitstream.Read(p2);

				// Now we get the position of all the pieses

			}
			break;
		case ID_USER_MOVE_PIECE:
			{
				// This is the server issuing a move piece command
				RakNet::BitStream moveBitstream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data
				moveBitstream.IgnoreBytes(sizeof(RakNet::MessageID));
	
				// Position1 Source
				// Position2 Dest
				int srcPosition = 0; int destPosition = 0;
				moveBitstream.Read(srcPosition); moveBitstream.Read(destPosition);

				// CURRENT PLAYER
				RakNet::RakNetGUID currentPlayer; moveBitstream.Read(currentPlayer);


				

				pPieceController->setSource(static_cast<Ogre::SceneNode*>(pBoard->getSceneNode(srcPosition)->getChild(0)));
				pPieceController->setDestination(pBoard->getSceneNode(destPosition));
				pPieceController->movePiece();

				if(currentPlayerTurn == currentPlayer)
				{
					// The server is given the currentPlayer another turn don't swap turns
				}
				else
				{
					playerOne->setPlayerTurn(playerTwo->getPlayerTurn());
					playerTwo->setPlayerTurn(!playerOne->getPlayerTurn());
				}
				
				currentPlayerTurn = currentPlayer;

			}
			break;
		case ID_USER_KING_PIECE:
			{
				// This is the server issuing a command to kind a certain piece.
			}
			break;
		case ID_USER_TAKE_PIECE:
			{
				// The server is issueing a command to say that a piece was taken
				RakNet::BitStream takeBitstream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data
				takeBitstream.IgnoreBytes(sizeof(RakNet::MessageID));  //TypeID
				
				// Get the boardsquare and piece ids
				int boardSquareID = 0; takeBitstream.Read(boardSquareID);
				int pieceID = 0; takeBitstream.Read(pieceID);

				// Remove the checker
				Ogre::SceneNode* node;
		
				// gets the boardsquare node
				node = pBoard->getSceneNode(boardSquareID);

				// get the piece node
				Ogre::SceneNode* pieceNode = static_cast<Ogre::SceneNode*>(node->getChild(0));
				
				// removes the child
				node->removeChild(pieceNode);
				
				// Set the piece to be invalid
				(*mpPieces)[pieceID+1]->setBoardSquareID(500);
				//(*mpPieces)[pieceID+1]->setVisible(false);
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

void 
Client::SendMovement(int source, int dest)
{
	// This function will send the movement the user requested to the server
	// for verification and to update the other peers
	RakNet::MessageID sendMovement = ID_USER_MOVE_PIECE;
	RakNet::BitStream command;
	command.Write(sendMovement);
	command.Write(lobbyID);
	command.Write(currentMatchID);
	command.Write(source);
	command.Write(dest);

	peer->Send(&command, HIGH_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
}

void 
Client::GetTurn()
{

}

std::vector<LobbyMsg>*
Client::GetLobbies()
{
	// Return the vector of Lobbies that includes the name and network ids.
	return &lobbyNetworkID;
}


void
Client::RefreshLobbies()
{
	// Ask the server to refresh us on the current lobbies
	// Create the bitstream that we will send to the server	
	RakNet::BitStream bitstreamLobby;
	RakNet::MessageID typeID = ID_USER_GET_LOBBYS;
	bitstreamLobby.Write(typeID);

	// Send the bitsteam to the server
	peer->Send(&bitstreamLobby, HIGH_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);	
}

void 
Client::JoinLobby(int lobbyIndex)
{
	// Ask the server to join us to the lobby
	RakNet::BitStream bitstreamLobby;
	RakNet::MessageID typeID = ID_USER_JOIN_LOBBY;
	bitstreamLobby.Write(typeID);

	// Write the networkID of the lobby that we want to join
	bitstreamLobby.Write(lobbyNetworkID[lobbyIndex].networkID);
	bitstreamLobby.Write(name);

	lobbyID = lobbyNetworkID[lobbyIndex].networkID;

	// Send the bitsteam to the server
	peer->Send(&bitstreamLobby, HIGH_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);	
}

void 
Client::TakePiece()
{

}

void 
Client::CreateLobby(RakNet::RakString& name)
{
	// Create the bitstream that we will send to the server
	RakNet::BitStream createLobbyBitstream;

	// Write the message id into the bitstream
	// Note we don't specify the network ID as the server owns
	// the actual object
	RakNet::MessageID typeID = ID_USER_CREATE_LOBBY;
	createLobbyBitstream.Write(typeID);

	// Write the Name into the bitstream
	createLobbyBitstream.Write(name);

	// Send the bitsteam to the server
	peer->Send(&createLobbyBitstream, HIGH_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);

}

void Client::SendMasterChat(const char* _chat)
{
	// Create the bitstream that we will send to the server
	RakNet::BitStream createMsgStream;

	// Write the message id into the bitstream
	// Note we don't specify the network ID as the server owns
	// the actual object
	RakNet::MessageID typeID = ID_USER_MASTER_CHAT;
	createMsgStream.Write(typeID);

	// Write the chat message to a rakstring then send it over the network
	RakNet::RakString msg = _chat;
	createMsgStream.Write(msg);

	// Send the bitsteam to the server
	peer->Send(&createMsgStream, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
}

void Client::SendLobbyChat(const char* _chat)
{
	// Create the bitstream that we will send to the server
	RakNet::BitStream createMsgStream;

	// Write the message id into the bitstream
	// Note we don't specify the network ID as the server owns
	// the actual object
	RakNet::MessageID typeID = ID_USER_LOBBY_CHAT;
	createMsgStream.Write(typeID);

	createMsgStream.Write(lobbyID);

	// Write the chat message to a rakstring then send it over the network
	RakNet::RakString msg = _chat;
	createMsgStream.Write(msg);

	// Send the bitsteam to the server

	peer->Send(&createMsgStream, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
}

void Client::SendMatchChat(const char* _chat)
{
	//// Create the bitstream that we will send to the server
	//RakNet::BitStream createMsgStream;

	//// Write the message id into the bitstream
	//// Note we don't specify the network ID as the server owns
	//// the actual object
	//RakNet::MessageID typeID = ID_USER_MASTER_CHAT;
	//createMsgStream.Write(typeID);

	//// Write the chat message to a rakstring then send it over the network
	//RakNet::RakString msg = _chat;
	//createMsgStream.Write(msg);

	//// Send the bitsteam to the server
	//peer->Send(&createMsgStream, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
}

std::deque<RakNet::RakString>* 
Client::GetMasterChatLog()
{
	return &raknetMasterChat;
}

std::deque<RakNet::RakString>* 
Client::GetLobbyChatLog()
{
	return &raknetLobbyChat;
}

std::deque<RakNet::RakString>* 
Client::GetMatchChatLog()
{
	return 0;
}

bool 
Client::getIsConnected()
{
	return isConnected;
}

std::vector<RakNet::RakString>* 
Client::GetLobbyUsers()
{
	return &LobbyUsers;
}

void 
Client::sendReady()
{
	RakNet::BitStream createMsgStream;

	// Write the message id into the bitstream
	// Note we don't specify the network ID as the server owns
	// the actual object
	RakNet::MessageID typeID = ID_USER_LOBBY_READY;

	// Write the type id and our lobby id
	createMsgStream.Write(typeID);
	createMsgStream.Write(lobbyID);


	// Send the bitsteam to the server
	peer->Send(&createMsgStream, MEDIUM_PRIORITY,RELIABLE_ORDERED,0,serverGUID,false);
}

bool Client::getTransitionMatch()
{
	return transitionMatch;
}

void Client::setTransitionMatch(bool doTransition)
{
	transitionMatch = doTransition;
}

void 
Client::setPieceController(PieceController* _pControler)
{
	pPieceController = _pControler;
}

PieceController* 
Client::getPieceController()
{
	return pPieceController;
}

void
Client::setBoard(Board* _pBoard)
{
	pBoard = _pBoard;
}

bool 
Client::isOurTurn()
{
	if(currentPlayerTurn == peer->GetMyGUID())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void 
Client::setPlayers(Player* p1, Player* p2)
{
	playerOne = p1;
	playerTwo = p2;
}
