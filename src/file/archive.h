#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "file/filedevice.h"
#include "file/datastream.h"

#include "mz.h"
#include "mz_zip.h"
#include "unzip.h"

namespace engine
{
	class Archive
	{
	public:
		Archive(const std::string& filename);
		~Archive();

		void load(const std::string& filename);

		bool hasFile(const std::string& filename);

		DataStreamPtr openFile(const std::string& filename);

	private:
		unzFile m_zipFile;
		//struct ArchiveHeader
		//{
		//	size_t version;
		//	size_t filecount;
		//};

		//struct ArchiveFileEntry
		//{
		//	char filename[260];
		//	size_t size;
		//	size_t pointerOffset;
		//};

		//std::unordered_map<std::string, ArchiveFileEntry> m_files;
	};
}

#endif // !ARCHIVE_H
