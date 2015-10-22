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