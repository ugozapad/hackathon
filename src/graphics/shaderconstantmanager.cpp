#include "pch.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/shader.h"
#include "graphics/rendercontext.h"
#include "core/timer.h"

namespace engine
{
	ShaderConstantManager ShaderConstantManager::ms_instance;

	void ShaderConstantManager::applyDefaultContants(Shader* shader)
	{
		// Render constants
		RenderContext& ctx = RenderContext::getContext();
		shader->setMatrix("u_model", ctx.model);
		shader->setMatrix("u_view", ctx.view);
		shader->setMatrix("u_proj", ctx.proj);

		glm::mat4 mvp = glm::mat4(1.0f);
		mvp = ctx.proj * ctx.view * ctx.model;

		shader->setMatrix("u_mvp", mvp);
		shader->setMatrix("u_modelViewProjection", mvp);

		// Engine constants
		shader->setFloat("u_time", Timer::getInstance()->getDelta());
	}

}