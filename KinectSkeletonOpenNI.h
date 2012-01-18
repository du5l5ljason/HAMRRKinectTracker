#pragma once
#include "kinectskeleton.h"
#include "ImgWnd.h"
#include  <XnCppWrapper.h>
using namespace xn;

class KinectSkeletonOpenNI :
	public KinectSkeleton
{
private:
	Context*			m_pContext;
	DepthGenerator*		m_pDepthGen;
	UserGenerator*		m_pUserGen;
	int					m_curUserId;


public:
	KinectSkeletonOpenNI(Context* context, DepthGenerator* depthGen, int jointCount);
	KinectSkeletonOpenNI(void);
	~KinectSkeletonOpenNI(void);

	POINT2D convertJoint2DAt(int idx) { POINT2D pt; return pt; };
	bool update();
};

