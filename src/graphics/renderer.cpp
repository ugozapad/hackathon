#include "pch.h"
#include "graphics/gl/glad/glad.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"
#include "graphics/deferredrenderer.h"
#include "graphics/gfxcommon.h"
#include "graphics/view.h"
#include "graphics/graphicsdevice.h"

// render stuff
#include "graphics/postprocessing.h"
#include "graphics/shaderconstantmanager.h"

#include "graphics/image.h"

#include "file/filedevice.h"

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
		Viewport vp = { 0 };
		vp.m_width = view->m_width;
		vp.m_height = view->m_height;

		GraphicsDevice::getInstance()->setViewport(&vp);
	}

	bool fileExist(const eastl::string& filename)
	{
		File* file = FileDevice::getInstance()->openFile(filename, FileAccess::Read);
		bool valid = file->isValid();
		FileDevice::getInstance()->closeFile(file);

		return valid;
	}

	void Renderer::makeScreenshot()
	{
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		int width = m_viewport[2];
		int height = m_viewport[3];

		size_t bufferSize = 3 * width * height;
		uint8_t* screenBuffer = new uint8_t[bufferSize];

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

		Image image;
		image.createRaw((void*)screenBuffer, width, height, 3);
		image.setFlip(true);

		char buffer[256];
		for (int i = 0;; i++)
		{
			sprintf(buffer, "sshot_%i.png", i);
			if (!fileExist(buffer))
				break;
		}

		image.save(buffer);

		delete[] screenBuffer;
	}

}