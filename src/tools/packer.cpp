#include "StdAfx.h"
#include "FS.h"

struct ArchiveHeader
{
	size_t version;
	size_t filecount;
};

struct ArchiveFileEntry
{
	char filename[260];
	size_t size;
	size_t pointerOffset;
};

std::string createCommandLine(int argc, char* argv[])
{
	std::string commandline;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i])
		{
			commandline.append(argv[i]);
			commandline.append(" ");
		}
	}

	return commandline;
}

void createPack(const std::string& name)
{
	FILE* file = fopen(name.c_str(), "wb");
	if (!file)
		assert(true && "failed to open file!!!");


}

int main(int argc, char* argv[])
{
	std::string cmdline = createCommandLine(argc, argv);

	if (strstr(cmdline.c_str(), "-pack"))
	{
		std::string filename = strstr(cmdline.c_str(), "-pack");
		if (!filename.empty())
		{
			createPack(filename);
		}
		else
		{
			printf("where is package name for -pack ???");
		}
	}
	else
	{
		printf("where is -pack ???");
	}

	system("pause");
	return 0;
}