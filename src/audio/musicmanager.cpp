#include "pch.h"
#include "audio/musicmanager.h"
#include "audio/audiomanagerfmod.h"
#include "audio/audiosourcefmod.h"

namespace engine
{
	MusicManager MusicManager::ms_instance;

	void MusicManager::init()
	{
		ASSERT(reinterpret_cast<AudioManagerFMOD*>(AudioManager::getInstance())->getFMOD());
		reinterpret_cast<AudioManagerFMOD*>(AudioManager::getInstance())->getFMOD()->createChannelGroup("Music channel group", &m_musicChannel);
	}

	void MusicManager::shutdown()
	{
		if (m_musicSource && m_musicSource->isPlaying())
			m_musicSource->stop();

		if (m_musicSource)
			AudioManager::getInstance()->deleteSource(m_musicSource);

		m_musicSource = nullptr;

		m_musicChannel->release();
		m_musicChannel = nullptr;
	}

	void MusicManager::play(const std::string& filename, bool looped /*= false*/)
	{
		if (m_musicSource)
			AudioManager::getInstance()->deleteSource(m_musicSource);

		m_musicSource = AudioManager::getInstance()->createSource(filename);
	}
}