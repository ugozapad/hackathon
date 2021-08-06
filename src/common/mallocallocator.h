#ifndef MALLOCALLOCATOR_H
#define MALLOCALLOCATOR_H

#include "common/allocator.h"

namespace engine
{
	class MallocAllocator : public AllocatorBase
	{
	public:
		MallocAllocator();

		void* allocate(size_t size, size_t align) override;
		void free(void* ptr) override;
	};
}

#endif // !MALLOCALLOCATOR_H
