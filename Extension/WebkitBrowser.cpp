#include "stdafx.h"
#include "WebkitBrowser.h"
#include "wke.h"

class WebkitBrowserWnd : public DuiLib::CWindowWnd, protected wkeBufHandler
{
	friend class WebkitBrowserUI;
public:
	WebkitBrowserWnd();
	void Init(WebkitBrowserUI* pOwer);
	LPCTSTR GetWindowClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnFinalMessage(HWND hWnd);
	virtual void onBufUpdated(const HDC hdc, int x, int y, int cx, int cy);
	void OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnIMEStartCompostion(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	WebkitBrowserUI* pOwner;
	bool bIsInited;
	wkeWebView webView;
};

WebkitBrowserWnd::WebkitBrowserWnd() 
:pOwner(nullptr), bIsInited(false), webView(nullptr)
{

}

void WebkitBrowserWnd::OnFinalMessage(HWND hWnd)
{
	webView->destroy();
	delete this;
}

LPCTSTR WebkitBrowserWnd::GetWindowClassName() const
{
	//return L"WebkitBrowserWnd";
	return L"#32770";
}

void WebkitBrowserWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	webView = wkeCreateWebView();
	webView->setBufHandler(this);
	webView->setTransparent(false);
}

void WebkitBrowserWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcClip;
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(m_hWnd, &ps);
	::GetClipBox(hDc, &rcClip);
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	RECT rcInvalid;
	::IntersectRect(&rcInvalid, &rcClip, &rcClient);

	webView->paint(hDc, rcInvalid.left, rcInvalid.top,
		rcInvalid.right - rcInvalid.left, rcInvalid.bottom - rcInvalid.top,
		rcInvalid.left - rcClient.left,
		rcInvalid.top - rcClient.top, true);
	EndPaint(m_hWnd, &ps);
}

void WebkitBrowserWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDuiRect rcWnd;
	::GetWindowRect(m_hWnd, &rcWnd);
	webView->resize(rcWnd.GetWidth(), rcWnd.GetHeight());
	webView->tick();
	::InvalidateRect(m_hWnd, NULL, FALSE);
}

BOOL WebkitBrowserWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		SetFocus(m_hWnd);
		SetCapture(m_hWnd);
	}
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_RBUTTONUP)
	{
		ReleaseCapture();
	}

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	unsigned int flags = 0;

	if (wParam & MK_CONTROL)
		flags |= WKE_CONTROL;
	if (wParam & MK_SHIFT)
		flags |= WKE_SHIFT;

	if (wParam & MK_LBUTTON)
		flags |= WKE_LBUTTON;
	if (wParam & MK_MBUTTON)
		flags |= WKE_MBUTTON;
	if (wParam & MK_RBUTTON)
		flags |= WKE_RBUTTON;
	bHandled = webView->mouseEvent(uMsg, x, y, flags);
	return bHandled;
}

BOOL WebkitBrowserWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	unsigned int virtualKeyCode = wParam;
	unsigned int flags = 0;
	if (HIWORD(lParam) & KF_REPEAT)
		flags |= WKE_REPEAT;
	if (HIWORD(lParam) & KF_EXTENDED)
		flags |= WKE_EXTENDED;
	bHandled = webView->keyDown(virtualKeyCode, flags, false);
	return bHandled;
}

BOOL WebkitBrowserWnd::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	unsigned int virtualKeyCode = wParam;
	unsigned int flags = 0;
	if (HIWORD(lParam) & KF_REPEAT)
		flags |= WKE_REPEAT;
	if (HIWORD(lParam) & KF_EXTENDED)
		flags |= WKE_EXTENDED;
	bHandled = webView->keyUp(virtualKeyCode, flags, false);
	return bHandled;
}

BOOL WebkitBrowserWnd::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	unsigned int charCode = wParam;
	unsigned int flags = 0;
	if (HIWORD(lParam) & KF_REPEAT)
		flags |= WKE_REPEAT;
	if (HIWORD(lParam) & KF_EXTENDED)
		flags |= WKE_EXTENDED;
	bHandled = webView->keyPress(charCode, flags, false);
	return bHandled;
}

BOOL WebkitBrowserWnd::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	if (pt.x != -1 && pt.y != -1)
		ScreenToClient(m_hWnd, &pt);

	unsigned int flags = 0;

	if (wParam & MK_CONTROL)
		flags |= WKE_CONTROL;
	if (wParam & MK_SHIFT)
		flags |= WKE_SHIFT;

	if (wParam & MK_LBUTTON)
		flags |= WKE_LBUTTON;
	if (wParam & MK_MBUTTON)
		flags |= WKE_MBUTTON;
	if (wParam & MK_RBUTTON)
		flags |= WKE_RBUTTON;

	bHandled = webView->contextMenuEvent(pt.x, pt.y, flags);
	return bHandled;
}

