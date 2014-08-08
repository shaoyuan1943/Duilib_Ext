#include "..\stdafx.h"
#include "GifAnimation.h"

GifAnimationUI::GifAnimationUI(void)
{
	m_pGifImage			=	NULL;
	m_pPropertyItem		=	NULL;
	m_nFrameCount		=	0;	
	m_nFramePosition	=	0;	
	GdiplusStartup(&m_gdiplusToken,&m_gdiplusStartupInput, NULL); // 加载GDI接口
}

GifAnimationUI::~GifAnimationUI(void)
{
	DeleteGif();
	m_pManager->KillTimer(this, EVENT_TIEM_ID);
	GdiplusShutdown( m_gdiplusToken );	//  卸载GDI接口
}

LPCTSTR GifAnimationUI::GetClass() const
{
	return _T("GifAnimUI");
}

LPVOID GifAnimationUI::GetInterface( LPCTSTR pstrName )
{
	if(_tcscmp(pstrName, _T("GifAnimation")) == 0) 
		return static_cast<GifAnimationUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

void GifAnimationUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if(!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) 
		return;
	if (NULL == m_pGifImage)
	{		
		InitGifImage();
	}
	DrawFrame(hDC);
}

void GifAnimationUI::DoEvent(TEventUI& event)
{
	if(event.Type == UIEVENT_TIMER)
		OnTimer((UINT_PTR)event.wParam);
}

void GifAnimationUI::InitGifImage()
{
	CDuiString sImageName = GetBkImage();
	CDuiString sImagePath = CPaintManagerUI::GetResourcePath();
	sImagePath += sImageName;
	if ( NULL == sImagePath ) 
		return;
	m_pGifImage = Image::FromFile(sImagePath);
	if (NULL == m_pGifImage) 
		return;
	UINT nCount	= 0;
	nCount = m_pGifImage->GetFrameDimensionsCount();
	GUID* pDimensionIDs	= new GUID[nCount];
	m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
	m_nFrameCount = m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
	int nSize = m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
	m_pPropertyItem	= (PropertyItem*) malloc(nSize);
	m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
	delete pDimensionIDs;
	pDimensionIDs = NULL;

	long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
	if ( lPause == 0 ) 
		lPause = 100;
	m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);
}

void GifAnimationUI::DeleteGif()
{
	if (m_pGifImage != NULL)
	{
		delete m_pGifImage;
		m_pGifImage = NULL;
	}

	if (m_pPropertyItem != NULL)
	{
		free( m_pPropertyItem );
		m_pPropertyItem = NULL;
	}
	m_nFrameCount =	0;	
	m_nFramePosition = 0;	
}

void GifAnimationUI::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != EVENT_TIEM_ID)
		return;
	m_pManager->KillTimer(this, EVENT_TIEM_ID);
	m_nFramePosition++;
	if (m_nFramePosition == m_nFrameCount)
		m_nFramePosition = 0;
	this->Invalidate();
	long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
	if ( lPause == 0 ) 
		lPause = 100;
	m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);
}

void GifAnimationUI::DrawFrame(HDC hDC)
{
	if (NULL == hDC || NULL == m_pGifImage) 
		return;
	GUID pageGuid = Gdiplus::FrameDimensionTime;
	Graphics graphics(hDC);
	graphics.DrawImage(m_pGifImage, m_rcItem.left, m_rcItem.top);
	m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
}


