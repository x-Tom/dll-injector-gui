#pragma once
#include "WProcessListView.h"
#include "winutils.h"

class WModuleListView : public WProcessListView {
public:
	using WProcessListView::WProcessListView;
	virtual BOOL InitListViewColumns() override;
	virtual BOOL InitImageList() override;
	virtual BOOL Create(HWND) override; 
	virtual BOOL Update() override;
	std::vector<int> column_widths = {200,100};
	HANDLE process = nullptr;
};

