#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "injector/injector.hpp"
#include "ini.h"
#include "CrashPatch.h"

DWORD WINAPI MainTHREAD(LPVOID)
{
	Init();
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (*(DWORD*)0x4000A8 == 0x29BB90)	// Checking the entry point of 1.2 executable
		{
			CreateThread(0, 0, MainTHREAD, 0, 0, 0);
		}
		else
			return FALSE;
	}

	return TRUE;
}
