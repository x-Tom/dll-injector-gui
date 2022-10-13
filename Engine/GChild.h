#pragma once

#include "GWindow.h"


class GChild : public GWindow
{
public:

	//static constexpr wchar_t wndClassName[] = L"EngineWndClass";
	GChild(HINSTANCE, LPCWSTR, LPWNDCLASSEX, HWND, HMENU, int, int, int, int);
	GChild() = default;
	~GChild() = default;
	virtual bool Create(HWND) = 0;
	GChild(const GChild&) = delete;
	GChild& operator=(const GChild&) = delete;


protected:
	HWND parent;
	HMENU id;

	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

