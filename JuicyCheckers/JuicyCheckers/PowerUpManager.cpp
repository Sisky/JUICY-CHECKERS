/*
*   Author:  Dave Brown <wpv9142@aut.ac.nz>
*   Filename: PowerUpManager.cpp
*   Description:  This is the implementation of the PowerUpManager class which handles how the player interacts with the powerups
*
*/

#include "stdafx.h"
#include "PowerUpManager.h"

#include "Powerup.h"
#include "Piece.h"
#include "Player.h"

PowerUpManager::PowerUpManager()
{
}


PowerUpManager::~PowerUpManager()
{
}

// takes the piece, queries its available powerups and return the bitmask
PowerUpManager::PowerUpMask
PowerUpManager::getPowerUps(Piece & p)
{
	return 	p.getPowerUps()->getPowerUpMask();
}

// takes the player, queries its available powerups and returns the bitmask
PowerUpManager::PowerUpMask
PowerUpManager::getPowerUps(Player & p)
{
	return p.getPowerUps()->getPowerUpMask();
}

void 
PowerUpManager::setPowerUpBarGradient(int level, Ogre::SceneNode * powerupBarNode)
{
	// 0--2--4--6
	// |  |  |  |
	// 1--3--5--7
	// get the manual object from the scenenode
	Ogre::ManualObject* mo = static_cast<Ogre::ManualObject*>(powerupBarNode->getAttachedObject(0));
	// to update vertex colours... the whole thing needs to be redefined and colours applied at that point!...dumb

	switch (level) {
	case 0:
		// set all the colour nodes to red 
		
		mo->beginUpdate(0);

			mo->position(-150, 0, 50); mo->colour(1, 0, 0, 1);	// 0
			mo->position(-150, 0, -50); mo->colour(1, 0, 0, 1);	// 1
			mo->position(-50, 0, 50); mo->colour(1, 0, 0, 1);	// 2
			mo->position(-50, 0, -50); mo->colour(1, 0, 0, 1);	// 3
			mo->position(50, 0, 50); mo->colour(1, 0, 0, 1);	// 4
			mo->position(50, 0, -50); mo->colour(1, 0, 0, 1);	// 5
			mo->position(150, 0, 50); mo->colour(1, 0, 0, 1);	// 6
			mo->position(150, 0, -50); mo->colour(1, 0, 0, 1);	// 7

			// create using indexes.. essentially 6 triangles
			// bottom square
			mo->index(2);
			mo->index(3);
			mo->index(0);

			mo->index(3);
			mo->index(1);
			mo->index(0);

			// middle square
			mo->index(4);
			mo->index(5);
			mo->index(2);

			mo->index(5);
			mo->index(3);
			mo->index(2);

			// top square
			mo->index(6);
			mo->index(7);
			mo->index(4);

			mo->index(7);
			mo->index(5);
			mo->index(4);

		mo->end();
		
		break;
	case 1:
		// set the first 2 vertices to green, the next 2 to orange
		mo->beginUpdate(0);

			mo->position(-150, 0, 50); mo->colour(0, 1, 0, 1);	// 0
			mo->position(-150, 0, -50); mo->colour(0, 1, 0, 1);	// 1
			mo->position(-50, 0, 50); mo->colour(1, 1, 0, 1);	// 2
			mo->position(-50, 0, -50); mo->colour(1, 1, 0, 1);	// 3
			mo->position(50, 0, 50); mo->colour(1, 0, 0, 1);	// 4
			mo->position(50, 0, -50); mo->colour(1, 0, 0, 1);	// 5
			mo->position(150, 0, 50); mo->colour(1, 0, 0, 1);	// 6
			mo->position(150, 0, -50); mo->colour(1, 0, 0, 1);	// 7

			// create using indexes.. essentially 6 triangles
			// bottom square
			mo->index(2);
			mo->index(3);
			mo->index(0);

			mo->index(3);
			mo->index(1);
			mo->index(0);

			// middle square
			mo->index(4);
			mo->index(5);
			mo->index(2);

			mo->index(5);
			mo->index(3);
			mo->index(2);

			// top square
			mo->index(6);
			mo->index(7);
			mo->index(4);

			mo->index(7);
			mo->index(5);
			mo->index(4);

		mo->end();
		break;
	case 2:
		// set the first 2 vertices to green, the next 2 to orange
		mo->beginUpdate(0);

			mo->position(-150, 0, 50); mo->colour(0, 1, 0, 1);	// 0
			mo->position(-150, 0, -50); mo->colour(0, 1, 0, 1);	// 1
			mo->position(-50, 0, 50); mo->colour(0, 1, 0, 1);	// 2
			mo->position(-50, 0, -50); mo->colour(0, 1, 0, 1);	// 3
			mo->position(50, 0, 50); mo->colour(1, 1, 0, 1);	// 4
			mo->position(50, 0, -50); mo->colour(1, 1, 0, 1);	// 5
			mo->position(150, 0, 50); mo->colour(1, 0, 0, 1);	// 6
			mo->position(150, 0, -50); mo->colour(1, 0, 0, 1);	// 7

			// create using indexes.. essentially 6 triangles
			// bottom square
			mo->index(2);
			mo->index(3);
			mo->index(0);

			mo->index(3);
			mo->index(1);
			mo->index(0);

			// middle square
			mo->index(4);
			mo->index(5);
			mo->index(2);

			mo->index(5);
			mo->index(3);
			mo->index(2);

			// top square
			mo->index(6);
			mo->index(7);
			mo->index(4);

			mo->index(7);
			mo->index(5);
			mo->index(4);

		mo->end();
		break;
	case 3:
		// all green
		mo->beginUpdate(0);

			mo->position(-150, 0, 50); mo->colour(0, 1, 0, 1);	// 0
			mo->position(-150, 0, -50); mo->colour(0, 1, 0, 1);	// 1
			mo->position(-50, 0, 50); mo->colour(0, 1, 0, 1);	// 2
			mo->position(-50, 0, -50); mo->colour(0, 1, 0, 1);	// 3
			mo->position(50, 0, 50); mo->colour(0, 1, 0, 1);	// 4
			mo->position(50, 0, -50); mo->colour(0, 1, 0, 1);	// 5
			mo->position(150, 0, 50); mo->colour(0, 1, 0, 1);	// 6
			mo->position(150, 0, -50); mo->colour(0, 1, 0, 1);	// 7

			// create using indexes.. essentially 6 triangles
			// bottom square
			mo->index(2);
			mo->index(3);
			mo->index(0);

			mo->index(3);
			mo->index(1);
			mo->index(0);

			// middle square
			mo->index(4);
			mo->index(5);
			mo->index(2);

			mo->index(5);
			mo->index(3);
			mo->index(2);

			// top square
			mo->index(6);
			mo->index(7);
			mo->index(4);

			mo->index(7);
			mo->index(5);
			mo->index(4);

		mo->end();
		
		break;
		


	}
}


