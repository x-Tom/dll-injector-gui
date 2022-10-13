#include "WMain.h"

WMain::WMain(HINSTANCE hInst, const std::wstring& title, int x, int y, int w, int h) : GWindow(hInst, L"EngineMainWindow", nullptr, x, y, w, h), title(title) 
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = _WindowProcSetup; // 
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinst;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = wndClassName;
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);
}

bool WMain::Create()
{
    hwnd = CreateWindowEx(
        0, wndClassName,
        title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        x, y, h, w,
        nullptr, nullptr, hinst, this // pass in this to WinAPI so it can get access to member function from static function
    );

    if (hwnd == nullptr) {
        throw "Something went very wrong";
    };

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

LRESULT WMain::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Kill();
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_CREATE:
        //CreateWindowEx(0, WC_STATIC, L"Hello WOrld", WS_CHILD | WS_VISIBLE, 10, 10, 50, 50, hwnd, (HMENU)100, hinst, nullptr);
        CreateChildren();
        break;
    case WM_SIZE:
        //this->ConfigureEnumChildWindows();
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool WMain::ProcessMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // send message to Window Message Handler Procedure, WindowProc

        if (msg.message == WM_QUIT)
        {
            return false;
            //exit(0);
        }
    }
    return true;
}

void WMain::Kill()
{
    PostQuitMessage(0);
}

void WMain::MsgBox(const std::wstring& title, const std::wstring& msg)
{
    MessageBox(hwnd, title.c_str(), msg.c_str(), MB_OK);
}
