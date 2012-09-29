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
	POINT3D restLShoulderPos;
	POINT3D restRShoulderPos;
	POINT3D restTorsoPos;
	POINT3D currentPos;
	Plane3D restTorsoPlane;
	float torsoComps;
	float shoulderRot;
	bool m_bIsReady;
public:

	POINT3D getRestLShoulderPos(){return restLShoulderPos;};
	POINT3D getRestRShoulderPos(){return restRShoulderPos;};
	POINT3D getRestTorsoPos(){return restTorsoPos;};
	void setRestLShoulderPos( POINT3D pos ) {
		restLShoulderPos.x = pos.x;
		restLShoulderPos.y = pos.y;
		restLShoulderPos.z = pos.z;
	};
	void setRestRShoulderPos( POINT3D pos ){
		restRShoulderPos.x = pos.x;
		restRShoulderPos.y = pos.y;
		restRShoulderPos.z = pos.z;
	};
	void setRestTorsoPos( POINT3D pos ){
		restTorsoPos.x = pos.x;
		restTorsoPos.y = pos.y;
		restTorsoPos.z = pos.z;
	};

	float getTorsoComps(){return torsoComps;};
	float getShoulderRot(){return shoulderRot;};
	POINT3D getCurrentPos(){return currentPos;};
	bool isReady(){return m_bIsReady;};
	void setReady(bool flag){m_bIsReady = flag;};

	void setCurrentPos(POINT3D point){ 
		currentPos.x = point.x;
		currentPos.y = point.y;
		currentPos.z = point.z;
	};
	void setRestPlane(Plane3D plane){ restTorsoPlane = plane;};
	void setTorsoComps(float tc){torsoComps = tc;};
	void setShoulderRot(float sr){shoulderRot = sr;};
	bool update( BaseBuf*, KinectSkeleton*, KinectCalibration* , DepthGenerator* );
public:
	KSTorsoData(void):torsoComps(-10000.0f),shoulderRot(-10000.0f), m_bIsReady(false){
		restLShoulderPos.x = 0.0f;
		restLShoulderPos.y = 0.0f;
		restLShoulderPos.z = 0.0f;
		restRShoulderPos.x = 0.0f;
		restRShoulderPos.y = 0.0f;
		restRShoulderPos.z = 0.0f;
		restTorsoPos.x = 0.0f;
		restTorsoPos.y = 0.0f;
		restTorsoPos.z = 0.0f;
		currentPos.x = 0.0f;
		currentPos.y = 0.0f;
		currentPos.z = 0.0f;
		restTorsoPlane.A = 0.0f;
		restTorsoPlane.B = 0.0f;
		restTorsoPlane.C = 0.0f;
		restTorsoPlane.D = 0.0f;
	};
	~KSTorsoData(void){
	};

	Plane3D calcPlaneFrom3Points(POINT3D p1, POINT3D p2, POINT3D p3);

};

