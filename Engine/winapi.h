#pragma once

#include "WinHeader.h"
#include <tlhelp32.h>

#define wcsbytes(x) (wcslen(x)+1)*2

namespace winapi {



	enum PROCFIND {
		CREATETOOLHELP32SNAPSHOT = 0
	};

	//HANDLE ForEachProcess()

	HANDLE findProcess(const wchar_t* processName, LONG options = 0);
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