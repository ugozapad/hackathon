#include "pch.h"
#include "graphics/gl/glad/glad.h"
#include "graphics/deferredrenderer.h"
#include "graphics/graphicsdevice.h"
#include "graphics/view.h"
#include "graphics/screenquad.h"
#include "graphics/shadermanager.h"
#include "graphics/shader.h"
#include "graphics/light.h"
#include "graphics/model.h"
#include "engine/camera.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderconstantmanager.h"

namespace engine
{
	DeferredRenderer g_deferredRenderer;

	void DeferredRenderer::init(View* view)
	{
		int width = view->m_width, height = view->m_height;

		// create position texture
		TextureCreationDesc positionDesc = { 0 };
		positionDesc.m_width = width;
		positionDesc.m_height = height;
		positionDesc.m_format = ImageFormat::RGBA16F;
		m_textures[RT_POS] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create normal texture
		TextureCreationDesc normalDesc = { 0 };
		positionDesc.m_width = width;
		positionDesc.m_height = height;
		positionDesc.m_format = ImageFormat::RGBA16F;
		m_textures[RT_NORMAL] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create color texture
		TextureCreationDesc colorDesc = { 0 };
		colorDesc.m_width = width;
		colorDesc.m_height = height;
		colorDesc.m_format = ImageFormat::RGB32;
		m_textures[RT_COLOR] = GraphicsDevice::instance()->createTexture2D(positionDesc);

		// create frame buffer
		m_framebuffer = GraphicsDevice::instance()->createFramebuffer(width, height);

		// attach our texture's
		m_framebuffer->setColorTexture(0, m_textures[RT_POS]);
		m_framebuffer->setColorTexture(1, m_textures[RT_NORMAL]);
		m_framebuffer->setColorTexture(2, m_textures[RT_COLOR]);

		//////////////////////////////////////////////////////////////////////////
		// ugly piece of shit
		GraphicsDevice::instance()->setFramebuffer(m_framebuffer);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Core::error("DeferredRenderer::init: Framebuffer is not complete.");
		}

		GraphicsDevice::instance()->setFramebuffer(0);
		//////////////////////////////////////////////////////////////////////////

		// create light pass shader.
		m_lightPassShader = ShaderManager::getInstance()->createShader("def_light");
		//m_lightPassShader = ShaderManager::getInstance()->createShader("lighting", "quad", "");

		////////////////////
		// load light sphere
		VertexDeclaration pos;
		pos.name = "position";
		pos.size = 3;
		pos.type = VertexDeclaration::Float;

		std::vector<VertexDeclaration> decls;
		decls.push_back(pos);

