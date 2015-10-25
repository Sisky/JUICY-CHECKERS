/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: test.cpp
 *   Description:  This is a generic throwaway test file... for testing out random bits of code
 *
 */

#include "stdafx.h"
#include "main.h"
#include "Piece.h"



void
JuicyCheckers::testStuff(Ogre::SceneManager& sm)
{

	// define the string number 25
	Ogre::String number = Ogre::StringConverter::toString(26);

	// get the scenenode of the square
	Ogre::SceneNode* s = sm.getSceneNode("squareNode25");
		
	// create a new piece / entity
	Piece* p = new Piece();
	
	// create the entity
	p = static_cast<Piece*>(sm.createEntity("athene.mesh"));
	
	p->setQueryFlags(ROBOT_MASK);

	// create the node as a child of the boardsquare
	Ogre::SceneNode* c = s->createChildSceneNode("pieceNode26");
	
	// attach the entity to the child node
	c->attachObject(p);
	
	// Piece* p = s->createChildSceneNode("pieceNode" + number);
	// set the piece ID
	p->setPieceID(26);
	// set visibility
	p->setVisible(true);
	p->setPowerup(0);

	// push the piece to the array
	pPieces.push_back(p);
}