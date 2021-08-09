#include "pch.h"
#include "engine/camera.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/shader.h"
#include "graphics/rendercontext.h"
#include "core/timer.h"

namespace engine
{
	ShaderConstantManager ShaderConstantManager::ms_instance;

	void ShaderConstantManager::setDefaultContants(Shader* shader)
	{
		setEngineConstants(shader);
		setGraphicsConstants(shader);
	}

	void ShaderConstantManager::setGraphicsConstants(Shader* shader)
	{
		RenderContext& ctx = RenderContext::getContext();
		shader->setMatrix("u_model", ctx.model);
		shader->setMatrix("u_view", ctx.view);
		shader->setMatrix("u_proj", ctx.proj);

		glm::mat4 mvp = glm::mat4(1.0f);
		mvp = ctx.proj * ctx.view * ctx.model;

		shader->setMatrix("u_mvp", mvp);
		shader->setMatrix("u_modelViewProjection", mvp);
	}

	void ShaderConstantManager::setEngineConstants(Shader* shader)
	{
		shader->setFloat("u_time", Timer::getInstance()->getDelta());
		shader->setVec3("u_viewPos", CameraProxy::getInstance()->getPosition());
	}

}