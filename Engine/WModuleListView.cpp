#include "WModuleListView.h"

BOOL WModuleListView::Update(){
    
    ClearItems();
    
    MODULEENTRY32 me32;
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
    if (hModuleSnap == nullptr) OutputDebugString(L"Module Snapshot Failed!");
    
    auto ok = Module32First(hModuleSnap, &me32);
    if(!ok) {
        CloseHandle(hModuleSnap);
        return FALSE;
        OutputDebugString(L"Module32First failed\n");
    }

   // int i = 0;
    std::wstring mod;
    std::wstring baseaddr;
    HICON icon;
	do {

        mod = me32.szModule;
        baseaddr = std::to_wstring(std::reinterpret_cast<uintptr_t>(me32.modBaseAddr));

        icon = ExtractIcon(hinst, me32.szExePath, 0);
        if (icon == nullptr) {
            OutputDebugString(L"icon load failed\n");
            icon = ExtractAssociatedIcon(hinst, me32.szExePath, &pic);
            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
        }

        AddItem(mod, baseaddr, icon); //i++;

    } while (Module32Next(hModuleSnap, &pe32));
    
    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);

    return TRUE;
}



