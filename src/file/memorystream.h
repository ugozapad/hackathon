#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include "file/datastream.h"

namespace engine
{
	// little hack for override deletion function
	typedef void (*freeData_t) (void*);

	class MemoryStream : public DataStream
	{
	public:
		// Little hack
		MemoryStream();

		MemoryStream(void* data, size_t size);
		MemoryStream(void* data, size_t size, freeData_t freeData);
		~MemoryStream();

		virtual void read(void* buffer, size_t count);
		virtual void write(void* buffer, size_t count);

		virtual void seek(FileSeek dir, long offset);
		virtual long tell();

		virtual bool eof();

		virtual void flush();
		
	private:
		int elapsed() const;

		void* get_pointer();

	protected:
		freeData_t m_freeData;
		char* m_data;
		size_t m_size;
		size_t m_pos;
	};
}

#endif