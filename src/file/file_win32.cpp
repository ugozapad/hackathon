#include "pch.h"
#include "file/file_win32.h"

namespace engine
{

	static const char* s_stdioOpeningMode[] = { "rb", "wb" };
	static int s_stdioSeekDir[] = { SEEK_SET, SEEK_CUR, SEEK_END };

	FileWin32::FileWin32(const eastl::string& path, FileAccess access)
	{
		m_filehandle = fopen(path.c_str(), s_stdioOpeningMode[(int)access]);
	}

	FileWin32::~FileWin32()
	{
		if (m_filehandle)
		{
			fclose(m_filehandle);
			m_filehandle = nullptr;
		}
	}

	void FileWin32::seek(FileSeek seekdir, long offset)
	{
		fseek(m_filehandle, offset, s_stdioSeekDir[(int)seekdir]);
	}

	long FileWin32::tell()
	{
		return ftell(m_filehandle);
	}

	size_t FileWin32::read(void* buffer, size_t size)
	{
		return fread(buffer, size, 1, m_filehandle);
	}

	size_t FileWin32::write(void* buffer, size_t size)
	{
		return fwrite(buffer, size, 1, m_filehandle);
	}

	void FileWin32::readStringBuffer(char* buffer, size_t bufferSize)
	{
		seek(FileSeek::End, 0);
		size_t length = tell();
		seek(FileSeek::Begin, 0);

		assert(length <= bufferSize);

		read(buffer, length);
		buffer[length] = '\0';
	}

}