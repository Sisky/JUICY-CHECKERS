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
#include <OgreConfigFile.h>
#include <OgreSceneManager.h>
#include "OgreRenderWindow.h"
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreMeshManager.h>
#include <OgreStringConverter.h>

#include "ParticleUniverseSystemManager.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication()
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mSceneMgr(0),
	mCamera(0),
	mWindow(0),
	currentDegree(0.0f),
	maxDegree(180.0f),
	minDegree(-180.0f),
	mMovableFound(false),
	mRayScnQuery(0),
	re_mCurObject(0)
{
}

TutorialApplication::~TutorialApplication()
{

	mSceneMgr->destroyQuery(mRayScnQuery);

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}

//Adjust mouse clipping area
void TutorialApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void TutorialApplication::windowClosed(Ogre::RenderWindow* rw)
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

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
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
bool TutorialApplication::keyPressed(const OIS::KeyEvent& ke) 
{ 
	return true; 
}

bool TutorialApplication::keyReleased(const OIS::KeyEvent& ke) 
{ 
	return true; 
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent& me) 
{ 
	Ogre::SceneNode* positionNode = mSceneMgr->getSceneNode("CAMERA_POSITION");
	Ogre::Vector3 pos = positionNode->getPosition();

	Ogre::Vector3 origin = Ogre::Vector3(0,0,0);
	Ogre::Vector3 scale = origin - pos;
	scale.normalise();

	int relativeZ = me.state.Z.rel;
	positionNode->setPosition(pos+(scale*relativeZ));

	// Move the camera around an origin point if the user is holding the right mouse button
	if(me.state.buttonDown(OIS::MB_Right))
	{
		int relativeX = me.state.X.rel;
		int relativeY = me.state.Y.rel;
		
		if(currentDegree > maxDegree){ currentDegree = maxDegree; }
		if(currentDegree < minDegree){ currentDegree = minDegree; }

		Ogre::SceneNode* rotationNode = mSceneMgr->getSceneNode("CAMERA_ROTATION");
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeX*0.1f), Ogre::Vector3(0,-1,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeY*0.1f), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_LOCAL);
	}
	return true; 
}

bool TutorialApplication::mousePressed(
	const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
	Ogre::Vector2 mousePos = 
		Ogre::Vector2(static_cast<Ogre::Real>(me.state.X.abs),static_cast<Ogre::Real>(me.state.Y.abs));

	Ogre::Ray mouseRay =
		mCamera->getCameraToViewportRay(
		mousePos.x / float(me.state.width),
		mousePos.y / float(me.state.height));
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setRay(mouseRay);
	mRayScnQuery->setSortByDistance(true);

	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();

	Ogre::SceneNode*  mCurObject;

	mMovableFound = false;

	//// remove existing effect if selected
	//if(re_mCurObject != nullptr) {
	//	if(re_mCurObject->getAttachedObject("pSys0")) {
	//		re_mCurObject ->detachObject("pSys0");
	//	}
	//}

	for ( ; it != result.end(); it++)
	{
		mMovableFound =
			it->movable &&
			it->movable->getName() != "" &&
			it->movable->getName() != "MainCam" &&
			it->movable->getName() != "Ground" && 
			it->movable->getName() != "ground";

		if (mMovableFound)
		{
			//Ogre::Vector3 intersect = it->worldFragment->singleIntersection;
			mCurObject = it->movable->getParentSceneNode();
			
			Ogre::LogManager::getSingletonPtr()->logMessage("Moveable object found: "+mCurObject->getName());
			//Ogre::LogManager::getSingletonPtr()->logMessage("Position: "+Ogre::StringConverter::toString(intersect));
			Ogre::Vector3 entityPos = mCurObject->getPosition();
			// detatch from scene node
			// ParticleUniverse::ParticleSystem* pSys0 = pManager->createParticleSystem("pSys0", "mp_torch", mSceneMgr);
			// mCurObject->detachObject(pSys0);

			//mCurObject->createChildSceneNode("pSys0")->attachObject(pSys0);
			//re_mCurObject = mCurObject;
			//// mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys0); // stuff
			//pSys0->getTechnique(0)->position = entityPos;
			//pSys0->start();
			

			break;
		}
	}

	if(mMovableFound)
	{
		// We have selected an entity
		std::cout << "Moveable object found" << std::endl;
		Ogre::LogManager::getSingletonPtr()->logMessage("Moveable object found");
	}

	return true; 
}

bool TutorialApplication::mouseReleased(
	const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
	return true; 
}

