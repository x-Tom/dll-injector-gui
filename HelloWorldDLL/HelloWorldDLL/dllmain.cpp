#define WIN32_LEAN_AND_MEAN
#include <windows.h>

DWORD WINAPI MessageBoxThread(LPVOID lpParam) {
    while (true) {
        MessageBox(NULL, L"Hello world!", L"Hello World!", MB_OK);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, MessageBoxThread, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
