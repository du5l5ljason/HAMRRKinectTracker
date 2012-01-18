
// MRRKinectDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MRRKinect.h"
#include "MRRKinectDlg.h"
#include "afxdialogex.h"

//for test
#include "windows.h"
#include "time.h"
#include <stdio.h>

#include <ctime>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define	_JOINT_RHAND 15

bool g_RunKinect = true;
HANDLE g_hKinectThread;
extern int gnPointMode;
int gnSystemStatus = 0;
//Edited by Tingfang, 10/31/2011

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMRRKinectDlg dialog

CMRRKinectDlg::CMRRKinectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMRRKinectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMRRKinectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMRRKinectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMRRKinectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GREEN, &CMRRKinectDlg::OnBnClickedButtonGreen)
	ON_BN_CLICKED(IDC_BUTTON_BLACK, &CMRRKinectDlg::OnBnClickedButtonBlack)
	ON_BN_CLICKED(IDC_BUTTON_CALIB, &CMRRKinectDlg::OnBnClickedButtonCalib)
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_TRACK, &CMRRKinectDlg::OnBnClickedButtonTrack)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMRRKinectDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CMRRKinectDlg message handlers

BOOL CMRRKinectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitImgWnd();
	InitKinect();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMRRKinectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMRRKinectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMRRKinectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMRRKinectDlg::InitImgWnd() {
	CRect rect;
	GetDlgItem( IDC_RGBFRAME )->GetWindowRect( rect );
	ScreenToClient( rect );
	int nBorder = 8;
	rect.left += nBorder;
	rect.right -= nBorder-1;
	rect.top += nBorder;
	rect.bottom -= nBorder-1;
	m_wndRGB.Create( WS_VISIBLE , rect, this, IDR_RGBFRAME );

	GetDlgItem( IDC_DEPTHFRAME )->GetWindowRect( rect );
	ScreenToClient( rect );
	rect.left += nBorder;
	rect.right -= nBorder-1;
	rect.top += nBorder;
	rect.bottom -= nBorder-1;
	m_wndDepth.Create( WS_VISIBLE , rect, this, IDR_DEPTHFRAME );

	GetDlgItem( IDC_FULLFRAME )->GetWindowRect( rect );
	ScreenToClient( rect );
	rect.left += nBorder;
	rect.right -= nBorder-1;
	rect.top += nBorder+10;
	rect.bottom -= nBorder+10;
	m_wndFull = new CDrawWnd();
	m_wndFull->Create( WS_VISIBLE , rect, this, IDR_FULLFRAME );

	//Test Code, 10/30
	GetDlgItem( IDC_SLIDER1 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider1 = new CSliderCtrl();
	m_slider1->Create( WS_VISIBLE, rect, this, IDC_SLIDER1 );

	GetDlgItem( IDC_SLIDER2 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider2 = new CSliderCtrl();
	m_slider2->Create( WS_VISIBLE, rect, this, IDC_SLIDER2 );

	GetDlgItem( IDC_SLIDER3 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider3 = new CSliderCtrl();
	m_slider3->Create( WS_VISIBLE, rect, this, IDC_SLIDER3 );

	GetDlgItem( IDC_SLIDER4 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider4 = new CSliderCtrl();
	m_slider4->Create( WS_VISIBLE, rect, this, IDC_SLIDER4 );

	GetDlgItem( IDC_SLIDER5 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider5 = new CSliderCtrl();
	m_slider5->Create( WS_VISIBLE, rect, this, IDC_SLIDER5 );

	GetDlgItem( IDC_SLIDER6 )->GetWindowRect( rect );
	ScreenToClient( rect );
	m_slider6 = new CSliderCtrl();
	m_slider6->Create( WS_VISIBLE, rect, this, IDC_SLIDER6 );
	//Test Code Ends, 10/30
}

DWORD WINAPI ShowStreams(LPVOID lpParam) {
	CMRRKinectDlg* dlg = (CMRRKinectDlg*)(lpParam);
	KinectSensor* kinect = dlg->getKinect();

	CImgWnd* rgbWnd = dlg->getRGBWnd();
	int w = kinect->getRGBImg()->width();
	int h = kinect->getRGBImg()->height();
	int wb = kinect->getRGBImg()->widthBytes();
	rgbWnd->ShowDownSampleImg24(w, h, wb, kinect->getRGBImg()->getData());

	CImgWnd* depthWnd = dlg->getDepthWnd();
	wb = kinect->getDepthImg()->widthBytes();
	depthWnd->ShowDownSampleImg16(w, h, wb, kinect->getDepthImg()->getData());

	CImgWnd* fullWnd = dlg->getFullWnd();
	wb = kinect->getRGBImg()->widthBytes();

	//Test code: Edited by Tingfang 10/28
	//fullWnd->ShowImg(w,h,wb,dlg->getHandTracker()->getImg()->getData());
	//fullWnd->ShowImg(w,h,wb,dlg->getBG()->getRgbBG()->getData());
	fullWnd->ShowImg(w, h, wb, kinect->getRGBImg()->getData());
	//Test Code ends
	fullWnd->showSkeleton(dlg->getSkeleton()->getJointPos());
	fullWnd->showHandJoint(dlg->getHandTracker()->getRightHandPos());
	
	fullWnd->Invalidate();
	
	return 0;
}

DWORD WINAPI KinectThread(LPVOID lpParam) {
	CMRRKinectDlg* dlg = (CMRRKinectDlg*)(lpParam);
	KinectOpenNI* kinect = dlg->getKinect();
	Background* bg = dlg->getBG();
	HandTrackMarker* handTrack = dlg->getHandTracker();
	KinectSkeletonOpenNI* skeleton = dlg->getSkeleton();
	
	//for debug, time duration per frame
	clock_t t_start, t_finish;
	int nStartX = 0;
	int nStartY = 0;
	while(g_RunKinect) {
		t_start = clock();
		kinect->update();
		switch(gnSystemStatus){
			case _SS_REST: break;
			case _SS_CALIB: dlg->getCalibration()->Calibrate(kinect->getRGBImg(),kinect->getDepthImg(), kinect->getDepthGenerator());break;
			case _SS_TRACK:	
				{
					skeleton->update();
					if(!bg->isBG())
					{
						bg->init(kinect->getRGBImg(), kinect->getDepthImg());
					}
		
					nStartX = skeleton->getJointPosAt(_JOINT_RHAND).x;
					nStartY = skeleton->getJointPosAt(_JOINT_RHAND).y;
					if(gnPointMode==2)
					{
						nStartX = dlg->getHandTracker()->getRightHandPos().x;
						nStartY = dlg->getHandTracker()->getRightHandPos().y;
					}
					if( bg->isBG() && skeleton->update() )
					{
						handTrack->track(						
							kinect->getRGBImg(),
							kinect->getDepthImg(),
							bg->getRgbBG(),
							bg->getDepthBG(),
							nStartX,			//Modified later
							nStartY,
							60,
							60,
							true
						);
					}
					break;
				}
		}

		t_finish = clock();
		CString str;
		str.Format(_T("%f"), (double)(t_finish-t_start)/CLOCKS_PER_SEC);
		//dlg->SetDlgItemText(IDC_EDIT7, str);

		HANDLE hShowStreams = CreateThread(NULL, 0, ShowStreams, dlg, 0, NULL);
		WaitForSingleObject(hShowStreams, INFINITE);
	}
	return 0;
}

void CMRRKinectDlg::InitKinect() {
	kinect.init();
	m_pSkeleton = new KinectSkeletonOpenNI(kinect.getContext(), kinect.getDepthGenerator(), 24);
	int nWidth = kinect.getRGBImg()->width();
	int nHeight = kinect.getRGBImg()->height(); 
	m_pHandTracker = new HandTrackMarker(nWidth, nHeight);
	m_pBG = new Background(nWidth,nHeight);	//Is it good to put it here?
	m_pImgProc = new ImageProc;
	m_pModel = new MarkerModel;
	m_pCalib = new KinectCalibration;
	kinect.open();

	g_hKinectThread = CreateThread(NULL, 0, KinectThread, this, 0, NULL);
}

void CMRRKinectDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	g_RunKinect = false;
	WaitForSingleObject(g_hKinectThread, 200);
	CDialogEx::OnCancel();
}

void CMRRKinectDlg::OnBnClickedButtonGreen()
{
	// TODO: Add your control notification handler code here
	//Test Code, Edited by Tingfang, 10/30
	//m_pHandTracker->setColorMode(_CM_PINK);
	gnPointMode = 1;
	CString str;
	m_slider1->SetRange(0,255);
	m_slider1->SetPos(191);
	m_pCalib->setValue1(m_slider1->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue1());
	SetDlgItemText(IDC_EDIT1, str);

	m_slider2->SetRange(0,255);
	m_slider2->SetPos(30);
	m_pCalib->setValue2(m_slider2->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue2());
	SetDlgItemText(IDC_EDIT2, str);

	m_slider3->SetRange(0,255);
	m_slider3->SetPos(174);
	m_pCalib->setValue3(m_slider3->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue3());
	SetDlgItemText(IDC_EDIT3, str);

	m_slider4->SetRange(0,100);
	m_slider4->SetPos(33);
	m_pCalib->setRangeV1(m_slider4->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV1());
	SetDlgItemText(IDC_EDIT4, str);

	m_slider5->SetRange(0,100);
	m_slider5->SetPos(81);

	m_pCalib->setRangeV2(m_slider5->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV2());
	SetDlgItemText(IDC_EDIT5, str);

	m_slider6->SetRange(0,100);
	m_slider6->SetPos(92);

	m_pCalib->setRangeV3(m_slider6->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV3());
	SetDlgItemText(IDC_EDIT6, str);
	//Test Code ends.
}

void CMRRKinectDlg::OnBnClickedButtonBlack()
{
	// TODO: Add your control notification handler code here
	//Test Code, Edited by Tingfang, 10/30	
	gnPointMode = 2;
	//m_pHandTracker->setColorMode(_CM_BLACK);

	CString str;
	m_slider1->SetRange(0,255);
	m_slider1->SetPos(166);
	m_pCalib->setValue1(m_slider1->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue1());
	SetDlgItemText(IDC_EDIT1, str);

	m_slider2->SetRange(0,255);
	m_slider2->SetPos(159);
	m_pCalib->setValue2(m_slider2->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue2());
	SetDlgItemText(IDC_EDIT2, str);

	m_slider3->SetRange(0,255);
	m_slider3->SetPos(22);
	m_pCalib->setValue3(m_slider3->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue3());
	SetDlgItemText(IDC_EDIT3, str);

	m_slider4->SetRange(0,100);
	m_slider4->SetPos(40);
	m_pCalib->setRangeV1(m_slider4->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV1());
	SetDlgItemText(IDC_EDIT4, str);

	m_slider5->SetRange(0,100);
	m_slider5->SetPos(40);
	m_pCalib->setRangeV2(m_slider5->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV2());
	SetDlgItemText(IDC_EDIT5, str);

	m_slider6->SetRange(0,100);
	m_slider6->SetPos(100);
	m_pCalib->setRangeV3(m_slider6->GetPos());
	str.Format(_T("%d"), m_pCalib->getRangeV3());
	SetDlgItemText(IDC_EDIT6, str);
	//Test code ends.
}

void CMRRKinectDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	UpdateData(TRUE);

	if( pScrollBar == this->GetDlgItem(IDC_SLIDER1))
	{	
		m_slider1 = (CSliderCtrl*)pScrollBar;
		m_slider1->SetRange(0,400);
		m_pCalib->setValue1(m_slider1->GetPos());
		str.Format(_T("%d"), m_pCalib->getValue1());
		SetDlgItemText(IDC_EDIT1, str);
	}
	if( pScrollBar == this->GetDlgItem(IDC_SLIDER2))
	{
		m_slider2 = (CSliderCtrl*)pScrollBar;
		m_slider2->SetRange(0, 255);
		m_pCalib->setValue2(m_slider2->GetPos());
		str.Format(_T("%d"), m_pCalib->getValue2());
		SetDlgItemText(IDC_EDIT2, str);
	}
	if( pScrollBar == this->GetDlgItem(IDC_SLIDER3))
	{
		m_slider3 = (CSliderCtrl*)pScrollBar;
		m_slider3->SetRange(0, 255);
		m_pCalib->setValue3(m_slider3->GetPos());
		str.Format(_T("%d"), m_pCalib->getValue3());
		SetDlgItemText(IDC_EDIT3, str);
	}
	if( pScrollBar == this->GetDlgItem(IDC_SLIDER4))
	{		
		m_slider4 = (CSliderCtrl*)pScrollBar;
		m_slider4->SetRange(0, 100);
		m_pCalib->setRangeV1(m_slider4->GetPos());
		str.Format(_T("%d"), m_pCalib->getRangeV1());
		SetDlgItemText(IDC_EDIT4, str);
	}
	if( pScrollBar == this->GetDlgItem(IDC_SLIDER5))
	{		
		m_slider5 = (CSliderCtrl*)pScrollBar;
		m_slider5->SetRange(0, 100);
		m_pCalib->setRangeV2(m_slider5->GetPos());
		str.Format(_T("%d"), m_pCalib->getRangeV2());
		SetDlgItemText(IDC_EDIT5, str);
	}
	if( pScrollBar == this->GetDlgItem(IDC_SLIDER6))
	{		
		m_slider6 = (CSliderCtrl*)pScrollBar;
		m_slider6->SetRange(0, 100);
		m_pCalib->setRangeV3(m_slider6->GetPos());
		str.Format(_T("%d"), m_pCalib->getRangeV3());
		SetDlgItemText(IDC_EDIT6, str);
	}
	
	UpdateData(FALSE);


	//CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMRRKinectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
}

BOOL CMRRKinectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEMOVE)
	{
		CPoint point;
		GetCursorPos(&point);
		
		CRect rect;
		float h,s,v; 
		GetDlgItem( IDC_FULLFRAME )->GetClientRect( rect );
		GetDlgItem( IDC_FULLFRAME )->ClientToScreen( rect );

		if(point.x>rect.left && point.x<rect.right && point.y>rect.top && point.y<rect.bottom)
		{
			HDC hDC = ::GetDC(NULL);
			COLORREF color;

			color = GetPixel(hDC, point.x, point.y); //If you move the dialog window, the position is not accurate.
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);
			
			//Test code, Edited 11/3, for test to check the value of h,s,v
			m_pImgProc->rgb2hsv(b,g,r,v,s,h);
			CString szColor;
			//szColor.Format(_T("%d, %d, %d"), r,g,b);
			szColor.Format(_T("%d, %d, %d"), (int)h,(int)s,(int)v);
			SetDlgItemText(IDC_EDIT8, szColor);
		}
		else
		{
		}
		
		point.x = point.x - 482; //600 offset + 8 nboarder
		point.y = point.y - 99;  //12 offset + 18 nboarder, Problem, how to get positions invariant to window movement?
		CString szPos;
		szPos.Format(_T("%d, %d"),point.x,point.y);
		SetDlgItemText(IDC_EDIT7, szPos);

	}
	if(pMsg->message == WM_KEYDOWN)
	{
		//listen to the specific keyboard message
		if(pMsg->wParam == 'T')
			m_pCalib->setCalibStage(1); //test calib
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMRRKinectDlg::OnBnClickedButtonCalib()
{
	// TODO: Add your control notification handler code here
	gnSystemStatus = 1;
}

void CMRRKinectDlg::OnBnClickedButtonTrack()
{
	// TODO: Add your control notification handler code here
	gnSystemStatus = 2;
}

void CMRRKinectDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	gnSystemStatus = 0;
}
