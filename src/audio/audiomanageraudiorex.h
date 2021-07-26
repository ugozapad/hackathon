#ifndef AUDIOMANAGERAUDIOREX_H
#define AUDIOMANAGERAUDIOREX_H

#include "audio/audiomanager.h"

// Audiorex
#include "emitters_system.h"

namespace engine
{
	class AudioManagerAudiorex : public AudioManager
	{
		static AudioManagerAudiorex ms_instance;
	public:
		void init() override;
		void shutdown() override;

		void update() override;

	private:
		std::shared_ptr<arex::game_mixer> m_soundMixer;
		std::shared_ptr<arex::emitters_callback> m_soundCallback;
		std::shared_ptr<arex::audio_hardware> m_hardware;
	};
}

#endif // !AUDIOMANAGERAUDIOREX_H
