#ifndef STATICMESHCOMPONENT_H
#define STATICMESHCOMPONENT_H

#include "graphics/drawablecomponent.h"
#include "graphics/model.h"

namespace engine
{
	// Component who has static model
	class StaticMeshComponent : public DrawableComponent
	{
		ImplementObject(StaticMeshComponent, DrawableComponent);
	public:
		static void registerObject();

	public:
		StaticMeshComponent();
		virtual ~StaticMeshComponent();
			
		virtual void render();

		virtual void addModel(std::shared_ptr<ModelBase>& model);

	private:
		std::vector<std::shared_ptr<ModelBase>> m_models;
	};
}

#endif // !VISUALCOMPONENT_H
