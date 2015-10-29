/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: BoardSquare.cpp
 *   Description:  This is the implementation of the BoardSquare class which defines each square on the board, each square 
 *				   is comprised of a visual plane and also contains an ID, the ID of the piece that occupies it, its occupancy 
 *				   status and some basic getters and setters
 *
 */

#include "BoardSquare.h"


BoardSquare::BoardSquare(void)
{

	m_Occupied = false;
}


BoardSquare::~BoardSquare(void)
{
}

void
BoardSquare::createPlane(int planeNumber) 
{
	// plane number is also the id of the plane (for sorting/searching purposes)
	setID(planeNumber);
	// convert the id to a string 
}