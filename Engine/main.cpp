#include <iostream>
#include "WinHeader.h"
#include "WMain.h"
#include "WButton.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    GWindow::INIT_COMMON_CONTROLS();
    GWindow::INIT_GDIPLUS();

    int const width = MV_SCREENWIDTH/3.5, height = MV_SCREENHEIGHT/2.5;

    WMain MainWindow(hInstance, L"DLL Injector", CW_USEDEFAULT, CW_USEDEFAULT, width, height);
    //WText Header(hInstance, (HMENU)100, 10, 10, 150, 50, L"HELLO WORLD"); // main window has no handle create must 
    //MainWindow.Add(&Header);
    
    WButton Button(hInstance, (HMENU)100, width-150, height-100, 100, 20, L"Inject", BS_PUSHBUTTON | BS_FLAT | WS_BORDER);
    MainWindow.Add(&Button);
    MainWindow.Create();
    //MainWindow.Text(L"DLL injection is a technique used for executing code within the address space\nof another process by forcing it to load a dynamic-link library.", 10, height/4, width, 200, RGB(0,0,0), L"Verdana", 10);
    MainWindow.Image(L"638x50final.png", (width-638)/2, 10, 638, 50);

    //init child windows calls create of each child
           
    // need WM_RESIZE listener


    while (MainWindow.ProcessMessage()) {
        // Game loop here - Can abstract loop into app/engine class !!!
        // Window class needs lots of fleshing out, more features from api need to be implemented, messages boxes, HDC and Drawing etc etc, subwindows etc
        // Could write DLL Injector with WIN32 Window Api Class!!!
    }

    


    return 0;
};