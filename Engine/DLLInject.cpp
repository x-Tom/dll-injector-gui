#include "DLLInject.h"

dllinject::dllinject() : pnorid{ true }, inj_exec_idx{ 0 }, inj_load_idx{ 0 }, dllrpath{}, procname{}, procid{0} {
	//wchar_t* szFile = new wchar_t[MAX_PATH];
	//wchar_t* szFileTitle = new wchar_t[MAX_PATH];
	//HWND ohwnd = nullptr;

	//ZeroMemory(szFile, MAX_PATH);
	//ZeroMemory(szFileTitle, MAX_PATH);

	//ZeroMemory(&opfn, sizeof(opfn));
	//opfn.lStructSize = sizeof(opfn);
	//opfn.hwndOwner = ohwnd;
	//opfn.lpstrFile = szFile;
	//opfn.lpstrFile[0] = '\0';
	//opfn.nMaxFile = MAX_PATH;
	//opfn.lpstrFilter = L"DLL\0*.DLL\0\0";
	//opfn.nFilterIndex = 1;
	//opfn.lpstrFileTitle = szFileTitle;
	//opfn.nMaxFileTitle = MAX_PATH;
	//opfn.lpstrInitialDir = NULL;
	//opfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//opfninit();

}

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

	HANDLE proc = (pnorid) ? winapi::findProcess(procname.c_str()) : winapi::findProcess(procid);

	DWORD opts = 0;

	switch(inj_load_idx){
		case 0:
			opts |= LOADLIBRARYEXW;
			break;
	}

	switch(inj_exec_idx){
		case 0:
			opts |= CREATEREMOTETHREADEX;
			break;
	}

	return dllinject::_injectfpath(opfn.lpstrFile, proc, opts);
}

DWORD dllinject::_injectfpath(LPWSTR dllpath, HANDLE process, DWORD options) {
	void* dllpath_address = VirtualAllocEx(process, 0, wcslen(dllpath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); // may need to put this in switch
	int write_status = WriteProcessMemory(process, dllpath_address, dllpath, wcslen(dllpath), NULL);  // may need to put this in switch
	//if (!write_status) return 1;
	DWORD dword;
	LPVOID funcpointer = nullptr;
	HANDLE hthread;

	switch (options & 0xffff) {
	case LOADLIBRARYEXW:
		
		funcpointer = (LPVOID)GetProcAddress(LoadLibrary(L"kernel32"), "LoadLibraryW");
		if (funcpointer == nullptr) return 1;
		break;
	}

	switch (options & (0xffff << 16)) {
	case CREATEREMOTETHREADEX:

		hthread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcpointer, dllpath_address, 0, &dword);
		break;
	}


	
	
	//if()

	return 0;
}


DWORD dllinject::_injectrpath(std::wstring dll, HANDLE process, DWORD options) {
	wchar_t dllpath[100] = { 0 };
	GetFullPathName(dll.c_str(), 13, dllpath, NULL);
	void* dllpath_address = VirtualAllocEx(process, 0, wcslen(dllpath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); // may need to put this in switch
	int write_status = WriteProcessMemory(process, dllpath_address, dllpath, wcslen(dllpath), NULL);  // may need to put this in switch
	//if (!write_status) return 1;
	DWORD dword;
	LPVOID funcpointer = nullptr;
	HANDLE hthread;

	switch (options & 0xffff) {
	case LOADLIBRARYEXW:
		
		funcpointer = (LPVOID)GetProcAddress(LoadLibrary(L"kernel32"), "LoadLibraryW");
		if (funcpointer == nullptr) return 1;
		break;
	}

	switch (options & (0xffff << 16)) {
	case CREATEREMOTETHREADEX:

		hthread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcpointer, dllpath_address, 0, &dword);
		break;
	}


	
	
	//if()

	return 0;
}

//void dllinject::INJLoadSet(int i){
//	inj_load_idx = i;
//}
//void dllinject::INJExecSet(int i){
//	inj_exec_idx = i;
//}
//int  dllinject::INJLoadGet(){
//	return inj_load_idx;
//}
//int  dllinject::INJExecGet() {
//	return inj_exec_idx;
//}
