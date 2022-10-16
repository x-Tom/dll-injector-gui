#include <iostream>
#include "WinHeader.h"
#include "WMain.h"
#include "WButton.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    GWindow::INIT_COMMON_CONTROLS();

    WMain MainWindow(hInstance, L"DLL Injector");
    //WText Header(hInstance, (HMENU)100, 10, 10, 150, 50, L"HELLO WORLD"); // main window has no handle create must 
    //MainWindow.Add(&Header);
    
    WButton Button(hInstance, (HMENU)100, 50, 50, 150, 50, L"Inject", BS_PUSHBUTTON);
    MainWindow.Add(&Button);
    MainWindow.Create();
    MainWindow.Text(L"DLL Injector", 10, 10, 150, 50);


    //init child windows calls create of each child
           
    // need WM_RESIZE listener


    while (MainWindow.ProcessMessage()) {
        // Game loop here - Can abstract loop into app/engine class !!!
        // Window class needs lots of fleshing out, more features from api need to be implemented, messages boxes, HDC and Drawing etc etc, subwindows etc
        // Could write DLL Injector with WIN32 Window Api Class!!!
    }

    


    return 0;
};