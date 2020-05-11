#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <iostream>

#define buf 1024
#define maxFilesCount 10

using namespace std;

void printError() {
	setlocale(LC_ALL, "Russian");
	cout << "Error!" << endl;

	LPVOID errMessage;
	DWORD eCode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errMessage, 0, NULL);
	char* errorMessage = (char*)errMessage;
	wprintf(L"Error: failed with 0x%x code\n", eCode);
	wprintf(L"Error message: %s\n", errorMessage);
}

bool packFilesToArchive()
{
	bool result = true;
	LPSTR* inputFiles = new CHAR *[maxFilesCount];
	int inputFilesCount;
	cout << "Input count of files to archive: ";
	scanf("%d", &inputFilesCount);
	cout << "Input full path to files to archive:\n";
	LPSTR fileName = new CHAR[MAX_PATH];

	for (int i = 0; i < inputFilesCount; i++) {
		cout << "Input full path to file:";
		scanf("%s", fileName);
		inputFiles[i] = new CHAR[MAX_PATH];
		strcpy(inputFiles[i], fileName);
	}

	LPSTR archName = new CHAR[MAX_PATH];

	cout << "Input full path to new archive:";
	scanf("%s", archName);

	HANDLE hndlInReadPipe;
	HANDLE hndlInWritePipe;
	HANDLE hndlOutReadPipe;
	HANDLE hndlOutWritePipe;

	CreatePipe(&hndlInReadPipe,&hndlInWritePipe,NULL,0);
	CreatePipe(&hndlOutReadPipe, &hndlOutWritePipe, NULL, 0);

	if (hndlInReadPipe == INVALID_HANDLE_VALUE || hndlInWritePipe == INVALID_HANDLE_VALUE ||
		hndlOutReadPipe == INVALID_HANDLE_VALUE || hndlOutWritePipe == INVALID_HANDLE_VALUE) {
		printError();
		result = false;
	}

	STARTUPINFOA info;
	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);
	info.hStdInput = hndlInReadPipe;
	info.hStdOutput = hndlOutWritePipe;
	info.hStdError = hndlOutWritePipe;
	info.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));

	LPSTR cmdLine = new CHAR[MAX_PATH];
	ZeroMemory(cmdLine, MAX_PATH);

	strncpy(cmdLine, "7z.exe a ", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, archName, MAX_PATH - strlen(cmdLine));

	for (int i = 0; i < inputFilesCount; i++)
	{
		strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));
		strncat(cmdLine, inputFiles[i], MAX_PATH - strlen(cmdLine));
	}

	strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));

	if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &info, &processInfo)) {
		printError();
		result = false;
	}

	if (result) {
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		DWORD dwReadBytes = 0;
		LPSTR lpOutBuffer = new CHAR[buf];
		ZeroMemory(lpOutBuffer, buf);
		OVERLAPPED overlapped;
		ReadFile(hndlOutReadPipe, lpOutBuffer, buf, &dwReadBytes, &overlapped);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}

	CloseHandle(hndlOutReadPipe);
	CloseHandle(hndlOutWritePipe);
	CloseHandle(hndlInReadPipe);
	CloseHandle(hndlInWritePipe);
	return result;
}

void dataError() {
	cout << "Error" << endl;
	cout << "Please use one of the next argumnets:" << endl;
	cout << "-a - to pack files" << endl;
	cout << "-u - to unpack files" << endl;
	exit(0);
}

bool unpackFilesFromArchive()
{
	bool result = true;
	LPSTR inFileName = new CHAR[MAX_PATH];
	LPSTR outFileName = new CHAR[MAX_PATH];
	cout << "Input full path to archive:";
	scanf("%s", inFileName);
	cout << "Input full output path:";
	scanf("%s", outFileName);
	LPSTR cmdLine = new CHAR[MAX_PATH];
	ZeroMemory(cmdLine, MAX_PATH);

	strncpy(cmdLine, "7z.exe e ", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, inFileName, MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, " -o", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, outFileName, MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, " -y", MAX_PATH - strlen(cmdLine));

	STARTUPINFOA info;
	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);
	info.dwFlags |= STARTF_USESTDHANDLES;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &info, &procInfo)) 
	{
		printError();
		result = false;
	}
	return result;
}

int main(int argc, char* argv[])
{
	if (argc != 2 || ((strcmp(argv[1], "-a") != 0) && (strcmp(argv[1], "-u")) != 0)) {
		dataError();
	}
	if (strcmp(argv[1], "-a") == 0) {
		if (packFilesToArchive())
		{
			printf("The files were archived.");
		}
		else
		{
			printf("The files were not archived.");
		}
	}
	else {
		if ((unpackFilesFromArchive()))
		{
			printf("The files were unziped.");
		}
		else
		{
			printf("The files were not unziped.");
		}
	}
	return 0;
}