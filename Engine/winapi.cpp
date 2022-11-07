#include "winapi.h"


void* winapi::ProcFindFuncSwitch(LONG options) {
	void* function = nullptr;
	switch (options) {
	case PROCFIND::CREATETOOLHELP32SNAPSHOT:
		function = &_findProcess_ctlh32s;
		break;
	}
	return function;
};


HANDLE winapi::findProcess(DWORD pid, LONG options) {
	void* function = ProcFindFuncSwitch(options);
	return ((HANDLE(*)(const wchar_t*, DWORD))function)(nullptr, pid);
}

HANDLE winapi::findProcess(const wchar_t* processName, LONG options) {
	void* function = ProcFindFuncSwitch(options);
	return ((HANDLE(*)(const wchar_t*, DWORD))function)(processName, NULL);
}





HANDLE winapi::_findProcess_ctlh32s(const wchar_t* processName, const DWORD pid)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	if (processName == nullptr && pid != NULL) {
		do {

			if (pe32.th32ProcessID == pid) {

				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (hProcess != NULL) {
					return hProcess;
				}
				else {
					return NULL;
				}
			}

		} while (Process32Next(hProcessSnap, &pe32));
	}
	else if (processName != nullptr && pid == NULL) {
		do {

			if (!wcscmp(pe32.szExeFile, processName)) {

				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (hProcess != NULL) {
					return hProcess;
				}
				else {
					return NULL;
				}
			}

		} while (Process32Next(hProcessSnap, &pe32));
	}
	else return NULL;

	

	return NULL;
}


HWND winapi::find_main_window(DWORD process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}

BOOL CALLBACK winapi::enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}

BOOL winapi::is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0;
}