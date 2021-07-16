#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <new>

namespace engine
{
	class AllocatorBase
	{
	public:
		virtual ~AllocatorBase() {}

		virtual void* allocate(size_t size, size_t align) = 0;
		virtual void  free(void* ptr) = 0;
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
}

#endif // !ALLOCATOR_H
