#ifndef PHYSICSALLOCATOR_H
#define PHYSICSALLOCATOR_H

namespace engine
{
	class PhysicsAllocator
	{
	public:
		static AllocatorBase* ms_physicsAllocator;

	private:
		static void* ms_physicsAllocatorMemory;

	public:
		static void init();
		static void shutdown();
	};
}

#endif // !PHYSICSALLOCATOR_H
