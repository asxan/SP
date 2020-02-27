#include <stdio.h>
#include<stdlib.h>
#include<locale.h>
#include <limits.h>
#include <wchar.h >
#include<uchar.h>
#include<climits>
#include<float.h>
int main()
{
	int number = 0;
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        TYPE        |      SIZE      |      MIN      |      MAX      |\n");
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        char        |        %d       |     %d      |      %d      |\n", sizeof(char), CHAR_MIN, CHAR_MAX );
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|    unsigned char   |        %d       |        %d      |      %d      |\n", sizeof(unsigned char), number , UCHAR_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|    signed char     |        %d       |     %d      |      %d      |\n", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|       wchar_t      |        %d       |       %d       |      %d    |\n", sizeof(wchar_t), WCHAR_MIN, WCHAR_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|      char16_t      |        %d       |       %d       |      %d    |\n", sizeof(char16_t), WCHAR_MIN, WCHAR_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|      char32_t      |        %u       |       %u       |  %u   |\n", sizeof(char32_t), number, UINT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        short       |        %hi       |    %hi     |      %hi    |\n", sizeof(short), SHRT_MIN, SHRT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|   unsigned short   |        %hu       |       %hu       |     %hu     |\n", sizeof(unsigned short), number, USHRT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|         int        |        %d       |  %d  |  %d   |\n", sizeof(int), INT_MIN, INT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|    unsigned int    |        %u       |       %u       |  %u   |\n", sizeof(unsigned int), number, UINT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        long        |        %li       |  %li  |    %li |\n", sizeof(long), LONG_MIN, LONG_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|   unsigned long    |        %lu       |       %lu       |  %lu   |\n", sizeof(unsigned long), number, ULONG_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|     long long      |        %d       |%lli|%lli|\n", sizeof(long long), LLONG_MIN, LLONG_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        float       |        %d       | %e  |  %e |\n", sizeof(float), FLT_MIN, FLT_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        double      |        %d       | %le | %le |\n", sizeof(double), DBL_MIN, DBL_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|     long double    |        %d       | %le | %le |\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	printf_s("|        bool        |        %d       |       %d       |        %d      |\n", sizeof(bool), true, false);
	printf_s("|~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~|\n");
	return 0;
}

