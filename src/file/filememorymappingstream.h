#ifndef FILEMEMORYMAPPINGSTREAM_H
#define FILEMEMORYMAPPINGSTREAM_H

#include "file/memorystream.h"

namespace engine
{
	class FileMemoryMappingStream : public MemoryStream
	{
	public:
		FileMemoryMappingStream(const std::string& filename);
		~FileMemoryMappingStream();

	private:
		HANDLE m_hFileHandle;
		HANDLE m_hFileMapping;
	};
}

#endif // !FILEMEMORYMAPPINGSTREAM_H
