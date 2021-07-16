#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H

namespace engine
{
	class Shader;
	class GrFramebuffer;
	class GrTexture2D;
	class IVertexBuffer;
	class Camera;
	class Entity;
	class Light;

	class DeferredRenderer
	{
	public:
		enum RT
		{
			RT_POS,
			RT_NORMAL,
			RT_COLOR
		};
	public:
		void init();
		void shutdown();

		GrFramebuffer* getFramebuffer() { return m_framebuffer; }

		GrTexture2D* getTexture(int index);

		void drawGeometry(Camera* camera, Entity* entity);
		void drawLight(Camera* camera, Light** lights, size_t size);
		void drawNoLight(Camera* camera);

	private:
		GrTexture2D* m_textures[3];
		GrFramebuffer* m_framebuffer;
		Shader* m_lightPassShader;

	};

	extern DeferredRenderer g_deferredRenderer;
}

#endif // !DEFERREDRENDERER_H
