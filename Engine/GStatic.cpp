#include "GStatic.h"

GStatic::GStatic(HINSTANCE hInst, HWND parent, HMENU id, int x, int y, int w, int h) : GChild(hInst, WC_STATIC, nullptr, parent, id, x, y, w, h)
{

}

LRESULT GStatic::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//return DefWindowProc(hWnd, msg, wParam, lParam);
	return 0;
};