/*
*   Author:  Dave Brown <wpv9142@aut.ac.nz>
*   Filename: LineDrawing.cpp
*   Description:  This class defines a small selection square
*
*/

#include "stdafx.h"
#include "LineDrawing.h"



LineDrawing::LineDrawing(void)
{
}


LineDrawing::~LineDrawing(void)
{
}


// creates a small overlay to represent the current square that the mouse can select
void
LineDrawing::initSelectionSquare(Ogre::SceneManager& sm)
{
	// create manual object
	mSquare = sm.createManualObject("selectionSquare");

	// create a scene node that will hold the selector square
	Ogre::SceneNode* selectionNode = sm.getRootSceneNode()->createChildSceneNode("selectionNodeHighlight");
	
	Ogre::MaterialPtr mSquareMaterial = Ogre::MaterialManager::getSingleton().create("squareMaterial", "General");
	mSquareMaterial->setReceiveShadows(false);
	mSquareMaterial->getTechnique(0)->setLightingEnabled(true);
	mSquareMaterial->getTechnique(0)->getPass(0)->setDiffuse(0, 1, 0, 0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setAmbient(0, 1, 0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0, 1, 0);

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
	mSquare->setQueryFlags(SELECTION_FLAG);
	// attach the object to the scenenode
	selectionNode->attachObject(mSquare);
}

void 
LineDrawing::setSelectionSquareColor(Ogre::Vector3 col)
{
	Ogre::MaterialPtr mSquareMaterial = Ogre::MaterialManager::getSingleton().getByName("squareMaterial");
	// set the colour
	mSquareMaterial->getTechnique(0)->getPass(0)->setDiffuse(0, 1, 0, 0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setAmbient(0, 1, 0);
	mSquareMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0, 1, 0);
}

void
LineDrawing::initPowerUpBar(Ogre::SceneManager & sm)
{
	// create a scene node that will hold the powerup bar
	Ogre::SceneNode* powerupBarNodeP1 = sm.getRootSceneNode()->createChildSceneNode("powerupBarNodeP1");
	Ogre::SceneNode* powerupBarNodeP2 = sm.getRootSceneNode()->createChildSceneNode("powerupBarNodeP2");

	//player 1
	Ogre::SceneNode* pub1 = powerupBarNodeP1->createChildSceneNode("pub1p1");
	Ogre::SceneNode* pub2 = powerupBarNodeP1->createChildSceneNode("pub2p1");
	Ogre::SceneNode* pub3 = powerupBarNodeP1->createChildSceneNode("pub3p1");
	//player 2
	Ogre::SceneNode* pub4 = powerupBarNodeP2->createChildSceneNode("pub1p2");
	Ogre::SceneNode* pub5 = powerupBarNodeP2->createChildSceneNode("pub2p2");
	Ogre::SceneNode* pub6 = powerupBarNodeP2->createChildSceneNode("pub3p2");


	// add a plain coloured box
	Ogre::Entity* puBox1P1;
	Ogre::Entity* puBox2P1;
	Ogre::Entity* puBox3P1;

	Ogre::Entity* puBox1P2;
	Ogre::Entity* puBox2P2;
	Ogre::Entity* puBox3P2;

	puBox1P1 = sm.createEntity("puBox1P1", "cube.mesh");
	puBox2P1 = sm.createEntity("puBox2P1", "cube.mesh");
	puBox3P1 = sm.createEntity("puBox3P1", "cube.mesh");

	puBox1P2 = sm.createEntity("puBox1P2", "cube.mesh");
	puBox2P2 = sm.createEntity("puBox2P2", "cube.mesh");
	puBox3P2 = sm.createEntity("puBox3P2", "cube.mesh");

	puBox1P1->setMaterialName("Juicy/Red"); 
	puBox2P1->setMaterialName("Juicy/Red");
	puBox3P1->setMaterialName("Juicy/Red");

	puBox1P2->setMaterialName("Juicy/Red");
	puBox2P2->setMaterialName("Juicy/Red");
	puBox3P2->setMaterialName("Juicy/Red");

	pub1->setPosition(-150, -40, 0);
	pub2->setPosition(0, -40, 0);
	pub3->setPosition(150, -40, 0);

	pub4->setPosition(-150, -40, 0);
	pub5->setPosition(0, -40, 0);
	pub6->setPosition(150, -40, 0);

	pub1->scale(Ogre::Vector3(1, 1, 1));
	pub2->scale(Ogre::Vector3(1, 1, 1));
	pub3->scale(Ogre::Vector3(1, 1, 1));

	pub4->scale(Ogre::Vector3(1, 1, 1));
	pub5->scale(Ogre::Vector3(1, 1, 1));
	pub6->scale(Ogre::Vector3(1, 1, 1));

	pub1->attachObject(puBox1P1);
	pub2->attachObject(puBox2P1);
	pub3->attachObject(puBox3P1);

	pub4->attachObject(puBox1P2);
	pub5->attachObject(puBox2P2);
	pub6->attachObject(puBox3P2);

}


// initialize the powerup bar
void 
LineDrawing::initPowerUpBarGradient(Ogre::SceneManager & sm)
{
	// create manual object
	mBarPlayer1 = sm.createManualObject("powerUpBarP1");
	mBarPlayer2 = sm.createManualObject("powerUpBarP2");

	// create a scene node that will hold the powerup bar
	Ogre::SceneNode* powerupBarNodeP1 = sm.getRootSceneNode()->createChildSceneNode("powerupBarNodeP1");
	Ogre::SceneNode* powerupBarNodeP2 = sm.getRootSceneNode()->createChildSceneNode("powerupBarNodeP2");

	mBarPlayer1->setDynamic(true);
	mBarPlayer2->setDynamic(true);

	mBarPlayer1->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		// 0
	mBarPlayer1->position(-150, 0, 50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 1
	mBarPlayer1->position(-150, 0, -50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 2
	mBarPlayer1->position(-50, 0, 50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 3
	mBarPlayer1->position(-50, 0, -50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 4
	mBarPlayer1->position(50, 0, 50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 5
	mBarPlayer1->position(50, 0, -50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 6
	mBarPlayer1->position(150, 0, 50);
	mBarPlayer1->colour(1, 0, 0, 1);
		// 7
	mBarPlayer1->position(150, 0, -50);
	mBarPlayer1->colour(1, 0, 0, 1);

		// create using indexes.. essentially 6 triangles
		// bottom square
	mBarPlayer1->index(2);
	mBarPlayer1->index(3);
	mBarPlayer1->index(0);

	mBarPlayer1->index(3);
	mBarPlayer1->index(1);
	mBarPlayer1->index(0);

		// middle square
	mBarPlayer1->index(4);
	mBarPlayer1->index(5);
	mBarPlayer1->index(2);

	mBarPlayer1->index(5);
	mBarPlayer1->index(3);
	mBarPlayer1->index(2);

		// top square
	mBarPlayer1->index(6);
	mBarPlayer1->index(7);
	mBarPlayer1->index(4);

	mBarPlayer1->index(7);
	mBarPlayer1->index(5);
	mBarPlayer1->index(4);

	mBarPlayer1->end();
	
	// player 2s bar

	mBarPlayer2->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	// 0
	mBarPlayer2->position(-150, 0, 50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 1
	mBarPlayer2->position(-150, 0, -50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 2
	mBarPlayer2->position(-50, 0, 50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 3
	mBarPlayer2->position(-50, 0, -50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 4
	mBarPlayer2->position(50, 0, 50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 5
	mBarPlayer2->position(50, 0, -50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 6
	mBarPlayer2->position(150, 0, 50);
	mBarPlayer2->colour(1, 0, 0, 1);
	// 7
	mBarPlayer2->position(150, 0, -50);
	mBarPlayer2->colour(1, 0, 0, 1);

	// create using indexes.. essentially 6 triangles
	// bottom square
	mBarPlayer2->index(2);
	mBarPlayer2->index(3);
	mBarPlayer2->index(0);

	mBarPlayer2->index(3);
	mBarPlayer2->index(1);
	mBarPlayer2->index(0);

	// middle square
	mBarPlayer2->index(4);
	mBarPlayer2->index(5);
	mBarPlayer2->index(2);

	mBarPlayer2->index(5);
	mBarPlayer2->index(3);
	mBarPlayer2->index(2);

	// top square
	mBarPlayer2->index(6);
	mBarPlayer2->index(7);
	mBarPlayer2->index(4);

	mBarPlayer2->index(7);
	mBarPlayer2->index(5);
	mBarPlayer2->index(4);

	mBarPlayer2->end();
	// hopfully can set the value of the colours as follows
	// mBar[0].colour(0, 1, 0, 1);
	mBarPlayer1->setQueryFlags(SELECTION_FLAG);
	mBarPlayer2->setQueryFlags(SELECTION_FLAG);

	// attach to the game node
	powerupBarNodeP1->attachObject(mBarPlayer1);
	powerupBarNodeP2->attachObject(mBarPlayer2);

}
