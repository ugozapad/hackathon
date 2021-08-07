#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "engine/components/component.h"

#include "physics/physicsbody.h"

namespace engine
{
	class PhysicsComponent : public Component
	{
		ImplementObject(PhysicsComponent, Component);
	public:
		PhysicsComponent();
		virtual ~PhysicsComponent();

		static void registerObject();

		void createShape(PhysicsBody::ShapeType shapeType, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

	private:
		PhysicsBody* m_physicsBody;
	};
}

#endif // !PHYSICSCOMPONENT_H
