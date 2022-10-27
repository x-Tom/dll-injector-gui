#pragma once
#include "GChild.h"
class WEdit : public GChild {
public:
	WEdit(HINSTANCE, HMENU, int, int, int, int, LONG = ES_LEFT, BOOL = TRUE);
	WEdit() = default;
	~WEdit() = default;
	virtual bool Create(HWND) override;
	WEdit(const WEdit&) = delete;
	WEdit& operator=(const WEdit&) = delete;
protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	LONG styles;

};

