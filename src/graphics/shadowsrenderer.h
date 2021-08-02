#ifndef SHADOWSRENDERER_H
#define SHADOWSRENDERER_H

namespace engine
{
	class GrFramebuffer;
	class GrTexture2D;

	class ShadowsRenderer : public Singleton<ShadowsRenderer>
	{
		static ShadowsRenderer ms_instance;
	public:
		ShadowsRenderer();
		~ShadowsRenderer();

		void init();
		void shutdown();

	private:
		GrTexture2D* m_shadowMap;
		GrFramebuffer* m_shadowFb;
	};
}

#endif // !SHADOWSRENDERER_H
