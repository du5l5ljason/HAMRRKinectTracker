#include "StdAfx.h"
#include "KSElbowData.h"
#include "KinectCalibration.h"
#include <iostream>

using namespace std;

KinectCalibration gCalib;
void KSElbowData::update( KinectSkeleton* skeleton, DepthGenerator* depthGen)
{
	POINT3D pIpSet[3],pCampSet[3];
	//the elbow extension is related to the joints right shoulder, right elbow, right hand
	pIpSet[0] = skeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER);
	pIpSet[1] = skeleton->getJointPosAt(XN_SKEL_RIGHT_HAND);
	pIpSet[2] = skeleton->getJointPosAt(XN_SKEL_RIGHT_ELBOW);
	depthFilter( &skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER), 
				 &skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_HAND),
				 &skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_ELBOW) );

	gCalib.setGPAt(skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER),0);
	gCalib.setGPAt(skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_HAND),1);
	gCalib.setGPAt(skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_ELBOW),2);

	//cout << "Point 1 Position: " << "< " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).x << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).y << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER).z << " >" << endl;
	//cout << "Point 2 Position: " << "< " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_HAND).x << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_HAND).y << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_HAND).z << " >" << endl;
	//cout << "Point 3 Position: " << "< " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_ELBOW).x << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_ELBOW).y << ", " << skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_ELBOW).z << " >" << endl;

	gCalib.startCalib( pIpSet, depthGen );

	POINT3D tempPos[3];
	tempPos[0] = gCalib.cvtCamPtoGP( gCalib.getGPAt(0) );
	tempPos[1] = gCalib.cvtCamPtoGP( gCalib.getGPAt(1) );
	tempPos[2] = gCalib.cvtCamPtoGP( gCalib.getGPAt(2) );
	//cout << "Point 1 Position: " << "< " << tempPos[0].x << ", " << tempPos[0].y << ", " << tempPos[0].z << " >" << endl;
	//cout << "Point 2 Position: " << "< " << tempPos[1].x << ", " << tempPos[1].y << ", " << tempPos[1].z << " >" << endl;
	//cout << "Point 3 Position: " << "< " << tempPos[2].x << ", " << tempPos[2].y << ", " << tempPos[2].z << " >" << endl;

	float dist01, dist12, dist02;
	dist01 = gCalib.calcDist( tempPos[0], tempPos[1] );
	dist12 = gCalib.calcDist( tempPos[1], tempPos[2] );
	dist02 = gCalib.calcDist( tempPos[0], tempPos[2] );

	elbowExtension = acos ((dist01*dist01 - dist02*dist02 - dist12*dist12)/(2* dist02 * dist12 ))*180/PI  ;
	//elbowExtension = 0.5f;
	//do scale transform based on the largest and smallest joint engles.
	//cout << "The elbow extension is " << elbowExtension << endl;
	//cout << "----------------------------------------" << endl;
}

void KSElbowData::depthFilter( POINT3D* point1, POINT3D* point2, POINT3D* point3 )
{
	float depth1 = point1->z;
	float depth2 = point2->z;
	float depth3 = point3->z;

	float min = depth3<(depth1<depth2?depth1:depth2)?depth3:(depth1<depth2?depth1:depth2);

	if( min > 2000 )
		min = 1000;
	if( fabs(depth1 - min) > _ELBOWTHRESH )
		point1->z = min;
	if( fabs(depth2 - min ) > _ELBOWTHRESH )
		point2->z = min;
	if( fabs(depth3 - min ) > _ELBOWTHRESH )
		point3->z = min;
}