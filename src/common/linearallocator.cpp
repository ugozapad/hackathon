#include "pch.h"
#include "common/linearallocator.h"

namespace engine
{
	LinearAllocator::LinearAllocator(size_t size, void* start) : AllocatorBase(size, start), m_currentPos(start) { assert(size > 0); }

	LinearAllocator::~LinearAllocator() { m_currentPos = nullptr; }

	void* LinearAllocator::allocate(size_t size, size_t alignment)
	{
		ASSERT(size != 0);
		uint8_t adjustment = alignForwardAdjustment(m_currentPos, alignment);

		if (m_usedMemory + adjustment + size > m_size) return nullptr;

		uintptr_t aligned_address = (uintptr_t)m_currentPos + adjustment;
		m_currentPos = (void*)(aligned_address + size);
		m_usedMemory += size + adjustment;
		m_numAllocations++;

		return (void*)aligned_address;
	}

	void LinearAllocator::free(void* p)
	{
		assert(false && "Use clear() instead");
	}

	void LinearAllocator::clear()
	{
		m_numAllocations = 0;
		m_usedMemory = 0;
		m_currentPos = m_start;
	}
}