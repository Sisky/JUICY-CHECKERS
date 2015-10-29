/*
*   Author:  Dave Brown <wpv9142@aut.ac.nz>
*	Networking Revisions:  Jony Hill <xyc8034@aut.ac.nz>
*   Filename: Player.h
*   Description:  This is the Player class that will handle player specific interactions
*
*/
#ifndef __PLAYER_H__
#define __PLAYER_H__

class Powerup;


class Player
{
public:
	Player(void);
	~Player(void);

	Powerup* getPowerUps() { return mPowerUp; }
	void setPowerUps(Powerup* pu) { mPowerUp = pu; }
	void setPlayerTurn(bool isTurn) { mTurn = isTurn; }
	bool getPlayerTurn() { return mTurn; }

protected:
private:

	// member variables
public:
protected:
	Powerup* mPowerUp;	// defines the powerups associated with the player class
	bool mTurn; // is players turn t/f
private:
};

#endif __PLAYER_H__