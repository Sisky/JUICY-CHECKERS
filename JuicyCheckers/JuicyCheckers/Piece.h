#ifndef __PIECE_H__
#define __PIECE_H__

// each piece with have ....
// visible
// position
// model
// powerup state

#include <OgreRoot.h>

class Piece
{
public:
	Piece(void);
	~Piece(void);
	
	// visibility
	bool isVisible()				{ return m_Visible; }
	void setVisible(bool visible)	{ m_Visible = visible; }

	// mesh
	void setMesh(Ogre::String meshName) { m_Mesh = meshName; }
	Ogre::String getMesh()		{ return m_Mesh; }

	// powerups
	void setPowerup(int powerUp) { m_PowerUp = powerUp; }
	int getPowerup()			{ return m_PowerUp; }

	// owner
	void setOwner(int playerNum) { m_Owner = playerNum; }
	int getOwner() { return m_Owner; }

protected:
private:

// member variables
public:
protected:
	bool			m_Visible;	
	Ogre::String	m_Mesh;			// e.g. "ninja.mesh"
	int				m_PowerUp;		// 0 - 10 represeting different powerups
	int				m_Owner;		// piece owner .. player 1 = 1 player 2 = 2

private:
};

#endif __PIECE_H__
