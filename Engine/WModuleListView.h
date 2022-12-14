#pragma once
#include "WProcessListView.h"
#include "winutils.h"

class WModuleListView : public WProcessListView {
public:
	using WProcessListView::WProcessListView;
	virtual BOOL InitListViewColumns() override;
	virtual BOOL InitImageList() override;
	virtual bool Create(HWND) override; 
	virtual BOOL Update() override;
	std::vector<int> column_widths = {180,100};
	HANDLE process = nullptr;
};

