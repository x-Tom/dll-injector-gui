#include "GChild.h"

GChild::GChild(HINSTANCE hInst, LPCWSTR wndclassname, LPWNDCLASSEX pwc, HWND parent, HMENU id, int x, int y, int w, int h) : GWindow(hInst, wndclassname, pwc, x, y, w, h), parent(parent), id(id)
{
	
}

