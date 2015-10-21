#include "stdafx.h"
#include "BoardSquare.h"


BoardSquare::BoardSquare(void)
{

	m_Occupied = false;
	m_Piece = nullptr;
}


BoardSquare::~BoardSquare(void)
{
}

void
BoardSquare::createPlane(int planeNumber) 
{
	Ogre::String number = Ogre::StringConverter::toString(planeNumber);
	// define the plane object
	m_BoardSquare = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
	// define the plane as 200 x 200
	Ogre::MeshManager::getSingleton().createPlane("boardSquare" + number, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,m_BoardSquare, 200, 200, 1, 1,true,1, 2, 2,Ogre::Vector3::UNIT_Z);
}