#ifndef WORLD_H
#define WORLD_H

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
class World
{
public:
	World();
	~World();

	void updatePhysicWorld();
	void updateGraphicWorld();

private:
	typedef eastl::list<Entity*>::iterator EntityIt;
	eastl::list<Entity*> m_entities;
};

} // namespace sword

#endif // !WORLD_H
