#pragma once
using namespace DuiLib;
#define EVENT_TIEM_ID	100

class WebkitBrowserWnd;

class IWebkitBrowser
{
public:
	virtual void NavigationURL(const wchar_t* url) = 0;
	virtual void NavigationFile(const wchar_t* fileName) = 0;
	virtual void Refresh() = 0;
	virtual void Back() = 0;
	virtual void Forward() = 0;
	virtual void SetCookieEnabeled(bool enabled) = 0;
};

class WebkitBrowserUI : public DuiLib::CControlUI, public IWebkitBrowser
{
	friend class WebkitBrowserWnd;
public:
	WebkitBrowserUI();
	~WebkitBrowserUI();
	virtual LPCTSTR GetClass() const;
	void Init();
	virtual void SetPos(RECT rc);
	void DoEvent(TEventUI& event);
	void OnTimer(UINT_PTR idEvent);

	// ½Ó¿Ú
	virtual void NavigationURL(const wchar_t* url);
	virtual void NavigationFile(const wchar_t* fileName);
	virtual void Refresh();
	virtual void Back();
	virtual void Forward();
	virtual void SetCookieEnabeled(bool enabled);

	void Test();
private:
	WebkitBrowserWnd* pWebViewWnd;
};

