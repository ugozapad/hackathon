#include "pch.h"
#include "file/filestream.h"

namespace engine
{
	FileStream::FileStream(const std::string& filename, const char* mode)
	{
		FileAccess fileAccess;
		if (mode[0] == 'r')
			fileAccess = FileAccess::Read;
		if (mode[0] == 'w')
			fileAccess = FileAccess::Write;

		m_fileHandle = FileDevice::instance()->openFile(filename, fileAccess);
		assert(m_fileHandle->isValid());
	}

	FileStream::~FileStream()
	{
		if (m_fileHandle)
			FileDevice::instance()->closeFile(m_fileHandle);
	}

	void FileStream::read(void* buffer, size_t count)
	{
		m_fileHandle->read(buffer, count);
	}

	void FileStream::write(void* buffer, size_t count)
	{
		m_fileHandle->write(buffer, count);
	}

	void FileStream::seek(FileSeek dir, long offset)
	{
		m_fileHandle->seek(dir, offset);
	}

	long FileStream::tell()
	{
		return m_fileHandle->tell();
	}

	bool FileStream::eof()
	{
		return m_fileHandle->eof();
	}

	void FileStream::flush()
	{
		m_fileHandle->flush();
	}

}
