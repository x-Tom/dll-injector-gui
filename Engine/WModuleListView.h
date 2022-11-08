#pragma once
#include "WProcessListView.h"
#include "winutils.h"

class WModuleListView : public WProcessListView {
public:
	using WProcessListView::WProcessListView;
	using module_items = process_items;
	virtual BOOL Update() override;
	std::vector<int> column_widths = {100,100};
	HANDLE process = nullptr;
};

