#include "StdAfx.h"
#include "KSTorsoData.h"
#include <iostream>
#include <XnCppWrapper.h>
using namespace std;


void KSTorsoData::update( BaseBuf* depthImg, KinectSkeleton* skeleton, KinectCalibration* calib, DepthGenerator* depthGen )
{
	//estimate torso point from the skeleton data
	//currentPos.x = (skeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).x + skeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).x)/2;
	//currentPos.y = (skeleton->getJointPosAt(XN_SKEL_LEFT_SHOULDER).y + skeleton->getJointPosAt(XN_SKEL_RIGHT_SHOULDER).y)/2;
	//float avgDist = 0.0f;
	//float total = 0.0f;
	//int nBound = 5;

	//if(currentPos.x < HALF_RECTWIDTH + nBound)
	//	currentPos.x = HALF_RECTWIDTH  + nBound;
	//if( currentPos.y < HALF_RECTWIDTH + nBound)
	//	currentPos.y = HALF_RECTWIDTH + nBound;
	//if(currentPos.x > ( depthImg->width()-HALF_RECTWIDTH - nBound ))
	//	currentPos.x = depthImg->height()-HALF_RECTWIDTH - nBound;
	//if( currentPos.y > (depthImg->height()-HALF_RECTWIDTH - nBound))
	//	currentPos.y = depthImg->width()-HALF_RECTWIDTH - nBound;

	//BYTE* pData = (BYTE*)(depthImg->getData());

	//int nOffsetX = currentPos.x - HALF_RECTWIDTH;
	//int nOffsetY = currentPos.y - HALF_RECTWIDTH;


	//for( int j = 0 ; j < 2* HALF_RECTWIDTH ; ++j )
	//{
	//	for( int i = 0 ; i < 2* HALF_RECTWIDTH ; ++i)
	//	{
	//		//read data from depth map
	//		//if depth - compensation 
	//		//if( abs(dist-avgDist) > 100 ) 
	//		total += (float)depthImg->getPixel16At(j+nOffsetY, i+nOffsetX);

	//	}
	//}
	//
	//avgDist = total / (2*HALF_RECTWIDTH*2*HALF_RECTWIDTH);

	////cout << "The average distance is: " << avgDist << endl;

	//6/10/2012, Calculate Torso Comp Angle and Shoulder Rotation Angle
	//Calculate Torso Comp Angle
	POINT3D currentLShoulderPos = skeleton->getJoint3DPosAt(XN_SKEL_LEFT_SHOULDER);
	POINT3D currentRShoulderPos = skeleton->getJoint3DPosAt(XN_SKEL_RIGHT_SHOULDER);
	POINT3D currentTorsoPos = skeleton->getJoint3DPosAt(XN_SKEL_TORSO);
	POINT3D centerShoulderPos;
	centerShoulderPos.x = (currentLShoulderPos.x + currentRShoulderPos.x)/2;
	centerShoulderPos.y = (currentLShoulderPos.y + currentRShoulderPos.y)/2;	
	centerShoulderPos.z = (currentLShoulderPos.z + currentRShoulderPos.z)/2;	
	
	POINT3D centerRestShoulderPos;
	centerRestShoulderPos.x = (restLShoulderPos.x + restRShoulderPos.x)/2;
	centerRestShoulderPos.y = (restLShoulderPos.y + restRShoulderPos.y)/2;
	centerRestShoulderPos.z = (restLShoulderPos.z + restRShoulderPos.z)/2;

	int sign = (centerShoulderPos.z - (restLShoulderPos.z + restRShoulderPos.z)/2 ) > 0 ? 1:(-1);
	//cout << "The CenterShoulderPos is :" << " ( " << centerShoulderPos.x << ", " << centerShoulderPos.y << ", " << centerShoulderPos.z << " ) " << endl;
	//Calculate the distance between the point and the rest Plane
	float distBTShoulderPosAndTorsoPlane = abs( restTorsoPlane.A * centerShoulderPos.x + restTorsoPlane.B * centerShoulderPos.y + restTorsoPlane.C * centerShoulderPos.z + restTorsoPlane.D ) / sqrt( restTorsoPlane.A * restTorsoPlane.A + restTorsoPlane.B * restTorsoPlane.B + restTorsoPlane.C * restTorsoPlane.C ); 
	//cout << "The distance is " << distBTShoulderPosAndTorsoPlane << endl;
	float distBTShoulderPosAndRestTorsoPos = 2*sqrt ( ( centerRestShoulderPos.x - restTorsoPos.x ) * ( centerRestShoulderPos.x - restTorsoPos.x ) +
													( centerRestShoulderPos.y - restTorsoPos.y ) * ( centerRestShoulderPos.y - restTorsoPos.y ) +
													( centerRestShoulderPos.z - restTorsoPos.z ) * ( centerRestShoulderPos.z - restTorsoPos.z ) );

	torsoComps = sign * atan ( distBTShoulderPosAndTorsoPlane/distBTShoulderPosAndRestTorsoPos )*180/PI  ;
	//cout << "The Torso Comp is " << torsoComps << endl;

	//Calculate Shoulder Rotation Angle
	POINT3D tcLShoulderPos, tcRShoulderPos, tcTorsoPos;
	float centerShoulderPosDepth = (currentLShoulderPos.z + currentRShoulderPos.z )/2;
	tcLShoulderPos.x = currentLShoulderPos.x;
	tcLShoulderPos.y = currentLShoulderPos.y;
	tcLShoulderPos.z = centerShoulderPosDepth;

	tcRShoulderPos.x = currentRShoulderPos.x;
	tcRShoulderPos.y = currentRShoulderPos.y; 
	tcRShoulderPos.z = centerShoulderPosDepth;  //Use L Shoulder Z value
	
	tcTorsoPos.x = currentTorsoPos.x;
	tcTorsoPos.y = currentTorsoPos.y;
	tcTorsoPos.z = currentTorsoPos.z;

	Plane3D tcCurrentPlane = calcPlaneFrom3Points(	tcLShoulderPos,
													tcRShoulderPos,
													tcTorsoPos
												);
	float offsetZ = currentLShoulderPos.z - restLShoulderPos.z;

	//int sign2 = ( currentRShoulderPos.z - restRShoulderPos.z - offsetZ ) >0 ? 1:(-1);
	int sign2 = -1;
	Plane3D currentPlane;
	currentPlane = calcPlaneFrom3Points(	currentLShoulderPos,
											currentRShoulderPos,
											currentTorsoPos
										);

	shoulderRot = sign2* acos( (currentPlane.A * tcCurrentPlane.A + currentPlane.B * tcCurrentPlane.B + currentPlane.C * tcCurrentPlane.C ) / 
		(sqrt(( tcCurrentPlane.A* tcCurrentPlane.A) + ( tcCurrentPlane.C* tcCurrentPlane.C) + ( tcCurrentPlane.B* tcCurrentPlane.B) ) * sqrt(( currentPlane.A* currentPlane.A) + ( currentPlane.C* currentPlane.C) + ( currentPlane.B* currentPlane.B) ) )) * 180/PI;
	cout << "The Shoulder Rotation is " << shoulderRot << endl;

}

