/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: match.h
 *   Description:  This is the match class this handles the playing of a game between two players
 *					
 *	
 */

#ifndef __MATCH_H__
#define __MATCH_H__

#include "NetworkIDObject.h"
#include "RakNetTypes.h"

class Match : public RakNet::NetworkIDObject
{
	// Member Functions
public:
	Match();
	Match(RakNet::RakPeerInterface *_peer);
	~Match();

	// Do a general process on this match
	void Process(float _delta);

	// Process the packet that is intended for this match
	void ProcessPacket(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);

	void SetPlayers(RakNet::RakNetGUID p1, RakNet::RakNetGUID p2);

	bool GetStatus();
	RakNet::RakNetGUID GetWinner();

protected:

private:

	// Member Variables
public:

protected:

private:
	RakNet::RakNetGUID playerOne;
	RakNet::RakNetGUID playerTwo;

	// Hold a pointer to the RakPeerInterface
	RakNet::RakPeerInterface *peer;

	// The current players turn
	RakNet::RakNetGUID currentPlayer;

	RakNet::RakNetGUID winningPlayer;

	bool isRunning;

	// Probably will need to hold some kind of Game data here for 
	// the verification of moves
	

};

#endif //__MATCH_H__