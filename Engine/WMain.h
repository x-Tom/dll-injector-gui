#pragma once
#include "GWindow.h"
#include "GChild.h"
#include "DLLInject.h"


#define BUTTONINJ (HMENU)0100
#define BUTTONFILE (HMENU)101
#define RADIO1    (HMENU)1000
#define RADIO2    (HMENU)1001
#define EDIT1	  (HMENU)2000
#define EDIT2     (HMENU)2001
#define GROUPBOX1 (HMENU)3000
#define GROUPBOX2 (HMENU)3001
#define COMBOBOX1 (HMENU)4000
#define COMBOBOX2 (HMENU)4001

class WMain : public GWindow
{
public:
	WMain(HINSTANCE, const std::wstring&, int = 200, int = 200, int = MV_SCREENWIDTH/3, int = MV_SCREENHEIGHT/2.5);
	WMain() = default;
	~WMain() = default;

	bool Create(HWND=nullptr) override;
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	bool ProcessMessage();
	
	void App(dllinject*);

	void Kill();
	void MsgBox(const std::wstring&, const std::wstring&);
	bool AddFont(std::wstring, int);
	void Text(std::wstring, int,int,int,int, COLORREF = RGB(255,255, 255), std::wstring = L"Segoe UI", int = 12);
	void Image(std::wstring, int, int, int, int);
	//virtual bool LoadBitmaps() override;
	//bool CreateChildren();
protected:

	dllinject* app;


	const std::wstring title;
	std::unordered_map<std::wstring, HFONT> fonts;
	const std::wstring gfontname = L"Verdana";
	const int gfontsize = 10;
	bool SetGlobalFont();
};

