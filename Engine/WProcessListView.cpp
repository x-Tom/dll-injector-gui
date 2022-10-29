#include "WProcessListView.h"

WProcessListView::WProcessListView(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::vector<std::wstring> list, LONG styles) : GChild(hInst, WC_LISTVIEW, nullptr, id, x, y, w, h), styles(styles)
{

}

bool WProcessListView::Create(HWND prnt)
{
    hwnd = CreateWindow(WC_LISTVIEW,
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | styles,
        x, y, w, h,
        prnt,
        (HMENU)id,
        hinst,
        NULL);

    return (hwnd != nullptr);
}

LRESULT WProcessListView::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
