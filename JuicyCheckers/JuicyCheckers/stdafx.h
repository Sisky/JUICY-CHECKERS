// stdafx.h
 
// raknet
#include <winsock2.h>

// Ogre headers you need
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OgreMeshManager.h>
#include <OgreOverlay.h>

// particle universe
#include <ParticleUniverseSystemManager.h>
 


// any other header can be included, as usual
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#endif

