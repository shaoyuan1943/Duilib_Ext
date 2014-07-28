#pragma once
#ifndef _UICONTROLEXTEND_H_
#define _UICONTROLEXTEND_H_

using namespace DuiLib;
class IconControlUI : public CButtonUI
{
public:
	IconControlUI();
	~IconControlUI();
public:
	virtual void PaintBkImage(HDC hDC);
public:
	void SetIcon(HICON hIcon);
	void SetBitmap(HBITMAP hBmp);
	void SetSelect(bool bSelect);
	void SetDrawMask(bool bDraw);

	HBITMAP	GetBitmap();

	void SetIconSize(int cxWidth);
	int GetIconSize();
private:
	HICON	m_hIcon;
	HBITMAP m_hBmp;
	bool	m_bSelect;
	bool	m_bDrawMask;
	static  HBITMAP	m_hMaskBmp;
	static	HRGN	m_hMaskRgn;
	int icxWidth;
};

class IconCallBackUI : public IDialogBuilderCallback
{
public:
	IconCallBackUI();
	~IconCallBackUI();
public:
	virtual CControlUI* CreateControl(LPCTSTR pstrClass); 
	LPCTSTR ReturnControlName();
};

#endif //_UICONTROLEXTEND_H_