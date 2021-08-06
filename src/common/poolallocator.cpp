#include "pch.h"
#include "common/poolallocator.h"

namespace engine
{
	PoolAllocator::PoolAllocator(size_t objectSize, uint8_t objectAlignment, size_t size, void* mem) : 
		AllocatorBase(size, mem), 
		_objectSize(objectSize), _objectAlignment(objectAlignment)
	{
		ASSERT(objectSize >= sizeof(void*));

		//Calculate adjustment needed to keep object correctly aligned 
		uint8_t adjustment = alignForwardAdjustment(mem, objectAlignment);
		_free_list = (void**)pointer_help::add(mem, adjustment);
		size_t numObjects = (size - adjustment) / objectSize;
		void** p = _free_list;

		//Initialize free blocks list 
		for (size_t i = 0; i < numObjects - 1; i++)
		{
			*p = pointer_help::add(p, objectSize);
			p = (void**)*p;
		}

		*p = nullptr;
	}

	PoolAllocator::~PoolAllocator()
	{
		_free_list = nullptr;
	}

	void* PoolAllocator::allocate(size_t size, size_t alignment)
	{
		ASSERT(size == _objectSize && alignment == _objectAlignment);
		if (_free_list == nullptr) return nullptr;
		void* p = _free_list;
		_free_list = (void**)(*_free_list);
		m_usedMemory += size;
		m_numAllocations++;
		return p;
	}

	void PoolAllocator::free(void* p)
	{
		*((void**)p) = _free_list;
		_free_list = (void**)p;
		m_usedMemory -= _objectSize;
		m_numAllocations--;
	}
}