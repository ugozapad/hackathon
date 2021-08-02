#include "pch.h"

#include "graphics/gl/glad/glad.h"

#include "graphics/shadowsrenderer.h"
#include "graphics/texture.h"
#include "graphics/framebuffer.h"
#include "graphics/graphicsdevice.h"

namespace engine
{
	ShadowsRenderer ShadowsRenderer::ms_instance;

	ShadowsRenderer::ShadowsRenderer()
	{
		m_shadowMap = nullptr;
		m_shadowFb = nullptr;
	}

	ShadowsRenderer::~ShadowsRenderer()
	{
		m_shadowMap = nullptr;
		m_shadowFb = nullptr;
	}

	void ShadowsRenderer::init()
	{
		int shadowMapSize = 512;

		TextureCreationDesc shadowMapDesc;
		memset(&shadowMapDesc, 0, sizeof(shadowMapDesc));
		shadowMapDesc.m_width = shadowMapDesc.m_height = shadowMapSize;
		shadowMapDesc.m_format = ImageFormat::DEPTH32;
		m_shadowMap = GraphicsDevice::instance()->createTexture2D(shadowMapDesc);

		m_shadowFb = GraphicsDevice::instance()->createFramebuffer(shadowMapSize, shadowMapSize);
		m_shadowFb->setDepthTexture(m_shadowMap);

		//////////////////////////////////////////////////////////////////////////
		// ugly piece of shit
		GraphicsDevice::instance()->setFramebuffer(m_shadowFb);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Core::error("ShadowsRenderer::init: Framebuffer is not complete.");
		}

		GraphicsDevice::instance()->setFramebuffer(0);
		//////////////////////////////////////////////////////////////////////////
	}

	void ShadowsRenderer::shutdown()
	{
		GraphicsDevice::instance()->deleteFramebuffer(m_shadowFb);
		GraphicsDevice::instance()->deleteTexture2D(m_shadowMap);
	}

}