
// MRRKinectDlg.cpp : implementation file
//
#include "stdafx.h"
#include <afxwin.h>
#include "MRRKinect.h"
#include "MRRKinectDlg.h"
#include <afxdialogex.h>

//for test
#define _WINSOCKAPI_
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define	_JOINT_RHAND 15
#define maxFrameNum 10000

bool g_RunKinect = true;
HANDLE g_hKinectThread;
int gnPointMode=1;
int gnSystemStatus = _SS_REST;
int gCalibStatus = 0;
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

// Definition of a gettimeofday function
 
void getTimeofDay(struct timeval *tv, struct timezone *tz)
{
// Define a structure to receive the current Windows filetime
  FILETIME ft;
 
// Initialize the present time to 0 and the timezone to UTC
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
// The GetSystemTimeAsFileTime returns the number of 100 nanosecond 
// intervals since Jan 1, 1601 in a structure. Copy the high bits to 
// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
// Convert to microseconds by dividing by 10
    tmpres /= 10;
 
// The Unix epoch starts on Jan 1 1970.  Need to subtract the difference 
// in seconds from Jan 1 1601.
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
 
// Finally change microseconds to seconds and place in the seconds value. 
// The modulus picks up the microseconds.
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
  
// Adjust for the timezone west of Greenwich
      tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return;
}
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
	m_pImgProc = new ImageProc;
	m_pModel = new ColorModel;
	m_pCalib = new KinectCalibration;

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
	ON_BN_CLICKED(IDC_BUTTON_SETMODEL, &CMRRKinectDlg::OnBnClickedButtonSetmodel)
	ON_BN_CLICKED(IDC_BUTTON_ARCHIVE, &CMRRKinectDlg::OnBnClickedButtonArchive)
	ON_BN_CLICKED(IDC_BUTTON_ENDARCHIVE, &CMRRKinectDlg::OnBnClickedButtonEndarchive)
	ON_BN_CLICKED(IDC_BUTTON_SAVECALIB, &CMRRKinectDlg::OnBnClickedButtonSavecalib)
	ON_BN_CLICKED(IDC_BUTTON_TESTCALIB, &CMRRKinectDlg::OnBnClickedButtonTestcalib)
	ON_BN_CLICKED(IDC_BUTTON_RESETCALIB, &CMRRKinectDlg::OnBnClickedButtonResetcalib)
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
	fullWnd->ShowImg(w,h,wb,kinect->getRGBImg()->getData());
	//fullWnd->ShowImg(w,h,wb,dlg->getBG()->getRgbBG()->getData());

	//fullWnd->ShowImg(w, h, wb, dlg->getHandTrackData()->getImg()->getData());
	//Test Code ends
	Rect modRect;
	modRect.x = MODEL_StartX - (int)(MODEL_W/2);
	modRect.y = MODEL_StartY - (int)(MODEL_H/2);
	modRect.width = MODEL_W;
	modRect.height = MODEL_H;

	fullWnd->showRect( modRect );

	if(dlg->getSkeleton()->update())
	{
		if(dlg->getHandTrackData()->isReady())
			dlg->getSkeleton()->setJointPosAt(XN_SKEL_RIGHT_HAND, dlg->getHandTrackData()->getHandPos(), 1.0f);			//set hand position to the skeleton.
		fullWnd->showSkeleton(dlg->getSkeleton()->getJointPos());
	}
	fullWnd->showHandJoint(dlg->getHandTrackData()->getHandPos());
	fullWnd->Invalidate();

	return 0;
}

