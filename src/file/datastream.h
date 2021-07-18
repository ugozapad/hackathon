#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "file/filecommon.h"

namespace engine
{
	class DataStream
	{
	public:
		DataStream();
		virtual ~DataStream();

		virtual void read(void* buffer, size_t count);
		virtual void write(void* buffer, size_t count);

		virtual void seek(FileSeek dir, long offset);
		virtual long tell();

		virtual bool eof();

		virtual void flush();

		template <typename T>
		void read(T* buffer) { return read((void*)buffer, sizeof(T)); }

		template <typename T>
		void write(T* buffer) { return write((void*)buffer, sizeof(T)); }
	};

	typedef eastl::shared_ptr<DataStream> DataStreamPtr;
}

#endif // DATASTREAM_H
