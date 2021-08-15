#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "file/datastream.h"

namespace engine
{
	class FileSystem : public Singleton<FileSystem>
	{
		static FileSystem ms_instance;
	public:
		DataStreamPtr openReadFile(const std::string& filename);

		bool fileexist(const std::string& filename);
	};
}

#endif // !FILESYSTEM_H
