#include "pch.h"
#include "engine/components/component.h"

namespace engine
{
	void Component::registerObject()
	{
		Context::getInstance()->registerObject<Component>();
	}

	Component::Component()
	{
	}

	Component::~Component()
	{
	}

	void Component::setActive(bool active)
	{
		m_isActive.store(active);
	}

	bool Component::isActive()
	{
		return m_isActive.load();
	}

}