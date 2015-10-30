/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: PieceController.cpp
 *   Description:  This is the implementation of the Piece which class provides some helper functions for moving the pieces on the board
 *
 */

#include "stdafx.h"
#include "PieceController.h"
#include "client.h"
#include "Piece.h"
#include "main.h"
#include <OgreString.h>

PieceController::PieceController(void)
{
	m_sourceNode = nullptr;
	m_destNode = nullptr;
	m_clientNetwork = nullptr;
}


PieceController::~PieceController(void)
{
}

// move the piece from one node to the other
void
PieceController::movePiece(Ogre::SceneNode* source, Ogre::SceneNode* dest)
{
	// source node will be a child node... so will need to detach from the parent node
	source->getParentSceneNode()->removeChild(source);

	// destination : create a child node on the square and attach the source node
	dest->addChild(source);

	Ogre::SceneNode* c = static_cast<Ogre::SceneNode*>(m_destNode->getChild(0));
	Piece* e = static_cast<Piece*>(c->getAttachedObject(0));
	int destBoardSquare = stringToInt(m_destNode->getName());

	e->setBoardSquareID(destBoardSquare);
	// reset the source and dest pointers to null
	m_sourceNode = nullptr;
	m_destNode = nullptr;
}

// move the piece from one node to the other
void
PieceController::movePiece()
{
	// source node will be a child node... so will need to detach from the parent node
	m_sourceNode->getParentSceneNode()->removeChild(m_sourceNode);

	// destination : create a child node on the square and attach the source node
	m_destNode->addChild(m_sourceNode);

	//set childs new square

	Ogre::SceneNode* c = static_cast<Ogre::SceneNode*>(m_destNode->getChild(0));
	Piece* e = static_cast<Piece*>(c->getAttachedObject(0));
	int destBoardSquare = stringToInt(m_destNode->getName());

	e->setBoardSquareID(destBoardSquare);


	
	// reset the source and dest pointers to null
	m_sourceNode = nullptr;
	m_destNode = nullptr;
}


// move the designated piece in a direction... will return true if ts able... and false if unable
bool
PieceController::testMovePiece(Ogre::SceneNode* piece, PieceController::PieceDirection direction)
{
	return true;
}

// Network Methods
void 
PieceController::setNetworkInterface(Client* _client)
{
	m_clientNetwork = _client;
}

Client* 
PieceController::getNetworkInterface()
{
	return m_clientNetwork;
}

void 
PieceController::moveNetworkPiece()
{
	Ogre::String srcName = m_sourceNode->getParent()->getName();
	Ogre::String destName = m_destNode->getName(); //squareNode
	Ogre::String srcResult = Ogre::StringUtil::replaceAll(srcName, "squareNode", "");
	Ogre::String destResult = Ogre::StringUtil::replaceAll(destName, "squareNode", "");
	
	m_clientNetwork->SendMovement(atoi(srcResult.c_str()), atoi(destResult.c_str()));

	// reset the source and dest pointers to null
	m_sourceNode = nullptr;
	m_destNode = nullptr;
}

int
PieceController::stringToInt(Ogre::String string)
{

	for (int i = 0; i < (int)string.size() - 1; ++i)
	{
		if (!isdigit(string[i]))
		{
			string.erase(string.begin() + i);
			--i;
		}
	}
	int num = atoi(string.c_str());
	return num;
}
