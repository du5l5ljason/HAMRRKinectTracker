#pragma once
#include <XnCppWrapper.h>

using namespace xn;

#define _IMAGE_WIDTH 640
#define _IMAGE_HEIGHT 480
#define _RECORDVIDEO_WIDTH 640
#define _RECORDVIDEO_HEIGHT 480

#define MODEL_W 26		//hx	Width of the model
#define MODEL_H 26		//hy	Height of the model


#define _ELBOWTHRESH 1000
#define PI 3.1415926
#define MAX_DEPTH 4096

#define _RGB_THRESH 40		//For background removal
#define _DEP_THRESH 10		//For background removal

#define _MAXARCHIVENUM 10000

#define ROI_SIZE_W 100
#define ROI_SIZE_H 100
#define MODEL_StartX 405
#define MODEL_StartY 280




//Calibration
#define _fx 527.96
#define _fy 530.62
#define _cx 315.94
#define _cy 249.10
#define _NFRAMES 100

//Kinect Skeleton
#define MAXJOINT 24

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

enum DATASTREAMSTATUS{
	_DS_CLOSE,
	_DS_READY,
	_DS_OPEN,
};

#define _BGPATH "D:\\My Documents\\MRRKinectTracking Images\\BG\\"
#define _SAMPLE_XML_PATH "C:\\Program Files (x86)\\OpenNI\\Data\\SamplesConfig.xml"
#define _DATA_ARCHIVEFILE_PATH "C:\\Users\\Home System\\My Documents\\KinectArchive\\ArchiveData\\"
#define _CALIB_FILE_PATH "C:\\Users\\Home System\\My Documents\\KinectArchive\\CalibrationData\\"
#define _MODEL_FILE_PATH "C:\\Model.bmp"
#define _VIDEO_RECORD_FILE_PATH "C:\\Users\\Home System\\My Documents\\KinectArchive\\HAMRR VideoClips\\"