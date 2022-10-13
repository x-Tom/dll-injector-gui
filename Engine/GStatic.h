#include "GChild.h"


class GStatic : public GChild {
public:
	GStatic(HINSTANCE, HWND, HMENU, int, int, int, int);
	GStatic() = default;
	~GStatic() = default;
	virtual bool Create() = 0;
	GStatic(const GStatic&) = delete;
	GStatic& operator=(const GStatic&) = delete;

protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

};