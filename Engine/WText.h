#pragma once
#include "GStatic.h"
class WText : public GStatic
{
public:
	WText(HINSTANCE, HMENU, int,int,int,int, std::wstring);
	WText() = default;
	~WText() = default;
	virtual bool Create(HWND) override;
	WText(const WText&) = delete;
	WText& operator=(const WText&) = delete;
private:
	std::wstring text;
};

