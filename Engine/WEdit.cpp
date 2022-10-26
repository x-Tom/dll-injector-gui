#include "WEdit.h"

WEdit::WEdit(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, LONG styles) : GChild(hInst, WC_COMBOBOX, nullptr, id, x, y, w, h), styles(styles)
{

}

bool WEdit::Create(HWND prnt)
{
    hwnd = CreateWindow(WC_EDIT,
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | styles,
        x, y, w, h,
        prnt,
        (HMENU)id,
        hinst,
        NULL);

    return (hwnd != nullptr);
}

LRESULT WEdit::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
