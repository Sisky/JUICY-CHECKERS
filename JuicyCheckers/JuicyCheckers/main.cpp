/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
___                 __    __ _ _    _
/___\__ _ _ __ ___  / / /\ \ (_) | _(_)
//  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
|___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "main.h"

// #include "BoardSquare.h"
#include "Board.h"
#include "Piece.h"
#include "PieceController.h"


//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication()
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mSceneMgr(0),
	mCamera(0),
	mWindow(0),
	maxDegree(500),
	minDegree(340),
	mRayScnQuery(0),
	pManager(0),
	pBoard(0),
	pController(0)
{
}

TutorialApplication::~TutorialApplication()
{
	// destroy the ray query upon exit
	mSceneMgr->destroyQuery(mRayScnQuery);
	
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}

//Adjust mouse clipping area
void 
TutorialApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void 
TutorialApplication::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if(rw == mWindow)
	{
		if(mInputManager)
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool 
TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//processInput(evt);

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}


// Input Processing Methods
bool 
TutorialApplication::keyPressed(const OIS::KeyEvent& ke) 
{ 
	return true; 
}


bool 
TutorialApplication::keyReleased(const OIS::KeyEvent& ke) 
{ 
	return true; 
}

bool 
TutorialApplication::mouseMoved(const OIS::MouseEvent& me) 
{ 
	// as the mouse moves over each item it it highlighted... each square needs a particle effect
	Ogre::Vector2 mousePos = Ogre::Vector2(static_cast<Ogre::Real>(me.state.X.abs),static_cast<Ogre::Real>(me.state.Y.abs));
	// cast a ray into the scene through the camera to viewport matrix
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x / float(me.state.width), mousePos.y / float(me.state.height));
	// store the resultant information in the ray query 
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setRay(mouseRay);
	mRayScnQuery->setSortByDistance(true);
	mRayScnQuery->setQueryMask(BOARD_BLACK);	// should only query the board squares and nothing else
	// process the ray query and look at the results
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();

	Ogre::SceneNode*  mCurObject;
	// reset the found flag
	bool movableFound = false;

	// test the squares and show something to indicate that it is being moused over
	for ( ; it != result.end(); it++)
	{
		// display all result from the query
		// mCurObject = it->movable->getName()
		

		// exclude the board base
		movableFound = it->movable && it->movable->getName() != "boardBase";

		if(movableFound) {
			//debug
			// Ogre::LogManager::getSingletonPtr()->logMessage("Object found: " + it->movable->getName());
			mCurObject = it->movable->getParentSceneNode();
			// Ogre::LogManager::getSingletonPtr()->logMessage("Object found: " + mCurObject->getName());
			// mCurObject->showBoundingBox(true);

			// should only really display on the odd squares (black) as the others are never played on

			// SHOW GENERIC MOUSEOVER HIGHLIGHT HERE


			// if the left mouse button was pressed.. set the position of the 'selection particle effect' to be the position of the selected object
			// need a different particle effect here.. to show the selection
			//mSceneMgr->getSceneNode("selectionNode")->setPosition(mCurObject->getPosition());

			//// if the effect was not already started.. start it
			//if(pManager->getParticleSystem("psSelection")->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
			//	pManager->getParticleSystem("psSelection")->start();
			//}

			break;
		}
	}



	// handling mouse scroll wheel input
	Ogre::SceneNode* positionNode = mSceneMgr->getSceneNode("CAMERA_POSITION");
	Ogre::Vector3 pos = positionNode->getPosition();

	// the orgin need to change according to the currently selected item
	Ogre::Vector3 origin = Ogre::Vector3(0,0,0);
	Ogre::Vector3 scale = origin - pos;
	scale.normalise();
	int absoluteZ = me.state.Z.abs;
	int relativeZ = me.state.Z.rel;

	// no change in zoom at limits

	//Ogre::Real dist = pos.distance(origin);
	//if(dist <= 100) { scale = 0; }
	
	positionNode->setPosition(pos+(scale*relativeZ)); 

	// limit the scrolling amount here
	// Ogre::LogManager::getSingletonPtr()->logMessage("Current Zoom scale: " + Ogre::StringConverter::toString(dist));

	// Move the camera around an origin point if the user is holding the right mouse button
	if(me.state.buttonDown(OIS::MB_Right))
	{
		// delta mouse movement
		int relativeX = me.state.X.rel;
		int relativeY = me.state.Y.rel;
		
		// absolute mouse position
		int absoluteY = me.state.Y.abs;
		// 		
		// Ogre::LogManager::getSingletonPtr()->logMessage("Current Y value: " + relativeY);
		// std::cout << "Current Mouse Y : " << relativeY << std::endl;
		

		// limit the angle the camera can move
		// if(absoluteY > maxDegree){ me.state.Y.abs = maxDegree; }
		// if(absoluteY < minDegree){ me.state.Y.abs = minDegree; }

		Ogre::SceneNode* rotationNode = mSceneMgr->getSceneNode("CAMERA_ROTATION");
		
		//// get information about the current pitch of the camera.....working in quaternions >_<
		//Ogre::Real pitchAngleSign = rotationNode->getOrientation().x;
		//Ogre::Real pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(rotationNode->getOrientation().w)).valueDegrees());
		//if (pitchAngle > 90.0f)
		//{
		//	if (pitchAngleSign > 0)
		//		// Set orientation to 90 degrees on X-axis.
		//		rotationNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
		//															Ogre::Math::Sqrt(0.5f), 0, 0));
		//	else if (pitchAngleSign < 0)
		//		// Sets orientation to -90 degrees on X-axis.
		//		rotationNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
		//															-Ogre::Math::Sqrt(0.5f), 0, 0));
		//}

		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeX*0.1f), Ogre::Vector3(0,-1,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeY*0.1f), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_LOCAL);
	}
	return true; 
}

