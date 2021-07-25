#include "pch.h"
#include "audio/audiomanagerfmod.h"

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
			spdlog::error("[audio]: failed to create fmod !");
			spdlog::error("[audio]: FMOD ERROR: {}", getStringFromFMODResult(result));

			std::terminate();
		}

		unsigned int version = 0;
		m_fmodSystem->getVersion(&version);

		if (version < FMOD_VERSION)
		{
			spdlog::error("[audio]: FMOD lib version {} doesn't match header version {}", version, FMOD_VERSION);
			std::terminate();
		}

		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			spdlog::error("[audio]: failed to initialize fmod !");
			spdlog::error("[audio]: FMOD ERROR: {}", getStringFromFMODResult(result));

			std::terminate();
		}

		spdlog::info("[audio]: fmod initialized");

	}

	void AudioManagerFMOD::shutdown()
	{
		FMOD_RESULT result = m_fmodSystem->close();
		if (result != FMOD_OK)
		{
			spdlog::error("[audio]: failed to close fmod !");
			spdlog::error("[audio]: FMOD ERROR: {}", getStringFromFMODResult(result));

			std::terminate();
		}

		result = m_fmodSystem->release();
		if (result != FMOD_OK)
		{
			spdlog::error("[audio]: failed to release fmod !");
			spdlog::error("[audio]: FMOD ERROR: {}", getStringFromFMODResult(result));

			std::terminate();
		}

		m_fmodSystem = nullptr;
	}

	void AudioManagerFMOD::update()
	{
		m_fmodSystem->update();
	}
}