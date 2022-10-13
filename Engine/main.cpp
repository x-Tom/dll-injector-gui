#include <iostream>
#include "WinHeader.h"
#include "WMain.h"
#include "WText.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    GWindow::initCC();

    WMain MainWindow(hInstance, L"hello");
    WText Header(hInstance, MainWindow.Handle(), (HMENU)100, 10, 10, 50, 50, L"HELLO WORLD"); // main window has no handle create must 
    MainWindow.Add(&Header);
    MainWindow.Create(NULL);

    //init child windows calls create of each child
           
    // need WM_RESIZE listener


    while (MainWindow.ProcessMessage()) {
        // Game loop here - Can abstract loop into app/engine class !!!
        // Window class needs lots of fleshing out, more features from api need to be implemented, messages boxes, HDC and Drawing etc etc, subwindows etc
        // Could write DLL Injector with WIN32 Window Api Class!!!
    }

    


    return 0;
};