#include "DLLInject.h"

dllinject::dllinject() : pnorid{ true }, inj_exec_idx{ 0 }, inj_load_idx{ 0 }, dllrpath{}, procname{}, procid{0} {}

void dllinject::opfninit() {
	wchar_t* szFile = new wchar_t[MAX_PATH];
	wchar_t* szFileTitle = new wchar_t[MAX_PATH];
	HWND ohwnd = nullptr;

	ZeroMemory(szFile, MAX_PATH);
	ZeroMemory(szFileTitle, MAX_PATH);

	ZeroMemory(&opfn, sizeof(opfn));
	opfn.lStructSize = sizeof(opfn);
	opfn.hwndOwner = ohwnd;
	opfn.lpstrFile = szFile;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = MAX_PATH;
	opfn.lpstrFilter = L"DLL\0*.DLL\0\0";
	opfn.nFilterIndex = 1;
	opfn.lpstrFileTitle = szFileTitle;
	opfn.nMaxFileTitle = MAX_PATH;
	opfn.lpstrInitialDir = NULL;
	opfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

DWORD dllinject::inject(){
	wchar_t buf[MAX_PATH] = {0};
	wcscat_s(buf, L"Couldnt find process: ");
	wcscat_s(buf, procname);
	wcscat_s(buf, L" PID: ");
	wcscat_s(buf, procid);
	wcscat_s(buf, L" DLL to inject (file title): ");
	wcscat_s(buf, opfn.lpstrFileTitle);
	wcscat_s(buf, L" (file full path): ");
	wcscat_s(buf, opfn.lpstrFile);

	DWORD pid = _wtoi(procid);
	HANDLE proc = (pnorid) ? winutils::findProcess(procname) : winutils::findProcess(pid);
	if (proc == NULL) MessageBox(NULL, buf, L"Process not found, Injection Failed!", NULL);
	DWORD opts = (inj_load_idx+1) | ((inj_exec_idx+1) << 16);

	return dllinject::_injectfpath(opfn.lpstrFile, proc, opts);
}

DWORD dllinject::eject()
{
	DWORD pid = _wtoi(procid);
	HANDLE proc = (pnorid) ? winutils::findProcess(procname) : winutils::findProcess(pid);
	HMODULE remote_module = winutils::remoteModuleHandle(proc, opfn.lpstrFileTitle);
	if (proc == nullptr || remote_module == nullptr) return 1;
	// Call free library in the remote process;
	LPVOID pFreeLibraryW = (LPVOID)GetProcAddress(LoadLibrary(L"kernel32"), "FreeLibraryW");
	HANDLE hthread;
	DWORD dword;

	switch (inj_exec_idx) {
	case CREATEREMOTETHREADEX:

		hthread = CreateRemoteThread(proc, NULL, NULL, (LPTHREAD_START_ROUTINE)pFreeLibraryW, remote_module, 0, &dword);
		if (hthread == nullptr) {
			MessageBox(NULL, L"Failed to Create Remote Thread", L"Injection Failed", NULL);
			return 1;
		}
		break;
	case NTCREATETHREADEX:
		winutils::pNtCreateThreadEx NtCreateThreadEx = (winutils::pNtCreateThreadEx)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");
		NtCreateThreadEx(&hthread, 0x1FFFFF, NULL, proc, (LPTHREAD_START_ROUTINE)pFreeLibraryW, remote_module, FALSE, NULL, NULL, NULL, NULL);
		if (hthread == nullptr) {
			MessageBox(NULL, L"Failed to NtCreateThreadEx", L"Injection Failed", NULL);
			return 1;
		}
		break;
	}

	//CloseHandle(remote_module);
	//CloseHandle(proc);
	return 0;
}

DWORD dllinject::_injectfpath(LPWSTR dllpath, HANDLE process, DWORD options) {
	DWORD dword;
	LPVOID funcptr = nullptr;
	HANDLE hthread = nullptr;

	void* rparams; //remote params
	// allocate dllpath buffer into remote process and write bytes into it  
	void* dllpath_raddr = VirtualAllocEx(process, 0, wcsbytes(dllpath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (dllpath_raddr == nullptr) {
			MessageBox(NULL, L"VirtualAllocEx Failed to allocate in target process!", L"Injection Failed", NULL);
			return 1;
	}
	int write_status = WriteProcessMemory(process, dllpath_raddr, dllpath, wcsbytes(dllpath), NULL);  // may need to put this in switch
	if (write_status == FALSE) {
		MessageBox(NULL, L"WriteProcessMemory Failed to write memory in target process!", L"Injection Failed", NULL);
		return 1;
	}

	switch (options & 0xffff) {
	case LOADLIBRARYEXW:
		
		funcptr = (LPVOID)GetProcAddress(LoadLibrary(L"kernel32"), "LoadLibraryW");
		if (funcptr == nullptr) {
			MessageBox(NULL, L"Failed to load LoadLibraryW", L"Injection Failed", NULL);
			return 1;
		}

		rparams = dllpath_raddr;

		
		
		break;
	case LDRLOADDLL:

		funcptr = (LPVOID)winutils::LdrLoadDllWrapper;
		
		winutils::LDR_PARAMS ldr_params;
		UNICODE_STRING ustr;

		//rparams = ;


		break;
	}

	switch (options & (0xffff << 16)) {
	case CREATEREMOTETHREADEX:
		hthread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcptr, rparams, 0, &dword);
		if (hthread == nullptr) {
			MessageBox(NULL, L"Failed to Create Remote Thread", L"Injection Failed", NULL);
			return 1;
		}
		break;
	case NTCREATETHREADEX:
		winutils::pNtCreateThreadEx NtCreateThreadEx = (winutils::pNtCreateThreadEx)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");
		NtCreateThreadEx(&hthread, 0x1FFFFF, NULL, process, (LPTHREAD_START_ROUTINE) funcptr, rparams, FALSE, NULL, NULL, NULL, NULL);
		if (hthread == nullptr) {
			MessageBox(NULL, L"Failed to NtCreateThreadEx", L"Injection Failed", NULL);
			return 1;
		}
		break;
	}

	CloseHandle(hthread);
    CloseHandle(process);

	return 0;
}

