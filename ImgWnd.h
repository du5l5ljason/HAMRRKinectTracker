#pragma once


// CImgWnd

#include "DIBDC.h"
#include "StructDefine.h"

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	virtual ~CImgWnd();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void ShowImg(CDIBDC* pDIB);
	void ShowImg(int width, int height, int widthstep, BYTE* pData);
	void ShowImg(int width, int height, BYTE** pData);

	void ShowDownSampleImg24(int width, int height, int widthstep, BYTE* pData);
	void ShowDownSampleImg16(int width, int height, int widthstep, BYTE* pData);
	void ShowDownSampleImg8(int width, int height, int widthstep, BYTE* pData);

	CDIBDC* getDIBDC() {return &m_dibShow;};
	void showLine(int x1, int y1, int x2, int y2, int color[]);
	void showEllipse(int x1, int y1, int x2, int y2, int color[]);

	virtual void showSkeleton(POINT3D* jointPos){};
	virtual void showHandJoint(POINT3D RightPos){};
	virtual void showHandJoint(POINT3D leftPos, POINT3D RightPos){};

protected:
	CDIBDC m_dibShow;

protected:
	afx_msg void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()
};


