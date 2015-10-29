/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: PieceController.h
 *   Description:  This class provides some helper functions for moving the pieces on the board
 *
 */

#ifndef __PIECECONTROLLER_H__
#define __PIECECONTROLLER_H__

// this class allows the control of pieces using easy functions

// tracks the selected item (scenenode)
// performs the movement from one position to another

class Client;


class PieceController
{
// member functions
public:
	PieceController(void);
	~PieceController(void);

	// base this on the keypad for directional actions

	//	7, 8, 9
	//  4, 5, 6
	//  1, 2, 3

	enum PieceDirection
	{
		FORWARD_LEFT = 7,
		FORWARD_RIGHT = 9,
		BACKWARD_LEFT = 1,
		BACKWARD_RIGHT = 3
	};

	// keep track of the source and target nodes for movement
	void setSource(Ogre::Node* node) { m_sourceNode = static_cast<Ogre::SceneNode*>(node); }
	void setDestination(Ogre::Node* node) { m_destNode = static_cast<Ogre::SceneNode*>(node); }

	Ogre::SceneNode* getSource() { return m_sourceNode; }
	Ogre::SceneNode* getDest() { return m_destNode; }

	// action functions
	void movePiece(Ogre::SceneNode* source, Ogre::SceneNode* dest);
	
	void movePiece();
	int PieceController::stringToInt(Ogre::String string);
	// prefab movements
	bool testMovePiece(Ogre::SceneNode* piece, PieceDirection direction);

	// Network Methods
	void setNetworkInterface(Client* _client);
	Client* getNetworkInterface();

	void moveNetworkPiece(Ogre::SceneNode* source, Ogre::SceneNode* dest);

protected:
private:

// memeber variables

public:
protected:
	Ogre::SceneNode* m_sourceNode;
	Ogre::SceneNode* m_destNode;

	Client* m_clientNetwork;
private:

};

#endif __PIECECONTROLLER_H__