#include "pch.h"
#include "glframebuffer.h"
#include "gltextures.h"


namespace engine
{
	GLFramebuffer::GLFramebuffer(int width, int height)
{
		// generate frame buffer
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		// generate render buffer
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);

		// create default depth buffer.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		// and attach them to fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

		// unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLFramebuffer::~GLFramebuffer()
	{
		glDeleteRenderbuffers(1, &m_renderBuffer);
		glDeleteFramebuffers(1, &m_handle);
	}

	unsigned int GLFramebuffer::getHandle()
	{
		return m_handle;
	}

	void GLFramebuffer::setColorTexture(int index, GrTexture2D* colorTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, colorTexture->getHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFramebuffer::setDepthTexture(GrTexture2D* depthTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}

