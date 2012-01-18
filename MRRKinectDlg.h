
// MRRKinectDlg.h : header file
//

#pragma once

#include "ImgWnd.h"
#include "DrawWnd.h"
#include "KinectSensor.h"
#include "KinectSkeletonOpenNI.h"
#include "HandTrack.h"
#include "KinectCalibration.h"
#include "HandTrackMarker.h"
#include "Background.h"
#include "ImageProc.h"
//For test
#include "MarkerModel.h"

enum POINTMODE{
	_PM_SKEL,
	_PM_LAST,
};
enum SYMSTATUS{
	_SS_REST,
	_SS_CALIB,
	_SS_TRACK,
};
// CMRRKinectDlg dialog
class CMRRKinectDlg : public CDialogEx
{

private:
	CImgWnd m_wndRGB;
	CImgWnd m_wndDepth;
	CImgWnd* m_wndFull; 

	CSliderCtrl* m_slider1;
	CSliderCtrl* m_slider2;
	CSliderCtrl* m_slider3;
	CSliderCtrl* m_slider4;
	CSliderCtrl* m_slider5;
	CSliderCtrl* m_slider6;
	
	KinectSkeletonOpenNI* m_pSkeleton;
	HandTrackMarker* m_pHandTracker;
	Background* m_pBG;
	KinectOpenNI kinect;
	KinectCalibration* m_pCalib;
	ImageProc* m_pImgProc;

	//For test
	MarkerModel* m_pModel;
	void InitImgWnd();
	void InitKinect();

public:

	KinectOpenNI* getKinect(){return &kinect;}
	CImgWnd* getRGBWnd(){return &m_wndRGB;}
	CImgWnd* getDepthWnd(){return &m_wndDepth;}
	CImgWnd* getFullWnd(){return m_wndFull;}
	Background* getBG(){return m_pBG;};
	KinectSkeletonOpenNI* getSkeleton(){return m_pSkeleton;}
	HandTrackMarker* getHandTracker(){return m_pHandTracker;};
	KinectCalibration* getCalibration(){return m_pCalib;};
	ImageProc* getImgProc(){return m_pImgProc;};
	//For test
	MarkerModel* getModel(){return m_pModel;};

// Construction
public:
	CMRRKinectDlg(CWnd* pParent = NULL);	// standard constructor
	~CMRRKinectDlg(void){
		delete m_wndFull;
		delete m_pSkeleton;
		delete m_pHandTracker;
		delete m_pBG;
		delete m_pImgProc;
		delete m_pModel;
		delete m_pCalib;
		delete m_slider1, m_slider2, m_slider3,  m_slider4,  m_slider5,  m_slider6;
	}
// Dialog Data
	enum { IDD = IDD_MRRKINECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGreen();
	afx_msg void OnBnClickedButtonBlack();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCalib();
	afx_msg void OnBnClickedButtonTrack();
	afx_msg void OnBnClickedButtonReset();
};
