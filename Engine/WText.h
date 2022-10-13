#pragma once
#include "GStatic.h"
class WText : public GStatic
{
public:
	WText(HINSTANCE, HWND, HMENU, int,int,int,int, std::wstring);
	WText() = default;
	~WText() = default;
	virtual bool Create() override;
	WText(const WText&) = delete;
	WText& operator=(const WText&) = delete;
private:
	std::wstring text;
};

