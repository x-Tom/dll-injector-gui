WButton::WButton(HINSTANCE hInst, HMENU id, int x, int y, int w, int h, std::wstring txt, ) : GChild(hInst, WC_BUTTON, nullptr, id, x, y, w, h)
{
    
}

bool WButton::Create(HWND prnt){
    GChild::Create(prnt);
    hwnd = CreateWindowEx(0, WC_BUTTON, WS_CHILD | WS_VISIBLE | styles, x,y,w,h,prnt, id, hinst, nullptr)
    
}