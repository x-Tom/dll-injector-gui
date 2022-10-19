#pragma once
#include "GWindow.h"
#include "GChild.h"
class WMain : public GWindow
{
public:
	WMain(HINSTANCE, const std::wstring&, int = 200, int = 200, int = MV_SCREENWIDTH/3, int = MV_SCREENHEIGHT/2.5);
	WMain() = default;
	~WMain() = default;

	bool Create(HWND=nullptr) override;
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	bool ProcessMessage();
	
	void Kill();
	void MsgBox(const std::wstring&, const std::wstring&);
	void Text(std::wstring, int,int,int,int, COLORREF = RGB(255,255, 255), std::wstring = L"Segoe UI", int = 12);
	void Image(std::wstring, int, int, int, int);
	//virtual bool LoadBitmaps() override;
	//bool CreateChildren();
protected:

	const std::wstring title;
	

};

// ADD DRAW TEXT FUNCTION