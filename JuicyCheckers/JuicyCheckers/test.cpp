/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: test.cpp
 *   Description:  This is a generic throwaway test file... for testing out random bits of code
 *
 */

#include "stdafx.h"
#include "main.h"
#include "Piece.h"
#include "PowerUpManager.h"


// this is being called at the end of addPieces in main.cpp
void
JuicyCheckers::testStuff(Ogre::SceneManager& sm)
{
	// choose 3 pieces to have powerups applied

	// get scenenodes attached to the board nodes
	Ogre::SceneNode* c1 = static_cast<Ogre::SceneNode*>(sm.getSceneNode("boardSquareNode2")->getChild(0));
	Ogre::SceneNode* c2 = static_cast<Ogre::SceneNode*>(sm.getSceneNode("boardSquareNode4")->getChild(0));
	Ogre::SceneNode* c3 = static_cast<Ogre::SceneNode*>(sm.getSceneNode("boardSquareNode6")->getChild(0));
	Ogre::SceneNode* c4 = static_cast<Ogre::SceneNode*>(sm.getSceneNode("boardSquareNode8")->getChild(0));
	Ogre::SceneNode* c5 = static_cast<Ogre::SceneNode*>(sm.getSceneNode("boardSquareNode9")->getChild(0));

	// get pieces from the nodes
	Piece* e1 = static_cast<Piece*>(c1->getAttachedObject(0));
	Piece* e2 = static_cast<Piece*>(c2->getAttachedObject(0));
	Piece* e3 = static_cast<Piece*>(c3->getAttachedObject(0));
	Piece* e4 = static_cast<Piece*>(c4->getAttachedObject(0));
	Piece* e5 = static_cast<Piece*>(c5->getAttachedObject(0));

	// set the mask
	mPowerUpManager->setPowerUpMask(e1, PowerUpManager::KING, true);
	mPowerUpManager->setPowerUpMask(e2, PowerUpManager::LOCK, true);
	mPowerUpManager->setPowerUpMask(e3, PowerUpManager::SHIELD , true);
	// combining masks
	// create a powerup mask and assign values
	PowerUpManager::PowerUpMask mask1 = static_cast<PowerUpManager::PowerUpMask>(PowerUpManager::KING);
	mPowerUpManager->setPowerUpMask(e4, mask1, true);
	// adding to the mask
	mPowerUpManager->setPowerUpMask(e5, PowerUpManager::LOCK, true);	// set initial mask
	mPowerUpManager->setPowerUpMask(e5, PowerUpManager::SHIELD, false);   // add a powerup , set the overwrite flag to false to keep the previous entry

	// removing the shield powerup and adding a king to e5 but keeping the lock powerup
	PowerUpManager::PowerUpMask mask2 = static_cast<PowerUpManager::PowerUpMask>(PowerUpManager::KING | PowerUpManager::SHIELD);
	mPowerUpManager->setPowerUpMask(e5, mask2, false);	// keep lock powerup by setting overwrite to false

	// removing all powerups 
	// mask2 = static_cast<PowerUpManager::PowerUpMask>(PowerUpManager::BLANK);
	// mPowerUpManager->setPowerUpMask(e5, mask2, true);

	// apply the powerup
	mPowerUpManager->applyPowerUps(e1);
	mPowerUpManager->applyPowerUps(e2);
	mPowerUpManager->applyPowerUps(e3);
	mPowerUpManager->applyPowerUps(e4);
	mPowerUpManager->applyPowerUps(e5);

	// set the powerup bar for player 1 and player 2
	
	mPowerUpManager->setPowerUpBar(2, sm.getSceneNode("powerupBarNodeP1"));
	mPowerUpManager->setPowerUpBar(1, sm.getSceneNode("powerupBarNodeP2"));

}


	//// load the crown entity

	//Ogre::Entity* crown = sm.createEntity("crownTest", "crown.mesh");
	//// apply the query mask to the entity
	//crown->setQueryFlags(POWERUP_MASK);
	//// set attrbiutes
	//// select a piece and add a crown to it
	//Ogre::SceneNode* s = sm.getSceneNode("boardSquareNode2");	
	//if (s->numChildren() > 0) {
	//	Ogre::LogManager::getSingletonPtr()->logMessage("Number of child nodes: " + Ogre::StringConverter::toString(s->numChildren()));
	//	// query for the child node which contains the piece entity
	//	Ogre::SceneNode* c = static_cast<Ogre::SceneNode*>(s->getChild(0));
	//	// cast the piece entity 
	//	Piece* p = static_cast<Piece*>(c->getAttachedObject(0));
	//	// now that I have the piece entity... attach the crown to the node
	//	p->m_PowerUpNode->attachObject(crown);
	//	p->m_PowerUpNode->scale(50, 50, 50);
	//	p->m_PowerUpNode->setPosition(0, 90, 0);
	//}

	// have the crown rotating

	//// create an outline of the currently selected tile for mouseover

	//// manual object
	//Ogre::ManualObject* mSquare = sm.createManualObject("selectionSquare");
	//// attach this to sqaure 25
	//Ogre::SceneNode* s = sm.getSceneNode("squareNode25");

	//Ogre::MaterialPtr mSquareMaterial = Ogre::MaterialManager::getSingleton().create("squareMaterial","General");
	//mSquareMaterial->setReceiveShadows(false);
	//mSquareMaterial->getTechnique(0)->setLightingEnabled(true);
	//mSquareMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,1,0,0);
	//mSquareMaterial->getTechnique(0)->getPass(0)->setAmbient(0,1,0);
	//mSquareMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0);

	//


	//mSquare->begin("squareMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	//	// outer points  .. index 0 - 3
	//	mSquare->position(-110, 5, 110); // 0
	//	mSquare->position(-110, 5, -110); // 1
	//	mSquare->position(110, 5, -110); // 2
	//	mSquare->position(110, 5, 110); // 3

	//	// inner points .. index 4 - 7
	//	mSquare->position(-100, 5, 100);
	//	mSquare->position(-100, 5, -100);
	//	mSquare->position(100, 5, -100);
	//	mSquare->position(100, 5, 100);

	//	// set points using indexes

	//	// left side
	//	mSquare->index(0);
	//	mSquare->index(4);
	//	mSquare->index(1);
	//
	//	mSquare->index(4);
	//	mSquare->index(5);
	//	mSquare->index(1);

	//	// bottom side
	//	mSquare->index(1);
	//	mSquare->index(5);
	//	mSquare->index(2);

	//	mSquare->index(2);
	//	mSquare->index(5);
	//	mSquare->index(6);

	//	// right side
	//	mSquare->index(2);
	//	mSquare->index(6);
	//	mSquare->index(3);

	//	mSquare->index(3);
	//	mSquare->index(6);
	//	mSquare->index(7);

	//	// top side
	//	mSquare->index(3);
	//	mSquare->index(7);
	//	mSquare->index(0);

	//	mSquare->index(0);
	//	mSquare->index(7);
	//	mSquare->index(4);

	//mSquare->end();

	//// add tothe node
	//s->attachObject(mSquare);



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
