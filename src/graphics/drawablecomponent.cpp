#include "pch.h"
#include "graphics/drawablecomponent.h"

namespace engine
{
	DrawableComponent::DrawableComponent()
	{

	}

	DrawableComponent::~DrawableComponent()
	{

	}

	void DrawableComponent::registerObject()
	{
		Context::getInstance()->registerObject<DrawableComponent>();
	}
}