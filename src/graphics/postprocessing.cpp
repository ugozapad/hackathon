#include "pch.h"

// glad
#include "graphics/gl/glad/glad.h"

#include "graphics/postprocessing.h"
#include "graphics/view.h"
#include "graphics/graphicsdevice.h"
#include "graphics/texture.h"
#include "graphics/framebuffer.h"
#include "graphics/shader.h"

namespace engine
{
	PostProcessingManager PostProcessingManager::ms_instance;

	void PostProcessingManager::init(View* view)
	{
		GraphicsDevice* device = GraphicsDevice::getInstance();
		m_framebuffer = device->createFramebuffer(view->m_width, view->m_height);

		TextureCreationDesc colorTextureDesc = { 0 };
		colorTextureDesc.m_width = view->m_width;
		colorTextureDesc.m_height = view->m_height;
		colorTextureDesc.m_format = ImageFormat::RGBA32F; // float
		m_colorTexture = device->createTexture2D(colorTextureDesc);

		m_framebuffer->setColorTexture(0, m_colorTexture);

		//////////////////////////////////////////////////////////////////////////
		// ugly piece of shit
		GraphicsDevice::instance()->setFramebuffer(m_framebuffer);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			spdlog::error("PostProcessingManager::init: Framebuffer is not complete.");
			std::terminate();
		}

		GraphicsDevice::instance()->setFramebuffer(0);
		//////////////////////////////////////////////////////////////////////////

		m_fadeInOutShader = mem_new<Shader>(*g_sysAllocator, "fade_in_out", "data/shaders/fade_in_out.vsh", "data/shaders/fade_in_out.fsh");
	}

	void PostProcessingManager::shutdown()
	{
		mem_delete(*g_sysAllocator, m_fadeInOutShader);

		GraphicsDevice::getInstance()->deleteTexture2D(m_colorTexture);
		GraphicsDevice::getInstance()->deleteFramebuffer(m_framebuffer);
	}

}