/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Powerup.cpp
 *   Description:  This is the implementation of the Powerup class which defines the powerups for each piece and the visual attributes of each
 *
 */

#include "stdafx.h"
#include "Powerup.h"
#include "PowerUpManager.h"


Powerup::Powerup(void)
{
}


Powerup::~Powerup(void)
{
}

// process the powerup mask and apply the powerups .. should get the current state and add to this 

// sets the appropriate powerup to true otherwise false
void
Powerup::setPowerUpMask(PowerUpManager::PowerUpMask pu)
{
	// set the mask to be blank
	// mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(PowerUpManager::BLANK);
	mPowerUpState = PowerUpManager::BLANK;

	// set the boolean toggles and update the mask using bitwise OR 

	// FORCE was part of the mask
	if (pu & PowerUpManager::FORCE) {
		m_Force = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::FORCE);
	}
	else {
		m_Force = false;
	}

	// SHIELD was part of the mask
	if (pu & PowerUpManager::SHIELD) {
		m_Shield = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::SHIELD);
	}
	else {
		m_Shield = false;
	}

	// SWAP was part of the mask
	if (pu & PowerUpManager::SWAP) {
		m_Swap = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::SWAP);
	}
	else {
		m_Swap = false;
	}

	// LOCK was part of the mask
	if (pu & PowerUpManager::LOCK) {
		m_Locked = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::LOCK);
	}
	else {
		m_Locked = false;
	}

	// KING was part of the mask
	if (pu & PowerUpManager::KING) {
		m_King = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::KING);
	}
	else {
		m_King = false;
	}

	// DETHRONE was part of the mask
	if (pu & PowerUpManager::DETHRONE) {
		m_Dethrone = true;
		// add to the bitmask
		mPowerUpState = static_cast<PowerUpManager::PowerUpMask>(mPowerUpState | PowerUpManager::DETHRONE);
	}
	else {
		m_Dethrone = false;
	}


}

	//// FORCE was part of the mask
	//m_Force = pu & PowerUpManager::FORCE ? true : false;
	//// SHIELD was part of the mask
	//m_Shield = pu & PowerUpManager::SHIELD ? true : false;
	//// SWAP was part of the mask
	//m_Swap = pu & PowerUpManager::SWAP ? true : false;
	//// LOCK was part of the mask
	//m_Locked = pu & PowerUpManager::LOCK ? true : false;
	//// KING was part of the mask
	//m_King = pu & PowerUpManager::KING ? true : false;
	//// DETHRONE was part of the mask
	//m_Dethrone = pu & PowerUpManager::DETHRONE ? true : false;