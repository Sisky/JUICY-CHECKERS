/*
*   Author:  Dave Brown <wpv9142@aut.ac.nz>
*   Filename: PowerUpManager.h
*   Description:  This is the PowerUpManager class that handles how the player or piece interacts with the powerups
*
*/

#ifndef __POWERUPMANAGER_H__
#define __POWERUPMANAGER_H__

// powerup manager is instanced in the main game and will be responsible for informing the piece or player of the available powerups

/* PowerUp List
:: Force - Move an opponents piece 1 square
:: Shield - Piece cant be jumped by an opponent
:: Swap - Swap locations with another piece
:: Lock - Lock down an opponents piece for 1 turn
:: King - upgrade to king (happnes when piece is at the opposing side of the game board
:: Dethrone - downgrade a King to a normal piece
*/

class Piece;
class Player;

class PowerUpManager
{
public:
	PowerUpManager();
	~PowerUpManager();

	// use bitshifting on the power ups to allow for setting multiple types
	enum PowerUpMask
	{
		BLANK = 0,
		FORCE = 1 << 0,
		SHIELD = 1 << 1,
		SWAP = 1 << 2,
		LOCK = 1 << 3,
		KING = 1 << 4,
		DETHRONE = 1 << 5
	};


	// bitmask returning the powerups currently active on the piece
	PowerUpMask getPowerUps(Piece& p);
	// bitmask returning the powerups currently active on the player
	PowerUpMask getPowerUps(Player& p);

	// set the bitmask for the power up for the piece... either updating or overwrite
	void setPowerUpMask(Piece* p, PowerUpMask mask, bool overwrite);
	// set the bitmask for the power up for the player
	void setPowerUpMask(Player* p, PowerUpMask mask, bool overwrite);

	void applyPowerUps(Piece* p);
	void applyPowerUps(Player* p);
	// query functions to determine if the player or piece has certain powerups
	// PIECE
	bool hasShield(Piece* p);
	bool hasLock(Piece* p);
	bool hasKing(Piece* p);

	// PLAYER
	bool hasForce(Player* p);
	bool hasSwap(Player* p);
	bool hasDethrone(Player* p);


protected:
private:

	// member variables
public:
protected:
private:
};

#endif __POWERUPMANAGER_H__