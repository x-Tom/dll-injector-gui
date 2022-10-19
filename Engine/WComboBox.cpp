#include "WComboBox.h"

WComboBox::WComboBox(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::vector<std::wstring> list, LONG styles) : GChild(hInst, WC_COMBOBOX, nullptr, id, x, y, w, h), list(list), styles(styles)
{

}

bool WComboBox::Create(HWND parent)
{
	hwnd = CreateWindow(WC_COMBOBOX, L"", styles | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, x, y, w, h, parent, NULL, hinst, NULL);
	//SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	return true;
}
