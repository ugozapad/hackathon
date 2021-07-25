#include "pch.h"
#include "audio/audiosourcefmod.h"

namespace engine
{
	AudioSourceFMOD::AudioSourceFMOD(const std::string& filename, FMOD::System* system)
	{
		assert(system);
		m_system = system;

		FMOD_RESULT result = m_system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &m_sound);
	}

	AudioSourceFMOD::~AudioSourceFMOD()
	{

	}

	bool AudioSourceFMOD::isPlaying()
	{
		bool playing;
		m_soundChannel->isPlaying(&playing);
		return playing;
	}

	void AudioSourceFMOD::play()
	{
		assert(m_system);
		assert(m_sound);

		FMOD_RESULT result = FMOD_OK;

		if (!m_soundChannel)
		{
			result = m_system->playSound(m_sound, 0, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				spdlog::error("[audio]: failed to play sound!");
				spdlog::error("[audio]: FMOD ERROR: {}", getStringFromFMODResult(result));
				std::terminate();
			}
		}
	}

	void AudioSourceFMOD::stop()
	{
		if (!m_soundChannel)
		{
			spdlog::error("[audio]: trying to stop sound when his sounds channel is not created!");
			std::terminate();
		}
	}
}