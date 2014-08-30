#include "stdafx.h"
#include "DateTime.h"

#define IDC_DATETIMEWND	1201

class DateTimeWnd : public DuiLib::CWindowWnd
{
	friend class DateTimeUI;
public:
	DateTimeWnd();
	virtual ~DateTimeWnd();
	void Init(DateTimeUI* pOnwer);
	virtual void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	DateTimeUI* m_pOnwer;
	CDateTimePickerCtrl m_DateTimeCtrl;
};

DateTimeWnd::DateTimeWnd()
	:m_pOnwer(nullptr)
{
}

DateTimeWnd::~DateTimeWnd()
{

}

LPCTSTR DateTimeWnd::GetWindowClassName() const
{
	return L"#32770";
}

void DateTimeWnd::OnFinalMessage(HWND hWnd)
{
	m_pOnwer->Invalidate();
	m_pOnwer = nullptr;
	delete this;
}

void DateTimeWnd::Init(DateTimeUI* pOnwer)
{
	this->m_pOnwer = pOnwer;
	RECT pos = pOnwer->GetPos();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	Create(pOnwer->GetManager()->GetPaintWindow(), nullptr, dwStyle, 0, pos);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	assert(m_hWnd);
}

LRESULT DateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:	OnCreate(uMsg, wParam, lParam); break;
	default:
		bHandled = FALSE;
		break;
	}
	if (!bHandled)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

void DateTimeWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtUI = {0};
	DWORD dwStyle = WS_CHILD | 
					WS_VISIBLE | 
					DTS_RIGHTALIGN | 
					DTS_LONGDATEFORMAT | 
					WS_TABSTOP;
	m_DateTimeCtrl.Create(this->m_hWnd, &rtUI, nullptr, dwStyle, 0, IDC_DATETIMEWND);
	RECT rtParent = m_pOnwer->GetPos();
	// 由于treeControl作为子窗口存在，附着于父窗口的刷新，所以这里不用SetWinodwPos而用MoveWindow
	::MoveWindow(m_DateTimeCtrl.m_hWnd, rtParent.left, rtParent.top, rtParent.right - rtParent.left,
		rtParent.bottom - rtParent.top, TRUE);
}


DateTimeUI::DateTimeUI()
	:m_pDateTimeWnd(nullptr),
	m_iFontIndex(-1)
{
	m_pDateTimeWnd = new DateTimeWnd;
	assert(m_pDateTimeWnd);
}

DateTimeUI::~DateTimeUI()
{
	m_pDateTimeWnd->PostMessage(WM_CLOSE);
}

LPCTSTR DateTimeUI::GetClass() const
{
	return L"TreeViewUI";
}

void DateTimeUI::Init()
{
	m_pDateTimeWnd->Init(this);
}

void DateTimeUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	::SetWindowPos(m_pDateTimeWnd->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	::SetWindowPos(m_pDateTimeWnd->m_DateTimeCtrl.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

void DateTimeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("font")) == 0 ) 
		SetFont(_ttoi(pstrValue));
	else
		__super::SetAttribute(pstrName, pstrValue);
}

void DateTimeUI::SetFont(int index)
{
	m_iFontIndex = index;
	HFONT hFont = this->m_pManager->GetFont(index);
	if (hFont)
	{
		SetDateTimeFont(hFont);
	}
	Invalidate();
}

void DateTimeUI::SetTimeFormat(LPCTSTR lpstrFormat)
{
	m_pDateTimeWnd->m_DateTimeCtrl.SetFormat(lpstrFormat);
}

void DateTimeUI::SetDateTimeFont(HFONT hFont)
{
	m_pDateTimeWnd->m_DateTimeCtrl.SetFont(hFont);
}

SYSTEMTIME DateTimeUI::GetCurSelectTime()
{
	SYSTEMTIME systemTime;
	m_pDateTimeWnd->m_DateTimeCtrl.GetSystemTime(&systemTime);
	return systemTime;
}

void DateTimeUI::SetCalStyle(DWORD dwStyle)
{
	m_pDateTimeWnd->m_DateTimeCtrl.SetMonthCalStyle(dwStyle);
}

DWORD DateTimeUI::GetCalStyle()
{
	return m_pDateTimeWnd->m_DateTimeCtrl.GetMonthCalStyle();
}