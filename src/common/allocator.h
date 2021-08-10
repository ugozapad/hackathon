#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <new>
#include "core/debug.h"

#include <stdint.h>

namespace engine
{
	class AllocatorBase
	{
	public:
		AllocatorBase(size_t size, void* start)
		{
			m_start = start;
			m_size = size;
			m_usedMemory = 0;
			m_numAllocations = 0;
		}

		virtual ~AllocatorBase()
		{
			ASSERT(m_numAllocations == 0 && m_usedMemory == 0);
			m_start = nullptr; m_size                       = 0;
		}

		virtual void* allocate(size_t size, size_t align) = 0;
		virtual void  free(void* ptr)                     = 0;

		void*         getStart()          const { return m_start; }
		size_t        getSize()           const { return m_size; }
		size_t        getUsedMemory()     const { return m_usedMemory; }
		size_t        getNumAllocations() const { return m_numAllocations; }

	protected:
		void*  m_start;
		size_t m_size;
		size_t m_usedMemory;
		size_t m_numAllocations;
	};

	template <typename T, typename TAllocator, typename... Args>
	inline T* mem_new(TAllocator& allocator, Args... args)
	{
		static_assert(std::is_base_of<AllocatorBase, TAllocator>::value, "Your allocator class is not derived from AllocatorBase");
		return new(allocator.allocate(sizeof(T), __alignof(T))) T(args...);
	}

	template <typename TAllocator, typename T>
	inline void mem_delete(TAllocator& allocator, T*& ptr)
	{
		ptr->~T();
		allocator.free(ptr);
		ptr = nullptr;
	}

	template <typename TAllocator, typename T>
	T* allocateArray(TAllocator& allocator, size_t length)
	{
		assert(length != 0);
		u8 headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0) headerSize += 1;

		//Allocate extra space to store array length in the bytes before the array 
		T* p = ((T*)allocator.allocate(sizeof(T) * (length + headerSize), __alignof(T))) + headerSize;
		*(((size_t*)p) - 1) = length;

		for (size_t i = 0; i < length; i++)
			new (&p) T;

		return p;
	}

	template <typename TAllocator, typename T>
	void deallocateArray(TAllocator& allocator, T* array)
	{
		ASSERT(array != nullptr);
		size_t length = *(((size_t*)array) - 1);

		for (size_t i = 0; i < length; i++) array.~T();

		//Calculate how much extra memory was allocated to store the length before the array 
		u8 headerSize = sizeof(size_t) / sizeof(T);
		if (sizeof(size_t) % sizeof(T) > 0)
			headerSize += 1;
		allocator.deallocate(array - headerSize);
	}

	// HELP
	inline void* alignForward(void* address, uint8_t alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	inline uint8_t alignForwardAdjustment(const void* address, uint8_t alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

		if (adjustment == alignment)
			return 0; //already aligned

		return adjustment;
	}

	inline uint8_t alignForwardAdjustmentWithHeader(const void* address, uint8_t alignment, uint8_t headerSize)
	{
		uint8_t adjustment = alignForwardAdjustment(address, alignment);
		uint8_t neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header 
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0) adjustment += alignment;
		}

		return adjustment;
	}

	namespace pointer_help
	{
		inline void* add(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uintptr_t>(p) + x);
		}

		inline const void* add(const void* p, size_t x)
		{
			return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
		}

		inline void* subtract(void* p, size_t x)
		{
			return (void*)(reinterpret_cast<uintptr_t>(p) - x);
		}

		inline const void* subtract(const void* p, size_t x)
		{
			return (const void*)(reinterpret_cast<uintptr_t>(p) - x);
		}
	}

}

#endif // !ALLOCATOR_H
