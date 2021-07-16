#include "pch.h"
#include "file/filedevice.h"

namespace engine
{
	FileDevice FileDevice::ms_instance;

	void FileDevice::setDefaultPath(const eastl::string& path)
	{
		m_defaultPath = path;
	}

	File* FileDevice::openFile(const eastl::string& path, FileAccess access)
	{
		spdlog::info("opening file {}", path.c_str());

		eastl::string pathBuffer = m_defaultPath;
		pathBuffer += path;
		return mem_new<File>(*g_sysAllocator, pathBuffer, access);
	}

	void FileDevice::closeFile(File*& file)
	{
		mem_delete(*g_sysAllocator, file);
	}

}