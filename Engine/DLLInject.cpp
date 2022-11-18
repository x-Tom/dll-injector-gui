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
	LPWSTR dllname = (CONTAINING_RECORD(dllpath, OPENFILENAME, lpstrFile))->lpstrFileTitle;
	void* dllname_raddr = VirtualAllocEx(process, 0, wcsbytes(dllname), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (dllpath_raddr == nullptr) {
			MessageBox(NULL, L"VirtualAllocEx Failed to allocate in target process!", L"Injection Failed", NULL);
			return 1;
	}
	int ws = WriteProcessMemory(process, dllpath_raddr, dllpath, wcsbytes(dllpath), NULL);  // may need to put this in switch
	if (ws == FALSE) {
		MessageBox(NULL, L"WriteProcessMemory Failed to write memory in target process!", L"Injection Failed", NULL);
		return 1;
	}

	if (dllpath_raddr == nullptr) {
			MessageBox(NULL, L"VirtualAllocEx Failed to allocate in target process!", L"Injection Failed", NULL);
			return 1;
	}
	ws = WriteProcessMemory(process, dllname_raddr, dllname, wcsbytes(dllname), NULL);  // may need to put this in switch
	if (ws == FALSE) {
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

		// typedef struct _UNICODE_STRING {
		// 	USHORT Length;
		// 	USHORT MaximumLength;
		// 	PWSTR  Buffer;
		// } UNICODE_STRING, *PUNICODE_STRING;

		// struct LDR_PARAMS {
		// 	IN PWCHAR               PathToFile OPTIONAL,
		// 	IN ULONG                Flags OPTIONAL,
		// 	IN PUNICODE_STRING      ModuleFileName,
		// 	OUT PHANDLE             ModuleHandle 
    	// }

		// void

		PHANDLE rhmodule = VirtualAllocEx(process, 0, sizeof(HANDLE), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		UNICODE_STRING ustr;
		ustr.Length = wcsbytes(dllname) - sizeof(wchar_t);
		ustr.MaximumLength = wcsbytes(dllname);
		ustr.Buffer = dllname_raddr;


		void* ustr_raddr = VirtualAllocEx(process, 0, sizeof(ustr), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ustr_raddr == nullptr) {
			MessageBox(NULL, L"VirtualAllocEx Failed to allocate in target process!", L"Injection Failed", NULL);
			return 1;
		}
		ws = WriteProcessMemory(process, ustr_raddr, &ustr, sizeof(ustr), NULL);
		if (ws == FALSE) {
			MessageBox(NULL, L"WriteProcessMemory Failed to write memory in target process!", L"Injection Failed", NULL);
			return 1;
		}
		LDR_PARAMS ldr;
		ldr.PathToFile = dllpath_raddr;
		ldr.Flags = 0;
		ldr.ModuleFileName = ustr_raddr;
		ldr.ModuleHandle = rhmodule;
		
		void* ldr_raddr = VirtualAllocEx(process, 0, sizeof(ldr), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ldr_raddr == nullptr) {
			MessageBox(NULL, L"VirtualAllocEx Failed to allocate in target process!", L"Injection Failed", NULL);
			return 1;
		}
		ws = WriteProcessMemory(process, ldr_raddr, &ldr, sizeof(ldr), NULL);
		if (ws == FALSE) {
			MessageBox(NULL, L"WriteProcessMemory Failed to write memory in target process!", L"Injection Failed", NULL);
			return 1;
		}

		rparams = ldr_raddr;

		break;

	case MANUALMAP:



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

void dllinject::ManualMap(HANDLE process, LPCWSTR dllpath){
	BYTE* src_data = nullptr;
	IMAGE_NT_HEADERS* old_nt_header = nullptr;
	IMAGE_OPTIONAL_HEADER* old_opt_header = nullptr;
	IMAGE_FILE_HEADER* old_file_header = nullptr;
	BYTE* target_base = nullptr;
	//DWORD chk = 0;

	if(!GetFileAttributes(dllpath)) {
		errmsg("File doesnt exist");
		return;
	}

	std::ifstream DLLFileL(dllpath, std::ios::binary | std::ios::ate);
	if(DLLFile.fail()) {
		errmsg("File couldnt be opende for wrtie!");
		return;
	}

	auto size = DLLFile.tellg();

	if(size < 0x1000) {
		errmsg("File doesnt follow PE format and is invalid");
		DLLFile.close();
		return;
	}

	src_data = new BYTE[(uintptr_t)(size)];
	if(stc_date = nullptr) {
		errmsg("Allocation failed, not enough memory in current process");
		return;	
	}

	DLLFile.seekg(0, std::ios::beg);
	DLLFile.read((char*)src_data, size);
	DLLFile.close;

	if( ((IMAGE_DOS_HEADER*)src_data)->e_mageic != IMAGE_DOS_SIGNATURE){ // IMAGE_DOS_SIGNATURE == 5A4D == MZ
		errmsg("Not a PE file!");
		return;
	}
	old_nt_header = (IMAGE_NT_HEADERS*)(src_data + ((IMAGE_DOS_HEADER*)src_data)->e_lfanew);
	old_opt_header = &old_nt_header->OptionalHeader;
	old_file_header = &old_nt_header->FileHeader;

	if(old_nt_header->Signature != IMAGE_NT_SIGNATURE) {
		errmsg("Not x64!");
		return;
	}
	if(old_file_header->Machine != IMAGE_FILE_MACHINE_AMD64){
		errmsg("Not a PE file!");
		return;
	}

	target_base = (BYTE*)VirtualAllocEx(process, (void*)old_opt_header->ImageBase, old_opt_header->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(target_base == nullptr) target_base = (BYTE*)VirtualAllocEx(process, nullptr, old_opt_header->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(!target_base) {
		errmsg("Allocation in target failed!");
		delete[] src_data;
		return;
	}

	IMAGE_SECTION_HEADER* section_header = IMAGE_FIRST_SECTION(old_nt_header);
	for(uint32_t i = 0; i != old_file_header->NumberOfSections; i++, section_header++){
		if(!section_header->SizeOfRawData) continue;
		int ws = WriteProcessMemory(process, target_base + section_header->VirtualAddress, src_data + section_header->PointerToRawData, section_header->SizeOfRawData)
		if(!ws) {
			errmsg("Allocation in target failed!");
			delete[] src_data;
			VirtualFreeEx(process, target_base, MEM_RELEASE);
			return;
		}
	}


}
/*
#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

PE FILE:

	|  		DOS Header  	|
	|   	DOS Stub   		|
	|PE00|  File Header     |
	|     Optional Header   | -> ends with Data Directory table (offsets - https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_data_directory)
	|     Section Table     | -> Table of Section headers

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_NT_HEADERS64 {
  DWORD                   Signature;
  IMAGE_FILE_HEADER       FileHeader;
  IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_FILE_HEADER {
  WORD  Machine;
  WORD  NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD  SizeOfOptionalHeader;
  WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
  WORD                 Magic;
  BYTE                 MajorLinkerVersion;
  BYTE                 MinorLinkerVersion;
  DWORD                SizeOfCode;
  DWORD                SizeOfInitializedData;
  DWORD                SizeOfUninitializedData;
  DWORD                AddressOfEntryPoint;
  DWORD                BaseOfCode;
  ULONGLONG            ImageBase;
  DWORD                SectionAlignment;
  DWORD                FileAlignment;
  WORD                 MajorOperatingSystemVersion;
  WORD                 MinorOperatingSystemVersion;
  WORD                 MajorImageVersion;
  WORD                 MinorImageVersion;
  WORD                 MajorSubsystemVersion;
  WORD                 MinorSubsystemVersion;
  DWORD                Win32VersionValue;
  DWORD                SizeOfImage;
  DWORD                SizeOfHeaders;
  DWORD                CheckSum;
  WORD                 Subsystem;
  WORD                 DllCharacteristics;
  ULONGLONG            SizeOfStackReserve;
  ULONGLONG            SizeOfStackCommit;
  ULONGLONG            SizeOfHeapReserve;
  ULONGLONG            SizeOfHeapCommit;
  DWORD                LoaderFlags;
  DWORD                NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_SECTION_HEADER {
  BYTE  Name[IMAGE_SIZEOF_SHORT_NAME];
  union {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
  } Misc;
  DWORD VirtualAddress;
  DWORD SizeOfRawData;
  DWORD PointerToRawData;
  DWORD PointerToRelocations;
  DWORD PointerToLinenumbers;
  WORD  NumberOfRelocations;
  WORD  NumberOfLinenumbers;
  DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;


*/