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

#include "TutorialApplication.h"
#include <OgreConfigFile.h>
#include <OgreSceneManager.h>
#include "OgreRenderWindow.h"
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreMeshManager.h>
#include <OgreOverlay.h>
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
	minDegree(-180.0f)
{
}

TutorialApplication::~TutorialApplication()
{

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
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


	mWindow = mRoot->initialise(true, "Juicy Checkers");

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
	// Move the camera around an origin point if the user is holding the right mouse button
	if(me.state.buttonDown(OIS::MB_Right))
	{
		int relativeX = me.state.X.rel;
		int relativeY = me.state.Y.rel;
		int relativeZ = me.state.Z.rel;

		Ogre::SceneNode* positionNode = mSceneMgr->getSceneNode("CAMERA_POSITION");
		Ogre::Vector3 pos = positionNode->getPosition();

		Ogre::Vector3 origin = Ogre::Vector3(0,0,0);
		Ogre::Vector3 scale = origin - pos;
		scale.normalise();
		
		
		//if(pos.z > 50.0f)
		//{
			positionNode->setPosition(pos+(scale*relativeZ));
		//}

		//currentDegree += ;

		if(currentDegree > maxDegree){ currentDegree = maxDegree; }
		if(currentDegree < minDegree){ currentDegree = minDegree; }

		Ogre::SceneNode* rotationNode = mSceneMgr->getSceneNode("CAMERA_ROTATION");
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeX*0.1f), Ogre::Vector3(0,1,0)) , Ogre::Node::TransformSpace::TS_WORLD);
		rotationNode->rotate(Ogre::Quaternion(Ogre::Degree(relativeY*0.1f), Ogre::Vector3(1,0,0)) , Ogre::Node::TransformSpace::TS_LOCAL);
	}
	return true; 
}

bool TutorialApplication::mousePressed(
	const OIS::MouseEvent& me, OIS::MouseButtonID id) 
{ 
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
	


	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / 
		Ogre::Real(vp->getActualHeight()));

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	//Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

	//Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//ogreNode->attachObject(ogreEntity);

	Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
	ninjaEntity->setCastShadows(true);

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		1500, 1500, 20, 20, 
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

	groundEntity->setCastShadows(false);

	groundEntity->setMaterialName("Examples/Rockwall");

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);

	directionalLight->setDiffuseColour(Ogre::ColourValue(.8, .8, .8));
	directionalLight->setSpecularColour(Ogre::ColourValue(.8, .8, .8));

	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));
}

void
TutorialApplication::initParticleSystems() {
	// get the particle manager singleton pointer
	ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

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
	pSys4->setScale(Ogre::Vector3(50, 50, 50));

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

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);
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
