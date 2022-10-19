#pragma once
#include "GChild.h"
class WComboBox : public GChild
{
public:
	WComboBox(HINSTANCE, HMENU, int, int, int, int, std::vector<std::wstring> list, LONG styles = CBS_DROPDOWNLIST | CBS_HASSTRINGS);
	WComboBox() = default;
	~WComboBox() = default;
	virtual bool Create(HWND) override;
	WComboBox(const WComboBox&) = delete;
	WComboBox& operator=(const WComboBox&) = delete;
private:
	std::vector<std::wstring> list;
	LONG styles;
};

