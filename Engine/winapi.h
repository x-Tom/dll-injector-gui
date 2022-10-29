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

}