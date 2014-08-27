#pragma once
#include "./wtl/atlapp.h"
#include "./wtl/atlctrls.h"
#include "./wtl/atluser.h"

using namespace DuiLib;

class DateTimeWnd;
class DateTimeUI : public DuiLib::CControlUI
{
	friend class DateTimeWnd;
public:
	DateTimeUI();
	virtual ~DateTimeUI();
	virtual LPCTSTR GetClass() const;
	virtual void Init();
	virtual void SetPos(RECT rc);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetFont(int index);

	void SetTimeFormat(LPCTSTR lpstrFormat);
	SYSTEMTIME GetCurSelectTime();

	/*
		MCS_WEEKNUMBERS
		MCS_NOTODAY 
		MCS_SHORTDAYSOFWEEK
	*/
	void SetCalStyle(DWORD dwStyle);
	DWORD GetCalStyle();
private:
	void SetDateTimeFont(HFONT hFont);
private:
	DateTimeWnd* m_pDateTimeWnd;
	int m_iFontIndex;
};