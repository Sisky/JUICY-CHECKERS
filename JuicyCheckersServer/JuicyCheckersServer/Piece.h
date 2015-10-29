/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Revisions: Jony Hill <xyc8034@aut.ac.nz>
 *   Filename: Piece.h
 *	 Inherts : 
 *   Description:  This class defines the pieces, the ID of each piece, its visibility state and various
 *				   other bits of information pertaining to the piece state.
 *
 */

#ifndef __PIECE_H__
#define __PIECE_H__


class Piece// : public Ogre::Entity
{
public:
	Piece(void);
	virtual ~Piece(void);
	
	// visibility
	bool isVisible()				{ return m_Visible; }
	void setVisible(bool visible)	{ m_Visible = visible; }

	// powerups
	void setPowerup(int powerUp) { m_PowerUp = powerUp; }
	int getPowerup()			{ return m_PowerUp; }

	// owner
	void setOwner(int playerNum) { m_Owner = playerNum; }
	int getOwner() { return m_Owner; }

	// board ID
	void setBoardSquareID(int boardSquareID) { m_BoardSquareID = boardSquareID; }
	int getBoardSquareID() { return m_BoardSquareID; }

	// piece ID
	void setPieceID(int pieceID) { m_PieceID = pieceID; }
	int getPieceID() { return m_PieceID; }

	// piece postion
	//void setOrigin(Ogre::Vector3 pos) { m_Origin = pos; }
	//Ogre::Vector3 getOrigin() { return m_Origin; }

	// entity .. no longer needed as this IS the entity now
	// void createEntity(Ogre::SceneManager& sManager);
	// Ogre::Entity* getEntity()	 { return m_Entity; }

	// void setEntity(Ogre::Entity* entity) { m_Entity = entity; }

	//// node
	//void setNode(Ogre::SceneNode* node) { m_Node = node; }
	//Ogre::SceneNode* getNode() { return m_Node; }

	//// attaches the node to the specified node
	//void attach(Ogre::SceneNode* node);
	//// detaches the node from the specified node
	//void detach(Ogre::SceneNode* node);
protected:

private:

// member variables
public:
protected:
	bool			m_Visible;	
	//Ogre::String	m_Mesh;			// e.g. "ninja.mesh"
	//Ogre::Vector3	m_Origin;		// original position of the piece
	// Ogre::Entity*	m_Entity;		// the entity for the piece
	// Ogre::SceneNode* m_Node;		// the node for the entity... allows rotation, and translation etc
	int				m_PowerUp;		// 0 - 10 represeting different powerups
	int				m_Owner;		// piece owner .. player 1 = 1 player 2 = 2
	int				m_BoardSquareID;// ID of the board square upon which the piece sits
	int				m_PieceID;		// ID of the piece
	
private:
};

#endif __PIECE_H__
