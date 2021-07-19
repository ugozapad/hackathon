#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace engine
{
	class Shader;
	class GrFramebuffer;
	class GrTexture2D;
	class View;

	class PostProcessingManager : public Singleton<PostProcessingManager>
	{
	private:
		static PostProcessingManager ms_instance;

	public:
		void init(View* view);
		void shutdown();

		
	private:
		Shader* m_fadeInOutShader;
		GrFramebuffer* m_framebuffer;
		GrTexture2D* m_colorTexture;
	};
}

#endif