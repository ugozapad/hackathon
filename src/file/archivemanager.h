#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

#include "file/archive.h"
#include "file/datastream.h"

namespace engine
{
	class ArchiveManager : public Singleton<ArchiveManager>
	{
	private:
		static ArchiveManager ms_instance;

	public:
		void loadArchive(const std::string& filename);

		bool isAnyArchiveIsLoaded();

		bool fileExistInArchive(const std::string& filename);

		DataStreamPtr openFile(const std::string& filename);

	private:
		std::vector<std::unique_ptr<Archive>> m_archives;
	};
}

#endif // !ARCHIVEMANAGER_H
