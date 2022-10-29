#pragma once
#include "GChild.h"

class WProcessListView : public GChild {
public:
    WProcessListView(HINSTANCE, HMENU, int, int, int, int, std::vector<std::wstring>, LONG = ES_LEFT);
    virtual bool Create(HWND) override;

	WProcessListView() = default;
	~WProcessListView() = default;
	WProcessListView(const WProcessListView&) = delete;
	WProcessListView& operator=(const WProcessListView&) = delete;
protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	std::vector<std::wstring> list;
	LONG styles;
};

