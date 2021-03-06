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

		void setDefaultPath(const std::string& path);
		std::string getDefaultPath() { return m_defaultPath; }

		File* openFile(const std::string& path, FileAccess access);
		void closeFile(File*& file);

	private:
		std::string m_defaultPath;
	};

	inline bool fileExist(const std::string& filename)
	{
		File* file = FileDevice::instance()->openFile(filename, FileAccess::Read);
		bool valid = file->isValid();
		FileDevice::instance()->closeFile(file);

		return valid;
	}
}

#endif // !FILEDEVICE_H
