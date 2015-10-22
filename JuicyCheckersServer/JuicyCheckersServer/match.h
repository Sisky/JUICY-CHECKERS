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
	~Match();

	// Do a general process on this match
	void Process(float _delta);

	// Process the packet that is intended for this match
	void ProcessPacket(RakNet::Packet packet);

	void SetPlayers(RakNet::RakNetGUID p1, RakNet::RakNetGUID p2);

protected:

private:

	// Member Variables
public:

protected:

private:
	RakNet::RakNetGUID playerOne;
	RakNet::RakNetGUID playerTwo;

	// Probably will need to hold some kind of Game data here for 
	// the verification of moves

};

#endif //__MATCH_H__