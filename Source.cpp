#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <psapi.h>
#include <tlhelp32.h>

#include "xor.h"
#include "mac.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#define MAX_PROCESSES 1024
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))
#define color1 WORD)(0x0003 | 0x0000) 
#define color2 (WORD)(0x0002 | 0x0000)
#define color3 (WORD)(0x0001 | 0x0000)
#define color4 (WORD)(0x0008 | 0x0000)
#define color5 (WORD)(0x0004 | 0x0000)
#define color6 (WORD)(0x0005 | 0x0000)
#define color7 (WORD)(0x0006 | 0x0000)
#define color8 (WORD)(0x0007 | 0x0000)
#define color9 (WORD)(0x0009 | 0x0000)
#define color10 (WORD)(0x0010 | 0x0000)
#define color11 (WORD)(0x0011 | 0x0000)
#define color12 (WORD)(0x0012 | 0x0000)
#define COLOR(h, c) SetConsoleTextAttribute(h, c);
#define TEXTCOLOR(h,c1,c2,s) COLOR(h,c1); std::cout<<s; COLOR(h,c2);
#define LENGTH(a) (sizeof(a) / sizeof(a[0]))
#define WIN32_LEAN_AND_MEAN

MyMACAddr::MyMACAddr()
{
	srand((unsigned)time(0));
}

MyMACAddr::~MyMACAddr()
{
}

void MyMACAddr::showAdapterList()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		cerr << XorString("Error allocating memory needed to call GetAdaptersinfo.") << endl;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			cerr << XorString("Error allocating memory needed to call GetAdaptersinfo") << endl;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			for (i = 0; i < pAdapter->AddressLength; i++)
			{
				if (i == (pAdapter->AddressLength - 1))
					printf(XorString("%.2X\n"), (int)pAdapter->Address[i]);
				else
					printf(XorString("%.2X-"), (int)pAdapter->Address[i]);
			}
			pAdapter = pAdapter->Next;
		}
	}
	else {
		cerr << XorString("GetAdaptersInfo failed with error: ") << dwRetVal << endl;
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);
}

unordered_map<string, string> MyMACAddr::getAdapters()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	unordered_map<string, string> result;
	stringstream temp;
	string str_mac;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		cerr << XorString("Error allocating memory needed to call GetAdaptersinfo") << endl;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			cerr << XorString("Error allocating memory needed to call GetAdaptersinfo\n") << endl;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			for (UINT i = 0; i < pAdapter->AddressLength; i++) {
				temp << setfill('0') << setw(2) << hex << (int)pAdapter->Address[i];
				if (i != pAdapter->AddressLength - 1)
				{
					temp << "-";
				}
			}
			str_mac = temp.str();
			temp.str(XorString(""));
			temp.rdbuf();
			for (auto& c : str_mac)
			{
				c = toupper(c);
			}

			result.insert({ pAdapter->Description, str_mac });
			pAdapter = pAdapter->Next;
		}
	}
	else {
		cerr << XorString("GetAdaptersInfo failed with error: ") << dwRetVal << endl;
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return result;
}
unordered_map<string, string> getAdapters()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	unordered_map<string, string> result;
	stringstream temp;
	string str_mac;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		cerr << XorString("Error allocating memory needed to call GetAdaptersinfo") << endl;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			cerr << XorString("Error allocating memory needed to call GetAdaptersinfo\n") << endl;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			for (UINT i = 0; i < pAdapter->AddressLength; i++) {
				temp << setfill('0') << setw(2) << hex << (int)pAdapter->Address[i];
				if (i != pAdapter->AddressLength - 1)
				{
					temp << "-";
				}
			}
			str_mac = temp.str();
			temp.str(XorString(""));
			temp.rdbuf();
			for (auto& c : str_mac)
			{
				c = toupper(c);
			}

			result.insert({ pAdapter->Description, str_mac });
			pAdapter = pAdapter->Next;
		}
	}
	else {
		cerr << XorString("GetAdaptersInfo failed with error: ") << dwRetVal << endl;
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return result;
}

BOOL SetConsoleSizeXY(HANDLE hStdout, int iWidth, int iHeight)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD coordMax;

	coordMax = GetLargestConsoleWindowSize(hStdout);


	if (iHeight > coordMax.Y) iHeight = coordMax.Y;


	if (iWidth > coordMax.X) iWidth = coordMax.X;

	if (!GetConsoleScreenBufferInfo(hStdout, &info)) return FALSE;
	info.srWindow.Left = 0;
	info.srWindow.Right = info.dwSize.X - 1;
	info.srWindow.Top = 0;
	info.srWindow.Bottom = iHeight - 1;

	if (iHeight < info.dwSize.Y)
	{
		if (!SetConsoleWindowInfo(hStdout, TRUE, &info.srWindow))
			return FALSE;

		info.dwSize.Y = iHeight;

		if (!SetConsoleScreenBufferSize(hStdout, info.dwSize))
			return FALSE;
	}
	else if (iHeight > info.dwSize.Y)
	{
		info.dwSize.Y = iHeight;

		if (!SetConsoleScreenBufferSize(hStdout, info.dwSize))
			return FALSE;

		if (!SetConsoleWindowInfo(hStdout, TRUE, &info.srWindow))
			return FALSE;
	}

	if (!GetConsoleScreenBufferInfo(hStdout, &info))
		return FALSE;
	info.srWindow.Left = 0;
	info.srWindow.Right = iWidth - 1;
	info.srWindow.Top = 0;
	info.srWindow.Bottom = info.dwSize.Y - 1;

	if (iWidth < info.dwSize.X)
	{
		if (!SetConsoleWindowInfo(hStdout, TRUE, &info.srWindow))
			return FALSE;

		info.dwSize.X = iWidth;

		if (!SetConsoleScreenBufferSize(hStdout, info.dwSize))
			return FALSE;
	}
	else if (iWidth > info.dwSize.X)
	{
		info.dwSize.X = iWidth;

		if (!SetConsoleScreenBufferSize(hStdout, info.dwSize))
			return FALSE;

		if (!SetConsoleWindowInfo(hStdout, TRUE, &info.srWindow))
			return FALSE;
	}
	return TRUE;
}

