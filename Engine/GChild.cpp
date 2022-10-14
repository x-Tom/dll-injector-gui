#include "GChild.h"

GChild::GChild(HINSTANCE hInst, LPCWSTR wndclassname, LPWNDCLASSEX pwc, HMENU id, int x, int y, int w, int h) : GWindow(hInst, wndclassname, pwc, x, y, w, h), id(id)
{
	
}

GChild::setParent(HWND p) {
    parent = parent;
}

GChild::Create(HWND p){
    setParent(p);
    return true;
}