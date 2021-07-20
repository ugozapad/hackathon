#ifndef WORLD_H
#define WORLD_H

#include "engine/node.h"

namespace engine
{

class Entity;

struct WorldNode
{
	Entity* m_Entity;
	WorldNode* m_Root;
	WorldNode* m_Next;
};

// Entity container, world and other stuff.
class World : public Object
{
	ImplementObject(World, Object);
public:
	static void registerObject();

public:
	World();
	~World();

	void updatePhysicWorld();
	void updateLogicWorld();
	void updateGraphicWorld();

	eastl::shared_ptr<Node> createNodePtr();
	Node* createNode();

private:
	typedef eastl::list<eastl::shared_ptr<Node>>::iterator NodeIt;
	eastl::list<eastl::shared_ptr<Node>> m_nodes;
};

} // namespace sword

#endif // !WORLD_H
