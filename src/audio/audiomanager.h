#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace engine
{
	class AudioManager : public Singleton<AudioManager>
	{
	public:
		AudioManager();
		virtual ~AudioManager();

		virtual void init();
		virtual void shutdown();
	};
}

#endif // !AUDIOMANAGER_H
