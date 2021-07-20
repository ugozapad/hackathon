#include "pch.h"
#include "engine/components/logiccomponent.h"

namespace engine
{
	void LogicComponent::registerCompoent()
	{
		Context::getInstance()->registerObject<LogicComponent>();
	}

	LogicComponent::LogicComponent()
	{

	}

	LogicComponent::~LogicComponent()
	{

	}

	void LogicComponent::update(float delta)
	{

	}

}