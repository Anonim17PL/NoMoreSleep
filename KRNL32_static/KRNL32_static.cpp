//No More Sleep base lib
// Copyright (c) 2024 Anonim17PL

#include "pch.h"
#include "framework.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


//No More Sleep!
_declspec(dllexport) void __stdcall SleepWr(DWORD dwMilliseconds) {
	//Sleep(dwMilliseconds);
	Sleep(0); //Why zero? See: https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep
	return;
}

//Additional thread boost for some apps
_declspec(dllexport) bool __stdcall SetThreadPriorityWr(HANDLE hThread, int nPriority) {
	return SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
}
_declspec(dllexport) HANDLE __stdcall CreateThreadWr(LPSECURITY_ATTRIBUTES pThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) {
	HANDLE hT = CreateThread(pThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	if (hT != NULL)
		SetThreadPriority(hT, THREAD_PRIORITY_TIME_CRITICAL);
	return hT;
}