DWORD WINAPI KinectThread(LPVOID lpParam) {
	CMRRKinectDlg* dlg = (CMRRKinectDlg*)(lpParam);
	KinectOpenNI* kinect = dlg->getKinect();
	KinectCalibration* calib = dlg->getCalibration();
	Background* bg = dlg->getBG();
	KinectSkeletonOpenNI* skeleton = dlg->getSkeleton();
	KSHandTrackDataUsingMeanShift* handData = dlg->getHandTrackData();
	KSTorsoData* torsoData = dlg->getTorsoData();
	KSFrameData* frameData = dlg->getFrameData();
	KSElbowData* elbowData = dlg->getElbowData();
	KSArchivingData* archiveData = dlg->getArchivingData();

	//debug code ends
	int nCenterX = MODEL_StartX;
	int nCenterY = MODEL_StartY;
	struct timeval timeStamp;
	double lastFrameTime = 0.0f;
	double currentFrameTime = 0.0f;
	int frameID = 0;
	KSFrameDataSender sender;

	sender.openServer();
	while(g_RunKinect) {

		lastFrameTime = currentFrameTime;
		frameID = frameID + 1;

		kinect->update();
		skeleton->update();

		//if(skeleton->update() )
		//	dlg->getFilter()->smoothSkeleton( skeleton );
		//if(!bg->isBG())
		//{
		//	bg->init(kinect->getRGBImg(), kinect->getDepthImg()); //将背景文件拷进去
		//}

		switch( gnSystemStatus ){
			case _SS_REST: break;
			case _SS_CALIB:
				{

					switch( gCalibStatus ){
						case 0:
							calib->startCalib(kinect->getRGBImg(),kinect->getDepthImg(), kinect->getDepthGenerator());break;
						case 1:
							calib->testCalib(kinect->getRGBImg(),kinect->getDepthImg(), kinect->getDepthGenerator());break;	
						default:
							AfxMessageBox(_T("Please choose Calibration Status"));break;
					}
				}
			case _SS_TRACK:	
				{
					if( skeleton->update()&&handData->isReady()&&torsoData->isReady()&&elbowData->isReady() )
					{
						nCenterX = handData->getHandPos().x;
						nCenterY = handData->getHandPos().y;
						//The new interfaces 4.12/
						//------------
						//UPDATE DATA
						//------------
						//1. handData 2. torsoData 3. elbowData 
						handData->update(						
							kinect->getRGBImg(),
							kinect->getDepthImg(),
							nCenterX,			
							nCenterY,
							ROI_SIZE_W,																		//4.12 Can we change the ROI size depending on the tracking performance? if tracking lost, 
							ROI_SIZE_H
						);
						POINT3D p3DHandPos = calib->cvtIPtoGP(handData->getHandPos(),kinect->getDepthGenerator());
						dlg->getSkeleton()->setJoint3DPosAt(XN_SKEL_RIGHT_HAND, p3DHandPos, 1.0f);						//set hand position to the skeleton.
						torsoData->update( kinect->getDepthImg(), skeleton, calib, kinect->getDepthGenerator() );
						elbowData->update( skeleton, kinect->getDepthGenerator());
						getTimeofDay(&timeStamp, NULL);
						currentFrameTime = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);						
						
						frameData->update(frameID, 
							currentFrameTime,
							p3DHandPos.x,
							p3DHandPos.y,
							p3DHandPos.z,
							torsoData->getTorsoComps(),
							elbowData->getElbowOpening()
							);																				//if tracking is success, we record the data, frame data updates.

						sender.getData()->update(
							frameData->getFrameID(),
							frameData->getTimestamp(),
							frameData->getHandX(),
							frameData->getHandY(),
							frameData->getHandZ(),
							frameData->getTorsoZComp(),
							frameData->getElbowOpen()
							);

						sender.sendData();

						if( archiveData->isArchiving())
							archiveData->addAFrame(dlg->getFrameData());
						
						//------------
						//DISPLAY DATA
						//------------
						CString str;
						//str.Format(_T("%lf"), p3DHandPos.x);
						//dlg->SetDlgItemText(IDC_EDIT9, str);
						//str.Format(_T("%lf"), p3DHandPos.y);
						//dlg->SetDlgItemText(IDC_EDIT10, str);
						//str.Format(_T("%lf"), p3DHandPos.z);
						//dlg->SetDlgItemText(IDC_EDIT11, str);
						str.Format(_T("%lf"), dlg->getTorsoData()->getTorsoComps());
						dlg->SetDlgItemText(IDC_EDIT12, str);
						str.Format(_T("%lf"), dlg->getElbowData()->getElbowOpening());
						dlg->SetDlgItemText(IDC_EDIT13, str);
					}

					break;
				}
		}

		HANDLE hShowStreams = CreateThread(NULL, 0, ShowStreams, dlg, 0, NULL);
		WaitForSingleObject(hShowStreams, INFINITE);
		getTimeofDay(&timeStamp, NULL);
		currentFrameTime = timeStamp.tv_sec + (timeStamp.tv_usec/1000000.0);
		
		//Display Data info
		CString str;
		str.Format(_T("%d"), frameID);
		dlg->SetDlgItemText(IDC_EDIT4, str);
		str.Format(_T("%lf"), 1.0f/(currentFrameTime-lastFrameTime));
		dlg->SetDlgItemText(IDC_EDIT5, str);

	}
	sender.closeServer();
	return 0;
}

void CMRRKinectDlg::InitKinect() {
	
	kinect.init();

	int nWidth = kinect.getRGBImg()->width();
	int	nHeight = kinect.getRGBImg()->height(); 
	m_pBG = new Background(nWidth,nHeight);	//Is it good to put it here?
	m_pSkeleton = new KinectSkeletonOpenNI(kinect.getContext(), kinect.getDepthGenerator(), 24);
	
	m_pFrameData = new KSFrameData;
	m_pHandTrackData = new KSHandTrackDataUsingMeanShift(nWidth,nHeight);
	m_pTorsoData = new KSTorsoData;
	m_pElbowData = new KSElbowData;
	m_pArchivingData = new KSArchivingData;

	m_pFilter = new KSUtilsMAFilter(2);
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

	//Test Code ends.
}

