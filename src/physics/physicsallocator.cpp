#include "pch.h"
#include "physics/physicsallocator.h"

#include "common/freelistallocator.h"

namespace engine
{
	AllocatorBase* PhysicsAllocator::ms_physicsAllocator;
	void* PhysicsAllocator::ms_physicsAllocatorMemory;

	void PhysicsAllocator::init()
	{
		size_t physicsMemorySize = 1024 * 12;
		ms_physicsAllocatorMemory = malloc(physicsMemorySize);
		ms_physicsAllocator = new FreeListAllocator(physicsMemorySize, ms_physicsAllocatorMemory);
	}

	void PhysicsAllocator::shutdown()
	{
		delete ms_physicsAllocator;
		free(ms_physicsAllocatorMemory);
	}
}