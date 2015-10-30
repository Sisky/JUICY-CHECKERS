//
// Author: Jony Hill <xyc8034@aut.ac.nz>
// Date: 22/08/2015    ID: 1301787
// 
// This class is to handle the FMOD sound system.
//	

// library includes
#include "fmod.hpp"
#include <vector>

class SoundSystem
{
	// Member Methods
public:
	static SoundSystem& GetInstance();
	static void DestroyInstance();
	~SoundSystem();

	bool Initialise();
	void Process();

	enum SOUNDFILE
	{
		KNIFE,
		LASER,
		BG
	};
	void StopSound(SOUNDFILE _eSound);

	void PlaySound(SOUNDFILE _eSound);

protected:
	static SoundSystem* sm_pInstance;

private:
	SoundSystem();

	// Methods for playing sounds
	

	// Member Variables
protected:

private:
	FMOD::System* m_pSystem;
	std::vector<FMOD::Sound*> m_Sounds;
	std::vector<FMOD::Channel*> m_Channels;
};