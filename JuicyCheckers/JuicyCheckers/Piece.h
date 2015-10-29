/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Piece.h
 *	 Inherts : Ogre::Entity
 *   Description:  This class defines the pieces, the mesh, the ID of each piece, its visibility state and various
 *				   other bits of information pertaining to the piece state.
 *				   being as this inherits from Ogre::Entity is can be attached to Nodes.
 *
 */

#ifndef __PIECE_H__
#define __PIECE_H__

class Powerup;

class Piece : public Ogre::Entity
{
public:
	Piece(Ogre::SceneManager& sm);
	virtual ~Piece(void);
	
	// initialize
	void init();

	// visibility
	bool isVisible()				{ return m_Visible; }
	void setVisible(bool visible)	{ m_Visible = visible; }



	// powerups
	void setPowerUps(Powerup* powerUp);
	Powerup* getPowerUps()			{ return m_PowerUp; }

	// owner
	void setOwner(int playerNum) { m_Owner = playerNum; }
	int getOwner() { return m_Owner; }

	// board ID
	void setBoardSquareID(int boardSquareID) { m_BoardSquareID = boardSquareID; }
	int getBoardSquareID() { return m_BoardSquareID; }

	// piece ID
	void setPieceID(int pieceID) { m_PieceID = pieceID; }
	int getPieceID() const { return m_PieceID; }

	// piece postion
	void setOrigin(Ogre::Vector3 pos) { m_Origin = pos; }
	Ogre::Vector3 getOrigin() { return m_Origin; }


protected:

private:

// member variables
public:
	// powerup variables
	Ogre::SceneNode* m_PowerUpNode;	// a node that the powerup effects can be attached to

	Ogre::Entity*	m_puKing;		// entity that contains the King mesh and effects
	ParticleUniverse::ParticleSystem*	m_puLock;		// particle effect of the Locked effects
	ParticleUniverse::ParticleSystem*	m_puShield;		// particle effect of the Shield effects
	ParticleUniverse::ParticleSystem*   m_puSwap;		// particle effect of the Swap effect

protected:
	bool			m_Visible;	
	// Ogre::String	m_Mesh;			// e.g. "ninja.mesh"  // deprecated.. no longer required
	Ogre::Vector3	m_Origin;		// original position of the piece
	Powerup*		m_PowerUp;		// instace of the powerup class that tracks the powerups associated with the piece
	int				m_Owner;		// piece owner .. player 1 = 1 player 2 = 2
	int				m_BoardSquareID;// ID of the board square upon which the piece sits
	int				m_PieceID;		// ID of the piece
	bool			m_Movable;		// sets the ability for a piece to move


	
private:
};

#endif __PIECE_H__
