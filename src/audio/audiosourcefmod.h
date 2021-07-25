#ifndef AUDIOSOURCEFMOD_H
#define AUDIOSOURCEFMOD_H

#include "audio/audiosource.h"
#include "audio/audio_fmod_shared.h"

namespace engine
{
	class AudioSourceFMOD : public AudioSource
	{
	public:
		AudioSourceFMOD(const std::string& filename, FMOD::System* system);
		~AudioSourceFMOD();

		bool isPlaying() override;

		void play() override;
		void stop() override;

	private:
		FMOD::System* m_system;
		FMOD::Sound* m_sound;
		FMOD::Channel* m_soundChannel;
	};
}

#endif // !AUDIOSOURCEFMOD_H
