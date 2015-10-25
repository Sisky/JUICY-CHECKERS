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

Match::Match()
{

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
	switch(packet->data[0])
	{
		case ID_USER_CHAMPIONSHIP_REGISTER:
			{

			}
			break;
		case ID_USER_GAME_UPDATE:
			{

			}
			break;
		case ID_USER_MOVE_PIECE:
			{

			}
			break;
		case ID_USER_KING_PIECE:
			{

			}
			break;
		case ID_USER_TAKE_PIECE:
			{

			}
			break;
		case ID_USER_SPEND_UPGRADE:
			{

			}
			break;
		case ID_USER_GET_UPGRADES:
			{

			}
			break;
	}
}

void Match::SetPlayers(RakNet::RakNetGUID p1, RakNet::RakNetGUID p2)
{

}