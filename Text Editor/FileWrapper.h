#pragma once
#include <Windows.h>

class FileWrapper
{
public:
	FileWrapper(void);
	FileWrapper(char fileName[]);
	~FileWrapper(void);

	void initFile(const char fileName[]);
	void initFile();
	int readFile(char lpBuffer[]);						//reads a file and stores it into the lpBuffer 
														//character string proviuded by the caller. returns the size of the lpBuffer

private:
	HANDLE hFile;
	char* szFileName;
	int szFileSize;
};

