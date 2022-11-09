#include "winutils.h"


void* winutils::ProcFindFuncSwitch(LONG options) {
	void* function = nullptr;
	switch (options) {
	case PROCFIND::CREATETOOLHELP32SNAPSHOT:
		function = &_findProcess_ctlh32s;
		break;
	}
	return function;
};


HANDLE winutils::findProcess(DWORD pid, LONG options) {
	void* function = ProcFindFuncSwitch(options);
	return ((HANDLE(*)(const wchar_t*, DWORD))function)(nullptr, pid);
}

HANDLE winutils::findProcess(const wchar_t* processName, LONG options) {
	void* function = ProcFindFuncSwitch(options);
	return ((HANDLE(*)(const wchar_t*, DWORD))function)(processName, NULL);
}

HMODULE winutils::remoteModuleHandle(HANDLE process_handle, LPWSTR module_name) {
	DWORD bytesreq = 0;
	HMODULE module_array[1024] = { 0 };
	if (!EnumProcessModules(process_handle, module_array, sizeof(module_array), &bytesreq)) {
		DWORD err = GetLastError();
		OutputDebugString(std::to_wstring(err).c_str());
		return nullptr;
	}

	wchar_t mname_buffer[MAX_PATH] = {0};

	for (auto& hm : module_array) {
		if (!GetModuleBaseName(process_handle, hm, mname_buffer, sizeof(mname_buffer))) {
			DWORD err = GetLastError();
			OutputDebugString(std::to_wstring(err).c_str());
			return nullptr;
		}
		if (!wcscmp(mname_buffer, module_name)) return hm;
	}

	return nullptr;
}



HANDLE winutils::_findProcess_ctlh32s(const wchar_t* processName, const DWORD pid)
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


HWND winutils::find_main_window(DWORD process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}

BOOL CALLBACK winutils::enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}

BOOL winutils::is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0;
}

namespace winutils {

