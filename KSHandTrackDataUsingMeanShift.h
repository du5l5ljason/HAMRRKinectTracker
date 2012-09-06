#pragma once
//////////////////////////////////////////////////
//Class Name: HandTrackMeanShift derived from Class HandTrack
//Description: 
//Input: Kinect Image Datastream (per frame): 
//RGB Image frame - imgRGB
//Depth Image Frame - imgDepth
//Last Frame hand Position - Rect ROI
//Process: 
//Implement meanshift tracking algorithm to find the hand position in the current frame based on color model.
//Output: Current hand position - Rect ROI
//The algorithm is from "Dorin Comaniciu, Ramesh, Meer. "Real-Time Tracking of Non-Rigid Objects using Mean Shift""
///////////////////////////////////////////////////
#include <cv.h>
//#include "DIBDC.h"
#include "Buffer.h"
#include "KSHandTrackData.h"
#include "KalmanFilter.h"
#include "ColorModel.h"
#include "Kernel.h"
#include "UserConfigs.h"

 
class KSHandTrackDataUsingMeanShift :
	public KSHandTrackData
{
//private:
//	ColorModel* m_pTargetModelS;	//Qu for small
//	ColorModel* m_pTargetModelH;	//Qu with horizontal rotation
//	ColorModel* m_pTargetModelV;    //Qu with vertical rotation
//	ColorModel* m_pTargetModelN;	//Qu with normal size
//	ColorModel* m_pTargetCandidate; //Pu
//	Kernel*		m_pKernel;
//	KalmanFilter* m_pKF;
//	double**    m_Weight;
//	bool		m_bInit;
//	bool		m_bIsReady;
//	//CDIBDC*		m_dib;			//To open Image from file, we need to call class CDIBDC
//public:
//	KSHandTrackDataUsingMeanShift(void){};
//	KSHandTrackDataUsingMeanShift(int w, int h):KSHandTrackData(w,h),m_bInit(false), m_bIsReady(false){
//			m_pTargetModelN = new ColorModel;
//			m_pTargetModelV = new ColorModel;
//			m_pTargetModelS = new ColorModel;
//			m_pTargetModelH = new ColorModel;
//			m_pTargetCandidate = new ColorModel;
//			m_pKF = new KalmanFilter;
//			//m_dib = new CDIBDC;
//	}
//
//	~KSHandTrackDataUsingMeanShift(void){
//		if(m_pTargetModelH!=NULL)
//			delete m_pTargetModelH;
//		if(m_pTargetModelS!=NULL)
//			delete m_pTargetModelS;
//		if(m_pTargetModelV!=NULL)
//			delete m_pTargetModelV;
//		if(m_pTargetModelN!=NULL)
//			delete m_pTargetModelN;
//		if(m_pTargetCandidate!=NULL)
//			delete m_pTargetCandidate;
//		if(m_pKernel!=NULL)
//			delete m_pKernel;
//		if(m_pKF != NULL )
//			delete m_pKF;
//		};
//
//	bool init(BaseBuf*, Rect&);
//
//	void updateHandPos(BaseBuf*, BaseBuf*, Rect&);
//	bool isInit(){return m_bInit;};
//	void setInit(bool flag){m_bInit = flag;};
//	bool isReady(){return m_bIsReady;};
//	void setReady(bool){m_bIsReady = true;};
//
//private:
//	void updateWeight();
//	void calcDisplacement(int, int, int*, int*);
//
//	float getBhaDistance();
};

