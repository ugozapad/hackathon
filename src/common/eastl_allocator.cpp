#include "pch.h"
#include <EASTL/allocator.h>

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return engine::g_sysAllocator->allocate(size, 8);
}