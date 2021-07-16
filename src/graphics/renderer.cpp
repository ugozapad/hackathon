#include "pch.h"
#include "graphics/renderer.h"
#include "graphics/screenquad.h"

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

	void Renderer::init()
	{
		ScreenQuad::init();
	}

	void Renderer::shutdown()
	{
		ScreenQuad::shutdown();
	}
}