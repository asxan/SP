#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace std;

#define countOfNumbers 5

CRITICAL_SECTION critSection;
HANDLE* threadHandles;
int index;

void showArr(int* arr, int size)
{
	for (int i = 0; i < size; i++) {
		cout << " " << arr[i];
	}
}

int sumCount(int* arr, int size)
{
	int res = 0;
	for (int i = 0; i < size; i++) {
		res += arr[i];
	}

	return res;
}

int largestDiv(int num)
{
	int n = num / 2;
	for (int i = n; i >= 2; i--) {
		if (num % i == 0)
			return i;
	}
}

void fillLargestDiv(int* initArr, int* resArr)
{
	for (int i = 0; i < countOfNumbers; i++) {
		resArr[i] = largestDiv(initArr[i]);
	}
}



void dataError() {
	cout << "Error" << endl;
	cout << "Please use next arguments:" << endl;
	cout << "Threads count - more than 1" << endl;
	exit(0);
}
DWORD WINAPI workedByThread(LPVOID arr)
{
	EnterCriticalSection(&critSection);

	cout << "ID: " << GetCurrentThreadId() << endl;

	int* newArr = (int*)arr;
	TlsSetValue(index, (LPVOID)(new int[countOfNumbers]));
	int sum = 0;

	cout << "Generated array: ";
	showArr(newArr, countOfNumbers);
	fillLargestDiv(newArr, (int*)TlsGetValue(index));

	cout << endl << "Modyfied array: ";
	showArr((int*)TlsGetValue(index), countOfNumbers);
	sum = sumCount((int*)TlsGetValue(index), countOfNumbers);

	cout << "\nSum: " << sum << endl;
	LeaveCriticalSection(&critSection);
	return 0;
}

void runThreads(int threadsCount)
{
	cout << "Count of threads:" << threadsCount << endl;
	InitializeCriticalSection(&critSection);
	threadHandles = new HANDLE[threadsCount];
	DWORD* ids = new DWORD[threadsCount];
	int** arrays = new int* [threadsCount];
	time_t t;
	srand((unsigned)time(&t));

	index = TlsAlloc();
	for (int i = 0; i < threadsCount; i++)
	{
		arrays[i] = new int[countOfNumbers];
		for (int j = 0; j < countOfNumbers; j++) {
			arrays[i][j] = rand() % 90 + 10;
		}
		threadHandles[i] = CreateThread(
			NULL,
			0,
			workedByThread,
			arrays[i],
			NULL,
			&ids[i]);
	}
}



int main(int argc, char* argv[])
{
	if (argc != 2) {
		dataError();
	}
	else {
		int threadsCount = atoi(argv[1]);
		runThreads(threadsCount);
		WaitForMultipleObjects(threadsCount, threadHandles, TRUE, INFINITE);
		DeleteCriticalSection(&critSection);
	}
	return 0;
}
