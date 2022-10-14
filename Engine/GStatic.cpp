#include "GStatic.h"

GStatic::GStatic(HINSTANCE hInst, HMENU id, int x, int y, int w, int h) : GChild(hInst, WC_STATIC, nullptr, id, x, y, w, h)
{

}

LRESULT GStatic::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//return DefWindowProc(hWnd, msg, wParam, lParam);
	return 0;
};