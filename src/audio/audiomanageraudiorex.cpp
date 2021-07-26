#include "pch.h"
#include "audio/audiomanageraudiorex.h"

namespace engine
{
	AudioManagerAudiorex AudioManagerAudiorex::ms_instance;

	void AudioManagerAudiorex::init()
	{
		m_soundMixer = std::make_shared<arex::game_mixer>(arex::get_default_format());
		m_soundCallback = std::make_shared<arex::emitters_callback>(m_soundMixer);
		m_hardware = std::make_shared<arex::audio_hardware>(m_soundCallback);

		std::list<arex::audio_device_description>& deviceList = m_hardware->device_list();
		typedef std::list<arex::audio_device_description>::iterator DeviceIt;

		for (DeviceIt it = deviceList.begin(); it != deviceList.end(); ++it)
		{
			spdlog::info("[audio]: device={}", (*it).name());
		}
	}

	void AudioManagerAudiorex::shutdown()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void AudioManagerAudiorex::update()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
}