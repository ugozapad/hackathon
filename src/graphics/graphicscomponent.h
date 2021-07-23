#ifndef VISUALCOMPONENT_H
#define VISUALCOMPONENT_H

#include "engine/components/component.h"

#include "graphics/model.h"

namespace engine
{
	// Component who has any stuff with graphics
	class GraphicsComponent : public Component
	{
		ImplementObject(GraphicsComponent, Component);
	public:
		static void registerObject();

	public:
		GraphicsComponent();
		virtual ~GraphicsComponent();

		virtual void render();

		virtual void addModel(std::shared_ptr<ModelBase>& model);

	private:
		std::vector<std::shared_ptr<ModelBase>> m_models;
	};
}

#endif // !VISUALCOMPONENT_H
