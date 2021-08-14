#ifndef WORLD_H
#define WORLD_H

#include "engine/node.h"
#include "physics/physicsworld.h"

namespace engine
{

// Entity container, world and other stuff.
class World : public Object
{
	ImplementObject(World, Object);
public:
	static void registerObject();

public:
	World();
	~World();

	void release();

	void updatePhysicsWorld();
	void updateLogicWorld();
	void updateGraphicWorld();

	std::shared_ptr<Node> createNode();
	Node* createNode_NoSharedPtr();

	std::list<std::shared_ptr<Node>>& getNodeList() { return m_nodes; }

	PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

private:
	typedef std::list<std::shared_ptr<Node>>::iterator NodeIt;
	std::list<std::shared_ptr<Node>> m_nodes;

	PhysicsWorld* m_physicsWorld;
};

} // namespace sword

#endif // !WORLD_H
