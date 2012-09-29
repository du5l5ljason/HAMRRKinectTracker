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

 
class KSHandTrackMeanshiftTracker :
	public KSHandTrackData
{
private:
	ColorModel* m_pTargetModel;	    //Qu 
	ColorModel* m_pTargetCandidate; //Pu
	Kernel*		m_pKernel;
	KalmanFilter* m_pKF;
	double**    m_Weight;
	bool		m_bInit;


public:
	KSHandTrackMeanshiftTracker(void){};
	KSHandTrackMeanshiftTracker(int w, int h):KSHandTrackData(w,h),m_bInit(false){
			m_pTargetModel = new ColorModel;
			m_pTargetCandidate = new ColorModel;
			m_pKF = new KalmanFilter;
			//m_dib = new CDIBDC;
	}

	~KSHandTrackMeanshiftTracker(void){
		if(m_pTargetModel!=NULL)
			delete m_pTargetModel;
		if(m_pTargetCandidate!=NULL)
			delete m_pTargetCandidate;
		if(m_pKernel!=NULL)
			delete m_pKernel;
		if(m_pKF != NULL )
			delete m_pKF;
		};

	void init(BaseBuf*, Rect&);
	bool track(BaseBuf*, BaseBuf*, Rect&);
	
	bool isInit(){return m_bInit;};
	void setInit(bool flag){m_bInit = flag;};

private:
	void updateWeight();
	void calcDisplacement(int, int, int*, int*);

	float getBhaDistance();
};

