#include "GChild.h"

GChild::GChild(HINSTANCE hInst, LPCWSTR wndclassname, LPWNDCLASSEX pwc, HWND parent) : GWindow(hInst, wndclassname, pwc), parent(parent)
{
	
}
