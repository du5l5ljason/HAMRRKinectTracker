#include "StdAfx.h"
#include "KSTorsoData.h"
#include <iostream>
#include <XnCppWrapper.h>
using namespace std;


void KSTorsoData::update( BaseBuf* depthImg, KinectSkeleton* skeleton, KinectCalibration* calib, DepthGenerator* depthGen )
{
	//estimate torso point from the skeleton data
	currentPos.x = (skeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).x + skeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).x)/2;
	currentPos.y = (skeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).y + skeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).y)/2;
	float avgDist = 0.0f;
	float total = 0.0f;
	int nBound = 5;

	if(currentPos.x < HALF_RECTWIDTH + nBound)
		currentPos.x = HALF_RECTWIDTH  + nBound;
	if( currentPos.y < HALF_RECTWIDTH + nBound)
		currentPos.y = HALF_RECTWIDTH + nBound;
	if(currentPos.x > ( depthImg->width()-HALF_RECTWIDTH - nBound ))
		currentPos.x = depthImg->height()-HALF_RECTWIDTH - nBound;
	if( currentPos.y > (depthImg->height()-HALF_RECTWIDTH - nBound))
		currentPos.y = depthImg->width()-HALF_RECTWIDTH - nBound;

	BYTE* pData = (BYTE*)(depthImg->getData());

	int nOffsetX = currentPos.x - HALF_RECTWIDTH;
	int nOffsetY = currentPos.y - HALF_RECTWIDTH;

	//if(avgDist == 0.0f) 
	//{
	//	avgDist = depthImg->getPixel16At(nOffsetY, nOffsetX);return;
	//}

	for( int j = 0 ; j < 2* HALF_RECTWIDTH ; ++j )
	{
		for( int i = 0 ; i < 2* HALF_RECTWIDTH ; ++i)
		{
			//read data from depth map
			//if depth - compensation 
			//if( abs(dist-avgDist) > 100 ) 
			total += (float)depthImg->getPixel16At(j+nOffsetY, i+nOffsetX);

		}
	}
	
	avgDist = total / (2*HALF_RECTWIDTH*2*HALF_RECTWIDTH);

	//Because we want to calculate the torso compensation in our marker-based coordinate, we need to transfer the position into that coordinate
	//currentPos = calib->cvtCamPtoGP( tempPos );

	torsoComps = avgDist - restPos.z;

	//cout << "The torso Comps is " << torsoComps << endl;
}
