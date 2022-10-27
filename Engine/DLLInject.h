#pragma once
#include "WinHeader.h"
#include "winapi.h"
#include <iostream>

class dllinject {

public:

	enum {
		LOADLIBRARYEXW = 1,
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

	bool pnorid; // true for proc name false for proc id;
	int inj_load_idx;
	int inj_exec_idx;

	OPENFILENAME opfn;

	std::wstring dllrpath;
	std::wstring procname;
	DWORD procid;
	


private:
	
	static DWORD _injectfpath(LPWSTR, HANDLE, DWORD);
	static DWORD _injectrpath(std::wstring, HANDLE, DWORD);
	
};