    bool SetDebugPrivileges()
    {
        HANDLE hProc = GetCurrentProcess();
        if (!hProc)
            return false;

        HANDLE hToken = nullptr;
        if (!OpenProcessToken(hProc, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
        {
            CloseHandle(hProc);
            return false;
        }

        CloseHandle(hProc);

        TOKEN_PRIVILEGES TokenPrivileges = { 0 };
        TokenPrivileges.PrivilegeCount = 1;
        TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid))
        {
            CloseHandle(hToken);
            return false;
        }

        if (!AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
        {
            CloseHandle(hToken);
            return false;
        }

        CloseHandle(hToken);

        return true;
    }


    PVOID QueryProcessInformation(IN HANDLE Process, IN PROCESSINFOCLASS ProcessInformationClass, IN DWORD ProcessInformationLength) {

        PPROCESS_BASIC_INFORMATION pProcessInformation = NULL;
        pfnNtQueryInformationProcess gNtQueryInformationProcess;
        ULONG ReturnLength = 0;
        NTSTATUS Status;
        HMODULE hNtDll;

        if (!(hNtDll = LoadLibrary(L"ntdll.dll"))) {
            OutputDebugString(L"Cannot load ntdll.dll.\n");
            return NULL;
        }

        if (!(gNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess"))) {
            OutputDebugString(L"Cannot load NtQueryInformationProcess.\n");
            return NULL;
        }

        // Allocate the memory for the requested structure
        if ((pProcessInformation = (PPROCESS_BASIC_INFORMATION)malloc(ProcessInformationLength)) == NULL) {
            OutputDebugString(L"ExAllocatePoolWithTag failed.\n");
            return NULL;
        }

        // Fill the requested structure
        if (!NT_SUCCESS(Status = gNtQueryInformationProcess(Process, ProcessInformationClass, pProcessInformation, ProcessInformationLength, &ReturnLength))) {
            OutputDebugString(L"NtQueryInformationProcess should return NT_SUCCESS (Status = %#x).\n");
            free(pProcessInformation);
            return NULL;
        }

        // Check the requested structure size with the one returned by NtQueryInformationProcess
        if (ReturnLength != ProcessInformationLength) {
            OutputDebugString(L"Warning : NtQueryInformationProcess ReturnLength is different than ProcessInformationLength\n");
            return NULL;
        }

        return pProcessInformation;
    }

    PPEB GetPebProcess(HANDLE Process) {
        PPROCESS_BASIC_INFORMATION pProcessInformation = NULL;
        DWORD ProcessInformationLength = sizeof(PROCESS_BASIC_INFORMATION);
        //HANDLE Process = GetCurrentProcess();
        PPEB pPeb = NULL;

        // ProcessBasicInformation returns information about the PebBaseAddress
        if ((pProcessInformation = (PPROCESS_BASIC_INFORMATION)QueryProcessInformation(Process, ProcessBasicInformation, ProcessInformationLength)) == NULL) {
            OutputDebugString(L"Handle=%x : QueryProcessInformation failed.\n");
            return NULL;
        }

        // Check the correctness of the value returned
        if (pProcessInformation->PebBaseAddress == NULL) {
            OutputDebugString(L"Handle=%x : PEB address cannot be found.\n");
            free(pProcessInformation);
            return NULL;
        }

        pPeb = pProcessInformation->PebBaseAddress;

        // Cleaning
        free(pProcessInformation);

        return pPeb;
    }


    BOOL CopyProcessMemory(HANDLE process, void* destination, void* source, size_t size){
        DWORD br;
        BYTE* buffer = (BYTE*)malloc(size);
        if(!ReadProcessMemory(process, source, buffer, sizeof(buffer), &br)){
            DWORD err = GetLastError();
            errmsg("RPM Failed");
            return FALSE;
        }
        if(!WriteProcessMemory(process, buffer, destination, sizeof(buffer), &br)){
            errmsg("WPM Failed");
            return FALSE;
        }
        free(buffer);
    }

    PMODULE_INFORMATION_TABLE CreateModuleInformation(IN PPEB pPeb, IN HANDLE process) {

        ULONG Count = 0;
        ULONG CurCount = 0;
        PLIST_ENTRY pEntry = NULL;
        PLIST_ENTRY pHeadEntry = NULL;
        PPEB_LDR_DATA pLdrData = NULL;
        PMODULE_ENTRY CurModule = NULL;
        PLDR_DATA_TABLE_ENTRY pLdrEntry = NULL;
        PMODULE_INFORMATION_TABLE pModuleInformationTable = NULL;
        BOOL ret;
        //Every pointer dereference requires instead a ReadProcessMemory as PEB is in remote process
        size_t br;
        PEB pebcopy = {0};
        ret = ReadProcessMemory(process, pPeb, &pebcopy, sizeof(PEB), &br);
        if(!ret) errmsg("RPM Failed");
        //pLdrData = pPeb->Ldr;
        pLdrData = pebcopy.Ldr;

        PEB_LDR_DATA ldr_data = {0};
        ret = ReadProcessMemory(process, pLdrData, &ldr_data, sizeof(PEB_LDR_DATA), &br);
        if(!ret) errmsg("RPM Failed");
        //pHeadEntry = &pPeb->Ldr->InMemoryOrderModuleList;
        //pHeadEntry = (PLIST_ENTRY)((uintptr_t)pLdrData + (uintptr_t)32);
        pHeadEntry = (PLIST_ENTRY)((uintptr_t)pLdrData + (uintptr_t)(&(PEB_LDR_DATA*)0)->InMemoryOrderModuleList);
        //pHeadEntry = (PLIST_ENTRY)CONTAINING_RECORD(pLdrData, PEB_LDR_DATA, InMemoryOrderModuleList);
        // Count user modules : iterate through the entire list
        LIST_ENTRY entry = ldr_data.InMemoryOrderModuleList;
        pEntry = entry.Flink;
        while (pEntry != pHeadEntry) { // Infinite LOOP
            Count++;
            ret = ReadProcessMemory(process, pEntry, &entry, sizeof(LIST_ENTRY), &br);
            if(!ret) errmsg("RPM Failed");
            pEntry = entry.Flink;
        }

        // Allocate a MODULE_INFORMATION_TABLE
        if ((pModuleInformationTable = (PMODULE_INFORMATION_TABLE)malloc(sizeof(MODULE_INFORMATION_TABLE))) == NULL) {
            OutputDebugString(L"Cannot allocate a MODULE_INFORMATION_TABLE.\n");
            return NULL;
        }

        // Allocate the correct amount of memory depending of the modules count
        if ((pModuleInformationTable->Modules = (PMODULE_ENTRY)malloc(Count * sizeof(MODULE_ENTRY))) == NULL) {
            OutputDebugString(L"Cannot allocate a MODULE_INFORMATION_TABLE.\n");
            return NULL;
        }

        // Fill the basic information of MODULE_INFORMATION_TABLE
        pModuleInformationTable->ModuleCount = Count;

        // Fill all the modules information in the table
        entry = ldr_data.InMemoryOrderModuleList;
        pEntry = entry.Flink;
        //pEntry = pHeadEntry->Flink;
        while (pEntry != pHeadEntry) // TODO: some readprocessmemory copying to do in here
        {
            // Retrieve the current MODULE_ENTRY
            CurModule = &pModuleInformationTable->Modules[CurCount++];

            // Retrieve the current LDR_DATA_TABLE_ENTRY
            pLdrEntry = CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);

            // MAY NEED TO BE REPLACED WITH RPM
            // Fill the MODULE_ENTRY with the LDR_DATA_TABLE_ENTRY information
            // RtlCopyMemory(&CurModule->BaseName, &pLdrEntry->BaseDllName, sizeof(CurModule->BaseName));
            // RtlCopyMemory(&CurModule->FullName, &pLdrEntry->FullDllName, sizeof(CurModule->FullName));
            // RtlCopyMemory(&CurModule->SizeOfImage, &pLdrEntry->SizeOfImage, sizeof(CurModule->SizeOfImage));
            // RtlCopyMemory(&CurModule->BaseAddress, &pLdrEntry->DllBase, sizeof(CurModule->BaseAddress));
            // RtlCopyMemory(&CurModule->EntryPoint, &pLdrEntry->EntryPoint, sizeof(CurModule->EntryPoint));
            ret = CopyProcessMemory(process, &CurModule->BaseName, &pLdrEntry->BaseDllName, sizeof(CurModule->BaseName));
            if(!ret) return nullptr;
            ret = CopyProcessMemory(process, &CurModule->FullName, &pLdrEntry->FullDllName, sizeof(CurModule->FullName));
            if(!ret) return nullptr;
            ret = CopyProcessMemory(process, &CurModule->SizeOfImage, &pLdrEntry->SizeOfImage, sizeof(CurModule->SizeOfImage));
            if(!ret) return nullptr;
            ret = CopyProcessMemory(process, &CurModule->BaseAddress, &pLdrEntry->DllBase, sizeof(CurModule->BaseAddress));
            if(!ret) return nullptr;
            ret = CopyProcessMemory(process, &CurModule->EntryPoint, &pLdrEntry->EntryPoint, sizeof(CurModule->EntryPoint));
            if(!ret) return nullptr;


            // Iterate to the next entry
            //pEntry = pEntry->Flink;
            ret = ReadProcessMemory(process, pEntry, &entry, sizeof(LIST_ENTRY), &br);
            if(!ret) errmsg("RPM Failed");
            pEntry = entry.Flink;
        }

        return pModuleInformationTable;
    }

    PMODULE_INFORMATION_TABLE QueryModuleInformationProcess(HANDLE Process) {
        PPEB pPeb = NULL;
        PMODULE_INFORMATION_TABLE pModuleInformationTable = NULL;

        // Read the PEB from th process
        if ((pPeb = GetPebProcess(Process)) == NULL) {
            OutputDebugString(L"GetPebCurrentProcess failed.\n");
            return NULL;
        }

        // Convert the PEB into a MODULE_INFORMATION_TABLE
        if ((pModuleInformationTable = CreateModuleInformation(pPeb, Process)) == NULL) {
            OutputDebugString(L"CreateModuleInformation failed.");
            return NULL;
        }

        return pModuleInformationTable;
    }
}