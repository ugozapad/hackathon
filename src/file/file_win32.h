#ifndef FILE_WIN32_H
#define FILE_WIN32_H

#include "file/filecommon.h"

namespace engine
{
	class FileWin32
	{
	public:
		FileWin32(const eastl::string& path, FileAccess access);
		~FileWin32();

		bool isValid() { return !!m_filehandle; }

		FILE* getHandle() { return m_filehandle; }

		void seek(FileSeek seekdir, long offset);
		long tell();

		bool eof();

		void flush();

		size_t read(void* buffer, size_t size);
		size_t write(void* buffer, size_t size);

		// helpers
		void readStringBuffer(char* buffer, size_t bufferSize);
		void readString(eastl::string& buffer);

	private:
		FILE* m_filehandle;
	};
}

#endif // !FILE_WIN32_H
