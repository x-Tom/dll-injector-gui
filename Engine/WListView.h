#pragma once
#include "GChild.h"
class WListView : public GChild {
public:
    WListView(HINSTANCE, HMENU, int, int, int, int, LONG = ES_LEFT);
    virtual bool Create(HWND) override;

	WListView() = default;
	~WListView() = default;
	WListView(const WListView&) = delete;
	WListView& operator=(const WListView&) = delete;
protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	LONG styles;
};

