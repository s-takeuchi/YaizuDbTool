#pragma once

#include <windows.h>

DWORD WINAPI HandlerEx(DWORD, DWORD, PVOID, PVOID);
VOID WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv);
void StartMongoose();
void StopMongoose();
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
