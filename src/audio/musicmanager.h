#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "audio/audio_fmod_shared.h"

namespace engine
{
	class AudioSource;

	class MusicManager : public Singleton<MusicManager>
	{
		static MusicManager ms_instance;
	public:
		void init();
		void shutdown();

		void play(const std::string& filename, bool looped = false);

	private:
		FMOD::ChannelGroup* m_musicChannel;
		AudioSource* m_musicSource;
	};
}

#endif // !MUSICMANAGER_H
