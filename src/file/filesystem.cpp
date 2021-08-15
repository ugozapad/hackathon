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
		bool bFileExist = fileExist(filename);
		if (bFileExist)
			return std::make_shared<FileStream>(filename, "r");

		if (ArchiveManager::getInstance()->isAnyArchiveIsLoaded())
		{
			if (ArchiveManager::getInstance()->fileExistInArchive(filename))
				return ArchiveManager::getInstance()->openFile(filename);
		}

		if (!bFileExist)
			Core::error("FS: ERORR! File '%s' is not exist", filename.c_str());

		return std::make_shared<FileMemoryMappingStream>(filename);
	}

	bool FileSystem::fileexist(const std::string& filename)
	{
		if (ArchiveManager::getInstance()->isAnyArchiveIsLoaded() && ArchiveManager::getInstance()->fileExistInArchive(filename))
			return true;

		return fileExist(filename);
	}

}