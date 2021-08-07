#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

namespace engine
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		static PhysicsManager ms_instance;

	public:
		void init();
		void shutdown();
	};
}

#endif // !PHYSICSMANAGER_H