int main()
{
	vector <string> list;
	unordered_map<string, string> AdapterDetails = getAdapters();
	for (auto& itm : AdapterDetails)
	{
		list.push_back(itm.first);
	}
	int selection = 1;

	system(XorString("cls"));
	HANDLE hpStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	SetConsoleSizeXY(hpStdout, 80, 38);

	COLOR(hpStdout, color2);
	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("HARD DISK :  "));
	COLOR(hpStdout, color2);
	system(XorString("wmic diskdrive get Serialnumber"));

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("PROCESSOR ID :  "));
	COLOR(hpStdout, color2);
	system(XorString("wmic cpu get processorid"));

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("RAM :  "));
	COLOR(hpStdout, color2);
	system(XorString("wmic memorychip get serialnumber"));

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("MAC         -----> "));
	COLOR(hpStdout, color2);
	cout << AdapterDetails.at(list.at(selection - 1)) << endl;

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("SMBIOS      -----> "));
	COLOR(hpStdout, color2);
	char valueY[255];
	DWORD BufferSizeY = sizeof(valueY);
	LONG resY = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SYSTEM\\HardwareConfig\\Current"), XorString("SystemFamily"), RRF_RT_REG_SZ, NULL, valueY, &BufferSizeY);
	if (resY == 0) std::cout << valueY << std::endl;

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("MACHINE GUID ----> "));
	COLOR(hpStdout, color2);
	char value[255];
	DWORD BufferSize = sizeof(value);
	LONG res = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SOFTWARE\\Microsoft\\Cryptography"), XorString("MachineGuid"), RRF_RT_REG_SZ, NULL, value, &BufferSize);
	if (res == 0) std::cout << value << std::endl;
	COLOR(hpStdout, color2);
	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("GUID        -----> "));
	COLOR(hpStdout, color2);
	char valueW[255];
	DWORD BufferSizeW = sizeof(valueW);
	LONG resW = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SOFTWARE\\Microsoft\\Cryptography"), XorString("GUID"), RRF_RT_REG_SZ, NULL, valueW, &BufferSizeW);
	if (resW == 0) std::cout << valueW << std::endl;

	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("MOTHERBOARD -----> "));
	COLOR(hpStdout, color2);
	char valueX[255];
	DWORD BufferSizeX = sizeof(valueX);
	LONG resX = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SYSTEM\\HardwareConfig"), XorString("BaseBoardProduct"), RRF_RT_REG_SZ, NULL, valueX, &BufferSizeX);
	if (resX == 0) std::cout << valueX << std::endl;

	printf(XorString("\n\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("FORTNITE ID 1 ---> "));
	COLOR(hpStdout, color2);
	char valueH[255];
	DWORD BufferSizeOO = sizeof(valueH);
	LONG resoo = RegGetValueA(HKEY_CURRENT_USER, XorString("SOFTWARE\\Epic Games\\Unreal Engine\\Identifiers"), XorString("AccountId"), RRF_RT_REG_SZ, NULL, valueH, &BufferSizeOO);
	if (resoo == 0) std::cout << valueH << std::endl;
	printf(XorString("\n\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("FORTNITE ID 2 ---> "));
	COLOR(hpStdout, color2);
	char valueOO[255];
	DWORD BufferSizeWOO = sizeof(valueOO);
	LONG resWOO = RegGetValueA(HKEY_CURRENT_USER, XorString("SOFTWARE\\Epic Games\\Unreal Engine\\Identifiers"), XorString("MachineId"), RRF_RT_REG_SZ, NULL, valueOO, &BufferSizeWOO);
	if (resWOO == 0) std::cout << valueOO << std::endl;

	COLOR(hpStdout, color2);
	printf(XorString("\n\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("SYSTEM      -----> "));
	COLOR(hpStdout, color2);
	char valueV[255];
	DWORD BufferSizeV = sizeof(valueV);
	LONG resV = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SYSTEM\\HardwareConfig\\Current"), XorString("SystemSKU"), RRF_RT_REG_SZ, NULL, valueV, &BufferSizeV);
	if (resV == 0) std::cout << valueV << std::endl;

	COLOR(hpStdout, color2);
	printf(XorString("\n ["));
	COLOR(hpStdout, color3);
	printf(XorString(">"));
	COLOR(hpStdout, color2);
	printf(XorString("] "));
	COLOR(hpStdout, color4);
	printf(XorString("PC NAME     -----> "));
	COLOR(hpStdout, color2);
	char valueVET[255];
	DWORD BufferSizeVET = sizeof(valueVET);
	LONG resVET = RegGetValueA(HKEY_LOCAL_MACHINE, XorString("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters"), XorString("Hostname"), RRF_RT_REG_SZ, NULL, valueVET, &BufferSizeVET);
	if (resVET == 0) std::cout << valueVET << std::endl;
	printf(XorString("\n=================== ==========================================================\n"));

	COLOR(hpStdout, color7);
	printf(XorString("\n\n [<] Exit in 20 seconds..."));
	Sleep(20000);

	return 0;
}