#pragma once

#include "WinHeader.h"
//#include <iostream>
//#include <map>
//#include <iterator>
//#include <string>
//#include <cstdio>
//#include <stdexcept>
//#include <cassert>

//class Window
//{
//public:
//	Window(HINSTANCE, const std::wstring&, int w = 640, int h = 640);
//	Window() = default;
//	~Window();
//	bool Init();
//	Window(const Window&) = delete;
//	Window& operator=(const Window&) = delete;
//	bool ProcessMessage();
//	void Kill();
//	void MsgBox(const std::wstring&, const std::wstring&);
//
//protected:
//	int width;
//	int height;
//	const std::wstring title;
//
//
//	// static constexpr wchar_t wndClassName[] = L"EngineWndClass";
//
//
//	static LRESULT WINAPI _WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//	static LRESULT WINAPI _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//	
//	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//	bool AddChildWnd(LPCWSTR wndclass, LPCWSTR wndname, int x, int y, int w, int h, DWORD styles, DWORD extrastyles, HWND hwndparent);
//
//
//
//	virtual void InitChildWindows() {};
//	virtual void RegisterChildClass() {};
//
//	virtual void ConfigureEnumChildWindows() {}; 
//	// MUST CREATE A LPARAMSTRUCT WITH ->self = this, and pass its address as an LPARAM to _EnumChildProcStatic!!!!!
//	// MUST PASS _EnumChildProcStatic into EnumChildWindows!!!!
//	virtual BOOL CALLBACK EnumChildProc(HWND, LPARAM) { return 0; };
//	static BOOL CALLBACK _EnumChildProcStatic(HWND, LPARAM); 
//
//	struct LPARAMSTRUCT {
//		LPARAM self, param1, param2; // Possibly create LPARAM vect to hold multiple arguments
//	};
//
//
//};

// Find a way to sort out child windows, 