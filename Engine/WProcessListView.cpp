#include "WProcessListView.h"

WProcessListView::WProcessListView(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::vector<std::wstring> cn, LONG styles) : GChild(hInst, WC_LISTVIEW, nullptr, id, x, y, w, h), styles(styles), column_names(cn)
{

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

    Update();

    return (hwnd != nullptr);
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
        lvc.pszText = column_names[iCol].c_str();
        lvc.cx = 100;               // Width of column in pixels.

        lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
        if (ListView_InsertColumn(hwnd, iCol, &lvc) == -1)
            return FALSE;
    }
    
    return TRUE;
} 


LRESULT WProcessListView::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
