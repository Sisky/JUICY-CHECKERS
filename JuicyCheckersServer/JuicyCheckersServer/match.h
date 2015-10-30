/*
 *   Author:  Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: match.h
 *   Description:  This is the match class this handles the playing of a game between two players
 *					
 *	
 */

#ifndef __MATCH_H__
#define __MATCH_H__

#include <vector>

#include "NetworkIDObject.h"
#include "RakNetTypes.h"

class Piece;
class Player;
class Board;

class Match : public RakNet::NetworkIDObject
{
	// Member Functions
public:
	Match();
	Match(RakNet::RakPeerInterface *_peer);
	~Match();

	// Initialise all the game data
	void Initialise();

	// Do a general process on this match
	void Process(float _delta);

	// Process the packet that is intended for this match
	void ProcessPacket(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);

	void SetPlayers(RakNet::RakNetGUID p1, RakNet::RakNetGUID p2);

	bool GetStatus();
	RakNet::RakNetGUID GetWinner();

	bool isLegalMove(int srcID, int destID);
	bool canJump(Player* player);

protected:
	void sendPieceTake(int boardSquareID, int pieceID);

private:

	// Member Variables
public:

protected:

private:
	RakNet::RakNetGUID playerOneGUID;
	RakNet::RakNetGUID playerTwoGUID;

	// Hold a pointer to the RakPeerInterface
	RakNet::RakPeerInterface *peer;

	// The current players turn
	RakNet::RakNetGUID currentPlayer;

	RakNet::RakNetGUID winningPlayer;

	bool isRunning;

	// Probably will need to hold some kind of Game data here for 
	// the verification of moves
	// vector array of pieces.. total of 12 per side (24 in total)
	std::vector<Piece*> pPieces;

	// the player boards
	Board* pBoard;

	Player* playerOne;
	Player* playerTwo;


};

#endif //__MATCH_H__