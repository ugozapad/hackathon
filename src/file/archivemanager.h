#ifndef ARCHIVEMANAGER_H
#define ARCHIVEMANAGER_H

namespace engine
{
	class ArchiveManager : public Singleton<ArchiveManager>
	{
	private:
		static ArchiveManager ms_instance;

	public:
		void loadArchive(const std::string& filename);

	};
}

#endif // !ARCHIVEMANAGER_H
