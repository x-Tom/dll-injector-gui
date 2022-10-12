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
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

class GWindow
{
public:
	GWindow(HINSTANCE,LPCWSTR,LPWNDCLASSEX);
	GWindow() = default;
	~GWindow();
	virtual bool Create() = 0; //create window
	virtual bool Add(void*);
	GWindow(const GWindow&) = delete;
	GWindow& operator=(const GWindow&) = delete;
	
	static BOOL initCC();

protected:
	HWND hwnd;
	HINSTANCE hinst;
	LPCWSTR wndClassName;
	std::vector<void*> children; //recast as GChild*?

	

	static LRESULT WINAPI _WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT WINAPI _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual bool CreateChildren();


};

