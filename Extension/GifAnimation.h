#ifndef _UIGIFANIM_H_
#define _UIGIFANIM_H_

#include <gdiplus.h>
using namespace Gdiplus;
#pragma once
#define EVENT_TIEM_ID	100

class DuiLib::CLabelUI;
class GifAnimationUI : public DuiLib::CLabelUI
{
public:
	GifAnimationUI(void);
	~GifAnimationUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);
	void	DoPaint(HDC hDC, const RECT& rcPaint);
	void	DoEvent(TEventUI& event);
private:
	void	InitGifImage();
	void	DeleteGif();
	void    OnTimer(UINT_PTR idEvent);
	void	DrawFrame(HDC hDC);		// 绘制GIF每帧
private:
	Image	*m_pGifImage;
	UINT	m_nFrameCount;				// gif图片总帧数
	UINT	m_nFramePosition;			// 当前放到第几帧
	PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间
private:
	ULONG_PTR				m_gdiplusToken;
	GdiplusStartupInput		m_gdiplusStartupInput;
};
#endif // _UIGIFANIM_H_
