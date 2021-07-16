#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace engine
{
	class GrTexture2D;

	class GrFramebuffer
	{
	public:
		virtual ~GrFramebuffer() {}

		virtual unsigned int getHandle() = 0;

		virtual void setColorTexture(int index, GrTexture2D* colorTexture) = 0;
		virtual void setDepthTexture(GrTexture2D* depthTexture) = 0;
	};
}

#endif // !FRAMEBUFFER_H
