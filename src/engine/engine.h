#ifndef ENGINE_H
#define ENGINE_H

namespace engine
{
	class World;

	class Engine
	{
	public:
		static void init();

		static void shutdown();

	public:
		static World* ms_world;
	};
}

#endif // !ENGINE_H
