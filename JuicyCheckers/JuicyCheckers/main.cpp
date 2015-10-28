/***************************************************************************************************************
*
*
*
*/

#include "stdafx.h"
#include "main.h"

#include "Board.h"
#include "Piece.h"
#include "PieceController.h"
#include "MenuSystem.h"
#include "client.h"
#include "LineDrawing.h"
#include "PowerUpManager.h"
#include "Powerup.h"



JuicyCheckers::JuicyCheckers()
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mSceneMgr(0),
	mCamera(0),
	mWindow(0),
	maxDegree(500),
	minDegree(340),
	mRayScnQuery(0),
	mParticleManager(0),
	pBoard(0),
	pController(0),
	client(0),
	mPowerUpManager(0),
	shutdown(false),
	mMenuSystem(0)
{
}

JuicyCheckers::~JuicyCheckers()
{

	// destroy the ray query upon exit
	mSceneMgr->destroyQuery(mRayScnQuery);
	
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete client;
	client = 0;

	delete mRoot;
}




void 
JuicyCheckers::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


void 
JuicyCheckers::windowClosed(Ogre::RenderWindow* rw)
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
JuicyCheckers::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	// Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	client->Process(evt.timeSinceLastEvent);

	mMenuSystem->frameRenderingQueued(evt);

	// processInput(evt);

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	if(shutdown)
	{
		return false;
	}

	return true;
}



bool 
JuicyCheckers::keyPressed(const OIS::KeyEvent& ke) 
{ 
	return true; 
}


bool 
JuicyCheckers::keyReleased(const OIS::KeyEvent& ke) 
{ 
	return true; 
}

bool 
JuicyCheckers::mouseMoved(const OIS::MouseEvent& me) 
{ 
	// pass mouse event to the menu system
	mMenuSystem->MouseMoved(me);

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

	// reset the found flag
	bool movableFound = false;

	// test the squares and show something to indicate that it is being moused over
	for ( ; it != result.end(); it++)
	{
		// exclude the board base
		movableFound = it->movable && it->movable->getName() != "boardBase";


		if(movableFound) {
			Ogre::SceneNode* mCurObject = it->movable->getParentSceneNode();
			// SHOW GENERIC MOUSEOVER HIGHLIGHT HERE
			mSceneMgr->getSceneNode("selectionNodeHighlight")->setPosition(mCurObject->getPosition());
			
			break;
		}
	}

	// change in Z value (delta mousewheel)
	int relativeZ = me.state.Z.rel;
	// Ogre::LogManager::getSingletonPtr()->logMessage("Z mouse: " + Ogre::StringConverter::toString(relativeZ));
	// only do the zoom if there is a change in the mouse wheel
	if(relativeZ > 1 || relativeZ < -1) {
		// handling mouse scroll wheel input
		Ogre::SceneNode* positionNode = mSceneMgr->getSceneNode("CAMERA_POSITION");
		Ogre::Vector3 pos = positionNode->getPosition();

		// the origin need to change according to the currently selected item
		Ogre::Vector3 origin = Ogre::Vector3(0,0,0);
		Ogre::Vector3 scale = origin - pos;
		scale.normalise();

		Ogre::Vector3 newPos = pos+(scale*static_cast<Ogre::Real>(relativeZ));
		// check the distance between the new position node and the origin
		Ogre::Real dist = newPos.distance(origin); // expensive calculation
		// Ogre::LogManager::getSingletonPtr()->logMessage("Z mouse: " + Ogre::StringConverter::toString(dist));
		// set the limits on the zoom factor
		if(dist > 343 && dist < 4000) {
			// allow zooming
			positionNode->setPosition(newPos); 
		} 
	}
	// Move the camera around an origin point if the user is holding the right mouse button
	if(me.state.buttonDown(OIS::MB_Right))
	{
		// delta mouse movement
		int relativeX = me.state.X.rel;
		int relativeY = me.state.Y.rel;
		
		// rotation node is situated at the origin
		Ogre::SceneNode* rotationNode = mSceneMgr->getSceneNode("CAMERA_ROTATION");
		// normal yaw (rotation about the Y axis)
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeX*0.1f), Ogre::Vector3(0,-1,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		// normal pitch (rotation about the X axis)
		rotationNode->pitch(static_cast<Ogre::Degree>(relativeY * 0.1));
		
	}
	return true; 
}


