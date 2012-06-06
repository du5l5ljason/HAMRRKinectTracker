#include "StdAfx.h"
#include "KinectSkeletonNUI.h"

#include <iostream>

KinectSkeletonNUI::KinectSkeletonNUI(Context* context, DepthGenerator* depthGen, int jointCount): 
	 KinectSkeleton(jointCount), m_pContext(context), m_pDepthGen(depthGen){
	 


}