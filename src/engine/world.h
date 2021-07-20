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

private:
	typedef eastl::list<Node*>::iterator NodeIt;
	eastl::list<Node*> m_nodes;
};

} // namespace sword

#endif // !WORLD_H
