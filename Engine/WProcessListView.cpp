#include "WProcessListView.h"

WProcessListView::WProcessListView(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::vector<std::wstring> cn, LONG styles) : GChild(hInst, WC_LISTVIEW, nullptr, id, x, y, w, h), styles(styles), column_names(cn)
{

}

BOOL WProcessListView::Update(){
    
    ClearItems();
    
    HANDLE hProcessSnap;
    HANDLE hModuleSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
    MODULEENTRY32 me32;

    

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}

    std::wstring exe;
    std::wstring pid;
    HICON icon;
    WORD pic = 0;

	do {

        hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
        if (hModuleSnap == nullptr) OutputDebugString(L"Module Snapshot Failed!");
        auto ok = Module32First(hModuleSnap, &me32);
        if(!ok) {
            /*CloseHandle(hModuleSnap);
            return FALSE;*/
            OutputDebugString(L"Module32First failed\n");
        }

        exe = pe32.szExeFile;
        pid = std::to_wstring(pe32.th32ProcessID);
        HWND remote_process_main_window = winutils::find_main_window(pe32.th32ProcessID);
        HICON icon = (HICON)GetClassLong(remote_process_main_window, -14);

        //OutputDebugString((pid+L"\n").c_str());
        //icon = ExtractIcon(hinst, pe32.szExeFile, 0);
         //icon = ExtractIcon(hinst, pe32.szExeFile, 0);
        if (icon == nullptr) {
            icon = ExtractIcon(hinst, pe32.szExeFile, 0);
        }
        if (icon == nullptr) {
            OutputDebugString(L"icon load failed\n");
            icon = ExtractAssociatedIcon(hinst, me32.szExePath, &pic);
            if(icon == nullptr) OutputDebugString(L"icon load failed\n");
        }

        AddItem(exe, pid, icon);

    } while (Process32Next(hProcessSnap, &pe32));
    
    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);

    return TRUE;
}



BOOL WProcessListView::InitListViewColumns() 
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

BOOL WProcessListView::InitImageList(){
    if(image_list != nullptr) ImageList_Destroy(image_list);
    image_list = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK, MAX_PATH, MAX_PATH);
    //image_list = ImageList_Create(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), ILC_MASK, MAX_PATH, MAX_PATH);
    ListView_SetImageList(hwnd, image_list, LVSIL_SMALL); // call again later

    return TRUE;
}

void WProcessListView::ClearItems(){
    ListView_DeleteAllItems(hwnd);
    process_items.clear();
    index = 0;
}

BOOL WProcessListView::AddItem(std::wstring item, std::wstring subitem, HICON icon) 
{  
    // HICON icon = nullptr;
    
    PROCITEM procitem {index, item, subitem, icon};
    std::pair<std::wstring, PROCITEM> val = std::make_pair(item, procitem);
    auto [itr, ok] = process_items.insert(val);
    if(!ok) return FALSE;

    LVITEM lvI;
    LVITEM lvSubI;

    lvI.pszText   = &item[0];
    lvI.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem  = 0;
    lvI.state     = 0;
    lvI.iItem  = index;
    lvI.iImage = index;
    // Insert items into the list.
    if (ListView_InsertItem(hwnd, &lvI) == -1) return FALSE;
    
    lvSubI.pszText = &subitem[0];
    lvSubI.iSubItem = 1;
    lvSubI.mask = LVIF_TEXT;
    lvSubI.stateMask = 0;
    lvSubI.state = 0;
    lvSubI.iItem = index;
    lvSubI.iImage = index;

    if (ListView_SetItem(hwnd, &lvSubI) == -1) {
        OutputDebugString(L"Subitem insert failed!\n");
        //return FALSE;
    }

    ImageList_AddIcon(image_list, icon);
    index++;

    //ListView_SetImageList(hwnd, image_list, LVSIL_SMALL);
    return TRUE; 
} 

bool WProcessListView::Create(HWND prnt)
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
    Update();

    return (hwnd != nullptr);
}


LRESULT WProcessListView::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