bool 
TutorialApplication::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
	// get the x,y position of the mouse
	Ogre::Vector2 mousePos = Ogre::Vector2(static_cast<Ogre::Real>(me.state.X.abs),static_cast<Ogre::Real>(me.state.Y.abs));
	// cast a ray into the scene through the camera to viewport matrix
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x / float(me.state.width), mousePos.y / float(me.state.height));
	// store the resultant information in the ray query 
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setRay(mouseRay);
	mRayScnQuery->setSortByDistance(true);
	mRayScnQuery->setQueryMask(BOARD_BLACK);	// should only query the black board squares and nothing else
	// process the ray query and look at the results
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();

	Ogre::SceneNode*  mCurObject;

	bool movableFound = false;

	for ( ; it != result.end(); it++)
	{
		// display all result from the query
		// mCurObject = it->movable->getName()
		

		// exclude the board base
		movableFound = it->movable && it->movable->getName() != "boardBase";

		if(movableFound) {
			//debug
			// Ogre::LogManager::getSingletonPtr()->logMessage("Object found: " + it->movable->getName());
			mCurObject = it->movable->getParentSceneNode();
			
			// left mouse button was pressed
			if(id == OIS::MB_Left)
			{
				// test if a source object has been selected
				//if(pController->getSource() != nullptr) {
				//	// select a destination square
				//	pController->setDestination(mCurObject);
				//	Ogre::LogManager::getSingletonPtr()->logMessage("Target Object Selected : " + mCurObject->getName());
				//}

				// determine if the boardsqaure has an attached piece
				if(mCurObject->numChildren() > 0) {
					// number of children will be 0 is there is nothing attached to the square
					Ogre::Node* c = mCurObject->getChild(0);
					//Ogre::LogManager::getSingletonPtr()->logMessage("Child Object clicked   : " + c->getName());
					//Ogre::LogManager::getSingletonPtr()->logMessage("Child Object position (relative to parent) : " + Ogre::StringConverter::toString(c->getPosition()));
					//Ogre::LogManager::getSingletonPtr()->logMessage("Parent Object position : " + Ogre::StringConverter::toString(mCurObject->getPosition()));
					// check if there is a selected item already or not
					// if(pController->getSource() == nullptr) {
					// set the selected child object as the source... this can only happen if there is a child object .. aka a piece on a square
					pController->setSource(c);
					Ogre::LogManager::getSingletonPtr()->logMessage("Source Object Selected : " + c->getName());
					//}
				}
				else {
					// any square without a piece attached to it is a valid destination
					pController->setDestination(mCurObject);
					Ogre::LogManager::getSingletonPtr()->logMessage("Target Object Selected : " + mCurObject->getName());
				}

				// test if both source and destination are selected and offload to the PieceController
				if(pController->getSource() != nullptr && pController->getDest() != nullptr) {
					pController->movePiece();
				}

				// if the left mouse button was pressed.. set the position of the 'selection particle effect' to be the position of the selected object
				mSceneMgr->getSceneNode("selectionNode")->setPosition(mCurObject->getPosition());

				// if the selection effect was not already started.. start it
				if(pManager->getParticleSystem("psSelection")->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
					pManager->getParticleSystem("psSelection")->start();
				}
				
			}
			break;
		}

		//// sets a list of things to ignore on the query
		//mMovableFound =
		//	it->movable &&
		//	it->movable->getName() != "" &&
		//	it->movable->getName() != "MainCam" &&
		//	it->movable->getName() != "selectionNode";
		//	//it->movable->getName() != "boardBase" && 
		//	//it->movable->getName() != "ground";

		//if (mMovableFound)
		//{
		//	//Ogre::Vector3 intersect = it->worldFragment->singleIntersection;
		//	mCurObject = it->movable->getParentSceneNode();
		//	
		//	Ogre::LogManager::getSingletonPtr()->logMessage("Object found: " + mCurObject->getName());
		//	// Ogre::LogManager::getSingletonPtr()->logMessage("Position: "+Ogre::StringConverter::toString(intersect));
		//	Ogre::Vector3 entityPos = mCurObject->getPosition();
		//	
		//	// If the user pressed the MMB center the camera on this SceneNode
		//	if(id == OIS::MB_Middle)
		//	{
		//		mSceneMgr->getSceneNode("CAMERA_ROTATION")->setPosition(entityPos);
		//	}


		//}
	}

	//if(mMovableFound)
	//{
	//	// We have selected an entity
	//	std::cout << "Moveable object found" << std::endl;
	//	Ogre::LogManager::getSingletonPtr()->logMessage("Moveable object found");
	//}

	return true; 
}

