#pragma once

#include "GWindow.h"


class GChild : public GWindow
{
public:

	//static constexpr wchar_t wndClassName[] = L"EngineWndClass";
	GChild(HINSTANCE, LPCWSTR, LPWNDCLASSEX, HMENU, int, int, int, int);
	GChild() = default;
	~GChild() = default;
	virtual bool Create(HWND) override;
	GChild(const GChild&) = delete;
	GChild& operator=(const GChild&) = delete;
	HMENU GetID();

protected:
	HWND parent;
	HMENU id;


	void setParent(HWND);
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

