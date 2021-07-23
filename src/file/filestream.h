#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "file/datastream.h"
#include "file/filedevice.h"

namespace engine
{
	class FileStream : public DataStream
	{
	public:
		FileStream(const std::string& filename, const char* mode);
		~FileStream();

		void read(void* buffer, size_t count);
		void write(void* buffer, size_t count);

		void seek(FileSeek dir, long offset);
		long tell();

		bool eof();

		void flush();

	private:
		File* m_fileHandle;
	};
}

#endif // FILESTREAM_H