		m_lightSphereBuffer = createVBFromModel(decls.data(), decls.size(), "models/sphere.dae", &m_sphereVerticesCount);
	}

	void DeferredRenderer::shutdown()
	{
		//m_framebuffer->setColorTexture(0, 0);
		//m_framebuffer->setColorTexture(1, 0);
		//m_framebuffer->setColorTexture(2, 0);

		GraphicsDevice::instance()->deleteVertexBuffer(m_lightSphereBuffer);

		GraphicsDevice::instance()->deleteFramebuffer(m_framebuffer);
		GraphicsDevice::instance()->deleteTexture2D(m_textures[2]);
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

	//void DeferredRenderer::drawGeometry(Camera* camera, Entity* entity)
	//{
	//	assert(camera);
	//	assert(entity);
	//	//ASSERT(entity->m_Model);

	//	//RenderContext& ctx = g_render->GetRenderContext();
	//	//ctx.view = camera->GetViewMatrix();
	//	//ctx.proj = camera->GetProjectionMatrix();
	//	//ctx.model = entity->GetTranslation();

	//	//g_render->SetRenderContext(ctx);

	//	//// submit entity to render
	//	//entity->Render();
	//}

	//void DeferredRenderer::drawLight(Camera* camera, Light** lights, size_t size)
	//{
	//	assert(camera);
	//	assert(lights);

	//	//m_lightPassShader->Bind();

	//	//g_renderDevice->SetTexture2D(0, m_textures[RT_POS]);
	//	//m_lightPassShader->SetInteger("u_positionTexture", 0);

	//	//g_renderDevice->SetTexture2D(1, m_textures[RT_NORMAL]);
	//	//m_lightPassShader->SetInteger("u_normalTexture", 1);

	//	//g_renderDevice->SetTexture2D(2, m_textures[RT_COLOR]);
	//	//m_lightPassShader->SetInteger("u_colorTexture", 2);

	//	//m_lightPassShader->SetInteger("u_lightsCount", size);

	//	//for (int i = 0; i < size; i++)
	//	//{
	//	//	char uniformName[64];
	//	//	sprintf(uniformName, "u_lightsPos[%i]", i);

	//	//	m_lightPassShader->SetVec3(uniformName, lights[i]->m_Position);
	//	//}

	//	//m_lightPassShader->SetVec3("u_lightPos", lights[0]->m_Position);
	//	//m_lightPassShader->SetVec3("u_viewPos", camera->m_Position);

	//	//ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
	//}

	//void DeferredRenderer::drawNoLight(Camera* camera)
	//{
	//	assert(camera);

	//	//m_lightPassShader->Bind();

	//	//g_renderDevice->SetTexture2D(0, m_textures[RT_POS]);
	//	//m_lightPassShader->SetInteger("u_positionTexture", 0);

	//	//g_renderDevice->SetTexture2D(1, m_textures[RT_NORMAL]);
	//	//m_lightPassShader->SetInteger("u_normalTexture", 1);

	//	//g_renderDevice->SetTexture2D(2, m_textures[RT_COLOR]);
	//	//m_lightPassShader->SetInteger("u_colorTexture", 2);

	//	//m_lightPassShader->SetVec3("u_viewPos", camera->m_Position);

	//	//ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
	//}

	void DeferredRenderer::drawLights()
	{
		GraphicsDevice* graphicsDevice = GraphicsDevice::getInstance();

		m_lightPassShader->bind();

		graphicsDevice->setTexture2D(6, m_textures[RT_POS]);
		m_lightPassShader->setInteger("u_gbuf_positionTexture", 6);

		graphicsDevice->setTexture2D(7, m_textures[RT_NORMAL]);
		m_lightPassShader->setInteger("u_gbuf_normalTexture", 7);

		graphicsDevice->setTexture2D(8, m_textures[RT_COLOR]);
		m_lightPassShader->setInteger("u_gbuf_colorTexture", 8);

		Camera* camera = CameraProxy::getInstance();
		m_lightPassShader->setVec3("u_viewPos", camera->getPosition());

		ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);

	}

	void DeferredRenderer::lightPhase(std::vector<LightComponent*>& lights)
	{
		GraphicsDevice* graphicsDevice = GraphicsDevice::getInstance();

		m_lightPassShader->bind();

		graphicsDevice->setTexture2D(6, m_textures[RT_POS]);
		m_lightPassShader->setInteger("u_gbuf_positionTexture", 6);

		graphicsDevice->setTexture2D(7, m_textures[RT_NORMAL]);
		m_lightPassShader->setInteger("u_gbuf_normalTexture", 7);

		graphicsDevice->setTexture2D(8, m_textures[RT_COLOR]);
		m_lightPassShader->setInteger("u_gbuf_colorTexture", 8);

		Camera* camera = CameraProxy::getInstance();
		m_lightPassShader->setVec3("u_viewPos", camera->getPosition());

		m_lightPassShader->setInteger("u_lightsCount", lights.size());

		char uniformName[64];

		for (int i = 0; i < lights.size(); i++)
		{
			LightRenderData lightData;
			lights[i]->getRenderData(&lightData);

			snprintf(uniformName, 64, "u_lights[%i].pos", i);
			m_lightPassShader->setVec3(uniformName, lightData.m_pos);

			snprintf(uniformName, 64, "u_lights[%i].dir", i);
			m_lightPassShader->setVec3(uniformName, lightData.m_dir);

			snprintf(uniformName, 64, "u_lights[%i].color", i);
			m_lightPassShader->setVec3(uniformName, lightData.m_color);

			snprintf(uniformName, 64, "u_lights[%i].ambient", i);
			m_lightPassShader->setVec3(uniformName, lightData.m_ambientColor);

			snprintf(uniformName, 64, "u_lights[%i].specular", i);
			m_lightPassShader->setVec3(uniformName, lightData.m_specularColor);

			snprintf(uniformName, 64, "u_lights[%i].shininess", i);
			m_lightPassShader->setFloat(uniformName, lightData.m_shininess);

			snprintf(uniformName, 64, "u_lights[%i].constant", i);
			m_lightPassShader->setFloat(uniformName, 1.0f);

			snprintf(uniformName, 64, "u_lights[%i].linear", i);
			m_lightPassShader->setFloat(uniformName, 0.09f);

			snprintf(uniformName, 64, "u_lights[%i].quadratic", i);
			m_lightPassShader->setFloat(uniformName, 0.092f);
		}

		ScreenQuad::renderWithoutTextureBinding(m_lightPassShader);
	}
}

