#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

void printError();
void printInfoDir(LPCSTR);
void printInfoFile(LPCSTR);

int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	char command[80];
	char path[MAX_PATH];
	printf("\nФАЙЛОВЫЙ МЕНЕДЖЕР\n\n");
	do {
		GetCurrentDirectory(MAX_PATH, path);
		printf("\n%s>", path);
		scanf_s("%s", command, 80);

		if (strcmp(command, "cd") == 0)
		{
			scanf_s("%s", path, MAX_PATH);
			if (!SetCurrentDirectory(path)) 
			{
				printError();
			}
		}
		else if (strcmp(command, "dir") == 0)
		{
			printInfoDir(path);
		}
		else if (strcmp(command, "copy") == 0)
		{
			char newFileName[MAX_PATH];
			scanf_s("%s", path, MAX_PATH);
			scanf_s("%s", newFileName, MAX_PATH);
			if (CopyFile(path, newFileName, true))
			{
				printf("Файл успешно скопирован\n");
			}
			else
			{
				printError();
			}
		}
		else if (strcmp(command, "mkdir") == 0)
		{
			scanf_s("%s", path, MAX_PATH);
			if (CreateDirectory(path, NULL))
			{
				printf("Директория успешно создана\n");
			}
			else
			{
				printError();
			}
		}
		else if (strcmp(command, "rmfile") == 0)
		{
			scanf_s("%s", path, MAX_PATH);
			if (DeleteFile(path))
			{
				printf("Файл успешно удален\n");
			}
			else
			{
				printError();
			}
		}
		else if (strcmp(command, "finfo") == 0)
		{
			scanf_s("%s", path, MAX_PATH);
			printInfoFile(path);
		}

	} while (strcmp(command, "exit") != 0);

	return 0;
}

void printError()
{
	LPVOID lpMsgBuf;
	DWORD dwErrCode = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	_tprintf(_T("Код ошибки: №%d. %s"), dwErrCode, lpMsgBuf);

	LocalFree(lpMsgBuf);
}

void printInfoDir(LPCSTR lpDir)
{
	WIN32_FIND_DATA FindFileData;
	SYSTEMTIME sysTime;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char dirSpec[MAX_PATH + 1];  // directory specification
	DWORD dwError;

	strncpy_s(dirSpec, lpDir, strlen(lpDir) + 1);
	strncat_s(dirSpec, "\\*", 3);

	hFind = FindFirstFile(dirSpec, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printError();
	}
	else
	{
		printf("Содержимое папки %s\n\n", lpDir);
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (strcmp(FindFileData.cFileName, "..") != 0)
			{
				FileTimeToSystemTime(&FindFileData.ftCreationTime, &sysTime);
				printf("%02d.%02d.%04d %02d:%02d:%02d\t",
					sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					printf("<DIR>");
				}
				DWORD sizeFile = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
				printf("\t%4d байт \t %s\n", sizeFile, FindFileData.cFileName);

			}
				
		}

		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES)
		{
			printError();
		}
	}
}

void printInfoFile(LPCSTR lpFile) {
	BY_HANDLE_FILE_INFORMATION infoFile;
	SYSTEMTIME sysTime;
	
	HANDLE hFile = CreateFile(lpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printError();
	}
	if (GetFileInformationByHandle(hFile, &infoFile))
	{
		DWORD sizeFile = (infoFile.nFileSizeHigh * (MAXDWORD + 1)) + infoFile.nFileSizeLow;
		printf("Серийный номер тома: %04X-%04X\n",
			HIWORD(infoFile.dwVolumeSerialNumber),
			LOWORD(infoFile.dwVolumeSerialNumber));
		printf("Размер файла: %d байт\n", sizeFile);
		FileTimeToSystemTime(&infoFile.ftCreationTime, &sysTime);
		printf("Дата создания: %02d.%02d.%04d %02d:%02d:%02d\n",
			sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		FileTimeToSystemTime(&infoFile.ftLastWriteTime, &sysTime);
		printf("Дата изменения: %02d.%02d.%04d %02d:%02d:%02d\n",
			sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		printf("Число связей: %d", infoFile.nNumberOfLinks);
	}
	CloseHandle(hFile);
}