bool 
JuicyCheckers::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{
	mMenuSystem->MousePressed(me,id);

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
				
				// ensure that there is a source before being able to select a target
				

				// determine if the boardsqaure has an attached piece
				if(mCurObject->numChildren() > 0) {

					// select source piece here
					
					// number of children will be 0 if there is nothing attached to the square
					Ogre::SceneNode* c = static_cast<Ogre::SceneNode*>(mCurObject->getChild(0));
					Piece* e = static_cast<Piece*>(c->getAttachedObject(0));
					Ogre::LogManager::getSingletonPtr()->logMessage("Child Object Piece ID   : " + Ogre::StringConverter::toString(e->getPieceID()));


					// can access all the functions of the attached class now which makes like easier
					
					Ogre::LogManager::getSingletonPtr()->logMessage("Child Object Entity Name   : " + e->getName());
					

					//Ogre::LogManager::getSingletonPtr()->logMessage("Child Object clicked   : " + c->getName());
					//Ogre::LogManager::getSingletonPtr()->logMessage("Child Object position (relative to parent) : " + Ogre::StringConverter::toString(c->getPosition()));
					//Ogre::LogManager::getSingletonPtr()->logMessage("Parent Object position : " + Ogre::StringConverter::toString(mCurObject->getPosition()));
					// check if there is a selected item already or not
					// if(pController->getSource() == nullptr) {
					// set the selected child object as the source... this can only happen if there is a child object .. aka a piece on a square
					pController->setSource(c);
					Ogre::LogManager::getSingletonPtr()->logMessage("Source Object Selected : " + c->getName());
					
					// move and start the particle system
					mSceneMgr->getSceneNode("selectionNode")->setPosition(mCurObject->getPosition());
					// if the selection effect was not already started.. start it
					if(mParticleManager->getParticleSystem("psSelection")->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
						mParticleManager->getParticleSystem("psSelection")->start();
					}
				}
				else {
					// make sure that a target has been selected already
					if(pController->getSource() != nullptr) {
						// any square without a piece attached to it is a valid destination
						pController->setDestination(mCurObject);
						Ogre::LogManager::getSingletonPtr()->logMessage("Target Object Selected : " + mCurObject->getName());

						// if the left mouse button was pressed.. set the position of the 'selection particle effect' to be the position of the selected object
						mSceneMgr->getSceneNode("selectionNode")->setPosition(mCurObject->getPosition());
						// if the selection effect was not already started.. start it
						if(mParticleManager->getParticleSystem("psSelection")->getState() != ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STARTED) {
							mParticleManager->getParticleSystem("psSelection")->start();
						}
					}
				}

				// test if both source and destination are selected and offload to the PieceController
				if(pController->getSource() != nullptr && pController->getDest() != nullptr) {
					pController->movePiece();
					// stop the particle system
					mParticleManager->getParticleSystem("psSelection")->stop();
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
JuicyCheckers::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
	mMenuSystem->MouseReleased(me,id);
	return true; 
}

void
JuicyCheckers::addPieces()
{
	Ogre::String number;
	int count = 0;
	
	// populate the piece entity vector array
	for(int i = 1; i < 24 + 1; i++) {

		// convert the count to a string
		number = Ogre::StringConverter::toString(i);
		
		count += 2;
		
		// Piece Entity
		Piece* p = new Piece(*mSceneMgr);
		if (i <= 12) {
			// create the entity
			p = static_cast<Piece*>(mSceneMgr->createEntity("piece" + number, "robot.mesh"));
		}
		else {
			// create the entity
			p = static_cast<Piece*>(mSceneMgr->createEntity("piece" + number, "ninja.mesh"));
		}
		// set the entity query flag
		p->setQueryFlags(PIECE_MASK);

		// powerups
		Powerup* pu = new Powerup();
	
		p->setPowerUps(pu);
		//// set powerup state to a blank mask
		mPowerUpManager->setPowerUpMask(p, mPowerUpManager->BLANK, true);


		// use that board ID to get the scenenode of the boardsquare
		Ogre::SceneNode* s = pBoard->getSceneNode(count, *mSceneMgr);

		// create child node of the board square
		Ogre::SceneNode* pieceNode = s->createChildSceneNode("pieceNode" + number);



		// set the piece ID  1 - 24
		p->setPieceID(i);
		// set visibility
		p->setVisible(true);
		// set the board square ID
		p->setBoardSquareID(count);
		// store the original position of the board node in the piece class
		p->setOrigin(s->getPosition());

		// first 12 will be ninjas
		if(i <= 12) {
			// add the piece
			// p->setMesh("robot.mesh");
			p->setOwner(1);	// player 1
			// rotate
			pieceNode->yaw(Ogre::Degree(-90));
			// scale
			pieceNode->scale(Ogre::Vector3(3,2.5,2));
		}
		else {
		// next 12 will be robots
			// p->setMesh("ninja.mesh");
			p->setOwner(2); // player 2
		}	


		
		// attach the entity to the node
		pieceNode->attachObject(p);

		// push to the array
		pPieces.push_back(p);

		// player 1	(robots)
		if(count == 8) { count--; }
		if(count == 15) { count++; }
		// center of board
		if(count == 24) { count = 39; }
		// player 2 (ninjas)
		if(count == 47) { count++; }
		if(count == 56) { count--; }
	}

	// testStuff(*mSceneMgr);
}

 
void
JuicyCheckers::addParticleSystems() 
{
	// get the particle manager singleton pointer
	mParticleManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	// circle particle that is triggered when an object is clicked
	ParticleUniverse::ParticleSystem* psSelection = mParticleManager->createParticleSystem("psSelection", "example_010", mSceneMgr);
		
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
	ParticleUniverse::ParticleSystem* psTorch1 = mParticleManager->createParticleSystem("psTorch1", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch2 = mParticleManager->createParticleSystem("psTorch2", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch3 = mParticleManager->createParticleSystem("psTorch3", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* psTorch4 = mParticleManager->createParticleSystem("psTorch4", "mp_torch", mSceneMgr);

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

	// Start the particle systems
	psTorch1->start();
	psTorch2->start();
	psTorch3->start();
	psTorch4->start();


}

void
JuicyCheckers::drawPieces()
{
	// loop through the piece array
	for(auto& i : pPieces) { 
		// the piece is visible
		//if(i->isVisible()) {
		//	
		//}
	}
}

void
JuicyCheckers::createScene()
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


void
JuicyCheckers::initScene()
{

	mCamera = mSceneMgr->createCamera("MainCam");

	// initialize the playing board
	pBoard = new Board();

	// initialize the piece controller
	pController = new PieceController();

	// initialize the powerup manager
	mPowerUpManager = new PowerUpManager();

	// We want to create a scene node that we can rotate the camera around at the origin
	Ogre::SceneNode* cameraParent = mSceneMgr->getRootSceneNode()->createChildSceneNode("CAMERA_ROTATION");;
	Ogre::SceneNode* cameraChild = cameraParent->createChildSceneNode("CAMERA_POSITION");

	cameraChild->attachObject(mCamera);
	cameraChild->translate(10, 300,-500);


	// setup viewport
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	
	// Camera settings
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

	// directional light... pointing straight down
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

	// selection square
	mSelector = new LineDrawing();
	mSelector->initSelectionSquare(*mSceneMgr);
}


void 
JuicyCheckers::initInput()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("discl_foreground" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("discl_nonexclusive")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("discl_foreground")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("discl_nonexclusive")));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);
}


bool 
JuicyCheckers::go()
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


	mWindow = mRoot->initialise(true, "JuicyCheckers Render Window");

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// I had to move the creation of the scene manager here so I can create the overlay system before the call
	// to Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mMenuSystem = new MenuSystem();
	mMenuSystem->InitialiseOverlaySystem(mSceneMgr);

	// Make sure all resources are loaded
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Initialise the scene
	initScene();

	// Initialise OIS
	initInput();

	// create the scene
	createScene();

	// Initialise the Networking
	initNetworking();

	// Initialise the Menu
	initMenu();

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	// Init the menu
		

	mRoot->startRendering();

	return true;
}

void 
JuicyCheckers::initNetworking()
{
	client = new Client();
}

void 
JuicyCheckers::initMenu()
{
	mMenuSystem = new MenuSystem();

	OgreBites::InputContext mInputContext;
	mMenuSystem->Initialise(mWindow, mInputContext, mInputManager, mMouse, mKeyboard);

	mMenuSystem->SetMenu(MenuSystem::MENUS::STARTMENU);
}

void 
JuicyCheckers::setShutdown()
{
	shutdown = true;
}





//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
// #include <winsock2.h>
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

		JuicyCheckers app; 		// Create application object

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



