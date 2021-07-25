#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

namespace engine
{
	class AudioSource
	{
	public:
		virtual ~AudioSource() {}

		virtual bool isPlaying();

		virtual void play();
		virtual void stop();
	};
}

#endif // !AUDIOSOURCE_H
