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
		p->getPowerUps()->setPowerUpMask(static_cast<PowerUpManager::PowerUpMask>(p->getPowerUps()->getPowerUpMask() | mask));
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










