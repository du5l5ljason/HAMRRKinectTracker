#include "StdAfx.h"
#include "KinectSkeletonOpenNI.h"

UserGenerator g_UserGenerator;
XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";


#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
		return;														\
	}
#define XN_CALIBRATION_FILE_NAME "c:\\Temp\\UserCalibration.bin"

// Save calibration to file
void SaveCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);

	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who is already calibrated
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i]))
		{
			// Save user's calibration to file
			g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
			break;
		}
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
		XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
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
	//if( nId > 1 ) return; 
	XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(nId, XN_CALIBRATION_FILE_NAME);
		
	if (rc == XN_STATUS_OK)
	{
		// Make sure state is coherent
		g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	} else {
		printf("New User %d\n", nId);
		// New user found
		if (g_bNeedPose)
		{
			printf("start post detection \n");
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			printf("request calibration after reenter \n");
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}

	//generator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		
	
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("Lost user %d\n", nId);
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
// Callback: Finished calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("Calibration failed for user %d\n", nId);
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

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(nId, XN_CALIBRATION_FILE_NAME);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("Calibration failed for user %d\n", nId);
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
	m_curUserId = 0;
	XnStatus nRetVal = m_pContext->FindExistingNode(XN_NODE_TYPE_USER, *m_pUserGen);
	if (nRetVal != XN_STATUS_OK)
	{
		nRetVal = m_pUserGen->Create(*m_pContext);
		CHECK_RC(nRetVal, "Find user generator");
	}

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected, hCalibrationInProgress, hPoseInProgress;
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

	if (m_pUserGen->GetSkeletonCap().NeedPoseForCalibration())
	{
		g_bNeedPose = TRUE;
		if (!m_pUserGen->IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
		{
			printf("Pose required, but not supported\n");
			return;
		}
		nRetVal = m_pUserGen->GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
		CHECK_RC(nRetVal, "Register to Pose Detected");
		m_pUserGen->GetSkeletonCap().GetCalibrationPose(g_strPose);
	}
	
	m_pUserGen->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
}

bool KinectSkeletonOpenNI::update() {
	XnUserID aUsers[10];
	XnUInt16 userCount=1;
	g_UserGenerator.GetUsers(aUsers,userCount);
	
	if( !g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[m_curUserId])) {
		int i;
		for( i = 0 ;i < 10 ;i ++ )
			if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]) )
				m_curUserId = i;
		
		if( i>= 10 )
			return false;
	}
		
	XnPoint3D pt, pt2;
	for( int i = 1 ; i < MAXJOINT ; i ++ ) {
		XnSkeletonJointPosition pos;
		g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition( aUsers[m_curUserId], XnSkeletonJoint(i), pos);
		pt = pos.position;
		m_pDepthGen->ConvertRealWorldToProjective(1, &pt, &pt2);

		m_jointPos[i].x = pt2.X;
		m_jointPos[i].y = pt2.Y;
		m_jointPos[i].z = 0;
	}	
	return true;
}
