#pragma once

#include "WinHeader.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <SubAuth.h>
#include <winnt.h>
#include <iostream>
#include <string>

#define wcsbytes(x) (wcslen(x)+1)*sizeof(wchar_t)
#define errmsg(x) MessageBox(NULL, TEXT(x), NULL,NULL) 

namespace winutils {



    typedef NTSTATUS (NTAPI *pLdrLoadDll)(
        IN PWCHAR               PathToFile OPTIONAL,
        IN ULONG                Flags OPTIONAL,
        IN PUNICODE_STRING      ModuleFileName,
        OUT PHANDLE             ModuleHandle 
    );

    struct LDR_PARAMS {
        IN PWCHAR               PathToFile OPTIONAL,
        IN ULONG                Flags OPTIONAL,
        IN PUNICODE_STRING      ModuleFileName,
        OUT PHANDLE             ModuleHandle 
    }


	NTSTATUS NTAPI LdrLoadDllWrapper(LDR_PARAMS*);

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
    //BYTE = 1, PVOID = 8, LISTENTRY = 16 (2 pointers)
    // 8 bytes + 3 * 8 bytes = 32, 

    /* Windows structures */
    typedef struct _PEB_LDR_DATA {
        BYTE       Reserved1[8];
        PVOID      Reserved2[3];
        LIST_ENTRY InMemoryOrderModuleList;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _RTL_USER_PROCESS_PARAMETERS {
        UCHAR           Reserved1[16];
        PVOID           Reserved2[10];
        UNICODE_STRING  ImagePathName;
        UNICODE_STRING  CommandLine;
    } RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

    typedef struct _LDR_DATA_TABLE_ENTRY {
        LIST_ENTRY InLoadOrderLinks;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID DllBase;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG Flags;
        USHORT LoadCount;
        USHORT TlsIndex;
        union {
            LIST_ENTRY HashLinks;
            struct
            {
                PVOID SectionPointer;
                ULONG CheckSum;
            };
        };
        union {
            ULONG TimeDateStamp;
            PVOID LoadedImports;
        };
        PVOID EntryPointActivationContext;
        PVOID PatchInformation;
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

    typedef struct _PEB {
        BYTE                          Reserved1[2];
        BYTE                          BeingDebugged;
        BYTE                          Reserved2[1];
        PVOID                         Reserved3[2];
        PPEB_LDR_DATA                 Ldr;
        PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
        BYTE                          Reserved4[104];
        PVOID                         Reserved5[52];
        PVOID                         PostProcessInitRoutine;
        BYTE                          Reserved6[128];
        PVOID                         Reserved7[1];
        ULONG                         SessionId;
    } PEB, * PPEB;

    typedef struct _PROCESS_BASIC_INFORMATION {
        PVOID ExitStatus;
        PPEB PebBaseAddress;
        PVOID Reserved2[2];
        ULONG_PTR UniqueProcessId;
        PVOID Reserved3;
    } PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;

    typedef enum _PROCESSINFOCLASS {
        ProcessBasicInformation
        // We don't need the others
    } PROCESSINFOCLASS;

    // MODULE_ENTRY contains basic information about a module
    typedef struct _MODULE_ENTRY {
        UNICODE_STRING BaseName; // BaseName of the module
        UNICODE_STRING FullName; // FullName of the module
        ULONG SizeOfImage; // Size in bytes of the module
        PVOID BaseAddress; // Base address of the module
        PVOID EntryPoint; // Entrypoint of the module
    } MODULE_ENTRY, * PMODULE_ENTRY;

    // MODULE_INFORMATION_TABLE contains basic information about all the modules of a given process
    typedef struct _MODULE_INFORMATION_TABLE {
        ULONG Pid; // PID of the process
        ULONG ModuleCount; // Modules count for the above pointer
        PMODULE_ENTRY Modules; // Pointer to 0...* modules
    } MODULE_INFORMATION_TABLE, * PMODULE_INFORMATION_TABLE;

    typedef NTSTATUS(NTAPI* pfnNtQueryInformationProcess)(
        IN  HANDLE ProcessHandle,
        IN  PROCESSINFOCLASS ProcessInformationClass,
        OUT PVOID ProcessInformation,
        IN  ULONG ProcessInformationLength,
        OUT PULONG ReturnLength    OPTIONAL
    );

    void truncate_wstr_after_pattern(wchar_t* str,const wchar_t* pattern, size_t slen, size_t plen);

    bool SetDebugPrivileges();

    PVOID QueryProcessInformation(IN HANDLE Process, IN PROCESSINFOCLASS ProcessInformationClass, IN DWORD ProcessInformationLength);

    PPEB GetPebProcess(HANDLE Process);

    PMODULE_INFORMATION_TABLE CreateModuleInformation(IN PPEB pPeb, IN HANDLE process);

    PMODULE_INFORMATION_TABLE QueryModuleInformationProcess(HANDLE Process);



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