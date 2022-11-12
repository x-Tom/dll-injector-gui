#pragma once
#include "WinHeader.h"
#include "winutils.h"
#include <iostream>




#define wcsbytes(x) (wcslen(x)+1)*sizeof(wchar_t)


class dllinject {

public:

	enum {
		LOADLIBRARYEXW = 1,
		LDRLOADDLL,
		MANUALMAP,
		CREATEREMOTETHREADEX = 1 << 16,
		NTCREATETHREADEX,
		SETWINDOWSHOOKEX,
		QUEUEUSERAPC
	};

	//dllinject() = default;
	dllinject();
	void opfninit();
	~dllinject() = default;
	dllinject(const dllinject&) = delete;
	dllinject& operator=(const dllinject&) = delete;

	//void INJLoadSet(int);
	//void INJExecSet(int);

	//int INJLoadGet();
	//int INJExecGet();

	DWORD inject();
	DWORD eject();

	bool pnorid; // true for proc name false for proc id;
	int inj_load_idx;
	int inj_exec_idx;

	OPENFILENAME opfn;

	wchar_t dllrpath[MAX_PATH];
	wchar_t procname[MAX_PATH];
	wchar_t procid[MAX_PATH];
	


private:
	
	static DWORD _injectfpath(LPWSTR, HANDLE, DWORD);
	static DWORD _injectrpath(std::wstring, HANDLE, DWORD);
	
};