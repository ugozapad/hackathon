#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

#include "file/archive.h"

namespace engine
{
	class ArchiveManager : public Singleton<ArchiveManager>
	{
	private:
		static ArchiveManager ms_instance;

	public:
		void loadArchive(const std::string& filename);

		bool isAnyArchiveIsLoaded();

	private:
		std::vector<std::unique_ptr<Archive>> m_archives;
	};
}

#endif // !ARCHIVEMANAGER_H
