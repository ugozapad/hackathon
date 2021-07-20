#include "pch.h"
#include "graphics/graphicscomponent.h"

namespace engine
{
	void GraphicsComponent::registerObject()
	{
		Context::getInstance()->registerObject<GraphicsComponent>();
	}

	GraphicsComponent::GraphicsComponent()
	{

	}

	GraphicsComponent::~GraphicsComponent()
	{

	}

	void GraphicsComponent::render()
	{
		for (int i = 0; i < m_models.size(); i++)
		{
			m_models[i]->renderObjects();
		}
	}

	void GraphicsComponent::addModel(eastl::shared_ptr<ModelBase>& model)
	{
		m_models.push_back(model);
	}

}