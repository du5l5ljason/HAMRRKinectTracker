#pragma once
#include "UserTypes.h"
#include "Buffer.h"
#include "KinectSkeletonOpenNI.h"
#include "KinectCalibration.h"
#include <XnCppWrapper.h>
#define HALF_RECTWIDTH 10
using namespace xn;
class KSTorsoData
{
	POINT3D restPos;
	POINT3D currentPos;
	float torsoComps;
	bool m_bIsReady;
public:

	POINT3D getRestPos(){return restPos;};
	void setRestPos( POINT3D pos ) {
		restPos.x = pos.x;
		restPos.y = pos.y;
		restPos.z = pos.z;
	};
	float getTorsoComps(){return torsoComps;};

	POINT3D getCurrentPos(){return currentPos;};
	bool isReady(){return m_bIsReady;};
	void setReady(bool flag){m_bIsReady = flag;};

	void setCurrentPos(POINT3D point){ 
		currentPos.x = point.x;
		currentPos.y = point.y;
		currentPos.z = point.z;
	};
	void setTorsoComps(float f){torsoComps = f;};

	void update( BaseBuf*, KinectSkeleton*, KinectCalibration* , DepthGenerator* );
public:
	KSTorsoData(void):torsoComps(0.0f),m_bIsReady(false){
		restPos.x = 0.0f;
		restPos.y = 0.0f;
		restPos.z = 0.0f;
		currentPos.x = 0.0f;
		currentPos.y = 0.0f;
		currentPos.z = 0.0f;
	};
	~KSTorsoData(void){
	};
};

