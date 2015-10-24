#include "stdafx.h"
#include "Board.h"

#include "BoardSquare.h"

Board::Board(void)
{
}


Board::~Board(void)
{
}

// adds a square to the board vector and assign it an id
void
Board::addSquare(int id)
{
	// create the square
	BoardSquare* b = new BoardSquare();
	// assign the id
	b->createPlane(id);
	// pushback onto the boardsquare vector
	m_Board.push_back(b);
}

BoardSquare* 
Board::getSquare(int id) 
{
	// iterate through the array until there is a match on the id.. return that item.. otherwise return a null pointer
	for(auto& i : m_Board) {
		if(i->getID() == id) {
			return i;
		}
		else {
			return nullptr;
		}
	}
}

// returns the scenenode of the board square based on the passed square ID
Ogre::SceneNode* 
Board::getSceneNode(int id, Ogre::SceneManager& sManager)
{
	Ogre::String number = Ogre::StringConverter::toString(id);
	return sManager.getSceneNode("squareNode" + number);
}

// find the piece and return the scenenode based on the piece number 
Ogre::SceneNode* 
Board::getSceneNodeFromPiece(int id, Ogre::SceneManager& sManager)
{
	Ogre::String number = Ogre::StringConverter::toString(id);
	for(auto& i : m_Board) {
		if(i->getPieceID() == id) {
			return sManager.getSceneNode("squareNode" + i->getID());
		}
	}
}