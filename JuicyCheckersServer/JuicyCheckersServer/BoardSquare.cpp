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
	: attachedPiece(0)
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
	//Ogre::String number = Ogre::StringConverter::toString(planeNumber);
	//// define the plane object
	//m_BoardSquare = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
	//// define the plane as 200 x 200
	//Ogre::MeshManager::getSingleton().createPlane("boardSquare" + number, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,m_BoardSquare, 200, 200, 1, 1,true,1, 2, 2,Ogre::Vector3::UNIT_Z);
}

void 
BoardSquare::setAttachedPiece(Piece* _piece)
{
	attachedPiece = _piece;
}

Piece* 
BoardSquare::getAttachedPiece()
{
	return attachedPiece;
}