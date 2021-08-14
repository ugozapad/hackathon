#include "pch.h"
#include "graphics/staticmeshcomponent.h"

#include "graphics/renderer.h"

namespace engine
{
	void StaticMeshComponent::registerObject()
	{
		Context::getInstance()->registerObject<StaticMeshComponent>();
	}

	StaticMeshComponent::StaticMeshComponent()
	{

	}

	StaticMeshComponent::~StaticMeshComponent()
	{

	}

	void StaticMeshComponent::render()
	{
		for (int i = 0; i < m_models.size(); i++)
		{
			m_models[i]->renderObjects();
		}
	}

	void StaticMeshComponent::addModel(std::shared_ptr<ModelBase>& model)
	{
		m_models.push_back(model);
	}

}