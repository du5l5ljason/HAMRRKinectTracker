#include "StdAfx.h"
#include "DrawWnd.h"
#include <XnCppWrapper.h>

CDrawWnd::CDrawWnd(void)
{
}
CDrawWnd::~CDrawWnd(void)
{
}

void CDrawWnd::showSkeleton(POINT3D* jointPos)
{
	if(jointPos==NULL)return;

	int bluecolor[] = {0, 0, 255};
	int greencolor[] = {0, 255, 0};
	int redcolor[] = {255, 0, 0};

	int jointPair[][2] = { 
		{XN_SKEL_HEAD, XN_SKEL_NECK},					
		{XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER},
		{XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW},
		{XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND},
		{XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER},
		{XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW},
		{XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND},
		{XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO},
		{XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO},
		{XN_SKEL_TORSO, XN_SKEL_LEFT_HIP},
		//{XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE},
		//{XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT},
		{XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP},
		//{XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE},
		//{XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT},
		{XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP},
	};
	for( int i = 0 ;i < 12 ;i ++ ) {
		int id1 = jointPair[i][0];
		int id2 = jointPair[i][1];
		showLine(jointPos[id1].x, jointPos[id1].y, jointPos[id2].x, jointPos[id2].y, bluecolor);
	}
	
	int jointKey[3] = {
		XN_SKEL_HEAD,
		XN_SKEL_RIGHT_HAND,
		XN_SKEL_LEFT_HAND
	};

	int size1 = 40;
	int size2 = 5;

	//showEllipse(jointPos[jointKey[0]].x-size1, jointPos[jointKey[0]].y-size1,jointPos[jointKey[0]].x+size1, jointPos[jointKey[0]].y+size1, bluecolor);
	//showEllipse(jointPos[jointKey[1]].x-size2, jointPos[jointKey[1]].y-size2,jointPos[jointKey[1]].x+size2, jointPos[jointKey[1]].y+size2, redcolor);
	//showEllipse(jointPos[jointKey[2]].x-size2, jointPos[jointKey[2]].y-size2,jointPos[jointKey[2]].x+size2, jointPos[jointKey[2]].y+size2, redcolor);

}
	
void CDrawWnd::showHandJoint(POINT3D RightPos)
{
	int size = 10;
	int redcolor[] = {255, 0, 0};
	int bluecolor[] = {0, 0, 255};
	int greencolor[] = {0, 255, 0};

	showLine(RightPos.x-size, RightPos.y, RightPos.x+size, RightPos.y, redcolor);
	showLine(RightPos.x, RightPos.y-size, RightPos.x, RightPos.y+size, redcolor);
	//showEllipse(RightPos.x-size, RightPos.y-size, RightPos.x+size, RightPos.y+size, redcolor);
	//showEllipse(jointPos[jointKey[2]].x-size2, jointPos[jointKey[2]].y-size2,jointPos[jointKey[2]].x+size2, jointPos[jointKey[2]].y+size2, redcolor);
}
void CDrawWnd::showHandJoint(POINT3D leftPos, POINT3D RightPos)
{
}