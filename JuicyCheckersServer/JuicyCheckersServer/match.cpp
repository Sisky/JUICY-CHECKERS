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
#include "Board.h"
#include "Piece.h"
#include "BoardSquare.h"
#include "Player.h"



Match::Match()
	: winningPlayer(RakNet::UNASSIGNED_NETWORK_ID)
	, isRunning(true)
	, pBoard(0)
{
	// Trying to call this from elsewhere
	//Initialise();
}

Match::Match(RakNet::RakPeerInterface *_peer)
{
	peer = _peer;
	Match();
}

Match::~Match()
{

}

// There is certain game data that the server keeps a track of for checking of rules and such
// this is identical to the setup on the client except for no gui.
void 
Match::Initialise()
{

		playerOne = new Player();
	playerTwo = new Player();
	playerOne->setPlayerTurn(true);
	playerTwo->setPlayerTurn(false);


	pBoard = new Board();
	// populate the board with empty squares with an id of 1 - 64
	for(int i = 0; i < 64; i++) {
		pBoard->addSquare(i+1);
	}




	int count = 0;
	
	// populate the piece entity vector array
	for(int i = 1; i < 24 + 1; i++) {

		//// convert the count to a string
		//number = Ogre::StringConverter::toString(i);
		
		count += 2;
		
		//// Piece Entity
		Piece* p = new Piece();
		//if (i <= 12) {
		//	// create the entity
		//	p = static_cast<Piece*>(mSceneMgr->createEntity("piece" + number, "robot.mesh"));
		//}
		//else {
		//	// create the entity
		//	p = static_cast<Piece*>(mSceneMgr->createEntity("piece" + number, "ninja.mesh"));
		//}
		//// set the entity query flag
		//p->setQueryFlags(PIECE_MASK);

		//// powerups
		//Powerup* pu = new Powerup();
	
		//p->setPowerUps(pu);
		////// set powerup state to a blank mask
		//mPowerUpManager->setPowerUpMask(p, mPowerUpManager->BLANK, true);


		//// use that board ID to get the scenenode of the boardsquare
		//Ogre::SceneNode* s = pBoard->getSceneNode(count, *mSceneMgr);
		//
		(pBoard->getSquare(count))->setAttachedPiece(p);

		//// create child node of the board square
		//Ogre::SceneNode* pieceNode = s->createChildSceneNode("pieceNode" + number);



		// set the piece ID  1 - 24
		p->setPieceID(i);
		// set visibility
		p->setVisible(true);
		// set the board square ID
		p->setBoardSquareID(count);
		
		// store the original position of the board node in the piece class
		//p->setOrigin(s->getPosition());
		//set up boardsquare
		



		// first 12 will be ninjas
		if(i <= 12) {
			// add the piece
			// p->setMesh("robot.mesh");
			p->setOwner(playerOne);	// player 1
			// rotate
			//pieceNode->yaw(Ogre::Degree(-90));
			//// scale
			//pieceNode->scale(Ogre::Vector3(3,2.5,2));
		}
		else {
		// next 12 will be robots
			// p->setMesh("ninja.mesh");
			p->setOwner(playerTwo); // player 2
		}	


		
		// attach the entity to the node
		//pieceNode->attachObject(p);

		// push to the array
		pPieces.push_back(p);

		// player 1	(robots)
		if(count == 8) { count--; }
		if(count == 15) { count++; }
		// center of board
		if(count == 24) { count = 39; }
		// player 2 (ninjas)
		if(count == 47) { count++; }
		if(count == 56) { count--; }
	}
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
				RakNet::BitStream updateStream;

				// TYPE ID
				updateStream.Write((RakNet::MessageID)ID_USER_GAME_UPDATE);

				// CURRENT PLAYER
				updateStream.Write(currentPlayer);

				// PLAYER ONE
				updateStream.Write(playerOne);

				// PLAYER TWO
				updateStream.Write(playerTwo);

				// Send through the pieces, send the number of
				int pieces = pPieces.size();
				updateStream.Write(pieces);

				// Send the data of each piece
				for(std::vector<Piece*>::iterator pi = pPieces.begin();
					pi != pPieces.end(); 
					++pi)
				{   
					// Write the fields in this order
					updateStream.Write((*pi)->isVisible());	
					//Powerup*		m_PowerUp;		// instace of the powerup class that tracks the powerups associated with the piece
					//Player*				m_Owner;		// piece owner .. player 1 = 1 player 2 = 2
					updateStream.Write((*pi)->getBoardSquareID());// ID of the board square upon which the piece sits
					updateStream.Write((*pi)->getPieceID());		// ID of the piece					
				}
			}
			break;
		case ID_USER_MOVE_PIECE:
			{
				// The client is asking to move a piece
				// Make sure its this clients turn
				if(currentPlayer == packet->guid)
				{
					// Create the bitstream object, read the packet from the bitstream
					RakNet::BitStream moveBitstream(packet->data, packet->length, false);
					moveBitstream.IgnoreBytes(sizeof(RakNet::MessageID));
					moveBitstream.IgnoreBytes(sizeof(RakNet::NetworkID));
					moveBitstream.IgnoreBytes(sizeof(RakNet::NetworkID));
					int positionSrc = 0; int positionDest = 0;
					moveBitstream.Read(positionSrc); moveBitstream.Read(positionDest);

					BoardSquare* bs = pBoard->getSquare(positionSrc);
					Piece* piece = bs->getAttachedPiece();
					// Determine that the source boardsquare has an attached piece
					if(piece != 0)
					{
						// Determine that the destination boardsquare is empty
						if(pBoard->getSquare(positionDest)->getAttachedPiece() == 0)
						{
							bool jumped = false;
							if (isLegalMove(positionSrc, positionDest))
							{	
								if(jumped)
								{   // If the user jumped a piece don't swap turns
									if(currentPlayer == playerOneGUID)
									{
										currentPlayer = playerTwoGUID;
									} 
									else 
									{
										currentPlayer = playerOneGUID;
									}
								}
								// The move is legal we can forward it to all the peers
								RakNet::BitStream movePacket;
								movePacket.Write((RakNet::MessageID)ID_USER_MOVE_PIECE);
								movePacket.Write(positionSrc);
								movePacket.Write(positionDest);

								// Also write who's turn it is now
								movePacket.Write(currentPlayer);

								RakNet::SystemAddress sa1 = peer->GetSystemAddressFromGuid(playerOneGUID);
								RakNet::SystemAddress sa2 = peer->GetSystemAddressFromGuid(playerTwoGUID);
								peer->Send(&movePacket, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa1,false);
								peer->Send(&movePacket, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa2,false);

								// Perform the swap of data
								Piece* cur = pBoard->getSquare(positionSrc)->getAttachedPiece();
								cur->setBoardSquareID(positionDest);
								pBoard->getSquare(positionSrc)->setAttachedPiece(0);
								pBoard->getSquare(positionDest)->setAttachedPiece(cur);


							}
						}
						else
						{
							// Checky client this destination square has a checker
							setTurnError = true;
							break;
						}
					}
					else
					{
						// Checky client this source square has no checker
						setTurnError = true;
						break;
					}


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
	playerOneGUID = p1;
	playerTwoGUID = p2;

	currentPlayer = p1;

	// We will want to send a game update to both players now that the game is setup
	RakNet::BitStream noticationStream;

		// TYPE ID
	RakNet::MessageID typeID = ID_USER_GAME_UPDATE;
	noticationStream.Write(typeID);

	// CURRENT PLAYER
	noticationStream.Write(currentPlayer);

	// PLAYER ONE
	noticationStream.Write(playerOne);

	// PLAYER TWO
	noticationStream.Write(playerTwo);


	RakNet::SystemAddress sa1 = peer->GetSystemAddressFromGuid(p1);
	RakNet::SystemAddress sa2 = peer->GetSystemAddressFromGuid(p2);
	peer->Send(&noticationStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa1,false);
	peer->Send(&noticationStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa2,false);
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

bool
Match::canJump(Player* player)
{
	bool jumpPossible = false;
	bool sqFilled = false;

	if (currentPlayer == playerOneGUID) //check for playerOnes jumps
	{
		for (int i = 0; i < 12; i++) //only check player ones pieces
		{
			int sqID = pPieces[i]->getBoardSquareID(); //selected pieces square

			if (sqID % 8 < 7) //cant be on edges
			{
				for (int j = 0; j < pPieces.size(); j++) //check every piece
				{
					if (sqID + 9 == pPieces[j]->getBoardSquareID() && pPieces[j]->getOwner() == playerTwo) // adjacent square is filled
					{
						for (int k = 0; k < pPieces.size(); k++) // check if jump is possible
						{
							if (sqID + 18 == pPieces[k]->getBoardSquareID())
							{
								sqFilled = true;
							}
							
						}
						if (sqFilled == false)// not filled can jump
						{
							return true;
						}
					}
				}
			}
			if (sqID % 8 > 2) //cant be on edges
			{
				for (int j = 0; j < pPieces.size(); j++) //check every piece
				{
					if (sqID + 7 == pPieces[j]->getBoardSquareID() && pPieces[j]->getOwner() == playerTwo) // adjacent square is filled
					{
						for (int k = 0; k < pPieces.size(); k++) // check if jump is possible
						{
							if (sqID + 14 == pPieces[k]->getBoardSquareID())
							{
								sqFilled = true;
							}

						}
						if (sqFilled == false)// not filled can jump
						{
							return true;
						}
					}
				}
			}
		}
	}
	else //player two turn
		for (int i = 12; i < pPieces.size(); i++) //only check player ones pieces
		{
			int sqID = pPieces[i]->getBoardSquareID(); //selected pieces square

			if (sqID % 8 > 2) //cant be on edges
			{
				for (int j = 0; j < pPieces.size(); j++) //check every piece
				{
					if (sqID - 9 == pPieces[j]->getBoardSquareID() && pPieces[j]->getOwner() == playerOne) // adjacent square is filled
					{
						for (int k = 0; k < pPieces.size(); k++) // check if jump is possible
						{
							if (sqID - 18 == pPieces[k]->getBoardSquareID())
							{
								sqFilled = true;
							}

						}
						if (sqFilled == false)// not filled can jump
						{
							return true;
						}
					}
				}
			}
			if (sqID % 8 < 7) //cant be on edges
			{
				for (int j = 0; j < pPieces.size(); j++) //check every piece
				{
					if (sqID - 7 == pPieces[j]->getBoardSquareID() && pPieces[j]->getOwner() == playerOne) // adjacent square is filled
					{
						for (int k = 0; k < pPieces.size(); k++) // check if jump is possible
						{
							if (sqID - 14 == pPieces[k]->getBoardSquareID())
							{
								sqFilled = true;
							}

						}
						if (sqFilled == false)// not filled can jump
						{
							return true;
						}
					}
				}
			}
		}

	return jumpPossible;
}

bool 
Match::isLegalMove(int sourceID, int destID, bool& jumped)
{
	bool valid = false;
	jumped = false;
	//check whose turn
	if (currentPlayer == playerOneGUID)
	{
		if (sourceID + 9 == destID && canJump(playerOne) != true || sourceID + 7 == destID && canJump(playerOne) != true)//simple one space move, cant if a jump is possible
		{
			valid = true;
			jumped = false;
		}
		else if (sourceID + 18 == destID) //trying to jump right
		{
			
			for (int i = 0; i < pPieces.size(); i++) //check through piece vector
			{
				if (sourceID + 9 == pPieces[i]->getBoardSquareID()) //there is a piece
				{
					if (pPieces[i]->getOwner() == playerTwo) //is opponent piece
					{
						sendPieceTake(pPieces[i]->getBoardSquareID(), pPieces[i]->getPieceID());


						// Player deleted a tile
						pPieces[i]->setVisible(false);
						pBoard->getSquare(pPieces[i]->getBoardSquareID())->setAttachedPiece(0);

						pPieces[i]->setVisible(false);
						// gets the boardsquare node
						pPieces[i]->setBoardSquareID(500);
						valid = true;
						jumped = true;
						break;
					}
				}

			}	
		}
		else if (sourceID + 14 == destID) //trying to jump left
		{

			for (int i = 0; i < pPieces.size(); i++) //check through piece vector
			{
				if (sourceID + 7 == pPieces[i]->getBoardSquareID()) //there is a piece
				{
					if (pPieces[i]->getOwner() == playerTwo) //is opponent piece
					{
						sendPieceTake(pPieces[i]->getBoardSquareID(), pPieces[i]->getPieceID());


						pBoard->getSquare(pPieces[i]->getBoardSquareID())->setAttachedPiece(0);
						// Player deleted a tile
						pPieces[i]->setVisible(false);
						pPieces[i]->setBoardSquareID(500);
					
						valid = true;
						jumped = true;
						break;
					}
				}

			}
		}

	}
	else //player twos turn
	{
		if (sourceID - 9 == destID && canJump(playerTwo) != true || sourceID - 7 == destID && canJump(playerTwo) != true) //simple one space move
		{
			valid = true;
			jumped = false;
		}
		else if (sourceID - 18 == destID) //trying to jump right
		{

			for (int i = 0; i < pPieces.size(); i++) //check through piece vector
			{
				if (sourceID - 9 == pPieces[i]->getBoardSquareID()) //there is a piece
				{
					if (pPieces[i]->getOwner() == playerOne) //is opponent piece
					{
						// Player deleted a tile
						sendPieceTake(pPieces[i]->getBoardSquareID(), pPieces[i]->getPieceID());


						pBoard->getSquare(pPieces[i]->getBoardSquareID())->setAttachedPiece(0);
						pPieces[i]->setVisible(false);
						// gets the boardsquare node
						pPieces[i]->setBoardSquareID(500);
						valid = true;
						jumped = true;
						break;
					}
				}

			}
		}
		else if (sourceID - 14 == destID) //trying to jump left
		{

			for (int i = 0; i < pPieces.size(); i++) //check through piece vector
			{
				if (sourceID - 7 == pPieces[i]->getBoardSquareID()) //there is a piece
				{
					if (pPieces[i]->getOwner() == playerOne) //is opponent piece
					{
						// Player deleted a tile
						// Send the message to all users saying that this piece is taken
						sendPieceTake(pPieces[i]->getBoardSquareID(), pPieces[i]->getPieceID());
						
						pBoard->getSquare(pPieces[i]->getBoardSquareID())->setAttachedPiece(0);
						pPieces[i]->setVisible(false);
						// gets the boardsquare node
						pPieces[i]->setBoardSquareID(500);
						valid = true;
						jumped = true;
						break;
						
					}
				}

			}
		}
	}
	
	
	return valid;

}

void 
Match::sendPieceTake(int boardSquareID, int pieceID)
{
	RakNet::BitStream takeStream;

		// TYPE ID
	RakNet::MessageID typeID = ID_USER_TAKE_PIECE;
	takeStream.Write(typeID);

	// CURRENT PLAYER
	takeStream.Write(boardSquareID);

	// PLAYER ONE
	takeStream.Write(pieceID);


	RakNet::SystemAddress sa1 = peer->GetSystemAddressFromGuid(playerOneGUID);
	RakNet::SystemAddress sa2 = peer->GetSystemAddressFromGuid(playerTwoGUID);
	peer->Send(&takeStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa1,false);
	peer->Send(&takeStream, HIGH_PRIORITY,RELIABLE_ORDERED,0,sa2,false);
}