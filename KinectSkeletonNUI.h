#pragma once
#include "kinectskeleton.h"
#include "UserConfigs.h"
#include <XnCppWrapper.h>

using namespace xn;
class KinectSkeletonNUI :
	public KinectSkeleton
{
private:
	Context*			m_pContext;
	DepthGenerator*		m_pDepthGen;
	UserGenerator*		m_pUserGen;


public:
	KinectSkeletonNUI(void){};
	KinectSkeletonNUI(Context* context, DepthGenerator* depthGen, int jointCount);
	virtual ~KinectSkeletonNUI(void){};

public:
	//bool update();
};

