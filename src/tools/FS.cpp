// FS.cpp: implementation of the CFS class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "stdafx.h"
#include "FS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------
// memory
CFS_Memory::~CFS_Memory() 
{	free(data);	}

void CFS_Memory::write	(const void* ptr, DWORD count)
{
	if (position+count > mem_size) {
		// reallocate
		if (mem_size==0)	mem_size=128;
		while (mem_size <= (position+count)) mem_size*=2;
		data = (BYTE*) realloc(data,mem_size);
	}
	CopyMemory(data+position,ptr,count);
	position+=count;
	if (position>file_size) file_size=position;
}

//---------------------------------------------------
// base stream
CStream*	CStream::OpenChunk(DWORD ID)
{
	BOOL	bCompressed;
	DWORD	dwSize = FindChunk(ID,&bCompressed);
	if (dwSize!=0) {
			return new CStream(Pointer(),dwSize);
	} else return 0;
};

void	CStream::Close()
{	delete this; }

//---------------------------------------------------
// temp stream
CTempStream::~CTempStream()
{	free(data);	};
//---------------------------------------------------
// file stream
CFileStream::~CFileStream()
{	free(data);	};
//---------------------------------------------------
// compressed stream
CCompressedStream::~CCompressedStream()
{	free(data);	};


//---------------------------------------------------
ENGINE_API void *FileDownload(const char *fn, DWORD *pdwSize)
{
	int		hFile;
	DWORD	size;
	void*	buf;

	hFile	= _open(fn,O_RDONLY|O_BINARY|O_SEQUENTIAL);	
	printf("* FS: Download %s",fn);
	assert(hFile>0);
	size	= _filelength(hFile);
	buf		= malloc(size);
	_read	(hFile,buf,size);
	_close	(hFile);
	if (pdwSize) *pdwSize = size;
	return buf;
}

typedef char MARK[8];
IC void mk_mark(MARK& M, const char* S)
{	strncpy(M,S,8); }
