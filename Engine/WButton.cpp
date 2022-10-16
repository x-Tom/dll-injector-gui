#include "WButton.h"

WButton::WButton(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::wstring txt, LONG styles) : GChild(hInst, WC_BUTTON, nullptr, id, x, y, w, h), text(txt), styles(styles)
{
    
}

bool WButton::Create(HWND prnt){
    GChild::Create(prnt);
    hwnd = CreateWindowEx(0, WC_BUTTON, text.c_str(), WS_CHILD | WS_VISIBLE | styles, x, y, w, h, parent, id, hinst, nullptr);
    return true;
}

LRESULT WButton::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}
