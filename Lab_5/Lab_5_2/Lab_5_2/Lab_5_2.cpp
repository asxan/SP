// Lab_5_2

#include "stdafx.h"

#define SLEEPING_TIME_MIN 1000
#define SLEEPING_TIME_MAX 3000
#define WAITING_TIME_MIN 1000
#define WAITING_TIME_MAX 3000
#define FILE_NAME TEXT("File.txt")
#define MAX_BUF 50

#define TIME_OUT 1000

HANDLE sem;

int GetRand(int min, int max) {
	return ((int)rand()) % (max - min) + min;
}

HANDLE OpenRequiredFile() {

	WaitForSingleObject(sem, INFINITE);

	HANDLE hFile = CreateFile(
		FILE_NAME, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	SetFilePointer(hFile, 0, NULL, FILE_END);

	return hFile;
}

void CloseFile(HANDLE hFile) {
	CloseHandle(hFile);
	DWORD count;
	ReleaseSemaphore(sem, 1, (LPLONG)&count);
}

void ThreadStart() {
	DWORD currId = GetCurrentThreadId(), realC;
	srand((unsigned int)currId);

	//elapse time
	DWORD start = GetTickCount();

	//sleep
	DWORD waitTime = GetRand(SLEEPING_TIME_MIN, SLEEPING_TIME_MAX);
	Sleep(waitTime);

	//open and write
	HANDLE hFile = OpenRequiredFile();
	LPSTR buf = new CHAR[MAX_BUF];
	memset(buf, 0, MAX_BUF);

	realC = sprintf(buf, "ID: %d, waiting time: %d", currId, GetTickCount() - start);
	buf[realC] = '\n';
	printf("%s", buf);
	WriteFile(hFile, (LPVOID)buf, realC + 1, &realC, NULL);

	//wait
	waitTime = GetRand(WAITING_TIME_MIN, WAITING_TIME_MAX);
	Sleep(waitTime);

	//close
	delete[] buf;
	CloseFile(hFile);
}

int _tmain(INT argc, TCHAR** argv)
{
	if (argc != 3) {
		_tprintf(TEXT("Wrong number of args"));
	}

	INT nAvailablePointers = _ttoi(argv[1]);
	INT nThreads = _ttoi(argv[2]);

	//make semaphore
	sem = CreateSemaphore(
		NULL, nAvailablePointers, nAvailablePointers, NULL);

	//launch the threads
	HANDLE* threads = new HANDLE[nThreads];
	for (size_t i = 0; i < nThreads; i++)
	{
		DWORD id;
		threads[i] = CreateThread(
			NULL, 0, (LPTHREAD_START_ROUTINE)ThreadStart, NULL, 0, &id);
	}

	for (size_t i = 0; i < nThreads; i++)
		WaitForSingleObject(threads[i], INFINITE);

	delete[] threads;
	return 0;
}

