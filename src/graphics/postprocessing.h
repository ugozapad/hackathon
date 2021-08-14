#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

namespace engine
{
	class Shader;
	class GrFramebuffer;
	class GrTexture2D;
	class View;

	class PostProcessingRenderer : public Singleton<PostProcessingRenderer>
	{
	private:
		static PostProcessingRenderer ms_instance;

	public:
		void init(View* view);
		void shutdown();

		void gammaCorrection();

		GrFramebuffer* getFramebuffer() { return m_framebuffer; }
		
	private:
		Shader* m_fadeInOutShader;
		Shader* m_gammaCorrectionShader;
		GrFramebuffer* m_framebuffer;
		GrTexture2D* m_colorTexture;
	};
}

#endif