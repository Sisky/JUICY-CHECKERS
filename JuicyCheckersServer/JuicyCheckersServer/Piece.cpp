/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Piece.cpp
 *	 Inherts : Ogre::Entity
 *   Description:  This is the implementation of the Piece class defines the pieces, the mesh, the ID of each piece, its 
 *				   visibility state and various other bits of information pertaining to the piece state.
 *				   being as this inherits from Ogre::Entity is can be attached to Nodes.
 *
 */


#include "Piece.h"
//#include "Powerup.h"


Piece::Piece(/*Ogre::SceneManager& sm*/)
	:/*m_PowerUpNode(0),*/
	/*m_puKing(0),
	m_puLock(0),
	m_puShield(0),
	m_puSwap(0),*/
	m_PowerUp(0)
{
	// setup the scenenode... this needs to be attached to a 
	// m_PowerUpNode = sm.createSceneNode("powerUpSceneNode");

}


Piece::~Piece(void)
{
}

void Piece::setPowerUps(Powerup * powerUp)
{
	// assign the powerup to the class
	m_PowerUp = powerUp;

	// process the powerup and apply the appropriate effect


}



