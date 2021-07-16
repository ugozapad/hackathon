#ifndef RENDERER_H
#define RENDERER_H

namespace engine
{
	class Renderer : public Singleton<Renderer>
	{
	private:
		static Renderer* ms_instance;
	public:
		static void createInstance();
		static void destroyInstance();

	public:
		Renderer();
		~Renderer();

	public:
		void init();
		void shutdown();
	};
}

#endif // !RENDERER_H