void CMRRKinectDlg::OnBnClickedButtonBlack()
{
	// TODO: Add your control notification handler code here
	//Test Code, Edited by Tingfang, 10/30	

	CString str;
	m_slider1->SetRange(0,255);
	m_slider1->SetPos(234);
	m_pCalib->setValue1(m_slider1->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue1());
	SetDlgItemText(IDC_EDIT1, str);

	m_slider2->SetRange(0,255);
	m_slider2->SetPos(170);
	m_pCalib->setValue2(m_slider2->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue2());
	SetDlgItemText(IDC_EDIT2, str);

	m_slider3->SetRange(0,255);
	m_slider3->SetPos(22);
	m_pCalib->setValue3(m_slider3->GetPos());
	str.Format(_T("%d"), m_pCalib->getValue3());
	SetDlgItemText(IDC_EDIT3, str);

	////Test code ends.
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
		ScreenToClient( &point );
		CRect rect;
		float h,s,v; 
		GetDlgItem( IDC_FULLFRAME )->GetWindowRect( &rect );
		ScreenToClient( &rect );
		point.x = point.x - rect.TopLeft().x-4;
		point.y = point.y - rect.TopLeft().y-11;

		//@Tingfang 6/5/2012
		//if(pMsg->message == WM_LBUTTONDOWN)&& in calibration mode
			//if( m_Calib!=NULL )
			//pImgSet[0],pImgSet[1], pImgSet[2] Global
			//
		//if(point.x>0 && point.x<640 && point.y>0 && point.y<480)
		//{
		//	HDC hDC = ::GetDC(NULL);
		//	COLORREF color;

		//	color = GetPixel(hDC, point.x, point.y); //If you move the dialog window, the position is not accurate.
		//	int r = GetRValue(color);
		//	int g = GetGValue(color);
		//	int b = GetBValue(color);
		//	
		//	//Test code, Edited 11/3, for test to check the value of h,s,v
		//	m_pImgProc->rgb2hsv(b,g,r,v,s,h);
		//	//CString szColor;
		//	//szColor.Format(_T("%d, %d, %d"), r,g,b);
		//	//szColor.Format(_T("%d, %d, %d"), (int)h,(int)s,(int)v);
		//	SetDlgItemText(IDC_EDIT8, szColor);
		//}
		//else
		//{
		//}
		
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
	gnSystemStatus = _SS_CALIB;
}

void CMRRKinectDlg::OnBnClickedButtonTrack()
{
	// TODO: Add your control notification handler code here
	gnSystemStatus = _SS_TRACK;
}

void CMRRKinectDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	gnSystemStatus = _SS_REST;
}

void CMRRKinectDlg::OnBnClickedButtonSetmodel()
{
	// TODO: Add your control notification handler code here
	if(m_pHandTrackData!=NULL)
		m_pHandTrackData->setReady(true);
	if(m_pTorsoData!=NULL)
		m_pTorsoData->setReady(true);
	if(m_pElbowData!=NULL)
		m_pElbowData->setReady(true);

	POINT3D torso2DPos,torso3DPos;
	torso2DPos.x = (m_pSkeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).x + m_pSkeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).x )/2;
	torso2DPos.y = (m_pSkeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).y + m_pSkeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).y )/2;
	torso2DPos.z = 0.0f;
	torso3DPos.x = (m_pSkeleton->getJoint3DPosAt(XN_SKEL_LEFT_SHOULDER).x + m_pSkeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).x )/2;
	torso3DPos.y = (m_pSkeleton->getJoint3DPosAt(XN_SKEL_LEFT_SHOULDER).y + m_pSkeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).y )/2;
	torso3DPos.z = (m_pSkeleton->getJoint3DPosAt(XN_SKEL_LEFT_SHOULDER).z + m_pSkeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).z )/2;

	m_pTorsoData->setCurrentPos( torso2DPos );
	m_pTorsoData->setRestPos( torso3DPos );
	m_pHandTrackData->setInit( false );
	
	POINT3D RestHandPos = m_pCalib->cvtIPtoCamP(m_pHandTrackData->getHandPos(),kinect.getDepthGenerator());
	m_pHandTrackData->setRestPosDepth(torso3DPos.z);
	gnPointMode = 2;
}

void CMRRKinectDlg::OnBnClickedButtonArchive()
{
	// TODO: Add your control notification handler code here
	//if no data, return;
	if( m_pArchivingData != NULL )
		m_pArchivingData->setIsArchiving(true);
}

void CMRRKinectDlg::OnBnClickedButtonEndarchive()
{
	// TODO: Add your control notification handler code here
	if( m_pArchivingData != NULL )
	{
		m_pArchivingData->saveArchivingData();
		m_pArchivingData->setIsArchiving(false);
	}	
}

void CMRRKinectDlg::OnBnClickedButtonSavecalib()
{
	// TODO: Add your control notification handler code here
	//if(gnSystemStatus!=_SS_CALIB) return;		
	m_pCalib->saveCalibrationDatatoFile();
	cout << "Coordinate set, please test" << endl;
}

void CMRRKinectDlg::OnBnClickedButtonTestcalib()
{
	// TODO: Add your control notification handler code here
	gCalibStatus = 1;
}

void CMRRKinectDlg::OnBnClickedButtonResetcalib()
{
	// TODO: Add your control notification handler code here
	gCalibStatus = 0;
}
