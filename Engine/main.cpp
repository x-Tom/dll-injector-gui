#include <iostream>
#include "WinHeader.h"
#include "WMain.h"
#include "WButton.h"
#include "WComboBox.h"
#include "WEdit.h"
#include "WText.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    GWindow::INIT_COMMON_CONTROLS();
    GWindow::INIT_GDIPLUS();

    int const width = MV_SCREENWIDTH/3.5, height = MV_SCREENHEIGHT/2.5;
    std::vector<std::wstring> inj1 = {L"LoadLibraryExW", L"ManualMap", L"LdrLoadDll",  L"LdrpLoadDll", L"LdrpLoadDllInternal" };
    std::vector<std::wstring> inj2 = { L"CreateRemoteThreadEx",L"NtCreateThreadEx", L"Thread Hijacking", L"SetWindowsHookEx", L"QueueUserAPC", L"KernelCallback"};
   
    HMENU id = (HMENU)100; // CREATE ID DEFINES IN WMain.h  

 


    WMain MainWindow(hInstance, L"Dynamic-link Library Injector", CW_USEDEFAULT, CW_USEDEFAULT, width, height);

    dllinject injector{};
    MainWindow.App(&injector);

    WButton Button(hInstance, BUTTONINJ, width-150, height-100, 100, 20, L"Inject", BS_PUSHBUTTON | BS_FLAT | WS_BORDER);
    WButton GroupBoxSettings(hInstance, GROUPBOX1, 5, 100, 300, 250, L"Settings", BS_GROUPBOX);
    WButton Radio1(hInstance, RADIO1, 15, 125, 80, 20, L"Process:", BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP);
    WButton Radio2(hInstance, RADIO2, 15, 155, 80, 20, L"PID:", BS_AUTORADIOBUTTON | BS_LEFTTEXT);
    
    WText Text(hInstance, (HMENU)111, 16, 185, 80, 20, L"DLL: ");
    //MainWindow.Text(L"DLL: ", 15, 185, 80, 20);

    WEdit Edit1(hInstance, EDIT1, 110, 124, 180, 20);
    WEdit Edit2(hInstance, EDIT2, 110, 154, 180, 20);
    WEdit Edit3(hInstance, EDIT2, 110, 184, 180, 20);

    WButton GroupBoxINJ(hInstance, GROUPBOX2, 15, 220, 200, 100, L"Method of Injection", BS_GROUPBOX);
    WComboBox ComboBox_INJ1(hInstance, COMBOBOX1, 25, 250, 180, 400, inj1, CBS_DROPDOWNLIST | CBS_HASSTRINGS);
    WComboBox ComboBox_INJ2(hInstance, COMBOBOX2, 25, 280, 180, 400, inj2, CBS_DROPDOWNLIST | CBS_HASSTRINGS);

    MainWindow.Add(&Button);
    MainWindow.Add(&Radio1);
    MainWindow.Add(&Radio2);
    MainWindow.Add(&Text);
    MainWindow.Add(&Edit1);
    MainWindow.Add(&Edit2);
    MainWindow.Add(&Edit3);
    MainWindow.Add(&GroupBoxSettings);
    MainWindow.Add(&GroupBoxINJ);
    MainWindow.Add(&ComboBox_INJ1);
    MainWindow.Add(&ComboBox_INJ2);

    std::wstring info = L"Width: " + std::to_wstring(width) + L"\nHeight: " + std::to_wstring(height);
    
    

    //731x576
    MainWindow.Create();
    //MainWindow.MsgBox(info, L"Info: ");
   
    MainWindow.Image(L"638x50final.png", (width-638)/2, 10, 638, 50);
    //MainWindow.Text(L"Process: ", 50, 50, 100, 100, RGB(0, 0, 0), L"Verdana", 10);
    //init child windows calls create of each child
           
    // need WM_RESIZE listener


    while (MainWindow.ProcessMessage()) {
        // Game loop here - Can abstract loop into app/engine class !!!
        // Window class needs lots of fleshing out, more features from api need to be implemented, messages boxes, HDC and Drawing etc etc, subwindows etc
        // Could write DLL Injector with WIN32 Window Api Class!!!
    }

    


    return 0;
};