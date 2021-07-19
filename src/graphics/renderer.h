#ifndef RENDERER_H
#define RENDERER_H

namespace engine
{
	class View;

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

		void init(View* view);
		void shutdown();

		void renderView(View* view);
	};
}

#endif // !RENDERER_H
