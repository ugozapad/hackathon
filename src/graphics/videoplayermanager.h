#ifndef VIDEOPLAYERMANAGER_H
#define VIDEOPLAYERMANAGER_H

namespace engine
{
	class VideoPlayerManager : public Singleton<VideoPlayerManager>
	{
		static VideoPlayerManager ms_instance;
	public:
		void loadVideo(const std::string& filename);
	};
}

#endif