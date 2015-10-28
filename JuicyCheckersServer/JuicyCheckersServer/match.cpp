/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: match.h
 *   Description:  This is the implementation of the match class this handles the playing of a game between two players
 *					
 *	
 */

// This include
#include "match.h"
#include "MessageIdentifiers.h"
#include <BitStream.h>
#include "RakNetTypes.h"  // MessageID
#include <RakPeerInterface.h>

Match::Match()
	: winningPlayer(RakNet::UNASSIGNED_NETWORK_ID)
	, isRunning(true)
{

}

Match::Match(RakNet::RakPeerInterface *_peer)
{
	peer = _peer;
	Match();
}

Match::~Match()
{

}

// Do a general process on this match
void Match::Process(float _delta)
{

}

// Process the packet that is intended for this match
void Match::ProcessPacket(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	bool setTurnError = false;
	switch(packet->data[0])
	{
		case ID_USER_CHAMPIONSHIP_REGISTER:
			{

			}
			break;
		case ID_USER_GAME_UPDATE:
			{
				// The client is asking for an entire update of the game state
				// The structure of the game update is
				
				// TYPE ID
				// CURRENT PLAYER
				// PLAYER ONE
				// PLAYER TWO

				// Then we enter the locations of all the current pieses
			}
			break;
		case ID_USER_MOVE_PIECE:
			{
				// The client is asking to move a piece
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{



					// If the move was successful send the update to all the 
				}
				else
				{
					// Checky client it is not your turn
					setTurnError = true;
					break;
				}
			}
			break;
		case ID_USER_KING_PIECE:
			{
				// The client wants to king a piece
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{

				}
				else
				{
					// Checky client it is not your turn
					setTurnError = true;
					break;
				}
			}
			break;
		case ID_USER_TAKE_PIECE:
			{
				// The client wants to jump a piece
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{

				}
				else
				{
					// Checky client it is not your turn
					setTurnError = true;
					break;
				}
			}
			break;
		case ID_USER_SPEND_UPGRADE:
			{
				// The client wants to use an upgrade
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{

				}
				else
				{
					// Checky client it is not your turn
					setTurnError = true;
					break;
				}
			}
			break;
		case ID_USER_GET_UPGRADES:
			{
				// The client wants to get their upgrade points
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{

				}
				else
				{
					// Checky client it is not your turn
					setTurnError = true;
					break;
				}
			}
			break;
	}
	if(setTurnError)
	{
		RakNet::BitStream errorStream;
		errorStream.Write((RakNet::MessageID)ID_USER_TURN_ERROR);
		peer->Send(&errorStream, LOW_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);
	}
}

void Match::SetPlayers(RakNet::RakNetGUID p1, RakNet::RakNetGUID p2)
{
	playerOne = p1;
	playerTwo = p2;

	currentPlayer = p1;
}

bool 
Match::GetStatus()
{
	return isRunning;
}

RakNet::RakNetGUID 
Match::GetWinner()
{
	return winningPlayer;
}