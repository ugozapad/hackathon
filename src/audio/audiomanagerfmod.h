#ifndef AUDIOMANAGERFMOD_H
#define AUDIOMANAGERFMOD_H

#include "audio/audiomanager.h"
#include "audio/audio_fmod_shared.h"

namespace engine
{
	class AudioManagerFMOD : public AudioManager
	{
		static AudioManagerFMOD ms_instance;
	public:
		AudioManagerFMOD();
		~AudioManagerFMOD();

		virtual void init();
		virtual void shutdown();

		void update() override;

		virtual AudioSource* createSource(const std::string& filename);
		virtual void deleteSource(AudioSource* source);
	
	private:
		FMOD::System* m_fmodSystem;
	};
}

#endif // !AUDIOMANAGERFMOD_H
