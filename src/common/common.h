#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>

#include <assert.h>

#ifdef _MSC_VER
#	define Strdup _strdup
#else
#	define Strdup strdup
#endif

#include "common/sys_allocator.h"
#include "common/singleton.h"

#endif // !COMMON_H
