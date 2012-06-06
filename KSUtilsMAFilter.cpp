#include "StdAfx.h"
#include "KSUtilsMAFilter.h"

void KSUtilsMAFilter::smoothData( KSFrameData* frameData )
{
}

void KSUtilsMAFilter::smoothSkeleton( KinectSkeleton* skeletonData )
{
	if( m_pointSet.size() < m_nSize )
	{
		//Add data to the vector
		POINT3D jointPos[24];
		for( int i = 0; i< 24; ++i )
		{
			jointPos[i].x = skeletonData->getJoint3DPosAt(i).x;
			jointPos[i].y = skeletonData->getJoint3DPosAt(i).y;
			jointPos[i].z = skeletonData->getJoint3DPosAt(i).z;
		}
		m_pointSet.push_back(jointPos);
		return;
	}


	// if we have enough moments.
	// delete the first element, and insert the current element.
	// set the skeleton data to the smoothed data
	m_pointSet.erase(m_pointSet.begin(), m_pointSet.begin()+1);
	POINT3D jointPos[24];

	int weight = _FILTER_WEIGHT;
	for( int i = 0; i< 24; ++i )
	{
		jointPos[i].x = skeletonData->getJoint3DPosAt(i).x;
		jointPos[i].y = skeletonData->getJoint3DPosAt(i).y;
		jointPos[i].z = skeletonData->getJoint3DPosAt(i).z;
	}
	m_pointSet.push_back(jointPos);
	int size = m_pointSet.size();
	for( int i = 0; i< 24; ++i )
	{
		POINT3D temp;
		temp.x = weight*(m_pointSet.at( size - 1 ))[i].x + (1 - weight)*(m_pointSet.at( size - 2 ))[i].x;
		temp.y = weight*(m_pointSet.at( size - 1 ))[i].y + (1 - weight)*(m_pointSet.at( size - 2 ))[i].y;
		temp.z = weight*(m_pointSet.at( size - 1 ))[i].z + (1 - weight)*(m_pointSet.at( size - 2 ))[i].z;
		skeletonData->setJoint3DPosAt( i , temp, 1.0f );
	}

}