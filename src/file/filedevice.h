#ifndef FILEDEVICE_H
#define FILEDEVICE_H

#ifdef WIN32
#include "file/file_win32.h"
namespace engine { typedef FileWin32 File; }
#endif // WIN32

namespace engine
{
	class FileDevice
	{
	public:
		static FileDevice* instance();
	public:
		void setDefaultPath(const char* path);

		File* openFile(const char* path, FileAccess access);
		void closeFile(File*& file);

	private:
		const char* m_defaultPath;
	};
}

#endif // !FILEDEVICE_H
