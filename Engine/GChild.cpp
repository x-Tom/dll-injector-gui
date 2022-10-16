#include "GChild.h"

GChild::GChild(HINSTANCE hInst, LPCWSTR wndclassname, LPWNDCLASSEX pwc, HMENU id, int x, int y, int w, int h) : GWindow(hInst, wndclassname, pwc, x, y, w, h), id(id), parent{}
{
	
}

void GChild::setParent(HWND p) {
    parent = p;
}

bool GChild::Create(HWND p){
    setParent(p);
    return true;
}