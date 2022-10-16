#include "GWindow.h"

GWindow::GWindow(HINSTANCE hInst, LPCWSTR wcn, LPWNDCLASSEX pwc, int x, int y, int w, int h) : hwnd(nullptr), hinst(hInst), wndClassName(wcn), x(x), y(y), w(w), h(h)
{
    if (pwc != nullptr) {
        pwc->lpfnWndProc = GWindow::_WindowProcSetup;
        RegisterClassEx(pwc);
    }
}

GWindow::~GWindow()
{
	UnregisterClass(wndClassName, hinst);
}

LRESULT __stdcall GWindow::_WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const LPCREATESTRUCTW const pCreate = reinterpret_cast<LPCREATESTRUCTW>(lParam);
        GWindow* const pWnd = reinterpret_cast<GWindow*>(pCreate->lpCreateParams); // lpCreateParams takes this when window was created in constructor/init
        // sanity check
        assert(pWnd != nullptr);

        pWnd->hwnd = hWnd;

        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); //sets internal window custom user data to this
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&GWindow::_WindowProc)); // changes window procedure to the next static 
        // forward message to window class handler
        return pWnd->WindowProc(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT __stdcall GWindow::_WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve ptr to window class
    GWindow* const pWnd = reinterpret_cast<GWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->WindowProc(hWnd, msg, wParam, lParam);
}

bool GWindow::CreateChildren() {
    for (void* pchild : children) {
        if (pchild == nullptr) continue;
        ((GWindow*)pchild)->Create(hwnd);
    }
    // loop through children vec, if chidren vec has non nullptr element call create on it
    return true;
}

bool GWindow::Add(void* pchild) {
    children.push_back(pchild);
    return true;
}



BOOL GWindow::INIT_COMMON_CONTROLS() {
    INITCOMMONCONTROLSEX iccex;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccex.dwICC = ICC_LISTVIEW_CLASSES;
    return InitCommonControlsEx(&iccex);
}

HWND GWindow::Handle()
{
    return hwnd;
}

bool LoadBitmaps(){return true;}