bool 
TutorialApplication::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
	return true; 
}

void
TutorialApplication::addPieces()
{
	Ogre::String number;


	// populate the vector array

	for(int i = 0; i < 24; i++) {
		Piece* p = new Piece();
		// convert the count to a string
		number = Ogre::StringConverter::toString(i+1);

		// set the piece ID
		p->setPieceID(i+1);
		// set visibility
		p->setVisible(true);
		p->setPowerup(0);

		// first 12 will be ninjas
		if(i < 12) {
			// add the piece
			p->setMesh("robot.mesh");
			p->setOwner(1);
		}
		else {
		// next 12 will be robots
			p->setMesh("ninja.mesh");
			p->setOwner(2);
		}	

		// push to the array
		pPieces.push_back(p);
	}

	// once all pieces are populated... set their associated board id and position and attach the entity to the board square node
	int count = 0;
	for(auto& i : pPieces) {

		count += 2;
		i->setBoardSquareID(count);

		// use that board ID to get the scenenode of the boardsquare
		Ogre::SceneNode* s = pBoard->getSceneNode(count, *mSceneMgr);
		// create the scene node
		number = Ogre::StringConverter::toString(i->getPieceID());
		// create child node of the board square
		Ogre::SceneNode* pieceNode = s->createChildSceneNode("pieceNode" + number);

		// create the piece entity
		Ogre::Entity* pieceEntity = mSceneMgr->createEntity("piece" + number, i->getMesh());
		// set the query mask
		pieceEntity->setQueryFlags(PIECE_MASK); 

		// check which player owns the piece and rotate / scale accordingly
		if(i->getOwner() == PLAYER_ONE) { // robots... need to be rotated and scaled
			// rotate
			pieceNode->yaw(Ogre::Degree(-90));
			// scale
			pieceNode->scale(Ogre::Vector3(3,2.5,2));
		}

		// attach the entity to the piece node
		pieceNode->attachObject(pieceEntity);

		// store the node
		i->setNode(pieceNode);
		// store the position of the board node in the piece class
		i->setPosition(s->getPosition());
		// store the entity in the piece class
		i->setEntity(pieceEntity);
		
		// player 1	(robots)
		if(count == 8) { count--; }
		if(count == 15) { count++; }
		// center of board
		if(count == 24) { count = 39; }
		// player 2 (ninjas)
		if(count == 47) { count++; }
		if(count == 56) { count--; }
		

	}
}

