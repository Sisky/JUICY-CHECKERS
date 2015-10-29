/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Piece.cpp
 *	 Inherts : Ogre::Entity
 *   Description:  This is the implementation of the Piece class defines the pieces, the mesh, the ID of each piece, its 
 *				   visibility state and various other bits of information pertaining to the piece state.
 *				   being as this inherits from Ogre::Entity is can be attached to Nodes.
 *
 */

#include "stdafx.h"
#include "main.h"
#include "Piece.h"
#include "Powerup.h"



Piece::Piece(Ogre::SceneManager& sm)
	:m_PowerUpNode(0),
	m_puKing(0),
	m_puLock(0),
	m_puShield(0),
	m_puSwap(0),
	m_PowerUp(0)
{
	// setup the scenenode... this needs to be attached to a 
	// m_PowerUpNode = sm.createSceneNode("powerUpSceneNode");

}


Piece::~Piece(void)
{
}


// initialize the Piece... prime the powerups for each piece
void Piece::initPowerups(Ogre::SceneManager* sm, ParticleUniverse::ParticleSystemManager* pm)
{
	m_PowerUp = new Powerup();

	Ogre::String id = Ogre::StringConverter::toString(getPieceID());

	// setup the Crown entity and node
	m_puKing = sm->createEntity("crownEntity" + id, "crown.mesh");
	Ogre::SceneNode* crownNode = m_PowerUpNode->createChildSceneNode("crownNode" + id);
	m_puKing->setQueryFlags(JuicyCheckers::POWERUP_MASK);
	// attach the entity to the node
	crownNode->attachObject(m_puKing);
	// scale and position the node
	crownNode->scale(70, 70, 70);
	crownNode->setPosition(0, 80, 0);
	// set visibility and query flags
	crownNode->setVisible(false);
	
	// setup the shield effect and node
	m_puShield = pm->createParticleSystem("shieldParticle" + id, "example_004", sm);
	Ogre::SceneNode* shieldNode = m_PowerUpNode->createChildSceneNode("shieldNode" + id);
	m_puShield->setQueryFlags(JuicyCheckers::POWERUP_MASK);
	// scale and position the particle effect
	m_puShield->setScaleVelocity(10);
	m_puShield->setScale(Ogre::Vector3(20, 40, 20));
	m_puShield->getTechnique(0)->position= Ogre::Vector3(0, 2.5, 0);
	// attach effect to the scene node
	shieldNode->attachObject(m_puShield);

	
	// setup the lock effect and node
	m_puLock = pm->createParticleSystem("lockParticle" + id, "Shield", sm);
	Ogre::SceneNode* lockNode = m_PowerUpNode->createChildSceneNode("lockNode" + id);
	m_puLock->setQueryFlags(JuicyCheckers::POWERUP_MASK);
	// scale and position the particle effect
	m_puLock->setScaleVelocity(10);
	m_puLock->setScale(Ogre::Vector3(10, 120, 10));
	m_puLock->getTechnique(0)->position = Ogre::Vector3(0, 1, 0);
	// attach effect to the scene node
	lockNode->attachObject(m_puLock);

	// setup the swap effect and node
	m_puSwap = pm->createParticleSystem("swapParticle" + id, "teleport", sm);
	Ogre::SceneNode* swapNode = m_PowerUpNode->createChildSceneNode("swapNode" + id);
	m_puSwap->setQueryFlags(JuicyCheckers::POWERUP_MASK);
	// scale and position the particle effect
	m_puSwap->setScaleVelocity(10);
	m_puSwap->setScale(Ogre::Vector3(30, 30, 30));
	m_puSwap->getTechnique(0)->position = Ogre::Vector3(0, 0, 0);
	// attach effect to the scene node
	swapNode->attachObject(m_puSwap);

	// scale and rotate the nodes as needed
}

void Piece::setPowerUps(Powerup * powerUp)
{
	// assign the powerup to the class
	m_PowerUp = powerUp;

	// process the powerup and apply the appropriate effect


}

void 
Piece::toggleKing()
{
	Ogre::String id = Ogre::StringConverter::toString(getPieceID());
	Ogre::SceneNode*  m = static_cast<Ogre::SceneNode*>(m_PowerUpNode->getChild("crownNode" + id));
	m->flipVisibility();
}

void 
Piece::toggleLock()
{
	if (m_puLock->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
		m_puLock->start();
	} 
	else {
		m_puLock->stop();
	}
}

void 
Piece::toggleShield()
{
	if (m_puShield->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
		m_puShield->start();
	}
	else {
		m_puShield->stop();
	}
}

void 
Piece::toggleSwap()
{
	if (m_puSwap->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
		m_puSwap->start();
	}
	else {
		m_puSwap->stop();
	}
}



