#include "stdafx.h"


int main()
{
	HANDLE mutex = CreateMutex(NULL, FALSE, TEXT("UNIQUE_MUTEX"));
	if (WaitForSingleObject(mutex, 0) == WAIT_OBJECT_0) {

		_tprintf(TEXT("Running..."));
	}
	else {
		_tprintf(TEXT("Program is already running. Fatal Error."));
	}
	getchar();
	return 0;
}