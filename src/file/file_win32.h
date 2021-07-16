#ifndef FILE_WIN32_H
#define FILE_WIN32_H

#include "file/filecommon.h"

namespace engine
{
	class FileWin32
	{
	public:
		FileWin32(const char* path, FileAccess access);
		~FileWin32();

		FILE* getHandle() { return m_filehandle; }

		void seek(FileSeek seekdir, long offset);
		long tell();

		size_t read(void* buffer, size_t size);
		size_t write(void* buffer, size_t size);

		// helpers
		void readStringBuffer(char* buffer, size_t bufferSize);

	private:
		FILE* m_filehandle;
	};
}

#endif // !FILE_WIN32_H
