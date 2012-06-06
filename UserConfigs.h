#pragma once
#include <XnCppWrapper.h>

using namespace xn;
#define MODEL_W 24		//hx	Width of the model
#define MODEL_H 24		//hy	Height of the model
#define _MODEL_FILE_PATH "D:\\Model.bmp"


#define _ELBOWTHRESH 1000
#define PI 3.1415926
#define MAX_DEPTH 4096

#define _RGB_THRESH 40		//For background removal
#define _DEP_THRESH 10		//For background removal

#define _MAXARCHIVENUM 10000
#define _DATA_ARCHIVEFILE_PATH "D:\\"

#define ROI_SIZE_W 100
#define ROI_SIZE_H 100
#define MODEL_StartX 390
#define MODEL_StartY 350




//Calibration
#define _fx 527.96
#define _fy 530.62
#define _cx 315.94
#define _cy 249.10
#define _CALIB_FILE_PATH "D:\\"
#define _NFRAMES 100

//Kinect Skeleton
#define MAXJOINT 24

#define _BGPATH "D:\\My Documents\\MRRKinectTracking Images\\BG\\"

enum CVTMODE{
	RGB2GRAY,
	BGR2GRAY,
	RGB2HSV,
	BGR2HSV,
	RGB2BGR,
};

#define _FILTER_WEIGHT 0.7

enum POINTMODE{
	_PM_SKEL,
	_PM_LAST,
};
enum SYMSTATUS{
	_SS_REST,
	_SS_CALIB,
	_SS_TRACK,
};

#define SAMPLE_XML_PATH "E:\\Program Files\\OpenNI\\Data\\SamplesConfig.xml"