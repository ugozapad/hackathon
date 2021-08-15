#include "pch.h"
#include "common/mallocallocator.h"

#include <malloc.h>

namespace engine
{

	MallocAllocator::MallocAllocator() :
		AllocatorBase(0, 0)
	{

	}

	void* MallocAllocator::allocate(size_t size, size_t align)
	{
		m_usedMemory += size;
		m_numAllocations++;
		return _aligned_malloc(size, align);
	}

	void MallocAllocator::free(void* ptr)
	{
		//m_numAllocations--;
		//m_usedMemory -= block_size;

		return _aligned_free(ptr);
	}

}