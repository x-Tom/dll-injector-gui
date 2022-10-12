#pragma once

#include "GWindow.h"


class GChild : public GWindow
{
public:

	//static constexpr wchar_t wndClassName[] = L"EngineWndClass";
	GChild(HINSTANCE, LPCWSTR, LPWNDCLASSEX, HWND);
	GChild() = default;
	~GChild() = default;
	virtual bool Create() = 0;
	GChild(const GChild&) = delete;
	GChild& operator=(const GChild&) = delete;

protected:
	HWND parent;

	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

