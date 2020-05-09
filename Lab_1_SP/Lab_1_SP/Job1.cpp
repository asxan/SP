// declaration part
#include <stdio.h>
#include "windows.h"
#include "iostream"
#include <locale.h>

using namespace std;

#define MAX_ARGUMENT_LENGTH 2
// functions for implementation -s argument
void ImplementationShowErrorArg();
void ImplementationShowSystemInfoArg();
void StartWithoutArgs();
void GlobalMemoryInfoPrint();
void GetSystemInfoPrint();

// functions for implementation -e argument
bool ThrowOutOfMemoryError();
void ShowError();


// implementation part

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "rus");

    if (argc != 2)
    {
        StartWithoutArgs();
    }
    else
    {
        if (strcmp(argv[1], "-e") == 0)
        {
            ImplementationShowErrorArg();
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            ImplementationShowSystemInfoArg();
        }
        else // error argument
        {
            StartWithoutArgs();
        }
    }
    return 0;
}

void StartWithoutArgs()
{
	printf("Вы не вводите аргумент или вводите неверный аргумент. Программа поддерживает 2 аргумента:\n");
	printf("'-e' - вывести ошибку\n");
	printf("'-s' - вывести информацию о системе\n\n");
	exit(EXIT_FAILURE);
}

// Func for GlobalMemoryStatus() information print
void GlobalMemoryInfoPrint()
{
    MEMORYSTATUS memStatusStruct;
    GlobalMemoryStatus(&memStatusStruct);
    cout << "   ~~~ Информация о текущем состоянии как физической, так и виртуальной памяти ~~~" << endl;
    cout << "Приблизительный процент используемой физической памяти:"
        << memStatusStruct.dwMemoryLoad << endl;
    // phisical memory
    cout << "Объем фактической физической памяти в байтах:"
        << memStatusStruct.dwTotalPhys << endl;
    cout << "Объем доступной физической памяти в байтах:"
        << memStatusStruct.dwAvailPhys << endl;
    // memory limit for process
    cout << "Текущий размер выделенного лимита памяти в байтах:"
        << memStatusStruct.dwTotalPageFile << endl;
    cout << "Макс. объем памяти, который текущий процесс может зафиксировать в байтах:"
        << memStatusStruct.dwAvailPageFile << endl;
    // virtual memory
    cout << "Общий объем виртуальной памяти, в байтах:"
        << memStatusStruct.dwTotalVirtual << endl;
    cout << "Объем доступной виртуальной памяти в байтах:"
        << memStatusStruct.dwAvailVirtual << endl;

    cout << "\nИспользуемая функция - GlobalMemoryStatus ()\n" << endl;
}

// Func for GetSystemInfo information print
void GetSystemInfoPrint()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("    ~~~ Информация об оборудовании ~~~\n");
    printf("  Архитектура процессора: %u\n",       sysInfo.wProcessorArchitecture);
    printf("  Тип процессора: %u\n",               sysInfo.dwProcessorType);
    printf("  Number of processors: %u\n",         sysInfo.dwNumberOfProcessors);
    printf("  Активная маска процессора: %u\n",        sysInfo.dwActiveProcessorMask);
    printf("  Page size: %u\n",                    sysInfo.dwPageSize);
    printf("  Минимальный адрес приложения: %p\n",  sysInfo.lpMinimumApplicationAddress);
    printf("  Максимальный адрес приложения: %p\n",  sysInfo.lpMaximumApplicationAddress);
    printf("  Детализация начального адреса, " 
           "  по которому может быть выделена"
           "  виртуальная память: %u\n",      sysInfo.dwAllocationGranularity);
    printf("\nИспользуемая функция - GetSystemInfo ()\n");
}


void ShowError()
{
	LPVOID lpMsgBuf;
	DWORD dwLastError = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	printf("\n%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
	return;
}

void ImplementationShowErrorArg()
{
	// call function for print error 
	if (ThrowOutOfMemoryError())
	{
		ShowError();
	}
	else
	{
		printf("Слишком много свободной памяти");
	}
}

void ImplementationShowSystemInfoArg()
{
	// call function for  print information about system
	GlobalMemoryInfoPrint();
	GetSystemInfoPrint();
}

bool ThrowOutOfMemoryError()
{
	DWORD userMemCount = 0;
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	HLOCAL pAllocMem = LocalAlloc(LMEM_FIXED, memStatus.dwTotalVirtual + 5000000000); // total virtual memory + 5 GB
	if (pAllocMem == NULL)
	{
		return true;
	}
	else
	{
		LocalFree(pAllocMem);
		return false;
	}
}
