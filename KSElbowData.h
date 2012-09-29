#pragma once

#include "KinectSkeleton.h"
#include <XnCppWrapper.h>
//#include "cv.h"
#include "UserConfigs.h"

using namespace xn;
class KSElbowData
{
	float elbowExtension;
	bool m_bIsReady;
	//KinectCalibration* calib;
public:
	KSElbowData(void):elbowExtension(-10000.0f),m_bIsReady(false){
		//calib = new KinectCalibration();
	};
	~KSElbowData(void){
		//if(calib!=NULL) delete calib;	
	};
public:
	float getElbowOpening(){return elbowExtension;};
	void setElbowOpening(float eo){elbowExtension = eo;};
	bool isReady(){return m_bIsReady;};
	void setReady(bool flag){m_bIsReady = flag;};
	bool update( KinectSkeleton*, DepthGenerator* );
	void depthFilter( POINT3D*, POINT3D*, POINT3D* );
};

