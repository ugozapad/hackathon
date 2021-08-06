#include "pch.h"
#include "common/parse.h"

// q_parse.c -- support for parsing text files
//#include "q_shared.h"

namespace engine
{
	void Q_strncpyz(char* dest, const char* src, int destsize) {
		if (!src) {
			Core::error("Q_strncpyz: NULL src");
		}
		if (destsize < 1) {
			Core::error("Q_strncpyz: destsize < 1");
		}

		strncpy(dest, src, destsize - 1);
		dest[destsize - 1] = 0;
	}

	void Q_strcat(char* dest, int size, const char* src) {
		int		l1;

		l1 = strlen(dest);
		if (l1 >= size) {
			Core::error("Q_strcat: already overflowed");
		}

		Q_strncpyz(dest + l1, src, size - l1);
	}


}