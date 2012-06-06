#pragma once
#include "KSFrameData.h"
#include "KinectSkeleton.h"
#include "UserTypes.h"
class KSUtilsFilter
{
public:
	KSUtilsFilter(void){};
	virtual ~KSUtilsFilter(void){};

	virtual void smoothData( KSFrameData* frameData )=0;
	virtual void smoothSkeleton( KinectSkeleton* skeletonData ) = 0;
};

