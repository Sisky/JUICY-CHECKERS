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
