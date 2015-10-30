//
// Author: Jony Hill <xyc8034@aut.ac.nz>
// Date: 22/08/2015    ID: 1301787
// 
// This class is to handle the FMOD sound system.
//	

// This include
#include "stdafx.h"
#include "SoundSystem.h"

// Static Members:
SoundSystem* SoundSystem::sm_pInstance = 0;

SoundSystem&
	SoundSystem::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new SoundSystem();
	}



	//assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
	SoundSystem::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

SoundSystem::SoundSystem()
	: m_pSystem(0)
{

}

SoundSystem::~SoundSystem()
{
	// Release all the sounds
	std::vector<FMOD::Sound*>::iterator soundIterator;
	for(soundIterator = m_Sounds.begin();
		soundIterator != m_Sounds.end();
		++soundIterator)
	{
		(*soundIterator)->release();
	}
	std::vector<FMOD::Channel*>::iterator chanelIterator;
	for(chanelIterator = m_Channels.begin();
		chanelIterator != m_Channels.end();
		++chanelIterator)
	{
		(*chanelIterator)->stop();
	}

	FMOD_RESULT result = m_pSystem->release();


}

bool
	SoundSystem::Initialise()
{
	// Try to initialise the FMOD sound system
	FMOD_RESULT result;
	m_pSystem = NULL;

	result = FMOD::System_Create(&m_pSystem);      // Create the main system object.
	if (result != FMOD_OK)
	{
		return false;
	}

	result = m_pSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		return false;
	}

	// if fmod initialised load all the sounds
	FMOD::Sound* s = 0;
	result = m_pSystem->createSound("Audio\\knife.mp3", FMOD_DEFAULT, 0, &s);	
	s->setMode(FMOD_LOOP_OFF);
	if (result != FMOD_OK)
	{
		return false;
	}
	m_Sounds.push_back(s);

	// if fmod initialised load all the sounds
	s = 0;
	result = m_pSystem->createSound("Audio\\laser.mp3", FMOD_DEFAULT, 0, &s);	
	s->setMode(FMOD_LOOP_OFF);
	if (result != FMOD_OK)
	{
		return false;
	}
	m_Sounds.push_back(s);

	// if fmod initialised load all the sounds
	s = 0;
	result = m_pSystem->createSound("Audio\\quest_study.mp3", FMOD_DEFAULT, 0, &s);	
	s->setMode(FMOD_LOOP_NORMAL);
	if (result != FMOD_OK)
	{
		return false;
	}
	m_Sounds.push_back(s);

	

	for(int i = 0; i < 5; ++i)
	{
		m_Channels.push_back(0);
	}


	return true;
}

void 
	SoundSystem::PlaySound(SOUNDFILE _eSound)
{
	FMOD_RESULT result;
	result = m_pSystem->playSound(m_Sounds[_eSound], 0, false, &(m_Channels[_eSound]));
	if (result != FMOD_OK)
	{
		
	}

	if(_eSound == BG)
	{
		m_Channels[_eSound]->setVolume(0.2f);
	}
	
}

void 
	SoundSystem::Process()
{
	FMOD_RESULT result = m_pSystem->update();
	if (result != FMOD_OK)
	{
	}
}

void
	SoundSystem::StopSound(SOUNDFILE _eSound)
{
	m_Channels[_eSound]->stop();
}

