#include "..\stdafx.h"
#include "IconControl.h"

HBITMAP IconControlUI::m_hMaskBmp = NULL;
HRGN	IconControlUI::m_hMaskRgn = NULL;

HRGN BitmapToRgn(HBITMAP hBitmap, COLORREF col)
{
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	HDC hMemDc = CreateCompatibleDC(NULL);
	HBITMAP hOld = (HBITMAP)SelectObject(hMemDc, hBitmap);
	HRGN hGoal = CreateRectRgn(0, 0, bm.bmWidth, bm.bmHeight);
	for(int x = 0; x <= bm.bmWidth; x++)
	{
		for(int y = 0; y <= bm.bmHeight; y++)
		{
			COLORREF clrPixel = GetPixel(hMemDc, x, y);
			if(clrPixel < col)
			{
				HRGN rTemp = CreateRectRgn(x, y, x + 1, y + 1);
				CombineRgn(hGoal, hGoal, rTemp, RGN_XOR);
				DeleteObject(rTemp);
			}
		}
	}
	SelectObject(hMemDc,hOld);
	DeleteDC(hMemDc);
	return hGoal;
}

IconControlUI::IconControlUI()
{
	m_hIcon = NULL;
	m_hBmp = NULL;
	m_bSelect = false;
	m_bDrawMask = false;
	icxWidth = 32;
}

IconControlUI::~IconControlUI()
{
	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}
	if (m_hBmp)
	{
		::DeleteObject(m_hBmp);
		m_hBmp = NULL;
	}
	m_bSelect = false;
}

void IconControlUI::SetIconSize(int cxWidth)
{
	if ( cxWidth != 0)
	{
		icxWidth = cxWidth;
	}
}

int IconControlUI::GetIconSize()
{
	return icxWidth;
}

void IconControlUI::PaintBkImage(HDC hDC)
{
	if (m_bSelect)
	{
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(0xff99CCFF));
	}
	if (NULL == m_hMaskBmp)
	{
		CPaintManagerUI*	pManager = m_pManager;
		const TImageInfo* pInfo = pManager->AddImage(L"mask.bmp", 0, 0);
		if (pInfo)
		{
			m_hMaskBmp = pInfo->hBitmap;
			m_hMaskRgn = BitmapToRgn(m_hMaskBmp, RGB(240, 240, 240));
		}
	}
	if (m_hBmp)
	{
		BITMAP bm;
		GetObject(m_hBmp, sizeof(bm), &bm);
		int nTop = (m_rcItem.bottom + m_rcItem.top - bm.bmHeight)/2;
		int	nLeft = (m_rcItem.right + m_rcItem.left - bm.bmWidth)/2;
		if (m_bDrawMask)
		{
			HDC hMemDC = CreateCompatibleDC(hDC);
			if (hMemDC)
			{
				HBITMAP hBitmap = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
				if (hBitmap)
				{
					SelectObject(hMemDC, hBitmap);
					::DrawState(hMemDC, NULL, NULL, (LPARAM)m_hBmp, 0, 0, 0, bm.bmWidth, bm.bmHeight, DST_BITMAP|DSS_NORMAL);
					for (int i = 0; i < bm.bmWidth; ++i)
					{
						for (int j = 0; j < bm.bmHeight; ++j)
						{
							if (!PtInRegion(m_hMaskRgn, i, j))
							{
								SetPixel(hMemDC, i, j, RGB(8, 9, 10));
							}
						}
					}
					COLORREF clrTrans = GetPixel(hMemDC, 0, 0);
					// msimg32.lib Ïà¹Ø
					::TransparentBlt(hDC, nLeft, nTop, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, clrTrans);
					DeleteObject(hBitmap);
				}
				DeleteDC(hMemDC);
			}
		}
		else
		{
			::DrawState(hDC, NULL, NULL, (LPARAM)m_hBmp, 0, nLeft, nTop, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, DST_BITMAP|DSS_NORMAL);
		}
		return;
	}
	if (m_hIcon)
	{
		//int nTop = (m_rcItem.bottom + m_rcItem.top - 40)/2;
		//int nLeft = (m_rcItem.right + m_rcItem.left - 40)/2;
		::DrawIconEx(hDC, this->GetPos().left, this->GetPos().top, m_hIcon, icxWidth, icxWidth, 0, NULL, DI_NORMAL);
		return __super::PaintBkImage(hDC);
	}
	return __super::PaintBkImage(hDC);
}

void IconControlUI::SetIcon(HICON hIcon)
{
	m_hIcon = hIcon;
}

void IconControlUI::SetBitmap(HBITMAP hBmp)
{
	m_hBmp = hBmp;
}

void IconControlUI::SetSelect(bool bSelect)
{
	if (m_bSelect != bSelect)
	{
		m_bSelect = bSelect;
		Invalidate();
	}
}

void IconControlUI::SetDrawMask(bool bDraw)
{
	m_bDrawMask = bDraw;
}

HBITMAP	IconControlUI::GetBitmap()
{
	return m_hBmp;
}


IconCallBackUI::IconCallBackUI()
{

}

IconCallBackUI::~IconCallBackUI()
{

}

CControlUI*	IconCallBackUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, L"IconControl") == 0)
	{
		return new IconControlUI;
	}
	return NULL;
}

LPCTSTR IconCallBackUI::ReturnControlName()
{
	return L"IconControl";
}