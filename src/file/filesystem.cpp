#include "pch.h"
#include "file/filesystem.h"
#include "file/filedevice.h"
#include "file/filestream.h"
#include "file/filememorymappingstream.h"
#include "file/archivemanager.h"

namespace engine
{
	FileSystem FileSystem::ms_instance;

	DataStreamPtr FileSystem::openReadFile(const std::string& filename)
	{
		if (ArchiveManager::getInstance()->isAnyArchiveIsLoaded())
		{

		}

		if (!fileExist(filename))
			Core::error("FS: ERORR! File '%s' is not exist", filename.c_str());

		return std::make_shared<FileMemoryMappingStream>(filename);
	}

}