BOOL WebkitBrowserWnd::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	ScreenToClient(m_hWnd, &pt);

	int delta = GET_WHEEL_DELTA_WPARAM(wParam);

	unsigned int flags = 0;

	if (wParam & MK_CONTROL)
		flags |= WKE_CONTROL;
	if (wParam & MK_SHIFT)
		flags |= WKE_SHIFT;

	if (wParam & MK_LBUTTON)
		flags |= WKE_LBUTTON;
	if (wParam & MK_MBUTTON)
		flags |= WKE_MBUTTON;
	if (wParam & MK_RBUTTON)
		flags |= WKE_RBUTTON;

	bHandled = webView->mouseWheel(pt.x, pt.y, delta, flags);
	return bHandled;
}

void WebkitBrowserWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	webView->focus();
}

void WebkitBrowserWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	webView->unfocus();
}

void WebkitBrowserWnd::OnIMEStartCompostion(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wkeRect caret = webView->getCaret();

	CANDIDATEFORM form;
	form.dwIndex = 0;
	form.dwStyle = CFS_EXCLUDE;
	form.ptCurrentPos.x = caret.x;
	form.ptCurrentPos.y = caret.y + caret.h;
	form.rcArea.top = caret.y;
	form.rcArea.bottom = caret.y + caret.h;
	form.rcArea.left = caret.x;
	form.rcArea.right = caret.x + caret.w;

	HIMC hIMC = ImmGetContext(m_hWnd);
	ImmSetCandidateWindow(hIMC, &form);
	ImmReleaseContext(m_hWnd, hIMC);
}

LRESULT WebkitBrowserWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	
	switch (uMsg)
	{
	case WM_CREATE:		OnCreate(uMsg, wParam, lParam); break;
	case WM_PAINT:		OnPaint(uMsg, wParam, lParam); break;
	case WM_SIZE:		OnSize(uMsg, wParam, lParam); break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:		bHandled = OnMouseMove(uMsg, wParam, lParam); break;
	case WM_KEYDOWN:		bHandled = OnKeyDown(uMsg, wParam, lParam); break;
	case WM_KEYUP:			bHandled = OnKeyUp(uMsg, wParam, lParam); break;
	case WM_CHAR:			bHandled = OnChar(uMsg, wParam, lParam); break;
	case WM_CONTEXTMENU:	bHandled = OnContextMenu(uMsg, wParam, lParam); break;
	case WM_MOUSEWHEEL:		bHandled = OnMouseWheel(uMsg, wParam, lParam); break;
	case WM_SETFOCUS:		OnSetFocus(uMsg, wParam, lParam); break;
	case WM_KILLFOCUS:		OnKillFocus(uMsg, wParam, lParam); break;
	case WM_IME_STARTCOMPOSITION: OnIMEStartCompostion(uMsg, wParam, lParam); break;
	default:
		bHandled = false;
		break;
	}

	if (!bHandled)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

void WebkitBrowserWnd::Init(WebkitBrowserUI* pOwer)
{
	this->pOwner = pOwer;
	RECT pos = pOwner->GetPos();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	Create(pOwner->GetManager()->GetPaintWindow(), nullptr, dwStyle, 0, pos);
	assert(m_hWnd);
}

void WebkitBrowserWnd::onBufUpdated(const HDC hdc, int x, int y, int cx, int cy)
{
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	RECT rcInvalid = { rcClient.left + x, rcClient.top + y, rcClient.left + x + cx, rcClient.top + y +cy};
	InvalidateRect(m_hWnd, &rcInvalid, TRUE);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
WebkitBrowserUI::WebkitBrowserUI() :pWebViewWnd(nullptr)
{
	pWebViewWnd = new WebkitBrowserWnd;
	assert(pWebViewWnd);
}

WebkitBrowserUI::~WebkitBrowserUI()
{
	pWebViewWnd->SendMessage(WM_CLOSE);
}

LPCTSTR WebkitBrowserUI::GetClass() const
{
	return L"WebkitBrowserUI";
}

void WebkitBrowserUI::Init()
{
	pWebViewWnd->Init(this);
	m_pManager->SetTimer(this, EVENT_TIEM_ID, 100);
}

void WebkitBrowserUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	::SetWindowPos(pWebViewWnd->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

void WebkitBrowserUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
		OnTimer((UINT_PTR)event.wParam);
}

void WebkitBrowserUI::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != EVENT_TIEM_ID)
		return;
	pWebViewWnd->webView->tick();
}

void WebkitBrowserUI::NavigationURL(const wchar_t* url)
{
	pWebViewWnd->webView->loadURL(url);
}

void WebkitBrowserUI::NavigationFile(const wchar_t* fileName)
{
	pWebViewWnd->webView->loadFile(fileName);
}

void WebkitBrowserUI::Back()
{
	if (pWebViewWnd->webView->canGoBack())
	{
		pWebViewWnd->webView->goBack();
	}
}

void WebkitBrowserUI::Forward()
{
	if (pWebViewWnd->webView->canGoForward())
	{
		pWebViewWnd->webView->goForward();
	}
}

void WebkitBrowserUI::Refresh()
{
	pWebViewWnd->webView->reload();
}

void WebkitBrowserUI::SetCookieEnabeled(bool enabled)
{
	pWebViewWnd->webView->setCookieEnabled(enabled);
}

void WebkitBrowserUI::Test()
{
	jsExecState es = pWebViewWnd->webView->globalExec();
	jsValue func = jsGetGlobal(es, "changeLabelText");
	jsCallGlobal(es, func, nullptr, 0);
}