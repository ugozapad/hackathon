#include "pch.h"
#include "file/filesystem.h"
#include "file/filedevice.h"

namespace engine
{
	FileSystem FileSystem::ms_instance;

	DataStreamPtr FileSystem::openReadFile(const std::string& filename)
	{
		if (!fileExist(filename))
			Core::error("FS: ERORR! File '%s' is not exist", filename.c_str());


	}

}