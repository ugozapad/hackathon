#ifndef FILEDEVICE_H
#define FILEDEVICE_H

#ifdef WIN32
#include "file/file_win32.h"
namespace engine { typedef FileWin32 File; }
#endif // WIN32

namespace engine
{
	class FileDevice : public Singleton<FileDevice>
	{
	private:
		static FileDevice ms_instance;

	public:
		void setDefaultPath(const eastl::string& path);

		File* openFile(const eastl::string& path, FileAccess access);
		void closeFile(File*& file);

	private:
		eastl::string m_defaultPath;
	};
}

#endif // !FILEDEVICE_H
