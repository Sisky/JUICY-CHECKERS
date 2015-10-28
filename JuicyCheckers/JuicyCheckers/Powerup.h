/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Powerup.h
 *   Description:  This class defines the powerups for each piece and the visual attributes of each
 *
 */

#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "PowerUpManager.h"
/* PowerUp List
	:: Force - Move an opponents piece
	:: Shield - Piece cant be jumped by an opponent
	:: Swap - Swap locations with another piece
	:: Lock - Lock down an opponents piece for 1 turn
	:: King - upgrade to king (happnes when piece is at the opposing side of the game board
	:: Dethrone - downgrade a King to a normal piece
*/

// Each powerup has an associated sound effect and visual effect
class PowerUpManager;

class Powerup
{
public:
	Powerup(void);
	~Powerup(void);


	// this is where you set and get the powerups using the mask via the powerup manager

	void setPowerUpMask(PowerUpManager::PowerUpMask pu);
	PowerUpManager::PowerUpMask getPowerUpMask() { return mPowerUpState; }

protected:
private:

	// member variables
public:
protected:
	
private:
	PowerUpManager::PowerUpMask mPowerUpState;
	// a set of boolean variables that can be queried that will effect the game state
	bool m_Force;			// sets the ability to move an opponents piece
	bool m_Shield;			// sets the ability for the piece to be jumped
	bool m_Swap;			// sets the ability to swap a piece
	bool m_Locked;			// sets if the piece is locked in place
	bool m_King;			// sets the king state
	bool m_Dethrone;		// sets the ability to dethrone a king


};

#endif __POWERUP_H__