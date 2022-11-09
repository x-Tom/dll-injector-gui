#include "WModuleListView.h"

BOOL WModuleListView::Update(){
    
    ClearItems();
    
    MODULEENTRY32 me32;
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(process));
    if (hModuleSnap == nullptr) {
        OutputDebugString(L"Module Snapshot Failed!");
        MessageBoxW(NULL, L"Module Snapshot Failed!", NULL, NULL);
    }
    
    auto ok = Module32First(hModuleSnap, &me32);
    if(!ok) {
        OutputDebugString(L"Module32First failed\n");
        MessageBoxW(NULL, L"Module32First Failed!", NULL, NULL);
        CloseHandle(hModuleSnap);
        return FALSE;
        
    }

   // int i = 0;
    std::wstring mod;
    std::wstring baseaddr;
    HICON icon;
    WORD pic;
	do {

        mod = me32.szModule;
        baseaddr = std::to_wstring((uintptr_t)me32.modBaseAddr);

        icon = ExtractIcon(hinst, me32.szExePath, 0);
        if (icon == nullptr) {
            OutputDebugString(L"icon load failed\n");
            icon = ExtractAssociatedIcon(hinst, me32.szExePath, &pic);
            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
        }

        AddItem(mod, baseaddr, icon); //i++;

    } while (Module32Next(hModuleSnap, &me32));
    
    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);

    return TRUE;
}