void 
	TutorialApplication::createScene()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	mCamera = mSceneMgr->createCamera("MainCam");

	//mCamera->setPosition(Ogre::Vector3(0, 300, -500));
	// We want to create a scene node that we can rotate the camera around at the origin
	Ogre::SceneNode* cameraParent = mSceneMgr->getRootSceneNode()->createChildSceneNode("CAMERA_ROTATION");;
	Ogre::SceneNode* cameraChild = cameraParent->createChildSceneNode("CAMERA_POSITION");

	cameraChild->attachObject(mCamera);
	cameraChild->translate(10, 300,-500);


	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
	
	// setup a skybox
	mSceneMgr->setSkyBox(true, "Examples/TrippySkyBox");

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / 
		Ogre::Real(vp->getActualHeight()));
    // create your scene here :)
        mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
        Ogre::Entity* ninjaEntity[12];
        Ogre::SceneNode* ninjaNode[12];
 
        for (int i = 0; i < sizeof(ninjaEntity) / sizeof(ninjaEntity[0]); i++) {
                Ogre::String number= Ogre::StringConverter::toString(i + 1);
                ninjaEntity[i] = mSceneMgr->createEntity("ninja " + number, "ninja.mesh");
                ninjaNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node " + number);
 
 
        }
 
        ninjaNode[0]->setPosition(700, 0, 500);
        ninjaNode[1]->setPosition(500, 0, 700);
        ninjaNode[2]->setPosition(300, 0, 500);
        ninjaNode[3]->setPosition(100, 0, 700);
        ninjaNode[4]->setPosition(-100, 0, 500);
        ninjaNode[5]->setPosition(-300, 0, 700);
        ninjaNode[6]->setPosition(-500, 0, 500);
        ninjaNode[7]->setPosition(-700, 0, 700);
        ninjaNode[8]->setPosition(100, 0, 300);
        ninjaNode[9]->setPosition(500, 0, 300);
        ninjaNode[10]->setPosition(-300, 0, 300);
        ninjaNode[11]->setPosition(-700, 0, 300);
        ninjaNode[0]->attachObject(ninjaEntity[0]);
        ninjaNode[1]->attachObject(ninjaEntity[1]);
        ninjaNode[2]->attachObject(ninjaEntity[2]);
        ninjaNode[3]->attachObject(ninjaEntity[3]);
        ninjaNode[4]->attachObject(ninjaEntity[4]);
        ninjaNode[5]->attachObject(ninjaEntity[5]);
        ninjaNode[6]->attachObject(ninjaEntity[6]);
        ninjaNode[7]->attachObject(ninjaEntity[7]);
        ninjaNode[8]->attachObject(ninjaEntity[8]);
        ninjaNode[9]->attachObject(ninjaEntity[9]);
        ninjaNode[10]->attachObject(ninjaEntity[10]);
        ninjaNode[11]->attachObject(ninjaEntity[11]);
 
		// ground plane
        Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
        Ogre::MeshManager::getSingleton().createPlane(
                "ground",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                plane,
                1600, 1600, 1, 1,
                true,
                1, 2, 2,
                Ogre::Vector3::UNIT_Z);
 
        Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
        groundEntity->setCastShadows(false);
        groundEntity->setMaterialName("Examples/Checkers");
 
		// create a box here
		// Ogre::MeshManager::getSingleton().createPrefabCube();
		// Ogre::Entity* boardBox = mSceneMgr->createEntity("boardBox");
		// mSceneMgr->createEntity("mycube", "Prefab_Cube");
		// mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject("mycube");
		
		// lights

        mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		


        Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
 
        spotLight->setDiffuseColour(0, 0, 1.0);
        spotLight->setSpecularColour(0, 0, 1.0);
        spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
        spotLight->setDirection(-1, -1, 0);
        spotLight->setPosition(Ogre::Vector3(200, 200, 0));
        spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
        Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
        directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
        directionalLight->setDiffuseColour(Ogre::ColourValue(256, 256, 256));
        directionalLight->setSpecularColour(Ogre::ColourValue(256, 256, 256));
        directionalLight->setDirection(Ogre::Vector3(0, -1, 0));
        Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setDiffuseColour(.3, .3, .3);
        pointLight->setSpecularColour(.3, .3, .3);
        pointLight->setPosition(Ogre::Vector3(0, 150, 250));
}

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
bool TutorialApplication::go()
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

	// Create and Initialise the scene
	createScene();

	// Initialise OIS
	initInput();

	// Setup Particle System
	initParticleSystems();

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mRoot->startRendering();

	return true;
}

void
TutorialApplication::initParticleSystems() {
	// get the particle manager singleton pointer
	ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	// stuff
	pSys0 = pManager->createParticleSystem("pSys0", "explosionSystem", mSceneMgr);

	// create the particle systems
	ParticleUniverse::ParticleSystem* pSys1 = pManager->createParticleSystem("pSys1", "mp_torch", mSceneMgr);
	ParticleUniverse::ParticleSystem* pSys2 = pManager->createParticleSystem("pSys2", "mp_torch", mSceneMgr);
	// cool shield thing
	ParticleUniverse::ParticleSystem* pSys3 = pManager->createParticleSystem("pSys3", "flareShield", mSceneMgr);
	// cool round thing system
	ParticleUniverse::ParticleSystem* pSys4 = pManager->createParticleSystem("pSys4", "example_010", mSceneMgr);

	// attach the particle systems to the scene
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys1);	// torch 1
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys2); // torch 2
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys3); // sheild
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys4); // cool round thing


	// Scale the particle systems
	pSys1->setScaleVelocity(10);
	pSys1->setScale(Ogre::Vector3(10, 10, 10));
	pSys2->setScaleVelocity(10);
	pSys2->setScale(Ogre::Vector3(10, 10, 10));
	pSys3->setScaleVelocity(10);
	pSys3->setScale(Ogre::Vector3(15, 15, 15));
	pSys4->setScaleVelocity(10);
	pSys4->setScale(Ogre::Vector3(10, 10, 10));
	pSys0->setScaleVelocity(10);
	pSys0->setScale(Ogre::Vector3(10, 10, 10));

		// Adjust the position of the particle systems a bit by repositioning their ParticleTechnique (there is only one technique in mp_torch)
	// Normally you would do that by setting the position of the SceneNode to which the Particle System is attached, but in this
	// demo they are both attached to the same rootnode.
	pSys1->getTechnique(0)->position = Ogre::Vector3(5, 0, 0);
	pSys2->getTechnique(0)->position = Ogre::Vector3(-5, 0, 0);
	pSys3->getTechnique(0)->position = Ogre::Vector3(0,0,0);
	pSys4->getTechnique(0)->position = Ogre::Vector3(0,0,0);

	// Start the particle systems
	pSys1->start();
	pSys2->start();
	pSys3->start();
	pSys4->start();

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

