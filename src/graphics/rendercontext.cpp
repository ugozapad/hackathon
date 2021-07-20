#include "pch.h"
#include "graphics/rendercontext.h"

namespace engine
{
	RenderContext RenderContext::ms_instance;

	RenderContext& RenderContext::getContext()
	{
		return ms_instance;
	}

	void RenderContext::setContext(RenderContext& ctx)
	{
		ms_instance = ctx;
	}
}