void 
PowerUpManager::setPowerUpBar(int level, Ogre::SceneNode * powerUpBarNode)
{
	// set the material of the entites attached to the scenenode

	Ogre::SceneNode* s1 = static_cast<Ogre::SceneNode*>(powerUpBarNode->getChild(0));
	Ogre::SceneNode* s2 = static_cast<Ogre::SceneNode*>(powerUpBarNode->getChild(1));
	Ogre::SceneNode* s3 = static_cast<Ogre::SceneNode*>(powerUpBarNode->getChild(2));

	// there will be 3 entities
	Ogre::Entity* e1 = static_cast<Ogre::Entity*>(s1->getAttachedObject(0));
	Ogre::Entity* e2 = static_cast<Ogre::Entity*>(s2->getAttachedObject(0));
	Ogre::Entity* e3 = static_cast<Ogre::Entity*>(s3->getAttachedObject(0));

	switch (level) {
	case 0:
		e1->setMaterialName("Juicy/Red");
		e2->setMaterialName("Juicy/Red");
		e3->setMaterialName("Juicy/Red");
		break;
	case 1:
		e1->setMaterialName("Juicy/Green");
		e2->setMaterialName("Juicy/Red");
		e3->setMaterialName("Juicy/Red");
		break;
	case 2:
		e1->setMaterialName("Juicy/Green");
		e2->setMaterialName("Juicy/Green");
		e3->setMaterialName("Juicy/Red");
		break;
	case 3:
		e1->setMaterialName("Juicy/Green");
		e2->setMaterialName("Juicy/Green");
		e3->setMaterialName("Juicy/Green");
		break;
	}
}

// sets the powerup mask on the passed piece
void 
PowerUpManager::setPowerUpMask(Piece * p, PowerUpMask mask, bool overwrite)
{
	// if overwrite... set as a new mask
	if (overwrite) {
		p->getPowerUps()->setPowerUpMask(mask);
	}
	else {
		// get existing mask and add to it
		p->getPowerUps()->setPowerUpMask(static_cast<PowerUpManager::PowerUpMask>(p->getPowerUps()->getPowerUpMask() ^ mask));
	}
}

// sets the powerup mask on the passed player
void 
PowerUpManager::setPowerUpMask(Player * p, PowerUpMask mask, bool overwrite)
{
	if (overwrite) {
		p->getPowerUps()->setPowerUpMask(mask);
	}
	else {
		// get existing mask and add to it
		p->getPowerUps()->setPowerUpMask(static_cast<PowerUpManager::PowerUpMask>(p->getPowerUps()->getPowerUpMask() | mask));
	}
}

// activates the powerups of the passed piece
void 
PowerUpManager::applyPowerUps(Piece * p)
{
	// process the powerups
	if (hasKing(p)) {
		p->toggleKing();
	}

	if (hasShield(p)) {
		p->toggleShield();
	}

	if (hasLock(p)) {
		p->toggleLock();
	}

}

// activates the powerups of the passed player
void 
PowerUpManager::applyPowerUps(Player * p)
{

}

// returns true if the piece has the associated powerup in their bitmask
bool 
PowerUpManager::hasShield(Piece * p)
{
	if (p->getPowerUps()->getPowerUpMask() & SHIELD) {
		return true;
	}
	else {
		return false;
	}
}

bool 
PowerUpManager::hasLock(Piece * p)
{
	if (p->getPowerUps()->getPowerUpMask() & LOCK) {
		return true;
	}
	else {
		return false;
	}
}

bool 
PowerUpManager::hasKing(Piece * p)
{
	if (p->getPowerUps()->getPowerUpMask() & KING) {
		return true;
	}
	else {
		return false;
	}
}

bool 
PowerUpManager::hasForce(Player * p)
{
	if (p->getPowerUps()->getPowerUpMask() & FORCE) {
		return true;
	}
	else {
		return false;
	}
}

bool 
PowerUpManager::hasSwap(Player * p)
{
	if (p->getPowerUps()->getPowerUpMask() & SWAP) {
		return true;
	}
	else {
		return false;
	}
}

bool 
PowerUpManager::hasDethrone(Player * p)
{
	if (p->getPowerUps()->getPowerUpMask() & DETHRONE) {
		return true;
	}
	else {
		return false;
	}
}










