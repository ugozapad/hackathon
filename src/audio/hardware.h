#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>

#if defined(_WIN32) || defined(_WIN64)
#ifdef DLL_PLATFORM
#ifdef LIB_EXPORTS
#define AREX_API __declspec(dllexport)
#else
#define AREX_API __declspec(dllimport)
#endif
#else 
#define AREX_API
#endif
#else
#define AREX_API
#endif

#define MA_NO_DSOUND
#define MA_NO_WINMM

#define MA_HAS_VORBIS
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "dr_flac.h"
#include "dr_wav.h"
#include "dr_mp3.h"

#define DEVICE_FORMAT           ma_format_f32
#define DEVICE_CHANNELS         2
#define DEVICE_SAMPLE_RATE      48000
#define PCM_FRAME_CHUNK_SIZE    1024

namespace arex {
	class AREX_API audio_callback {
	public:
		virtual ~audio_callback() {}
		virtual void process(void* pOutput, void* pInput, int frames_count) = 0;
	};

	class AREX_API audio_device_format {
	private:
		int device_frames_count = 0;
		int device_sample_rate = 0;
		int device_bitrate = 0;
		int device_channels = 0;

	public:
		audio_device_format() {}
		audio_device_format(int frames_count, int samplerate, int bit_rate, int channel_count) : 
			device_frames_count(frames_count), device_sample_rate(samplerate), device_bitrate(bit_rate), device_channels(channel_count) {
		}

		int frames() { return device_frames_count; }
		int sample_rate() { return device_sample_rate; }
		int bitrate() { return device_bitrate; }
		int channels() { return device_channels; }
	};

	class AREX_API audio_device_description {
	private:
		int device_id = 0;
		std::string device_uuid;
		std::string device_name;
		audio_device_format device_format;

	public:
		audio_device_description(int a1, std::string a2, std::string a3, audio_device_format a4) : device_id(a1), device_uuid(a2), device_name(a3), device_format(a4) {}

		int id() { return device_id; }
		std::string uuid() { return device_uuid; }
		std::string name() { return device_name; }
		audio_device_format& format() { return device_format; }
	};

	class AREX_API audio_hardware {
	private:
		std::shared_ptr<audio_callback> ringbuffer_callback;
		ma_context context = {};
		ma_waveform_config waveformConfig = {};
		ma_device_config deviceConfig = {};
		ma_device device = {};

		std::list<audio_device_description> hardware_device_list;

	public:
		audio_hardware(std::shared_ptr<audio_callback> mixer_callback) : ringbuffer_callback(mixer_callback) {
			if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
				printf("Failed to initialize context.\n");
			}
		}

		void callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

		bool initialize_device();
		bool initialize_device(int device_id);
		bool initialize_device(std::string device_uuid);

		bool destroy_device();

		std::list<audio_device_description>& device_list();
	};


	inline audio_device_format get_default_format() {
		return audio_device_format(PCM_FRAME_CHUNK_SIZE, DEVICE_SAMPLE_RATE, 32, DEVICE_CHANNELS);
	}
}
