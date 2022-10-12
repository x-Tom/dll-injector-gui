#pragma once
#include "GWindow.h"
#include "GChild.h"
class WMain : public GWindow
{
public:
	WMain(HINSTANCE, const std::wstring&, int w = 640, int h = 640);
	WMain() = default;
	~WMain() = default;

	bool Create() override;
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	bool ProcessMessage();
	
	void Kill();
	void MsgBox(const std::wstring&, const std::wstring&);
protected:
	int width;
	int height;
	const std::wstring title;

};

