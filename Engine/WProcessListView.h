#pragma once
#include "GChild.h"
#include "winutils.h"

struct PROCITEM {
	unsigned int index;
	std::wstring item_name;
	std::wstring subitem_name;
	HICON icon;
};

class WProcessListView : public GChild {
public:
    WProcessListView(HINSTANCE, HMENU, int, int, int, int, std::vector<std::wstring>, LONG = 0);
    
	virtual bool Create(HWND) override;
	BOOL Update();

	BOOL InitListViewColumns();
	BOOL InitImageList();
	void ClearItems();
	BOOL AddItem(std::wstring, std::wstring, HICON);
	// bool Update();

	WProcessListView() = default;
	~WProcessListView() = default;
	WProcessListView(const WProcessListView&) = delete;
	WProcessListView& operator=(const WProcessListView&) = delete;

	std::unordered_map<std::wstring, PROCITEM> process_items;
protected:
	int index = 0;
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	std::vector<std::wstring> column_names;
	HIMAGELIST image_list;
	LONG styles;
};

