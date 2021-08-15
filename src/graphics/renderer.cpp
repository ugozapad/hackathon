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

#include <future>

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

		m_graphicsDevice = GraphicsDevice::getInstance();

		ShaderManager::getInstance()->init();

		ScreenQuad::init();

		g_deferredRenderer.init(view);

		m_postProcessingRenderer = PostProcessingRenderer::getInstance();
		m_postProcessingRenderer->init(view);

		//Im3d_Init();

		//ShadowsRenderer::getInstance()->init();
	}

	void Renderer::shutdown()
	{
		//Im3d_Shutdown();

		//ShadowsRenderer::getInstance()->shutdown();

		m_postProcessingRenderer->shutdown();
		m_postProcessingRenderer = nullptr;

		g_deferredRenderer.shutdown();

		ScreenQuad::shutdown();

		ShaderManager::getInstance()->shutdown();

		m_graphicsDevice = nullptr;
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
		Viewport vp;
		memset(&vp, 0, sizeof(vp));
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;
		m_graphicsDevice->setViewport(&vp);

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

		m_graphicsDevice->setFramebuffer(deferredFramebuffer);

		m_graphicsDevice->clearColor(0.0, 0.0, 0.0, 1.0);
		m_graphicsDevice->clear(ClearRenderTarget | ClearDepth);

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

		m_graphicsDevice->setFramebuffer(m_postProcessingRenderer->getFramebuffer());

		// let's sort the fucking lights
		std::vector<LightComponent*> lights = LightManager::getInstance()->getLights();
		g_deferredRenderer.lightPhase(lights);

		m_graphicsDevice->setFramebuffer(0);

		m_postProcessingRenderer->gammaCorrection();
		//ScreenQuad::render(g_deferredRenderer.getTexture(DeferredRenderer::RT_COLOR));
	}

	void writeImageAsync(const std::string& filename, Image& image)
	{
		image.save(filename);
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

		auto h = std::async(std::launch::async, &writeImageAsync, buffer, image);
		h.wait();

		FileDevice::instance()->setDefaultPath(defpath);

		delete[] screenBuffer;
	}

	void Renderer::endFrame()
	{
		resetDeviceState();

		// swap buffers
		m_graphicsDevice->swapBuffers();
	}

	void Renderer::resetDeviceState()
	{
		// #TODO: !!!
		
		// enable depth test and disable depth mask
		//GraphicsDevice::instance()->depthMask(false);
		//GraphicsDevice::instance()->depthTest(true);

		// unbind all texture slots
		for (int i = 0; i < 12; i++)
			m_graphicsDevice->setTexture2D(i, nullptr);

		resetTextureBindingsAfterModelRender();

		// unbind vb and ib
		m_graphicsDevice->setVertexBuffer(nullptr);
		m_graphicsDevice->setIndexBuffer(nullptr);

		// unbind shader : CRASH !! No active program.
		//glUseProgram(0);

		// unbind framebuffer
		m_graphicsDevice->setFramebuffer(nullptr);
	}

	void Renderer::resetTextureBindingsAfterModelRender()
	{
		for (int i = 0; i < 6; i++)
			m_graphicsDevice->setTexture2D(i, nullptr);
	}

}