#include <iostream>
#include "WinHeader.h"
#include "WMain.h"
#include "WButton.h"
#include "WComboBox.h"
#include "WEdit.h"
#include "WText.h"
#include "WProcessListView.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    GWindow::INIT_COMMON_CONTROLS();
    GWindow::INIT_GDIPLUS();

    int const width = MV_SCREENWIDTH/3.5, height = MV_SCREENHEIGHT/2.5;
    std::vector<std::wstring> inj1 = {L"LoadLibraryExW", L"ManualMap", L"LdrLoadDll",  L"LdrpLoadDll", L"LdrpLoadDllInternal" };
    std::vector<std::wstring> inj2 = { L"CreateRemoteThreadEx",L"NtCreateThreadEx", L"Thread Hijacking", L"SetWindowsHookEx", L"QueueUserAPC", L"KernelCallback"};

    WMain MainWindow(hInstance, L"Dynamic-link Library Injector", CW_USEDEFAULT, CW_USEDEFAULT, width, height);

    dllinject injector{};
    MainWindow.App(&injector);

    WButton Button(hInstance, BUTTONINJ, width-150, height-100, 100, 20, L"Inject", BS_PUSHBUTTON | BS_FLAT | WS_BORDER);
    WButton GroupBoxSettings(hInstance, GROUPBOX1, 5, 100, 300, 250, L"Settings", BS_GROUPBOX);
    WButton Radio1(hInstance, RADIO1, 15, 125, 80, 20, L"Process:", BS_AUTORADIOBUTTON | BS_LEFTTEXT | WS_GROUP);
    WButton Radio2(hInstance, RADIO2, 15, 155, 80, 20, L"PID:", BS_AUTORADIOBUTTON | BS_LEFTTEXT);
    
    WText Text(hInstance, (HMENU)111, 16, 185, 27, 20, L"DLL:");
    

    WEdit Edit1(hInstance, EDIT1, 110, 124, 180, 20);
    WEdit Edit2(hInstance, EDIT2, 110, 154, 180, 20);
    WEdit Edit3(hInstance, EDIT3, 110, 184, 180, 20);

    WButton ButtonF(hInstance, BUTTONFILE, 50, 184, 55, 20, L"Choose", BS_PUSHBUTTON | BS_FLAT);

    WButton   GroupBoxINJ(hInstance, GROUPBOX2, 15, 220, 200, 100, L"Method of Injection", BS_GROUPBOX);
    WComboBox ComboBoxINJ1(hInstance, COMBOBOX1, 25, 250, 180, 400, inj1, CBS_DROPDOWNLIST | CBS_HASSTRINGS);
    WComboBox ComboBoxINJ2(hInstance, COMBOBOX2, 25, 280, 180, 400, inj2, CBS_DROPDOWNLIST | CBS_HASSTRINGS);

    std::vector<std::wstring> columns = {L"Process",L"PID"};
    WProcessListView ProcessList(hInstance, LISTVIEW1, width/2.2, 100, 350, 350, columns);

    MainWindow.Add(&Button);
    MainWindow.Add(&ButtonF);
    MainWindow.Add(&Radio1);
    MainWindow.Add(&Radio2);
    MainWindow.Add(&Text);
    MainWindow.Add(&Edit1);
    MainWindow.Add(&Edit2);
    MainWindow.Add(&Edit3);
    MainWindow.Add(&GroupBoxSettings);
    MainWindow.Add(&GroupBoxINJ);
    MainWindow.Add(&ComboBoxINJ1);
    MainWindow.Add(&ComboBoxINJ2);
    MainWindow.Add(&ProcessList);

    std::wstring info = L"Width: " + std::to_wstring(width) + L"\nHeight: " + std::to_wstring(height);
    
    

    //731x576
    MainWindow.Create();
   
    MainWindow.Image(L"638x50final.png", (width-638)/2, 10, 638, 50);
           
    // no resizing WM_RESIZE listener

    

    while (MainWindow.ProcessMessage());

    return 0;
};