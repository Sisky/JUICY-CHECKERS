// stdafx.h
// Raknet headers
#include <WinSock2.h>
// #include "winsock2.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "datastructures.h"


// Ogre headers

// #include <Ogre.h>

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

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>

// #include <SdkCameraMan.h>

// #include <OgreFontManager.h>
// #include <OgreBorderPanelOverlayElement.h>
// #include <OgreTextAreaOverlayElement.h>


// Particle Universe headers
#include "ParticleUniverseSystemManager.h"
 


// any other header can be included, as usual
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#endif