//// Add the ninjas to the scene
//void
//TutorialApplication::addNinjas() 
//{
//	
//	Ogre::Entity* ninjaEntity[12];
//    Ogre::SceneNode* ninjaNode[12];
// 
//    for (int i = 0; i < sizeof(ninjaEntity) / sizeof(ninjaEntity[0]); i++) {
//            Ogre::String number= Ogre::StringConverter::toString(i + 1);
//            ninjaEntity[i] = mSceneMgr->createEntity("ninja " + number, "ninja.mesh");
//			ninjaEntity[i]->setQueryFlags(NINJA_MASK); // for sorting purposes
//            ninjaNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node " + number);
//			
//    }
//	
//    ninjaNode[0]->setPosition(700, 0, 500);
//    ninjaNode[1]->setPosition(500, 0, 700);
//    ninjaNode[2]->setPosition(300, 0, 500);
//    ninjaNode[3]->setPosition(100, 0, 700);
//    ninjaNode[4]->setPosition(-100, 0, 500);
//    ninjaNode[5]->setPosition(-300, 0, 700);
//    ninjaNode[6]->setPosition(-500, 0, 500);
//    ninjaNode[7]->setPosition(-700, 0, 700);
//    ninjaNode[8]->setPosition(100, 0, 300);
//    ninjaNode[9]->setPosition(500, 0, 300);
//    ninjaNode[10]->setPosition(-300, 0, 300);
//    ninjaNode[11]->setPosition(-700, 0, 300);
//    ninjaNode[0]->attachObject(ninjaEntity[0]);
//    ninjaNode[1]->attachObject(ninjaEntity[1]);
//    ninjaNode[2]->attachObject(ninjaEntity[2]);
//    ninjaNode[3]->attachObject(ninjaEntity[3]);
//    ninjaNode[4]->attachObject(ninjaEntity[4]);
//    ninjaNode[5]->attachObject(ninjaEntity[5]);
//    ninjaNode[6]->attachObject(ninjaEntity[6]);
//    ninjaNode[7]->attachObject(ninjaEntity[7]);
//    ninjaNode[8]->attachObject(ninjaEntity[8]);
//    ninjaNode[9]->attachObject(ninjaEntity[9]);
//    ninjaNode[10]->attachObject(ninjaEntity[10]);
//    ninjaNode[11]->attachObject(ninjaEntity[11]);
//}

