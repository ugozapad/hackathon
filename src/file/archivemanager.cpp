#include "pch.h"
#include "file/archivemanager.h"
#include "file/filedevice.h"

namespace engine
{
	ArchiveManager ArchiveManager::ms_instance;

	void ArchiveManager::loadArchive(const std::string& filename)
	{
		std::string origPath = FileDevice::instance()->getDefaultPath();
		FileDevice::instance()->setDefaultPath("");

		if (fileExist(filename))
			m_archives.push_back(std::make_unique<Archive>(filename));

		FileDevice::instance()->setDefaultPath(origPath);
	}

	bool ArchiveManager::isAnyArchiveIsLoaded()
	{
		return !!m_archives.size();
	}

	bool ArchiveManager::fileExistInArchive(const std::string& filename)
	{
		return m_archives.back()->hasFile(filename);
	}

	DataStreamPtr ArchiveManager::openFile(const std::string& filename)
	{
		return m_archives.back()->openFile(filename);
	}

}
