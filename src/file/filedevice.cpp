#include "pch.h"
#include "file/filedevice.h"

namespace engine
{

	FileDevice* FileDevice::instance()
	{
		static FileDevice s_fileDevice;
		return &s_fileDevice;
	}

	void FileDevice::setDefaultPath(const eastl::string& path)
	{
		m_defaultPath = path;
	}

	File* FileDevice::openFile(const eastl::string& path, FileAccess access)
	{
		eastl::string pathBuffer = m_defaultPath;
		pathBuffer += path;

		return mem_new<File>(*g_sysAllocator, pathBuffer, access);
	}

	void FileDevice::closeFile(File*& file)
	{
		mem_delete(*g_sysAllocator, file);
	}

}