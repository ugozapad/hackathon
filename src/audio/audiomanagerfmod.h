#ifndef AUDIOMANAGERFMOD_H
#define AUDIOMANAGERFMOD_H

#include "audio/audiomanager.h"

#include <fmod.hpp>

namespace engine
{
	class AudioManagerFmod : public AudioManager
	{
		static AudioManagerFmod ms_instance;

	public:
		AudioManagerFmod();
		~AudioManagerFmod();

		virtual void init();
		virtual void shutdown();

	private:
		FMOD::System* m_fmodSystem;
	};

	std::string getStringFromFMODResult(FMOD_RESULT result);
}

#endif // !AUDIOMANAGERFMOD_H
