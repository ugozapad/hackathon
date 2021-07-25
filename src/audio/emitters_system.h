#include <cstdio>
#include <cstdlib>
#include "hardware.h"

namespace arex {
	class AREX_API vec3 {
	private:
		float coord[3] = {};

	public:
		vec3() {}
		vec3(float x, float y, float z) { set(x, y, z); }

		void set(float x, float y, float z) {
			coord[0] = x;
			coord[1] = y;
			coord[2] = z;
		}

		void set(vec3 source) {
			coord[0] = source.x();
			coord[1] = source.y();
			coord[2] = source.z();
		}

		float x() {
			return coord[0];
		}

		float y() {
			return coord[1];
		}

		float z() {
			return coord[2];
		}

		float* get() {
			return coord;
		}
	};

	class AREX_API vec4 {
	private:
		float coord[4] = {};

	public:
		vec4() {}
		vec4(float x, float y, float z, float w) { set(x, y, z, w); }

		void set(float x, float y, float z, float w) {
			coord[0] = x;
			coord[1] = y;
			coord[2] = z;
			coord[3] = w;
		}

		void set(vec4 source) {
			coord[0] = source.x();
			coord[1] = source.y();
			coord[2] = source.z();
			coord[3] = source.w();
		}

		float x() {
			return coord[0];
		}

		float y() {
			return coord[1];
		}

		float z() {
			return coord[2];
		}

		float w() {
			return coord[3];
		}

		float* get() {
			return coord;
		}
	};

	class AREX_API game_listener {
	private:
		vec3 listener_pos;
		vec4 listener_rotation;

	public:
		game_listener() { }
		vec3 position() { return listener_pos; }
		vec4 rotation() { return listener_rotation; }
	};

	enum class AREX_API emitter_status {
		emitter_stopped = 0,
		emitter_paused,
		emitter_playing,
		emitter_looped
	};

	class AREX_API game_source {
	private:
		std::string source_path;
		ma_decoder decoder;
		audio_device_format fmt;
		ma_decoder_config config;

	public:
		game_source(std::string path_to_file, audio_device_format device_format) : source_path(path_to_file), fmt(device_format) {
			config = ma_decoder_config_init(ma_format_f32, device_format.channels(), device_format.sample_rate());
			ma_result result = ma_decoder_init_file(path_to_file.c_str(), &config, &decoder);
			if (result != MA_SUCCESS) {
#ifndef AREX_DISABLE_EXCEPTIONS
				throw std::exception();
#endif
			}
		}

		~game_source() { 
			ma_decoder_uninit(&decoder); 
		}

		void set_format(audio_device_format device_format) { fmt = device_format; }
		audio_device_format get_format() { return fmt; }

		bool set_position(size_t src_position) { return (ma_decoder_seek_to_pcm_frame(&decoder, src_position) == MA_SUCCESS); }
		size_t get_position() { return 0; }

		size_t process(float* pInput, float* pOutput, size_t frames) {
			return ma_decoder_read_pcm_frames(&decoder, pOutput, frames);
		}
	};

	class AREX_API game_emitter {
	private:
		float volume = 0.5f;
		std::shared_ptr<game_source> parent_ptr;
		game_listener emitter_listener;
		emitter_status status = emitter_status::emitter_stopped;

		void return_to_begin() {
			parent_ptr->set_position(0);
		}

		void internal_process(float* pInput, float* pOutput, size_t frames) {
			for (size_t i = 0; i < frames * get_format().channels(); i++) {
				pOutput[i] *= volume;
			}
		}

	public:
		game_emitter(std::shared_ptr<game_source> source_ptr) : parent_ptr(source_ptr), emitter_listener() {}

		void set_format(audio_device_format device_format) { parent_ptr->set_format(device_format); }
		audio_device_format get_format() { return parent_ptr->get_format(); }

		void set_position(vec3 pos) { emitter_listener.position().set(pos); }
		void set_rotation(vec4 rot) { emitter_listener.rotation().set(rot); }

		vec3 get_position() { return emitter_listener.position(); }
		vec4 get_rotation() { return emitter_listener.rotation(); }

		void set_volume(float value) { volume = value; }
		float get_volume() { return volume; }

		void set_source_position(size_t src_position) {} 
		size_t get_source_position() { return 0; }

		void set_status(emitter_status new_status) { status = new_status; }
		emitter_status get_status() { return status; }

		size_t process(float* pInput, float* pOutput, size_t frames)
		{
			if (status != emitter_status::emitter_looped && status != emitter_status::emitter_playing) {
				return 0;
			}

			size_t processed_frames = 0;
			size_t return_frames = parent_ptr->process(pInput, pOutput, frames);
			processed_frames += return_frames;
			if (return_frames < frames) {
				frames -= return_frames;
				return_to_begin();

				if (status != emitter_status::emitter_looped) {
					status = emitter_status::emitter_stopped;
				} else {
					return_frames = parent_ptr->process(pInput, pOutput, frames);
					processed_frames += return_frames;
					if (return_frames < frames) {
						return -1;
					}
				}
			}

			internal_process(pInput, pOutput, frames);
			return processed_frames;
		}
	};

	class AREX_API game_mixer {
	private:
		audio_device_format fmt;
		std::unordered_map<std::string, std::shared_ptr<game_emitter>> emitters_list;
		std::unordered_map<std::string, std::shared_ptr<game_source>> sources_list;
		std::vector<float> input_buffer;
		std::vector<float> output_buffer;

		void reserve_bigger(int frames_count) {
			if ((int)input_buffer.size() < frames_count * fmt.channels()) {
				input_buffer.resize(size_t(frames_count) * size_t(fmt.channels()));
			}

			if ((int)output_buffer.size() < frames_count * fmt.channels()) {
				output_buffer.resize(size_t(frames_count) * size_t(fmt.channels()));
			}
		}

	public:
		game_mixer(audio_device_format format) : fmt(format) {}

		void process(void* pOutput, void* pInput, int frames_count) {
			reserve_bigger(frames_count);

			for (auto elem : emitters_list) {
				size_t ret = elem.second->process(input_buffer.data(), output_buffer.data(), frames_count);
				if (ret != -1) {
					for (size_t i = 0; i < size_t(frames_count) * size_t(fmt.channels()); i++) {
						((float*)(pOutput))[i] += output_buffer[i];
					}
				}
			}
		}

		bool add_emitter(std::string name, std::shared_ptr<game_emitter> emitter) {
			if (name.empty()) return false;
			emitters_list.insert({ name, emitter });
			return true;
		}

		bool add_source(std::string name, std::shared_ptr<game_source> source) {
			if (name.empty()) return false;
			sources_list.insert({ name, source });
			return true;
		}

		bool delete_source(std::string name) {
			sources_list.erase(name);
			return true;
		}
	};

	class AREX_API emitters_callback : public audio_callback {
	private:
		std::shared_ptr<game_mixer> parent_mixer;

	public:
		emitters_callback(std::shared_ptr<game_mixer> mixer) : parent_mixer(mixer) {}
		virtual ~emitters_callback() override {}
		virtual void process(void* pOutput, void* pInput, int frames_count) override { 
			parent_mixer->process(pOutput, pInput, frames_count);
		}
	};
};
