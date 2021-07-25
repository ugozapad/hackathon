#include "pch.h"

#include "hardware.h"
#include <locale>
#include <codecvt>
#include <thread>

#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

namespace arex {
	static void hardware_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		((audio_hardware*)pDevice->pUserData)->callback(pDevice, pOutput, NULL, frameCount);
	}

	void audio_hardware::callback(
		ma_device* pDevice,
		void* pOutput, 
		const void* pInput, 
		ma_uint32 frameCount
	)
	{
		ringbuffer_callback->process(pOutput, const_cast<void*>(pInput), frameCount);
	}

	std::string
	convert_uuid_to_string(
		ma_device_id& device_id
	) 
	{
		// #TODO:
		std::string ret("no uuid");
		return ret;
	}

	bool
	audio_hardware::initialize_device()
	{
		deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format = DEVICE_FORMAT;
		deviceConfig.playback.channels = DEVICE_CHANNELS;
		deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
		deviceConfig.dataCallback = hardware_callback;
		deviceConfig.pUserData = this;

		if (ma_device_init(&context, &deviceConfig, &device) != MA_SUCCESS) {
			return false;
		}

		if (ma_device_start(&device) != MA_SUCCESS) {
			ma_device_uninit(&device);
			return false;
		}

		return true;
	}

	bool
	audio_hardware::initialize_device(
		int device_id
	)
	{
		return false;
	}

	bool
	audio_hardware::initialize_device(
		std::string device_uuid
	)
	{
		return false;
	}

	bool
	audio_hardware::destroy_device()
	{	
		ma_device_uninit(&device);
		return true;
	}

	std::list<audio_device_description>& 
	audio_hardware::device_list()
	{
		ma_result result;
		ma_device_info* pPlaybackDeviceInfos;
		ma_uint32 playbackDeviceCount;
		ma_device_info* pCaptureDeviceInfos;
		ma_uint32 captureDeviceCount;
		ma_uint32 iDevice;

		result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
		if (result == MA_SUCCESS) {
			hardware_device_list.clear();
			for (size_t i = 0; i < playbackDeviceCount; i++) {
				auto format = audio_device_format(PCM_FRAME_CHUNK_SIZE, DEVICE_SAMPLE_RATE, 32, DEVICE_CHANNELS);
				auto device_info = audio_device_description(i, convert_uuid_to_string(pPlaybackDeviceInfos[i].id), std::string(pPlaybackDeviceInfos[i].name), format);
				hardware_device_list.push_back(device_info);
			}
		}

		return hardware_device_list;
	}
};
