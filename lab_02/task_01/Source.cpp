#define MAX_SIZE 80
#include <Windows.h>
#include <stdio.h>
#include <cstring>

#define PRINT '1'
#define MODIFY '2'
#define DELETE1 '3'


struct HEADERFILE
{
	DWORD dwCountNotNull;
	DWORD dwSize;
};
struct RECORD
{
	DWORD dwId;
	FILETIME ftTime;
	CHAR cStr[MAX_SIZE];
	DWORD dwCountEdt;
};

int initNewFile(DWORD);
int initOldFile(DWORD);
RECORD addRecord(LPDWORD);
RECORD getInfo(HANDLE, LPDWORD);
void printInfo(RECORD);
int setInfo(HEADERFILE, RECORD, LPSTR);
void printError();
int delInfo(LPDWORD);

int main(int argc, char * argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if (argc != 3)
	{
		printf("Ошибка! Количество элементов не равно 3.");
		return 1;
	}

	DWORD dwCount;
	if (sscanf_s(argv[1], "%d", &dwCount) != 1)
	{
		printf("Ошибка! Аргументом должно быть число.");
		return 1;
	}

	DWORD n = 0, item = 0;
	CHAR *str = new CHAR[MAX_SIZE];
	DWORD readBytes;
	HEADERFILE head;
	RECORD rec;

	if (strcmp(argv[2], "-e") == 0)
	{
		int res = initOldFile(dwCount);
		if (res == -1)
		{
			return 1;
		}
		dwCount = res;
	}
	if (strcmp(argv[2], "-n") == 0)
	{
		if (initNewFile(dwCount) == -1)
		{
			return 1;
		}
	}

	while(true)
	{
		HANDLE rFile = CreateFile("data.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (0 == ReadFile(rFile, &head, sizeof(HEADERFILE), &readBytes, NULL))
		{
			printError();
			return 1;
		}

		printf("Количество непустых записей: %d\nРазмер файла: %d\n", head.dwCountNotNull, head.dwSize);

		printf("Введите ID(0-%d): ", dwCount-1);
		scanf_s("%i", &item);
		printf("\nЧто сделать?\n");
		printf("1. Просмотреть\n");
		printf("2. Модифицировать\n");
		printf("3. Удалить\n");
		printf("\nВведите номер: ");
		scanf_s("%i", &n);
		
		switch (n)
		{
			case PRINT:
				rec = getInfo(rFile, &item);
				CloseHandle(rFile);
				printInfo(rec);
				break;
			case MODIFY:
				printf("Введите строку: ");
				scanf_s("%s", str, MAX_SIZE);
				rec = getInfo(rFile, &item);
				CloseHandle(rFile);
				setInfo(head, rec, str);
				printf("Информация изменена\n\n");
				break;
			case DELETE1:
				CloseHandle(rFile);
				delInfo(&item);
				printf("Информация удалена\n\n");
				break;
			default:
				CloseHandle(rFile);
				printf("Ошибка! Неккоректный номер.\n\n");
				break;
		}
	}

	system("pause");
	return 0;
}

int initNewFile(DWORD dwCount)
{
	HEADERFILE head = { 0, dwCount * sizeof(RECORD) + sizeof(HEADERFILE) };
	HANDLE wFile = CreateFile("data.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (wFile == INVALID_HANDLE_VALUE)
	{
		printError();
		return -1;
	}
	SetFilePointer(wFile, 0, 0, FILE_BEGIN);
	WriteFile(wFile, &head, sizeof(HEADERFILE), NULL, NULL);
	SetFilePointer(wFile, 0, 0, FILE_END);
	for (DWORD i = 0; i < dwCount; i++)
	{
		RECORD record = addRecord(&i);
		WriteFile(wFile, &record, sizeof(RECORD), NULL, NULL);
	}
	FlushFileBuffers(wFile);
	CloseHandle(wFile);

	return 0;
}

int initOldFile(DWORD dwCount)
{
	HANDLE rFile = CreateFile("data.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (rFile == INVALID_HANDLE_VALUE)
	{
		printError();
		return -1;
	}
	RECORD rec;
	DWORD readBytes;
	SetFilePointer(rFile, -(LONG)sizeof(RECORD), 0, FILE_END);
	if (0 == ReadFile(rFile, &rec, sizeof(RECORD), &readBytes, NULL))
	{
		printError();
		return -1;
	}
	CloseHandle(rFile);
	dwCount += rec.dwId+1;

	HANDLE wFile = CreateFile("data.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (wFile == INVALID_HANDLE_VALUE)
	{
		printError();
		return -1;
	}
	HEADERFILE head = { 0, dwCount * sizeof(RECORD) + sizeof(HEADERFILE) };
	SetFilePointer(wFile, 0, 0, FILE_BEGIN);
	WriteFile(wFile, &head, sizeof(HEADERFILE), NULL, NULL);
	SetFilePointer(rFile, 0, 0, FILE_END);
	for (DWORD i = rec.dwId + 1; i < dwCount; i++)
	{
		RECORD record = addRecord(&i);
		WriteFile(wFile, &record, sizeof(RECORD), NULL, NULL);
	}
	FlushFileBuffers(wFile);
	CloseHandle(wFile);

	return dwCount;
}

RECORD addRecord(LPDWORD lpId)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	RECORD rec = { *lpId, ft, "", 0 };

	return rec;
}

RECORD getInfo(HANDLE hFile, LPDWORD lpId)
{
	DWORD readBytes;
	RECORD rec;
	SetFilePointer(hFile, sizeof(HEADERFILE)+sizeof(RECORD)*(*lpId), NULL, FILE_BEGIN);
	if (0 == ReadFile(hFile, &rec, sizeof(RECORD), &readBytes, NULL))
	{
		printError();
	}

	return rec;
}

void printInfo(RECORD rec)
{
	SYSTEMTIME sysTime;
	printf("ID: %d\n", rec.dwId);
	FileTimeToSystemTime(&rec.ftTime, &sysTime);
	printf("Время создания: %02d.%02d.%04d %02d:%02d:%02d\n",
		sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	printf("Строка: %s\nКоличество редактирований: %d\n\n", rec.cStr, rec.dwCountEdt);
}

int setInfo(HEADERFILE head, RECORD rec, LPSTR lpStr)
{
	HANDLE wFile = CreateFile("data.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (wFile == INVALID_HANDLE_VALUE)
	{
		printError();
		return 1;
	}
	if (rec.dwCountEdt == 0)
	{
		head.dwCountNotNull++;
		SetFilePointer(wFile, 0, NULL, FILE_BEGIN);
		WriteFile(wFile, &head, sizeof(HEADERFILE), NULL, NULL);
	}
	rec.dwCountEdt++;
	strcpy_s(rec.cStr, lpStr);
	SetFilePointer(wFile, sizeof(HEADERFILE)+sizeof(RECORD)*rec.dwId, NULL, FILE_BEGIN);
	WriteFile(wFile, &rec, sizeof(RECORD), NULL, NULL);
	FlushFileBuffers(wFile);
	CloseHandle(wFile);

	return 0;
}

int delInfo(LPDWORD lpId)
{
	RECORD rec = { *lpId, {0, 0}, NULL, 0 };
	HANDLE wFile = CreateFile("data.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (wFile == INVALID_HANDLE_VALUE)
	{
		printError();
		return 1;
	}
	SetFilePointer(wFile, sizeof(HEADERFILE) + sizeof(RECORD)*(*lpId), NULL, FILE_BEGIN);
	WriteFile(wFile, &rec, sizeof(RECORD), NULL, NULL);
	FlushFileBuffers(wFile);
	CloseHandle(wFile);

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

	printf("Код ошибки: №%d. %s", dwErrCode, lpMsgBuf);

	LocalFree(lpMsgBuf);
}