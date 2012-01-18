// ImgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ImgWnd.h"

#define MAX_DEPTH 4096
// CImgWnd

IMPLEMENT_DYNAMIC(CImgWnd, CWnd)

CImgWnd::CImgWnd()
{
	m_dibShow.CreateDIB( 100, -100, 24 );
	m_dibShow.SetFillColor( RGB(0,0,0) );
}

CImgWnd::~CImgWnd()
{
}


BEGIN_MESSAGE_MAP(CImgWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CImgWnd message handlers


BOOL CImgWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= WS_CHILD;
	//dwStyle |= WS_HSCROLL;
	//dwStyle |= WS_VSCROLL;

	BOOL nresult = CWnd::Create(NULL, NULL,dwStyle, rect, pParentWnd, nID, NULL);

	m_dibShow.CreateDIB( rect.right-rect.left+1, -(rect.bottom-rect.top+1), 24 );
	m_dibShow.SetFillColor( RGB(0,0,0) );
	
	return nresult;
}

void CImgWnd::OnPaint()
{
	CPaintDC lop(this);
	m_dibShow.Paste( &lop, 0,0 );
	CWnd::OnPaint();
}

void CImgWnd::ShowImg(CDIBDC* pDIB){
	m_dibShow.CreateDIB(*pDIB);
	Invalidate();
}

void CImgWnd::ShowImg(int width, int height, int widthstep, BYTE* pData) {
	m_dibShow.CreateDIB(width, -height, 24);
	for (int i = 0 ; i < height ; i ++ )
		for (int j = 0 ; j < width ; j++ ) {
			BYTE r = pData[i*widthstep+j*3];
			BYTE g = pData[i*widthstep+j*3+1];
			BYTE b = pData[i*widthstep+j*3+2];
			m_dibShow.SetAt(j,i,RGB(b,g,r));
		}
	//Invalidate();
}

void CImgWnd::ShowImg(int width, int height, BYTE** pData) {
	m_dibShow.CreateDIB(width, -height, 24);
	for (int i = 0 ; i < height ; i ++ )
		for (int j = 0 ; j < width ; j++ ) {
			BYTE g = pData[i][j];
			m_dibShow.SetAt(j,i,RGB(g,g,g));
		}
	Invalidate();
}

void CImgWnd::ShowDownSampleImg24(int width, int height, int widthstep, BYTE* pData) {
	m_dibShow.CreateDIB(width/2, -height/2, 24);
	for (int i = 0 ; i < height ; i +=2 )
		for (int j = 0 ; j < width ; j+=2 ) {
			BYTE r = pData[i*widthstep+j*3];
			BYTE g = pData[i*widthstep+j*3+1];
			BYTE b = pData[i*widthstep+j*3+2];
			m_dibShow.SetAt(j/2,i/2,RGB(b,g,r));
		}
	Invalidate();
}

void CImgWnd::ShowDownSampleImg16(int width, int height, int widthstep, BYTE* pData) {
	m_dibShow.CreateDIB(width/2, -height/2, 24);
	for (int i = 0 ; i < height ; i +=2 )
		for (int j = 0 ; j < width ; j+=2 ) {
			BYTE r = pData[i*widthstep+j*2];
			BYTE g = pData[i*widthstep+j*2+1];
			int x = int(g)*256+r;
			BYTE y = ( x>MAX_DEPTH) ? 0 : 255-x*255/MAX_DEPTH;
			m_dibShow.SetAt(j/2,i/2,RGB(y,y,y));
		}
	Invalidate();
}

void CImgWnd::ShowDownSampleImg8(int width, int height, int widthstep, BYTE* pData) {
	m_dibShow.CreateDIB(width/2, -height/2, 24);
	for (int i = 0 ; i < height ; i +=2 )
		for (int j = 0 ; j < width ; j+=2 ) {
			BYTE r = pData[i*widthstep+j];
			m_dibShow.SetAt(j/2,i/2,RGB(r,r,r));
		}
	Invalidate();
}

void CImgWnd::showLine(int x1, int y1, int x2, int y2, int color[]) {
	CPen newpen, *pOldpen;
	newpen.CreatePen( PS_SOLID, 3, RGB(color[0],color[1],color[2])  );
	pOldpen = m_dibShow.SelectObject( &newpen);

	m_dibShow.MoveTo( x1, y1 );
	m_dibShow.LineTo( x2, y2 );
	
	m_dibShow.SelectObject( pOldpen );
	newpen.DeleteObject();
}

void CImgWnd::showEllipse(int x1, int y1, int x2, int y2, int color[]) {
	CPen newpen, *pOldpen;
	newpen.CreatePen( PS_SOLID, 3, RGB(color[0],color[1],color[2]) );
	pOldpen = m_dibShow.SelectObject( &newpen);

	// Initialize a LOGBRUSH structure.
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_HOLLOW;

	// Declare an uninitialized CBrush ...
	CBrush brush;
	// ... and initialize it with the LOGBRUSH.
	brush.CreateBrushIndirect(&logBrush);

	// Select the brush (and perhaps a pen) into
	// the device context.
	CBrush* pOldBrush = (CBrush*)m_dibShow.SelectObject(&brush);


	m_dibShow.Ellipse(x1, y1, x2, y2);
	
	m_dibShow.SelectObject( pOldpen );
	m_dibShow.SelectObject( pOldBrush);
	newpen.DeleteObject();
}