
// MRRKinectDlg.h : header file
//

#pragma once

#include <XnCppWrapper.h>
#include "ImgWnd.h"
#include "DrawWnd.h"
#include "KinectSensor.h"
#include "KinectSensorOpenNI.h"
#include "KinectSkeletonOpenNI.h"
#include "KinectCalibration.h"
#include "KSUtilsFilter.h"
#include "KSUtilsMAFilter.h"
#include "KSUtilsVideoRecorder.h"
#include "Background.h"
#include "ImageProc.h"
#include "UserTypes.h"
#include "UserConfigs.h"
#include "KSHandTrackData.h"
#include "KSHandTrackMeanshiftTracker.h"
#include "KSHandTrackCamShiftTracker.h"
#include "KSTorsoData.h"
#include "KSElbowData.h"
#include "KSFrameData.h"
#include "OSFrameData.h"
#include "KSArchivingData.h"
#include "KSFrameDataSender.h"
#include "KSFrameDataReceiver.h"

//For test
#include "ColorModel.h"

using namespace xn;
// CMRRKinectDlg dialog
class CMRRKinectDlg : public CDialogEx
{

private:
	CImgWnd m_wndRGB;
	CImgWnd m_wndDepth;
	CImgWnd* m_wndFull; 
	
	KinectSkeletonOpenNI*	m_pSkeleton;
	Background*				m_pBG;
	KinectOpenNI			kinect;
	KinectCalibration*		m_pCalib;
	KSUtilsFilter*			m_pFilter;
	KSUtilsVideoRecorder*   m_pVideoRecorder;
	ImageProc*				m_pImgProc;
	KSHandTrackData*		m_pTracker;
	KSTorsoData*			m_pTorsoData;
	KSElbowData*			m_pElbowData;
	KSFrameData*			m_pFrameData;
	KSFrameDataSender*		m_pSender;
	//KSFrameDataReceiver*	m_pReceiver;
	OSFrameData*			m_pOSFrameData;
	KSArchivingData*		m_pArchivingData;
	//For test
	ColorModel*				m_pModel;
	int						m_nDisplayType;
	void InitImgWnd();
	void InitKinect();

public:

	KinectOpenNI*			getKinect(){return &kinect;}
	CImgWnd*				getRGBWnd(){return &m_wndRGB;}
	CImgWnd*				getDepthWnd(){return &m_wndDepth;}
	CImgWnd*				getFullWnd(){return m_wndFull;}
	Background*				getBG(){return m_pBG;};
	KSUtilsFilter*			getFilter(){return m_pFilter;};
	KSUtilsVideoRecorder*   getRecorder(){return m_pVideoRecorder;};
	KinectSkeletonOpenNI*	getSkeleton(){return m_pSkeleton;}
	KSHandTrackData*		getTracker(){return m_pTracker;};
	KinectCalibration*		getCalibration(){return m_pCalib;};
	ImageProc*				getImgProc(){return m_pImgProc;};
	KSTorsoData*			getTorsoData(){return m_pTorsoData;};
	KSElbowData*			getElbowData(){return m_pElbowData;};
	KSFrameData*			getFrameData(){return m_pFrameData;};
	KSFrameDataSender*		getSender(){return m_pSender;};
	//KSFrameDataReceiver*	getReceiver(){return m_pReceiver;};
	OSFrameData*			getOSFrameData(){return m_pOSFrameData;};
	KSArchivingData*		getArchivingData(){return m_pArchivingData;};
	//For test
	ColorModel*				getModel(){return m_pModel;};
	int						DisplayType(){return m_nDisplayType;};
//Methods
// Construction
public:
	CMRRKinectDlg(CWnd* pParent = NULL);	// standard constructor
	~CMRRKinectDlg(void){
		if(m_wndFull)delete m_wndFull;
		if(m_pSkeleton)delete m_pSkeleton;
		if(m_pTracker)delete m_pTracker;
		if(m_pBG)delete m_pBG;
		//if(m_pFilter!=NULL)delete m_pFilter;
		if(m_pVideoRecorder)delete m_pVideoRecorder;
		if(m_pImgProc)delete m_pImgProc;
		if(m_pModel)delete m_pModel;
		if(m_pCalib)delete m_pCalib;
		if(m_pTorsoData)delete m_pTorsoData;
		if(m_pElbowData)delete m_pElbowData;
		if(m_pFrameData)delete m_pFrameData;
		if(m_pOSFrameData)delete m_pOSFrameData;
		if(m_pSender)delete m_pSender;
		//if(m_pReceiver)delete m_pReceiver;
		if(m_pArchivingData)delete m_pArchivingData;
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
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCalib();
	afx_msg void OnBnClickedButtonTrack();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonSetmodel();
	afx_msg void OnBnClickedButtonArchive();
	afx_msg void OnBnClickedButtonEndarchive();
	afx_msg void OnBnClickedButtonSavecalib();
	afx_msg void OnBnClickedButtonTestcalib();
	afx_msg void OnBnClickedButtonResetcalib();
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnBnClickedButtonRecordEnd();
	afx_msg void OnBnClickedButtonNewcalib();
	afx_msg void OnBnClickedButtonLoadcalib();


	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnBnClickedButtonEndRun();
};
