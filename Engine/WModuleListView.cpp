#include "WModuleListView.h"

 //BOOL WModuleListView::Update(){
 //   
 //    ClearItems();
 //   
 //    MODULEENTRY32 me32;
 //    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(process));
 //    if (hModuleSnap == nullptr) {
 //        OutputDebugString(L"Module Snapshot Failed!");
 //        MessageBoxW(NULL, L"Module Snapshot Failed!", NULL, NULL);
 //    }
 //   
 //    auto ok = Module32First(hModuleSnap, &me32);
 //    if(!ok) {
 //        OutputDebugString(L"Module32First failed\n");
 //        MessageBoxW(NULL, L"Module32First Failed!", NULL, NULL);
 //        CloseHandle(hModuleSnap);
 //        return FALSE;
 //       
 //    }

 //   // int i = 0;
 //    std::wstring mod;
 //    std::wstring baseaddr;
 //    HICON icon;
 //    WORD pic;
 //	do {

 //        mod = me32.szModule;
 //        baseaddr = std::to_wstring((uintptr_t)me32.modBaseAddr);

 //        icon = ExtractIcon(hinst, me32.szExePath, 0);
 //        if (icon == nullptr) {
 //            OutputDebugString(L"icon load failed\n");
 //            icon = ExtractAssociatedIcon(hinst, me32.szExePath, &pic);
 //            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
 //        }

 //        AddItem(mod, baseaddr, icon); //i++;

 //    } while (Module32Next(hModuleSnap, &me32));
 //   
 //    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);

 //    return TRUE;
 //}


//BOOL WModuleListView::Update(){
//    
//    ClearItems();
//
//    std::wstring mod;
//    std::wstring baseaddr;
//    HICON icon;
//
//    WORD pic;
//    DWORD bytesreq = 0;
//	HMODULE module_array[1024] = { 0 };
//	if (!EnumProcessModules(process, module_array, sizeof(module_array), &bytesreq)) {
//		DWORD err = GetLastError();
//		OutputDebugString(std::to_wstring(err).c_str());
//        errmsg("EnumProcessModules Failed!");
//		return FALSE;
//	}
//
//    wchar_t mname_buffer[MAX_PATH] = {0};
//    wchar_t mpath_buffer[MAX_PATH] = {0};
//    MODULEINFO mi;
//
//
//	for (auto& hm : module_array) {
//		if (!GetModuleBaseName(process, hm, mname_buffer, sizeof(mname_buffer))) {
//			DWORD err = GetLastError();
//			OutputDebugString(std::to_wstring(err).c_str());
//            MessageBoxW(NULL, L"GetModuleBaseName failed", NULL, NULL);
//			return FALSE;
//		}
//        if (!GetModuleInformation(process, hm, &mi, sizeof(mi))) {
//			DWORD err = GetLastError();
//			OutputDebugString(std::to_wstring(err).c_str());
//            MessageBoxW(NULL, L"GetModuleInformation failed", NULL, NULL);
//
//			return FALSE;
//		}
//
//        if (!GetModuleFileNameEx(process, hm, mpath_buffer, sizeof(mpath_buffer))) {
//			DWORD err = GetLastError();
//			OutputDebugString(std::to_wstring(err).c_str());
//            MessageBoxW(NULL, L"GetModuleFileNameEx failed", NULL, NULL);
//
//			return FALSE;
//		}
//
//
//        icon = ExtractIcon(hinst, mpath_buffer, 0);
//        if (icon == nullptr) {
//            OutputDebugString(L"icon load failed\n");
//            icon = ExtractAssociatedIcon(hinst, mpath_buffer, &pic);
//            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
//        }
//
//
//        mod = mname_buffer;
//        baseaddr = std::to_wstring((uintptr_t)mi.lpBaseOfDll);
//        
//		AddItem(mod, baseaddr, icon);
//	}
//    
//    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);
//
//    return TRUE;
//}

BOOL WModuleListView::Update() {
    using namespace winutils;

    ClearItems();

    std::wstring mod;
    std::wstring fullmod;
    std::wstring baseaddr;
    HICON icon = nullptr;
    WORD pic = 0;

    MODULE_INFORMATION_TABLE* moduleTable = QueryModuleInformationProcess(process);
    if (moduleTable == nullptr) {
        errmsg("QueryModuleProcessInformationFailed");
        return FALSE;
    }

    //size_t moduleIndex;
    for (size_t moduleIndex = 0; moduleIndex < moduleTable->ModuleCount; moduleIndex++)
    {
        MODULE_ENTRY* moduleEntry = &moduleTable->Modules[moduleIndex];
        std::wstring mod(moduleEntry->BaseName.Buffer);
        std::wstring fullmod(moduleEntry->FullName.Buffer);
        std::wstring baseaddr(std::to_wstring((uintptr_t)moduleEntry->BaseAddress));

        icon = ExtractIcon(hinst, mod.c_str(), 0);
        if (icon == nullptr) {
            OutputDebugString(L"icon load failed\n");
            /*icon = ExtractAssociatedIcon(hinst, moduleEntry->FullName.Buffer, &pic);*/
            icon = ExtractAssociatedIcon(hinst, (LPWSTR)fullmod.c_str(), &pic);
            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
        }

        free(moduleEntry->BaseName.Buffer);
        free(moduleEntry->FullName.Buffer);

        AddItem(mod, baseaddr, icon);
    }


    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);

    return TRUE;
}


BOOL WModuleListView::InitListViewColumns() 
{ 
    //WCHAR szText[256];     // Temporary buffer.
    int const C_COLUMNS = column_names.size();
    LVCOLUMN lvc;
    int iCol;

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text,
    // and subitem members of the structure are valid.
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Add the columns.
    for (iCol = 0; iCol < C_COLUMNS; iCol++)
    {
        lvc.iSubItem = iCol;
        lvc.pszText = &column_names[iCol][0];
        (iCol) ? lvc.cx = column_widths[1] : lvc.cx = column_widths[0];               // Width of column in pixels.

        lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
        if (ListView_InsertColumn(hwnd, iCol, &lvc) == -1)
            return FALSE;
    }
    
    return TRUE;
} 

bool WModuleListView::Create(HWND prnt)
{
    hwnd = CreateWindow(WC_LISTVIEW,
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | LVS_REPORT | styles,
        x, y, w, h,
        prnt,
        (HMENU)id,
        hinst,
        NULL);

    InitListViewColumns();
    InitImageList();
    //Update();

    return (hwnd != nullptr);
}

BOOL WModuleListView::InitImageList(){
    if(image_list != nullptr) ImageList_Destroy(image_list);
    image_list = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK, MAX_PATH, MAX_PATH);
    //image_list = ImageList_Create(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), ILC_MASK, MAX_PATH, MAX_PATH);
    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL); // call again later

    return TRUE;
}
