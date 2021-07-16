#ifndef FILECOMMON_H
#define FILECOMMON_H

namespace engine
{
	static const int kMaxPathLength = 260;

	enum class FileAccess
	{
		Read,
		Write
	};

	enum class FileSeek
	{
		Begin,
		Current,
		End
	};
}

#endif // !FILECOMMON_H
