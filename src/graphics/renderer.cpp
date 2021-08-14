#include "pch.h"
#include "graphics/gl/glad/glad.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"
#include "graphics/deferredrenderer.h"
#include "graphics/gfxcommon.h"
#include "graphics/view.h"
#include "graphics/graphicsdevice.h"
#include "graphics/rendercontext.h"

// render stuff
#include "graphics/shadermanager.h"
#include "graphics/postprocessing.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/shadowsrenderer.h"

#include "graphics/image.h"

#include "engine/camera.h"
#include "engine/engine.h"
#include "engine/world.h"

#include "graphics/staticmeshcomponent.h"

#include "game/skyboxcomponent.h"

#include "file/filedevice.h"

#include "graphics/lightmanager.h"
#include "graphics/light.h"

extern bool Im3d_Init();
extern void Im3d_Shutdown();
extern void Im3d_NewFrame();
extern void Im3d_EndFrame();

namespace engine
{
	Renderer* Renderer::ms_instance;

	void Renderer::createInstance()
	{
		if (!ms_instance)
			ms_instance = mem_new<Renderer>(*g_sysAllocator);
	}

	void Renderer::destroyInstance()
	{
		if (ms_instance)
			mem_delete(*g_sysAllocator, ms_instance);
	}

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::init(View* view)
	{
		m_view = view;

		ShaderManager::getInstance()->init();

		ScreenQuad::init();

		g_deferredRenderer.init(view);

		PostProcessingRenderer::getInstance()->init(view);

		//Im3d_Init();

		//ShadowsRenderer::getInstance()->init();
	}

	void Renderer::shutdown()
	{
		//Im3d_Shutdown();

		//ShadowsRenderer::getInstance()->shutdown();

		PostProcessingRenderer::getInstance()->shutdown();

		g_deferredRenderer.shutdown();

		ScreenQuad::shutdown();

		ShaderManager::getInstance()->shutdown();
	}

	void Renderer::beginFrame()
	{
		GraphicsDevice::getInstance()->clear(/*ClearRenderTarget |*/ ClearDepth);
	}

	void Renderer::renderView(View* view)
	{
		// make context current
		glfwMakeContextCurrent(GraphicsDevice::getInstance()->getWindow());

		// set viewport
		Viewport vp = { 0 };
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;

		GraphicsDevice::getInstance()->setViewport(&vp);

		// initialize render context
		RenderContext& renderContext = RenderContext::getContext();
		renderContext.width = view->m_width;
		renderContext.height = view->m_height;
		renderContext.proj = view->m_projection;
		renderContext.view = view->m_view;
		renderContext.model = glm::mat4(1.0f);

		RenderContext::setContext(renderContext);

		GrFramebuffer* deferredFramebuffer = g_deferredRenderer.getFramebuffer();
		assert(deferredFramebuffer);

		GraphicsDevice::getInstance()->setFramebuffer(deferredFramebuffer);

		GraphicsDevice::getInstance()->clearColor(0.0, 0.0, 0.0, 1.0);
		GraphicsDevice::getInstance()->clear(ClearRenderTarget | ClearDepth);

		World* world = Engine::ms_world;

		auto skyboxNode = std::find_if(world->getNodeList().begin(), world->getNodeList().end(),
			[](const std::shared_ptr<Node>& node)
			{
				return node->getComponentByType<SkyboxComponent>();
			}
		);

		if (skyboxNode != world->getNodeList().end())
		{
			Node* node = skyboxNode->get();
			if (node->getComponentByType<SkyboxComponent>())
			{
				StaticMeshComponent* graphicsComponent = node->getComponentByType<StaticMeshComponent>();
				if (graphicsComponent)
				{
					// let's render our piece of shit.
					RenderContext& renderCtx = RenderContext::getContext();
					renderCtx.model = node->getTranslation();
					RenderContext::setContext(renderCtx);

					graphicsComponent->render();
				}
			}
		}

		if (world)
		{
			std::list<std::shared_ptr<Node>>& nodes = world->getNodeList();
			typedef std::list<std::shared_ptr<Node>>::iterator NodeIt;
			for (NodeIt it = nodes.begin(); it != nodes.end(); ++it)
			{
				Node* node = (*it).get();
				if (node)
				{
					StaticMeshComponent* graphicsComponent = node->getComponentByType<StaticMeshComponent>();
					if (graphicsComponent && !node->getComponentByType<SkyboxComponent>())
					{
						// let's render our piece of shit.

						RenderContext& renderCtx = RenderContext::getContext();
						renderCtx.model = node->getTranslation();
						RenderContext::setContext(renderCtx);

						graphicsComponent->render();
					}
				}
			}
		}

		// reset device states
		//resetDeviceState();

		GraphicsDevice::getInstance()->setFramebuffer(PostProcessingRenderer::getInstance()->getFramebuffer());

		// let's sort the fucking lights
		std::vector<LightComponent*> lights = LightManager::getInstance()->getLights();
		g_deferredRenderer.lightPhase(lights);

		GraphicsDevice::getInstance()->setFramebuffer(0);

		PostProcessingRenderer::getInstance()->gammaCorrection();
		//ScreenQuad::render(g_deferredRenderer.getTexture(DeferredRenderer::RT_COLOR));
	}

	void Renderer::makeScreenshot()
	{
		int width = m_view->m_width;
		int height = m_view->m_height;

		size_t bufferSize = 3 * width * height;
		uint8_t* screenBuffer = new uint8_t[bufferSize];

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

		Image image;
		image.createRaw((void*)screenBuffer, width, height, 3);
		image.setFlip(true);

		std::string defpath = FileDevice::instance()->getDefaultPath();
		FileDevice::instance()->setDefaultPath("");

		char buffer[256];
		for (int i = 0;; i++)
		{
			sprintf(buffer, "sshot_%i.png", i);
			if (!fileExist(buffer))
				break;
		}

		image.save(buffer);
		FileDevice::instance()->setDefaultPath(defpath);

		delete[] screenBuffer;
	}

	void Renderer::endFrame()
	{
		resetDeviceState();

		// swap buffers
		GraphicsDevice::instance()->swapBuffers();
	}

	void Renderer::resetDeviceState()
	{
		// #TODO: !!!
		
		// enable depth test and disable depth mask
		//GraphicsDevice::instance()->depthMask(false);
		//GraphicsDevice::instance()->depthTest(true);

		// unbind all texture slots
		for (int i = 0; i < 12; i++)
			GraphicsDevice::getInstance()->setTexture2D(i, nullptr);

		// unbind vb and ib
		GraphicsDevice::getInstance()->setVertexBuffer(nullptr);
		GraphicsDevice::getInstance()->setIndexBuffer(nullptr);

		// unbind shader : CRASH !! No active program.
		//glUseProgram(0);

		// unbind framebuffer
		GraphicsDevice::getInstance()->setFramebuffer(nullptr);
	}

	void Renderer::resetTextureBindingsAfterModelRender()
	{
		for (int i = 0; i < 6; i++)
			GraphicsDevice::getInstance()->setTexture2D(i, nullptr);
	}

}