// Initialize the particle system
void
TutorialApplication::addParticleSystems() 
{
	// get the particle manager singleton pointer
	pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	// circle particle that is triggered when an object is clicked
	ParticleUniverse::ParticleSystem* psSelection = pManager->createParticleSystem("psSelection", "example_010", mSceneMgr);
		
	// set the query mask
	psSelection->setQueryFlags(PARTICLE_MASK);
	// create the scene node
	Ogre::SceneNode* psSelectableParticle = mSceneMgr->getRootSceneNode()->createChildSceneNode("selectionNode");
	// set scale
	psSelection->setScaleVelocity(10);
	psSelection->setScale(Ogre::Vector3(20, 20, 20));
	// attach the particle system to the scenenode
	psSelectableParticle->attachObject(psSelection);

	// define the particle system
	// torches
	ParticleUniverse::ParticleSystem* psTorch1 = pManager->createParticleSystem("psTorch1", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch2 = pManager->createParticleSystem("psTorch2", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch3 = pManager->createParticleSystem("psTorch3", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch4 = pManager->createParticleSystem("psTorch4", "mp_torch", mSceneMgr);

	// create a node for the torches to attach
	Ogre::SceneNode* psTorchNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("torchNode1");
	Ogre::SceneNode* psTorchNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("torchNode2");
	Ogre::SceneNode* psTorchNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("torchNode3");
	Ogre::SceneNode* psTorchNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("torchNode4");

	// set the scale of the particle system
	psTorch1->setScaleVelocity(10); psTorch1->setScale(Ogre::Vector3(30, 30, 30));
	psTorch2->setScaleVelocity(10); psTorch2->setScale(Ogre::Vector3(30, 30, 30));
	psTorch3->setScaleVelocity(10); psTorch3->setScale(Ogre::Vector3(30, 30, 30));
	psTorch4->setScaleVelocity(10); psTorch4->setScale(Ogre::Vector3(30, 30, 30));
	
	// set the position of the particle system relative to its node
	psTorch1->getTechnique(0)->position = Ogre::Vector3(0,0,0);
	psTorch2->getTechnique(0)->position = Ogre::Vector3(0,0,0);
	psTorch3->getTechnique(0)->position = Ogre::Vector3(0,0,0);
	psTorch4->getTechnique(0)->position = Ogre::Vector3(0,0,0);

	// set the query mask
	psTorch1->setQueryFlags(PARTICLE_MASK);
	psTorch2->setQueryFlags(PARTICLE_MASK);
	psTorch3->setQueryFlags(PARTICLE_MASK);
	psTorch4->setQueryFlags(PARTICLE_MASK);

	// set positon of the nodes
	psTorchNode1->setPosition(Ogre::Vector3(-900,0,-900));
	psTorchNode2->setPosition(Ogre::Vector3(-900,0,900));
	psTorchNode3->setPosition(Ogre::Vector3(900,0,-900));
	psTorchNode4->setPosition(Ogre::Vector3(900,0,900));

	// attach systems to their nodes
	psTorchNode1->attachObject(psTorch1);
	psTorchNode2->attachObject(psTorch2);
	psTorchNode3->attachObject(psTorch3);
	psTorchNode4->attachObject(psTorch4);

	
	
	// cool shield thing
	//ParticleUniverse::ParticleSystem* pSys3 = pManager->createParticleSystem("pSys3", "flareShield", mSceneMgr);
	// cool round thing system
	//ParticleUniverse::ParticleSystem* pSys4 = pManager->createParticleSystem("pSys4", "example_010", mSceneMgr);
	
	// attach the particle systems to the scene

	// using the root scene node as these will remain static
	// add to the torches node


	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(psTorch1); // torch 1
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(psTorch2); // torch 2
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(psTorch3); // torch 2
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(psTorch4); // torch 2

	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys3); // sheild
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys4); // cool round thing

	

	// Scale the particle systems
	//pSys0->setScaleVelocity(10);
	//pSys0->setScale(Ogre::Vector3(10, 10, 10));


	
	// sheild
	//pSys3->setScaleVelocity(10);
	//pSys3->setScale(Ogre::Vector3(15, 15, 15));
	// cool round thing
	//pSys4->setScaleVelocity(10);
	//pSys4->setScale(Ogre::Vector3(10, 10, 10));


	// Adjust the position of the particle systems a bit by repositioning their ParticleTechnique (there is only one technique in mp_torch)
	// Normally you would do that by setting the position of the SceneNode to which the Particle System is attached, but in this
	// demo they are both attached to the same rootnode.

	// placing these at the 4 corners of the board
	
	//pSys3->getTechnique(0)->position = Ogre::Vector3(0,0,0);
	//pSys4->getTechnique(0)->position = Ogre::Vector3(0,0,0);

	// Start the particle systems
	psTorch1->start();
	psTorch2->start();
	psTorch3->start();
	psTorch4->start();


}

void
TutorialApplication::drawPieces()
{
	// loop through the piece array
	for(auto& i : pPieces) { 
		// the piece is visible
		if(i->isVisible()) {
			
		}
	}
}
// add the assets the game scene here
void
TutorialApplication::createScene()
{
	// populate the board with empty squares with an id of 1 - 64
	for(int i = 0; i < 64; i++) {
		pBoard->addSquare(i+1);
	}

	// create the playing board
	// create the board scenenode and name is so that we can filter for it later
	Ogre::SceneNode* boardSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BOARD_NODE");
	
	// entity handles the mesh and colour etc
	// node handles the position etc
	for(int j = 0; j < 8; j++) {
		for(int i = 0; i < 8; i++) {
			 // array iteration
			 int it = (i + (j * 8)) + 1;

			 Ogre::String number= Ogre::StringConverter::toString(it);
			 // obtain the plane from the boardsquare class
			 Ogre::Entity* squareEntity = mSceneMgr->createEntity("boardSquare" + number); 
			 
			 squareEntity->setCastShadows(false);			 
			 // set the material
			 int modTest = it % 2;

			 if(modTest == 1) {
				// ensure that each other row is offset to the last
				if(j % 2 == 1) { // odd
					squareEntity->setMaterialName("Juicy/CheckersBlack");
					// set the query mask
					squareEntity->setQueryFlags(BOARD_BLACK);	
				}
				else {
					squareEntity->setMaterialName("Juicy/CheckersWhite");
					// set the query mask
					squareEntity->setQueryFlags(BOARD_WHITE);	
				}
			 }
			 else {
				if(j % 2 == 1) { // odd
					squareEntity->setMaterialName("Juicy/CheckersWhite");
					// set the query mask
					squareEntity->setQueryFlags(BOARD_WHITE);	
				}
				else {

					squareEntity->setMaterialName("Juicy/CheckersBlack");
					// set the query mask
					squareEntity->setQueryFlags(BOARD_BLACK);	
				}
			 }

			 // create the child scene node and assign a name, attach it to the root node
			 // create a scene node and name it boardNode
			 Ogre::Vector3 pos = Ogre::Vector3((i*200)-700, 0, (j*200)-700);
			 //boardSceneNode->createChildSceneNode("squareNode" + number);
			 // boardSceneNode->getChild("squareNode" + number)->setPosition(pos);
			 // boardSceneNode->getChild("squareNode" + number)->attachObject(squareEntity);
			 mSceneMgr->getSceneNode("BOARD_NODE")->createChildSceneNode("squareNode" + number);
			 //mSceneMgr->getRootSceneNode()->createChildSceneNode("squareNode" + number);
			 // positon the node ... board is -800 to 800, each square is 200, 200
			 mSceneMgr->getSceneNode("squareNode" + number)->setPosition(pos);
			 // attach the entity to the node
			 mSceneMgr->getSceneNode("squareNode" + number)->attachObject(squareEntity);
			 

		} 
	}
	
	// populate the piece array
	addPieces();
	
	// add a wood textured box under the board plane
	Ogre::Entity* boardBase;
	Ogre::SceneNode* boardBaseSceneNode;
	boardBase = mSceneMgr->createEntity("boardBase", "cube.mesh");
	boardBase->setMaterialName("Juicy/WoodPanelTexture");
	boardBaseSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boardBaseNode");
	boardBaseSceneNode->setPosition(0,-51,0);
	boardBaseSceneNode->scale(Ogre::Vector3(20, 1, 20));
	boardBaseSceneNode->attachObject(boardBase);

	// draw the pieces on the board in their original position

	// loop through the piece array and match that to the board array
	
	
	

	// setup and initialize the particle sytems
	addParticleSystems();

}

// Initialize the aspects of the scene... camera, viewport, lighting, skybox and shadows
void
TutorialApplication::initScene()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	mCamera = mSceneMgr->createCamera("MainCam");

	// initialize the playing board
	pBoard = new Board();

	// initialize the piece controller
	pController = new PieceController();

	// We want to create a scene node that we can rotate the camera around at the origin
	Ogre::SceneNode* cameraParent = mSceneMgr->getRootSceneNode()->createChildSceneNode("CAMERA_ROTATION");;
	Ogre::SceneNode* cameraChild = cameraParent->createChildSceneNode("CAMERA_POSITION");

	cameraChild->attachObject(mCamera);
	cameraChild->translate(10, 300,-500);


	// setup viewport
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Camera settings
	//mCamera->setPosition(Ogre::Vector3(0, 300, -500));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / 	Ogre::Real(vp->getActualHeight()));
	
	// add a skybox
	mSceneMgr->setSkyBox(true, "Examples/TrippySkyBox");
		
	// shadow settings
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// lighting //

	// ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
	// spotlight	
    Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
    spotLight->setDiffuseColour(0, 0, 1.0);
    spotLight->setSpecularColour(0, 0, 1.0);
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight->setDirection(-1, -1, 0);
    spotLight->setPosition(Ogre::Vector3(200, 200, 0));
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	// directional light
    Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(256, 256, 256));
    directionalLight->setSpecularColour(Ogre::ColourValue(256, 256, 256));
    directionalLight->setDirection(Ogre::Vector3(0, -1, 0));

	// point light
    Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(.3, .3, .3);
    pointLight->setSpecularColour(.3, .3, .3);
    pointLight->setPosition(Ogre::Vector3(0, 150, 250));
}

// Initialize the input callback
void 
TutorialApplication::initInput()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);
}

// Initialise the Ogre3D rendering system
bool 
TutorialApplication::go()
{
	#ifdef _DEBUG
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif

	mRoot = new Ogre::Root(mPluginsCfg);

	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locType;

	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	///if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	//	return false;
	mRoot->showConfigDialog();


	mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Initialise the scene
	initScene();

	// Initialise OIS
	initInput();

	// create the scene
	createScene();

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mRoot->startRendering();

	return true;
}



//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		TutorialApplication app;

		try {
			app.go();
		} catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------

