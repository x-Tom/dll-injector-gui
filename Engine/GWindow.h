#pragma once


#include "WinHeader.h"
#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <cassert>
#include <vector>
#include <unordered_map>

#define MV_SCREENHEIGHT GetSystemMetrics(SM_CYVIRTUALSCREEN)
#define MV_SCREENWIDTH GetSystemMetrics(SM_CXVIRTUALSCREEN)

class GWindow
{
public:
	GWindow(HINSTANCE,LPCWSTR,LPWNDCLASSEX, int, int, int, int);
	GWindow() = default;
	~GWindow();
	virtual bool Create(HWND) = 0; //create window
	virtual bool Add(void*);
	GWindow(const GWindow&) = delete;
	GWindow& operator=(const GWindow&) = delete;
	virtual bool LoadBitmaps();
	static BOOL INIT_COMMON_CONTROLS();
	static BOOL INIT_GDIPLUS();
	virtual HWND Handle();
	LPCWSTR GetClass();

protected:
	HWND hwnd;
	HINSTANCE hinst;
	LPCWSTR wndClassName;
	int x, y, w, h;
	
	std::vector<void*> children; //recast as GChild*?

	

	static LRESULT WINAPI _WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT WINAPI _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual bool CreateChildren();


};

