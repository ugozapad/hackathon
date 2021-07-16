#include "pch.h"
#include <EASTL/allocator.h>

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	//return engine::g_sysAllocator->allocate(size, 8);
	return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	//return engine::g_sysAllocator->allocate(size, alignment);
	return malloc(size);
}