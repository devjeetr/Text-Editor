#include "FileWrapper.h"


FileWrapper::FileWrapper(void):szFileName(0), szFileSize(-1), hFile(0)
{
}

FileWrapper::FileWrapper(char fileName[])
{
	szFileName = fileName;
}

FileWrapper::~FileWrapper(void)
{
}

void FileWrapper::initFile()
{
	hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL,NULL);
	szFileSize = GetFileSize(hFile, NULL) + 20;
}

int FileWrapper::readFile(char lpBuffer[])
{
	DWORD* nBytesRead;
	OVERLAPPED* lpOverLapped;

	ZeroMemory(lpOverLapped, sizeof(OVERLAPPED));
	nBytesRead = 0;

	lpBuffer = new char[szFileSize];
	ZeroMemory(&lpBuffer, sizeof(lpBuffer));
	ReadFile(hFile, lpBuffer, sizeof(lpBuffer),nBytesRead, lpOverLapped);
}