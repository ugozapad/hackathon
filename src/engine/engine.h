#ifndef ENGINE_H
#define ENGINE_H

#include "engine/world.h"

namespace engine
{
	class Engine
	{
	public:
		static void init();
		static void shutdown();

		static void loadWorld(const std::string& filename);

		static void loadEmptyWorld();

		static void update();

	public:
		static std::shared_ptr<World> ms_world;
	};
}

#endif // !ENGINE_H
