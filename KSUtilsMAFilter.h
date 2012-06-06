#pragma once
#include "KSUtilsFilter.h"
#include "KSFrameData.h"
#include "KinectSkeleton.h"
#include "UserTypes.h"
#include "UserConfigs.h"
#include <vector>


using namespace std;
class KSUtilsMAFilter :
	public KSUtilsFilter
{
	vector<KSFrameData> m_frameDataSet;
	vector<POINT3D*> m_pointSet;
	int m_nSize;
public:
	KSUtilsMAFilter(int size):m_nSize(size){};
	~KSUtilsMAFilter(void){};

	int getFilterSize(){return m_nSize;};
	void setFilterSize( int size ){ m_nSize = size;};

public:

	void smoothData( KSFrameData* frameData );
	void smoothSkeleton( KinectSkeleton* skeletonData );
};

