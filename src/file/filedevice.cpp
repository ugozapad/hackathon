#include "pch.h"
#include "file/filedevice.h"

namespace engine
{

	FileDevice* FileDevice::instance()
	{
		static FileDevice s_fileDevice;
		return &s_fileDevice;
	}

	void FileDevice::setDefaultPath(const char* path)
	{
		m_defaultPath = Strdup(path);
	}

	File* FileDevice::openFile(const char* path, FileAccess access)
	{
		char pathBuffer[kMaxPathLength];
		sprintf(pathBuffer, "%s%s", m_defaultPath, path);

		return mem_new<File>(*g_sysAllocator, Strdup(pathBuffer), access);
	}

	void FileDevice::closeFile(File*& file)
	{
		mem_delete(*g_sysAllocator, file);
	}

}