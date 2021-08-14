#include "pch.h"
#include "file/filememorymappingstream.h"

namespace engine
{
	FileMemoryMappingStream::FileMemoryMappingStream(const std::string& filename)
	{
		m_hFileHandle = CreateFileA(filename.c_str(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (m_hFileHandle == INVALID_HANDLE_VALUE)
			Core::error("FileMemoryMappingStream::FileMemoryMappingStream: INVALID HANDLE VALUE !!!");

		DWORD dwFileSize = GetFileSize(m_hFileHandle, nullptr);
		if (dwFileSize == INVALID_FILE_SIZE)
		{
			CloseHandle(m_hFileHandle);
			Core::error("FileMemoryMappingStream::FileMemoryMappingStream: INVALID FILE SIZE !!!");
		}

		m_hFileMapping = CreateFileMapping(m_hFileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (m_hFileMapping == nullptr)
		{
			CloseHandle(m_hFileHandle);
			Core::error("FileMemoryMappingStream::FileMemoryMappingStream: CreateFileMapping FAILED !!!");
		}

		m_data = (char*)MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, dwFileSize);
		if (m_data == nullptr)
		{
			CloseHandle(m_hFileMapping);
			CloseHandle(m_hFileHandle);
			Core::error("FileMemoryMappingStream::FileMemoryMappingStream: MapViewOfFile FAILED !!!");
		}

		m_size = (size_t)dwFileSize;
		m_pos = 0;
	}

	FileMemoryMappingStream::~FileMemoryMappingStream()
	{
		UnmapViewOfFile(m_data);
		CloseHandle(m_hFileMapping);
		CloseHandle(m_hFileHandle);
	}
}