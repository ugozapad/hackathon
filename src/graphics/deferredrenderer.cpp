#include "pch.h"

#include "graphics/deferredrenderer.h"
#include "graphics/graphicsdevice.h"

namespace engine
{
	DeferredRenderer g_deferredRenderer;

	void DeferredRenderer::init()
	{
		int width, height;
		//glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);

		// create position texture
		TextureCreationDesc positionDesc = { 0 };
		positionDesc.m_width = width;
		positionDesc.m_height = height;
		positionDesc.m_format = ImageFormat::RGBA32F;
		m_textures[RT_POS] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create normal texture
		TextureCreationDesc normalDesc = { 0 };
		positionDesc.m_width = width;
		positionDesc.m_height = height;
		positionDesc.m_format = ImageFormat::RGBA32F;
		m_textures[RT_NORMAL] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create color texture
		TextureCreationDesc colorDesc = { 0 };
		colorDesc.m_width = width;
		colorDesc.m_height = height;
		colorDesc.m_format = ImageFormat::RGBA32F;
		m_textures[RT_COLOR] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create frame buffer
		m_framebuffer = GraphicsDevice::instance()->createFramebuffer(width, height);

		// attach our texture's
		m_framebuffer->setColorTexture(0, m_textures[RT_POS]);
		m_framebuffer->setColorTexture(1, m_textures[RT_NORMAL]);
		m_framebuffer->setColorTexture(2, m_textures[RT_COLOR]);

		//////////////////////////////////////////////////////////////////////////
		// ugly piece of shit
		//g_renderDevice->SetFramebuffer(m_framebuffer);

		//GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		//glDrawBuffers(3, DrawBuffers);

		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	Error("DeferredRenderer::init: Framebuffer is not complete.");

		//g_renderDevice->SetFramebuffer(0);
		//////////////////////////////////////////////////////////////////////////

		// create light pass shader.
		//m_lightPassShader = g_shaderSystem.CreateShader("def_light");
	}

	void DeferredRenderer::shutdown()
	{
		GraphicsDevice::instance()->deleteFramebuffer(m_framebuffer);
		GraphicsDevice::instance()->deleteTexture2D(m_textures[1]);
		GraphicsDevice::instance()->deleteTexture2D(m_textures[0]);
	}

	GrTexture2D* DeferredRenderer::getTexture(int index)
	{
		return m_textures[index];
	}

//#include "engine/camera.h"
//#include "engine/entity.h"
//#include "render/render.h"
//#include "game/light.h"

	void DeferredRenderer::drawGeometry(Camera* camera, Entity* entity)
	{
		assert(camera);
		assert(entity);
		//ASSERT(entity->m_Model);

		//RenderContext& ctx = g_render->GetRenderContext();
		//ctx.view = camera->GetViewMatrix();
		//ctx.proj = camera->GetProjectionMatrix();
		//ctx.model = entity->GetTranslation();

		//g_render->SetRenderContext(ctx);

		//// submit entity to render
		//entity->Render();
	}

	void DeferredRenderer::drawLight(Camera* camera, Light** lights, size_t size)
	{
		assert(camera);
		assert(lights);

		//m_lightPassShader->Bind();

		//g_renderDevice->SetTexture2D(0, m_textures[RT_POS]);
		//m_lightPassShader->SetInteger("u_positionTexture", 0);

		//g_renderDevice->SetTexture2D(1, m_textures[RT_NORMAL]);
		//m_lightPassShader->SetInteger("u_normalTexture", 1);

		//g_renderDevice->SetTexture2D(2, m_textures[RT_COLOR]);
		//m_lightPassShader->SetInteger("u_colorTexture", 2);

		//m_lightPassShader->SetInteger("u_lightsCount", size);

		//for (int i = 0; i < size; i++)
		//{
		//	char uniformName[64];
		//	sprintf(uniformName, "u_lightsPos[%i]", i);

		//	m_lightPassShader->SetVec3(uniformName, lights[i]->m_Position);
		//}

		//m_lightPassShader->SetVec3("u_lightPos", lights[0]->m_Position);
		//m_lightPassShader->SetVec3("u_viewPos", camera->m_Position);

		//ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
	}

	void DeferredRenderer::drawNoLight(Camera* camera)
	{
		assert(camera);

		//m_lightPassShader->Bind();

		//g_renderDevice->SetTexture2D(0, m_textures[RT_POS]);
		//m_lightPassShader->SetInteger("u_positionTexture", 0);

		//g_renderDevice->SetTexture2D(1, m_textures[RT_NORMAL]);
		//m_lightPassShader->SetInteger("u_normalTexture", 1);

		//g_renderDevice->SetTexture2D(2, m_textures[RT_COLOR]);
		//m_lightPassShader->SetInteger("u_colorTexture", 2);

		//m_lightPassShader->SetVec3("u_viewPos", camera->m_Position);

		//ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
	}

}

