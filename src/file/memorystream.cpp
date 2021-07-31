#include "pch.h"
#include "file/memorystream.h"

namespace engine
{
	MemoryStream::MemoryStream(void* data, size_t size)
	{
		m_data = (char*)data;
		m_size = size;
		m_pos = 0;
	}

	MemoryStream::~MemoryStream()
	{
	}

	void MemoryStream::read(void* buffer, size_t count)
	{
		assert(m_pos + count <= m_size);
		memcpy(buffer, get_pointer(), count);
		m_pos += count;
		assert((m_pos <= m_size) && (m_pos >= 0));
	}

	void MemoryStream::write(void* buffer, size_t count)
	{
		throw std::logic_error("MemoryStream::write: memory stream doesnt support any writing!");
	}

	void MemoryStream::seek(FileSeek dir, long offset)
	{
		switch (dir)
		{
		case FileSeek::Begin: 
			m_pos = 0;
			m_pos += offset;
			break;
		case FileSeek::Current:
			m_pos += offset;
			break;
		case FileSeek::End:
			m_pos = m_size;
			m_pos += offset;
			break;
		}
		assert((m_pos <= m_size) && (m_pos >= 0));
	}

	long MemoryStream::tell()
	{
		return m_pos;
	}

	bool MemoryStream::eof()
	{
		return elapsed() <= 0;
	}

	void MemoryStream::flush()
	{
		throw std::logic_error("MemoryStream::flush: memory stream doesnt support flushing!");
	}

	int MemoryStream::elapsed() const
	{
		return m_size - m_pos;
	}

	void* MemoryStream::get_pointer()
	{
		return &(m_data[m_pos]);;
	}
}