Plane3D KSTorsoData::calcPlaneFrom3Points(POINT3D p1, POINT3D p2, POINT3D p3)
{
	//zn.wikipedia.org/plane
	//P1: lshoulder, P2: rshoulder, P3: torso
	POINT3D pt1 = p1;
	POINT3D pt2 = p2;
	POINT3D pt3 = p3;
	Plane3D plane;
	plane.A = (pt2.y - pt1.y) * (pt3.z - pt1.z) - (pt2.z - pt1.z) * (pt3.y - pt1.y);
	plane.B = (pt2.z - pt1.z) * (pt3.x - pt1.x) - (pt2.x - pt1.x) * (pt3.z - pt1.z);
	plane.C = (pt2.x - pt1.x) * (pt3.y - pt1.y) - (pt2.y - pt1.y) * (pt3.x - pt1.x);
	plane.D =	pt1.x * ( (pt2.z - pt1.z) * (pt3.y - pt1.y) - (pt2.y - pt1.y) * (pt3.z - pt1.z ) ) + 
						pt1.y * ( (pt2.x - pt1.x) * (pt3.z - pt1.z) - (pt2.z - pt1.z) * (pt3.x - pt1.x) ) + 
						pt1.z * ( (pt2.y - pt1.y) * (pt3.x - pt1.x) - (pt2.x - pt1.x) * (pt3.y - pt1.y) );

	if( plane.A == 0 && plane.B == 0 && plane.C == 0 )
		cout << "Please Calibrate again!" << endl; // error
	return plane;
}
