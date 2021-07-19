#include "pch.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"
#include "graphics/deferredrenderer.h"
#include "graphics/gfxcommon.h"
#include "graphics/view.h"
#include "graphics/graphicsdevice.h"

// render stuff
#include "graphics/postprocessing.h"

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
		ScreenQuad::init();

		g_deferredRenderer.init();

		PostProcessingRenderer::getInstance()->init(view);
	}

	void Renderer::shutdown()
	{
		PostProcessingRenderer::getInstance()->shutdown();

		g_deferredRenderer.shutdown();

		ScreenQuad::shutdown();
	}

	void Renderer::renderView(View* view)
	{
		// set viewport
		Viewport vp = {0};
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;

		GraphicsDevice::getInstance()->setViewport(&vp);
	}
}