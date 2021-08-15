#include "pch.h"
#include "file/archive.h"
#include "file/filedevice.h"
#include "file/memorystream.h"

namespace engine
{
	Archive::Archive(const std::string& filename)
	{
		load(filename);
	}

	Archive::~Archive()
	{
		if (m_zipFile)
			unzClose(m_zipFile);
	}

	void Archive::load(const std::string& filename)
	{
		m_zipFile = unzOpen(filename.c_str());
		ASSERT(m_zipFile);

		

		//File* file = FileDevice::instance()->openFile(filename, FileAccess::Read);

		//ArchiveHeader h;
		//file->read(&h, sizeof(h));

		//if (h.version != 1)
		//	Core::error("Archive::load: Archive '%s' has version %i (current is %i)", filename.c_str(), h.version, 1);
		//if (h.filecount <= 0)
		//	Core::error("Archive::load: Archive '%s' has %i files (less that zero)", filename.c_str(), h.filecount);

		//std::vector<ArchiveFileEntry> fileEntrys;
		//for (int i = 0; i < h.filecount; i++)
		//{
		//	ArchiveFileEntry fileEntry;
		//	file->read(&fileEntry, sizeof(fileEntry));

		//	fileEntrys.push_back(fileEntry);
		//}


	}

	bool Archive::hasFile(const std::string& filename)
	{
		unz_file_info info;
		return unzLocateFile(m_zipFile, filename.c_str(), NULL) == UNZ_OK;
	}

	DataStreamPtr Archive::openFile(const std::string& filename)
	{
		//data is the zip resource attached elsewhere
		unz_file_info info;
		uint8_t* rwh;

		unzLocateFile(m_zipFile, filename.c_str(), NULL);
		unzOpenCurrentFile(m_zipFile);
		unzGetCurrentFileInfo(m_zipFile, &info, NULL, 0, NULL, 0, NULL, 0);
		rwh = (uint8_t*)malloc(info.uncompressed_size);
		unzReadCurrentFile(m_zipFile, rwh, info.uncompressed_size);

		return std::make_shared<MemoryStream>(rwh, info.uncompressed_size, [](void* pPtr) { free(pPtr); });
	}

}
