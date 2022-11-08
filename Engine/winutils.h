#pragma once

#include "WinHeader.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <iostream>
#include <string>

#define wcsbytes(x) (wcslen(x)+1)*2

namespace winutils {

	typedef NTSTATUS (NTAPI *pNtCreateThreadEx) (
		OUT PHANDLE hThread,
		IN ACCESS_MASK DesiredAccess,
		IN PVOID ObjectAttributes,
		IN HANDLE ProcessHandle,
		IN PVOID lpStartAddress,
		IN PVOID lpParameter,
		IN ULONG Flags,
		IN SIZE_T StackZeroBits,
		IN SIZE_T SizeOfStackCommit,
		IN SIZE_T SizeOfStackReserve,
		OUT PVOID lpBytesBuffer
	);

	enum PROCFIND {
		CREATETOOLHELP32SNAPSHOT = 0
	};

	//HANDLE ForEachProcess()

	HANDLE findProcess(const wchar_t* processName, LONG options = 0);
	HMODULE remoteModuleHandle(HANDLE, LPWSTR);
	HANDLE findProcess(const DWORD pid, LONG options = 0);

	void* ProcFindFuncSwitch(LONG options);


	HANDLE _findProcess_ctlh32s(const wchar_t* processName, const DWORD pid);


    struct handle_data {
        unsigned long process_id;
        HWND window_handle;
    };

    HWND find_main_window(DWORD process_id);
    BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);
	BOOL is_main_window(HWND handle);

}