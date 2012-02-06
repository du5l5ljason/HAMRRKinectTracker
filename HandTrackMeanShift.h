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
#include "DIBDC.h"
#include "Buffer.h"
#include "handtrack.h"
#include "ColorModel.h"
#include "Kernel.h"
#define MODEL_W 40		//hx	should be odd
#define MODEL_H 40		//hy	should be odd
#define FILE_PATH "D:\\Model.bmp" 
class HandTrackMeanShift :
	public HandTrack
{
private:
	ColorModel* m_pTargetModel;	//Qu
	ColorModel* m_pTargetCandidate; //Pu
	Kernel*		m_pKernel;
	double**    m_Weight;
	bool		m_bInit;
	bool		m_bShouldInit;
	CDIBDC*		m_dib;			//To open Image from file, we need to call class CDIBDC
public:
	HandTrackMeanShift(void){};
	HandTrackMeanShift(int w, int h):HandTrack(w,h),m_bInit(false), m_bShouldInit(false){
			m_pTargetModel = new ColorModel;
			m_pTargetCandidate = new ColorModel;
			m_dib = new CDIBDC;
	}

	~HandTrackMeanShift(void){
		if(m_pTargetModel!=NULL)
			delete m_pTargetModel;
		if(m_pTargetCandidate!=NULL)
			delete m_pTargetCandidate;
		if(m_dib!=NULL)
			delete m_dib;
		delete m_pKernel;
		};

	bool init(BaseBuf*, Rect&);

	void updateHandPos(BaseBuf*, BaseBuf*, Rect&);
	bool isInit(){return m_bInit;};
	bool shouldInit(){return m_bShouldInit;};
	void setShouldInit(bool){m_bShouldInit = true;};
private:
	void updateWeight();
	void calcDisplacement(int, int, int*, int*);
};

