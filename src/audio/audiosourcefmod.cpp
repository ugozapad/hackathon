#include "pch.h"
#include "audio/audiosourcefmod.h"

#include "file/filedevice.h"

#include "file/filesystem.h"

namespace engine
{
	AudioSourceFMOD::AudioSourceFMOD(const std::string& filename, FMOD::System* system)
	{
		m_soundChannel = nullptr;

		assert(system);
		m_system = system;

		/*File* file = FileDevice::instance()->openFile(filename, FileAccess::Read);
		assert(file->isValid());*/

		DataStreamPtr file = FileSystem::getInstance()->openReadFile(filename);

		file->seek(FileSeek::End, 0);
		size_t size = file->tell();
		file->seek(FileSeek::Begin, 0);

		char* data = (char*)malloc(size * sizeof(char));
		file->read(data, size);

		//std::string realFilename = FileDevice::instance()->getDefaultPath();
		//realFilename += filename;

		//FMOD_RESULT result = m_system->createSound(realFilename.c_str(), FMOD_DEFAULT, 0, &m_sound);

		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(exinfo));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;

		FMOD_RESULT result = m_system->createSound(data, FMOD_OPENMEMORY, &exinfo, &m_sound);
		if (result != FMOD_OK)
		{
			Core::error("[audio]: failed to create sound! FMOD ERROR: %s", getStringFromFMODResult(result).c_str());
		}

		free(data);
	}

	AudioSourceFMOD::~AudioSourceFMOD()
	{

	}

	bool AudioSourceFMOD::isPlaying()
	{
		bool playing = false;
		FMOD_RESULT result = m_soundChannel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			Core::error("[audio]: AudioSourceFMOD::isPlaying: %s", getStringFromFMODResult(result).c_str());
		}
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
				Core::error("[audio]: failed to play sound! [audio]: FMOD ERROR: %s", getStringFromFMODResult(result));
			}
		}

		if (m_soundChannel)
		{
			if (isPlaying())
				stop();

			result = m_system->playSound(m_sound, 0, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound![audio]: FMOD ERROR: %s", getStringFromFMODResult(result));
			}
		}
	}

	void AudioSourceFMOD::play(FMOD::ChannelGroup* channelGroup, bool looped /*= false*/)
	{
		assert(channelGroup);
		assert(m_system);
		assert(m_sound);

		FMOD_RESULT result = FMOD_OK;

		if (!m_soundChannel)
		{
			result = m_system->playSound(m_sound, channelGroup, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound! [audio]: FMOD ERROR: %s", getStringFromFMODResult(result));
			}

			m_soundChannel->setLoopCount(-1);
		}

		if (m_soundChannel)
		{
			if (isPlaying())
				stop();

			result = m_system->playSound(m_sound, channelGroup, false, &m_soundChannel);
			if (result != FMOD_OK)
			{
				Core::error("[audio]: failed to play sound![audio]: FMOD ERROR: %s", getStringFromFMODResult(result));
			}
		}
	}

	void AudioSourceFMOD::stop()
	{
		if (!m_soundChannel)
		{
			Core::error("[audio]: trying to stop sound when his sounds channel is not created!");
		}

		m_soundChannel->stop();
	}
}