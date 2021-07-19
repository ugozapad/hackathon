#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H

#include "graphics/framebuffer.h"

namespace engine
{
	class GLFramebuffer : public GrFramebuffer
	{
	public:
		GLFramebuffer(int width, int height);
		~GLFramebuffer();

		unsigned int getHandle();

		void setColorTexture(int index, GrTexture2D* colorTexture);
		void setDepthTexture(GrTexture2D* depthTexture);

		void finalize();

	private:
		uint32_t m_handle;
		uint32_t m_renderBuffer;
	};
}

#endif // !GLFRAMEBUFFER_H
