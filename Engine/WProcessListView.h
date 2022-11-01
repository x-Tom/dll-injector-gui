#pragma once
#include "GChild.h"

class WProcessListView : public GChild {
public:
    WProcessListView(HINSTANCE, HMENU, int, int, int, int, std::vector<std::wstring>, LONG);
    
	virtual bool Create(HWND) override;
	
	// bool Update();

	WProcessListView() = default;
	~WProcessListView() = default;
	WProcessListView(const WProcessListView&) = delete;
	WProcessListView& operator=(const WProcessListView&) = delete;
protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	
	std::vector<std::wstring> column_names;
	std::vector<std::wstring> item_names;

	std::vector<LVITEM> items;
	HIMAGELIST image_list;

	LONG styles;
};

