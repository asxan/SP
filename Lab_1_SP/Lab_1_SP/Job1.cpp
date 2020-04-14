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
	printf("�� �� ������� �������� ��� ������� �������� ��������. ��������� ������������ 2 ���������:\n");
	printf("'-e' - ������� ������\n");
	printf("'-s' - ������� ���������� � �������\n\n");
	exit(EXIT_FAILURE);
}

// Func for GlobalMemoryStatus() information print
void GlobalMemoryInfoPrint()
{
    MEMORYSTATUS memStatusStruct;
    GlobalMemoryStatus(&memStatusStruct);
    cout << "   ~~~ ���������� � ������� ��������� ��� ����������, ��� � ����������� ������ ~~~" << endl;
    cout << "��������������� ������� ������������ ���������� ������:"
        << memStatusStruct.dwMemoryLoad << endl;
    // phisical memory
    cout << "����� ����������� ���������� ������ � ������:"
        << memStatusStruct.dwTotalPhys << endl;
    cout << "����� ��������� ���������� ������ � ������:"
        << memStatusStruct.dwAvailPhys << endl;
    // memory limit for process
    cout << "������� ������ ����������� ������ ������ � ������:"
        << memStatusStruct.dwTotalPageFile << endl;
    cout << "����. ����� ������, ������� ������� ������� ����� ������������� � ������:"
        << memStatusStruct.dwAvailPageFile << endl;
    // virtual memory
    cout << "����� ����� ����������� ������, � ������:"
        << memStatusStruct.dwTotalVirtual << endl;
    cout << "����� ��������� ����������� ������ � ������:"
        << memStatusStruct.dwAvailVirtual << endl;

    cout << "\n������������ ������� - GlobalMemoryStatus ()\n" << endl;
}

// Func for GetSystemInfo information print
void GetSystemInfoPrint()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    printf("    ~~~ ���������� �� ������������ ~~~\n");
    printf("  ����������� ����������: %u\n",       sysInfo.wProcessorArchitecture);
    printf("  ��� ����������: %u\n",               sysInfo.dwProcessorType);
    printf("  Number of processors: %u\n",         sysInfo.dwNumberOfProcessors);
    printf("  �������� ����� ����������: %u\n",        sysInfo.dwActiveProcessorMask);
    printf("  Page size: %u\n",                    sysInfo.dwPageSize);
    printf("  ����������� ����� ����������: %p\n",  sysInfo.lpMinimumApplicationAddress);
    printf("  ������������ ����� ����������: %p\n",  sysInfo.lpMaximumApplicationAddress);
    printf("  ����������� ���������� ������, " 
           "  �� �������� ����� ���� ��������"
           "  ����������� ������: %u\n",      sysInfo.dwAllocationGranularity);
    printf("\n������������ ������� - GetSystemInfo ()\n");
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
		printf("������� ����� ��������� ������");
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
