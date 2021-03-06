#include "pch.h"
#include "audio/audiomanagerfmod.h"
#include "audio/audiosourcefmod.h"
#include "audio/musicmanager.h"

namespace engine
{
	AudioManagerFMOD AudioManagerFMOD::ms_instance;

	AudioManagerFMOD::AudioManagerFMOD()
	{
		m_fmodSystem = nullptr;
	}

	AudioManagerFMOD::~AudioManagerFMOD()
	{
		m_fmodSystem = nullptr;
	}

	void AudioManagerFMOD::init()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
		if (result != FMOD_OK)
		{
			spdlog::error("[audio]: failed to create fmod! FMOD ERROR: {}", getStringFromFMODResult(result));
			std::terminate();
		}

		m_fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
		spdlog::info("[audio]: fmod initialized");

		MusicManager::getInstance()->init();
	}

	void AudioManagerFMOD::shutdown()
	{
		MusicManager::getInstance()->shutdown();

		m_fmodSystem->close();
		m_fmodSystem->release();
		m_fmodSystem = nullptr;
	}

	void AudioManagerFMOD::update()
	{
		m_fmodSystem->update();
	}

	AudioSource* AudioManagerFMOD::createSource(const std::string& filename)
	{
		return (AudioSource*) new AudioSourceFMOD(filename, m_fmodSystem);
	}

	void AudioManagerFMOD::deleteSource(AudioSource* source)
	{
		if (source)
		{
			if (source->isPlaying())
				source->stop();

			delete source;
		}
	}

}
