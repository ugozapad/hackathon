#ifndef COMPONENT_H
#define COMPONENT_H

namespace engine
{

// Base class for node component
class Component : public Object
{
	ImplementObject(Component, Object);
public:
	static void registerObject();

public:
	Component();
	virtual ~Component();

	void setActive(bool active);
	bool isActive();

private:
	eastl::atomic<bool> m_isActive;
};

}

#endif // !COMPONENT_H
