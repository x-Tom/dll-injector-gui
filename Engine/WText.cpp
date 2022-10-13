#include "WText.h"

WText::WText(HINSTANCE hInst, HWND parent, HMENU ID, int x, int y, int w, int h, std::wstring text) : GStatic(hInst, parent, ID, x,y,w,h), text(text)
{

}

bool WText::Create() { // consider creating blankwindow and using GDI and HDC to draw text and set fonts?

	hwnd = CreateWindow(WC_STATIC, text.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER, x, y, w, h, parent, id, hinst, nullptr);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return (hwnd != nullptr) ? true : false;
}

