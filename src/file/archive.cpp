#include "pch.h"
#include "file/archive.h"
#include "file/filedevice.h"

namespace engine
{
	Archive::Archive(const std::string& filename)
	{
		load(filename);
	}

	Archive::~Archive()
	{

	}

	void Archive::load(const std::string& filename)
	{
		File* file = FileDevice::instance()->openFile(filename, FileAccess::Read);

		ArchiveHeader h;
		file->read(&h, sizeof(h));

		if (h.version != 1)
			Core::error("Archive::load: Archive '%s' has version %i (current is %i)", filename.c_str(), h.version, 1);
		if (h.filecount <= 0)
			Core::error("Archive::load: Archive '%s' has %i files (less that zero)", filename.c_str(), h.filecount);

		std::vector<ArchiveFileEntry> fileEntrys;
		for (int i = 0; i < h.filecount; i++)
		{
			ArchiveFileEntry fileEntry;
			file->read(&fileEntry, sizeof(fileEntry));

			fileEntrys.push_back(fileEntry);
		}


	}
}
