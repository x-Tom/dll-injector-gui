#pragma once
#include "WProcessListView.h"
#include "winutils.h"

class WModuleListView : public WProcessListView {
public:
	using WProcessListView::WProcessListView;
	virtual BOOL Update() override;
	std::vector<int> column_widths = {200,100};
	HANDLE process = nullptr;
};

