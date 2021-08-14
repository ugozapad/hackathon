#include "pch.h"
#include "file/archivemanager.h"

namespace engine
{
	ArchiveManager ArchiveManager::ms_instance;

	void ArchiveManager::loadArchive(const std::string& filename)
	{
		m_archives.push_back(std::make_unique<Archive>(filename));
	}

	bool ArchiveManager::isAnyArchiveIsLoaded()
	{
		return !!m_archives.size();
	}

}
