#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace engine
{
	class AudioSource;
	
	class AudioManager : public Singleton<AudioManager>
	{
	public:
		AudioManager();
		virtual ~AudioManager();

		virtual void init();
		virtual void shutdown();

		virtual void update();

		virtual AudioSource* create_source(const std::string& filename);
	};
}

#endif // !AUDIOMANAGER_H
