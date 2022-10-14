class WButton : public GChild {
public:
    WButton(HINSTANCE, HMENU, int, int, int, int);
	WButton() = default;
	~WButton() = default;
	virtual bool Create(HWND) override;
	WButton(const WButton&) = delete;
	WButton& operator=(const WButton&) = delete;

protected:
	virtual LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

}