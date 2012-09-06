
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
#include "KSHandTrackDataUsingMeanShift.h"
#include "KSTorsoData.h"
#include "KSElbowData.h"
#include "KSFrameData.h"
#include "KSArchivingData.h"
#include "KSFrameDataSender.h"

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
	KSHandTrackDataUsingMeanShift* m_pHandTrackData;
	KSTorsoData*			m_pTorsoData;
	KSElbowData*			m_pElbowData;
	KSFrameData*			m_pFrameData;
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
	KSHandTrackDataUsingMeanShift* getHandTrackData(){return m_pHandTrackData;};
	KinectCalibration*		getCalibration(){return m_pCalib;};
	ImageProc*				getImgProc(){return m_pImgProc;};
	KSTorsoData*			getTorsoData(){return m_pTorsoData;};
	KSElbowData*			getElbowData(){return m_pElbowData;};
	KSFrameData*			getFrameData(){return m_pFrameData;};
	KSArchivingData*		getArchivingData(){return m_pArchivingData;};
	//For test
	ColorModel*				getModel(){return m_pModel;};
	int						DisplayType(){return m_nDisplayType;};
//Methods
// Construction
public:
	CMRRKinectDlg(CWnd* pParent = NULL);	// standard constructor
	~CMRRKinectDlg(void){
		//if(m_wndFull!=NULL)delete m_wndFull;
		//if(m_pSkeleton!=NULL)delete m_pSkeleton;
		//if(m_pHandTrackData!=NULL)delete m_pHandTrackData;
		//if(m_pBG!=NULL)delete m_pBG;
		//if(m_pFilter!=NULL)delete m_pFilter;
		//if(m_pVideoRecorder!=NULL)delete m_pVideoRecorder;
		//if(m_pImgProc!=NULL)delete m_pImgProc;
		//if(m_pModel!=NULL)delete m_pModel;
		//if(m_pCalib!=NULL)delete m_pCalib;
		//if(m_pTorsoData!=NULL)delete m_pTorsoData;
		//if(m_pElbowData!=NULL)delete m_pElbowData;
		//if(m_pFrameData!=NULL)delete m_pFrameData;
		//if(m_pArchivingData!=NULL)delete m_pArchivingData;
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
};
