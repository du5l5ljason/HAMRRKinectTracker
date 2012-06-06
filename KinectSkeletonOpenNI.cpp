#include "StdAfx.h"
#include "KinectSkeletonOpenNI.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;
UserGenerator g_UserGenerator;
XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";
int gnUsers = 0;
int gnCurUserId = 0;
POINT3D gElbowPosAtMoment0, gElbowPosAtMoment1;
#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
		return;														\
	}
#define CALIBRATION_FILE_NAME "E:\\UserCalibration.bin"

// Save calibration to file
void SaveCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);

	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who is already calibrate
		// Save user's calibration to file
		g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], CALIBRATION_FILE_NAME);
		break;
	}
}
// Load calibration from file
void LoadCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who isn't calibrated or currently in pose
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
		if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;

		// Load user's calibration from file
		XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], CALIBRATION_FILE_NAME);
		if (rc == XN_STATUS_OK)
		{
			// Make sure state is coherent
			g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
			g_UserGenerator.GetSkeletonCap().StartTracking(aUserIDs[i]);
		}
		break;
	}
}
// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	gnUsers = gnUsers + 1;
	cout << "New User : " << nId << endl;


	XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(nId, CALIBRATION_FILE_NAME);
	if (rc == XN_STATUS_OK)
	{
		g_UserGenerator.GetSkeletonCap().StartTracking( nId );
		cout << "Load from preset Calibration file, ready for tracking." << endl;
	}
	else
	{
		if (g_bNeedPose)
		{
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	cout << "Lost user " << nId;
	gnUsers  = gnUsers - 1;
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose %s detected for user %d\n", strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	printf("Calibration started for user %d\n", nId);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		SaveCalibration();
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }

		if (g_bNeedPose)
		{
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}


KinectSkeletonOpenNI::KinectSkeletonOpenNI(void)
{
}


KinectSkeletonOpenNI::~KinectSkeletonOpenNI(void)
{
}

KinectSkeletonOpenNI::KinectSkeletonOpenNI(Context* context, DepthGenerator* depthGen, int jointCount) : 
	 KinectSkeleton(jointCount), m_pContext(context), m_pDepthGen(depthGen){
	
	m_pUserGen = &g_UserGenerator;
	XnStatus nRetVal = m_pContext->FindExistingNode(XN_NODE_TYPE_USER, *m_pUserGen);
	if (nRetVal != XN_STATUS_OK)
	{
		nRetVal = m_pUserGen->Create(*m_pContext);
		CHECK_RC(nRetVal, "Find user generator");
	}

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
	if (!m_pUserGen->IsCapabilitySupported(XN_CAPABILITY_SKELETON))
	{
		printf("Supplied user generator doesn't support skeleton\n");
		return;
	}
	nRetVal = m_pUserGen->RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	CHECK_RC(nRetVal, "Register to user callbacks");
	nRetVal = m_pUserGen->GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
	CHECK_RC(nRetVal, "Register to calibration start");
	nRetVal = m_pUserGen->GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
	CHECK_RC(nRetVal, "Register to calibration complete");

	if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
	{
		g_bNeedPose = TRUE;
		if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
		{
			printf("Pose required, but not supported\n");
			return;
		}
		nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
		CHECK_RC(nRetVal, "Register to Pose Detected");
		g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
	}


	nRetVal = m_pUserGen->GetSkeletonCap().SetSmoothing(XnFloat(0.7f));
	m_pUserGen->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_UPPER);
}

bool KinectSkeletonOpenNI::update() {
	XnUserID aUsers[10];
	XnUInt16 userCount=g_UserGenerator.GetNumberOfUsers();
	XnSkeletonJointPosition pos;	
	XnStatus rc = g_UserGenerator.GetUsers(aUsers,userCount);

	if( rc != XN_STATUS_OK)
		return false;
	
	if(userCount == 1)
		gnCurUserId = aUsers[0];


	if( g_UserGenerator.GetSkeletonCap().IsTracking( gnCurUserId )) 
	{
		XnPoint3D pt, pt2;
		for( int i = 1 ; i < MAXJOINT ; i ++ ) 
		{

			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition( gnCurUserId, XnSkeletonJoint(i), pos);	
			pt = pos.position;

			//save position info to the class
			m_joint3DPos[i].x = pos.position.X;
			m_joint3DPos[i].y = pos.position.Y;
			m_joint3DPos[i].z = pos.position.Z;

			m_pDepthGen->ConvertRealWorldToProjective(1, &pt, &pt2);

			m_jointPos[i].x = pt2.X;
			m_jointPos[i].y = pt2.Y;
			m_jointPos[i].z = 0;

			/*if( i == XN_SKEL_RIGHT_ELBOW && )
			{
				gElbowPosAtMoment1.x = pos.position.X;
				gElbowPosAtMoment1.y = pos.position.Y;
				gElbowPosAtMoment1.z = pos.position.Z;
				gElbowPosAtMoment0.x = gElbowPosAtMoment1.x;
				gElbowPosAtMoment0.y = gElbowPosAtMoment1.y;
				gElbowPosAtMoment0.z = gElbowPosAtMoment1.z; 
			}*/
		}	
	}

	return true;
}
