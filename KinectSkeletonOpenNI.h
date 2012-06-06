#pragma once
#include "kinectskeleton.h"
#include  <XnCppWrapper.h>

using namespace xn;


class KinectSkeletonOpenNI :
	public KinectSkeleton
{
private:
	Context*			m_pContext;
	DepthGenerator*		m_pDepthGen;
	UserGenerator*		m_pUserGen;



public:
	KinectSkeletonOpenNI(Context* context, DepthGenerator* depthGen, int jointCount);
	KinectSkeletonOpenNI(void);
	~KinectSkeletonOpenNI(void);
	bool update();
	//we can add on a Kalman filter function to smooth the position data we want to use. But we need to 
	//first to test the Kalman filter using matlab  4.12
};

