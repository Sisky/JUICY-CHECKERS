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

	// create an outline of the currently selected tile for mouseover

	// manual object?
	Ogre::ManualObject* mSquare = sm.createManualObject("selectionSquare");
	// attach this to sqaure 25
	Ogre::SceneNode* s = sm.getSceneNode("squareNode25");

	Ogre::MaterialPtr mSquareMaterial = Ogre::MaterialManager::getSingleton().create("squareMaterial","General");
	mSquareMaterial->setReceiveShadows(false);
	mSquareMaterial->getTechnique(0)->setLightingEnabled(true);
	mSquareMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,1,0,0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setAmbient(0,1,0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0);

	


	mSquare->begin("squareMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		// outer points  .. index 0 - 3
		mSquare->position(-110, 5, 110); // 0
		mSquare->position(-110, 5, -110); // 1
		mSquare->position(110, 5, -110); // 2
		mSquare->position(110, 5, 110); // 3

		// inner points .. index 4 - 7
		mSquare->position(-100, 5, 100);
		mSquare->position(-100, 5, -100);
		mSquare->position(100, 5, -100);
		mSquare->position(100, 5, 100);

		// set points using indexes

		// left side
		mSquare->index(0);
		mSquare->index(4);
		mSquare->index(1);
	
		mSquare->index(4);
		mSquare->index(5);
		mSquare->index(1);

		// bottom side
		mSquare->index(1);
		mSquare->index(5);
		mSquare->index(2);

		mSquare->index(2);
		mSquare->index(5);
		mSquare->index(6);

		// right side
		mSquare->index(2);
		mSquare->index(6);
		mSquare->index(3);

		mSquare->index(3);
		mSquare->index(6);
		mSquare->index(7);

		// top side
		mSquare->index(3);
		mSquare->index(7);
		mSquare->index(0);

		mSquare->index(0);
		mSquare->index(7);
		mSquare->index(4);

	mSquare->end();

	// add tothe node
	s->attachObject(mSquare);



	// testing the usage of changing the piece calss to inheit from Entity

	//// define the string number 25
	//Ogre::String number = Ogre::StringConverter::toString(26);

	//// get the scenenode of the square
	//Ogre::SceneNode* s = sm.getSceneNode("squareNode25");
	//	
	//// create a new piece / entity
	//Piece* p = new Piece();
	//
	//// create the entity
	//p = static_cast<Piece*>(sm.createEntity("athene.mesh"));
	//
	//p->setQueryFlags(ROBOT_MASK);

	//// create the node as a child of the boardsquare
	//Ogre::SceneNode* c = s->createChildSceneNode("pieceNode26");
	//
	//// attach the entity to the child node
	//c->attachObject(p);
	//
	//// Piece* p = s->createChildSceneNode("pieceNode" + number);
	//// set the piece ID
	//p->setPieceID(26);
	//// set visibility
	//p->setVisible(true);
	//p->setPowerup(0);

	//// push the piece to the array
	//pPieces.push_back(p);
}