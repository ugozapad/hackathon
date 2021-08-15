#ifndef RENDERER_H
#define RENDERER_H

namespace engine
{
	class View;
	class GraphicsDevice;
	class PostProcessingRenderer;

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

		void beginFrame();

		void renderView(View* view);

		void makeScreenshot();

		void endFrame();

		void resetDeviceState();
		void resetTextureBindingsAfterModelRender();

	private:
		View* m_view;
		GraphicsDevice* m_graphicsDevice;
		PostProcessingRenderer* m_postProcessingRenderer;
	};
}

#endif // !RENDERER_H
