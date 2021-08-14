#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "engine/components/logiccomponent.h"

#include "physics/physicsbody.h"

namespace engine
{
	class PhysicsComponent : public LogicComponent
	{
		ImplementObject(PhysicsComponent, LogicComponent);
	public:
		PhysicsComponent();
		virtual ~PhysicsComponent();

		static void registerObject();

		void createShape(PhysicsBody::ShapeType shapeType, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), btScalar mass = 0.0f, btVector3 size = btVector3(1.0f, 1.0f, 1.0f), btScalar radius = 1.0f);

		virtual void onNodeSet(Node* node);

		PhysicsBody* getPhysicsBody() { return m_physicsBody; }

		void setStatic(bool value);

		bool getStatic() { return m_isStatic; }


		void update(float delta) override;

	private:
		PhysicsBody* m_physicsBody;
		bool m_isStatic;
	};
}

#endif // !PHYSICSCOMPONENT_H
