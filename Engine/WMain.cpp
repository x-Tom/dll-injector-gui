#include "WMain.h"
#include "WText.h"
#include "WButton.h"


WMain::WMain(HINSTANCE hInst, const std::wstring& title, int x, int y, int w, int h) : GWindow(hInst, L"EngineMainWindow", nullptr, x, y, w, h), title(title) 
{
    WNDCLASSEX wc = { 0 };

    //COLORREF bgcolor = RGB(44, 44, 44);
    COLORREF bgcolor = RGB(255, 255, 255);
    //COLORREF bgcolor = RGB(31, 31, 31);

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = _WindowProcSetup; // 
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinst;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(bgcolor);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = wndClassName;
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);
}

bool WMain::Create(HWND)
{
    hwnd = CreateWindowEx(
        0, wndClassName,
        title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        x, y, w, h,
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
        SendMessage(hwnd, WM_CHANGEUISTATE, (WPARAM)MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
        SetGlobalFont();
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            switch (LOWORD(wParam)) {
            case (int)RADIO1: //set injector to inject by process name
                app->pnorid = true;
                //loop through children if id is EDIT1 set active, if id EDIT2 set inactive
                for (auto& chld : children) {
                    if (static_cast<GChild*>(chld)->GetID() == EDIT1) EnableWindow(static_cast<GChild*>(chld)->Handle(), TRUE);
                    if (static_cast<GChild*>(chld)->GetID() == EDIT2) EnableWindow(static_cast<GChild*>(chld)->Handle(), FALSE);
                }
                break;
            case (int)RADIO2: //set injector to inject by pid
                app->pnorid = false;
                //loop through children if id is EDIT1 set inactive, if id EDIT2 set active
                for (auto& chld : children) {
                    if (static_cast<GChild*>(chld)->GetID() == EDIT2) EnableWindow(static_cast<GChild*>(chld)->Handle(), TRUE);
                    if (static_cast<GChild*>(chld)->GetID() == EDIT1) EnableWindow(static_cast<GChild*>(chld)->Handle(), FALSE);
                }
                break;
            }
            break;
        case CBN_SELCHANGE:
            switch (LOWORD(wParam)) {
            case (int)COMBOBOX1:
                app->inj_load_idx = SendMessage((HWND)lParam, CB_GETCURSEL, NULL, NULL);
                break;
            case (int)COMBOBOX2:
                app->inj_exec_idx = SendMessage((HWND)lParam, CB_GETCURSEL, NULL, NULL);
                break;
            }
            break;
        }
        break;
    case WM_SIZE:
        //this->ConfigureEnumChildWindows();
        break;
    case WM_CTLCOLORSTATIC: // IDEALLY WE DONT WANT TO USE WC_STATIC CONTROL CHILD WINDOWS!!
        // void* pctl;
        // for(auto child : children){
        //     if(child.Handle() == lParam) pctl = child;
        // }

    {
        // pctl->Background((HDC) wParam);
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkMode(hdcStatic, TRANSPARENT);
        //return (INT_PTR)CreateSolidBrush(TRANSPARENT);
        //return (INT_PTR)GetSysColor(RGB(255,255,255));
        //return (LRESULT)GetStockObject(NULL_BRUSH);
        return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
        break;
    }
    case WM_CTLCOLORBTN:
        SetTextColor((HDC)wParam, RGB(255, 0, 0));
        SetBkColor((HDC)wParam, RGB(0, 0, 0));
        SetBkMode((HDC)wParam, TRANSPARENT);
        return(LRESULT)CreateSolidBrush(RGB(255, 255, 255));
        break;
    /*case WM_ERASEBKGND:
        return 1;
        break;*/
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

bool WMain::SetGlobalFont()
{
    AddFont(gfontname, gfontsize);
    AddFont(gfontname + L" Bold", gfontsize + 1);
    HFONT fntA = fonts.at(gfontname + L"\\" + std::to_wstring(gfontsize));
    HFONT fntB= fonts.at(gfontname + L" Bold" + L"\\" + std::to_wstring(gfontsize + 1));
    HFONT fnt;
   
    for (auto& child : children) {
        fnt = fntA;
        if (!lstrcmpW(WC_BUTTON, static_cast<GWindow*>(child)->GetClass())) {
            LONG bstyles = static_cast<WButton*>(child)->GetStyles();
            if ((bstyles & BS_GROUPBOX) == BS_GROUPBOX) fnt = fntB;
        }
            
            //fnt = fntB : fnt = fntA;
        SendMessage(static_cast<GWindow*>(child)->Handle(), WM_SETFONT, (LPARAM)fnt, (LPARAM)MAKELONG(TRUE, 0));
    }
    return false;
}

void WMain::Kill()
{
    PostQuitMessage(0);
}

void WMain::MsgBox(const std::wstring& title, const std::wstring& msg)
{
    MessageBox(hwnd, title.c_str(), msg.c_str(), MB_OK);
}

bool WMain::AddFont(std::wstring fontname, int fontsize)
{
    std::wstring tbhashed = fontname + L"\\" + std::to_wstring(fontsize);
    if (fonts.find(tbhashed) != fonts.end()) return false;

    HFONT font;
    LONG lfHeight;
    HDC hdc = GetDC(hwnd);
    lfHeight = -MulDiv(fontsize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, fontname.c_str());
    
    if (font != nullptr) {

        fonts[tbhashed] = font;
        return true;
    }
    else return false;
    
}

// find way to get call into wm_paint case in windowproc, add parameters and whether image or text into struct vect

void WMain::Text(std::wstring txt, int x, int y, int w, int h, COLORREF txtcolor, std::wstring fontname, int fontsize) // create init fonts function! initialising every call is gonna fuck shit up
{
    
    std::wstring tbhashed = fontname + L"\\" + std::to_wstring(fontsize);

    if (fonts.find(tbhashed) == fonts.end()) AddFont(fontname, fontsize);

    HFONT font = fonts.at(tbhashed);
    //HFONT font{};
    HDC hdc = GetDC(hwnd);
    //LONG lfHeight = -MulDiv(fontsize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    //font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, fontname.c_str());
    SelectObject(hdc, font);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, txtcolor);
    RECT txtbox = { x,y,w,h };
    DrawText(hdc, txt.c_str(), txt.length(), &txtbox, DT_LEFT );
}

void WMain::Image(std::wstring img, int x, int y, int w, int h){ //Change to BITMAPS if you want bmp resource to be embedded into EXE
    Gdiplus::Graphics graphics(GetDC(hwnd)); 
    Gdiplus::Image* image = new Gdiplus::Image(img.c_str(), TRUE);
    graphics.DrawImage(image, x, y, w, h);
}

//bool WMain::LoadBitmaps(){
//    LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
//}

//bool WMain::CreateChildren() {
//    for (void* pchild : children) {
//        if (pchild == nullptr) continue;
//        ((WText*)pchild)->Create();
//    }
//    // loop through children vec, if chidren vec has non nullptr element call create on it
//    return true;
//}


void WMain::App(dllinject* a){
    app = a;
}