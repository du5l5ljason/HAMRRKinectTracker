#pragma once
#include "StructDefine.h"
#include <XnCppWrapper.h>
#include "Buffer.h"
#include "ImageProc.h"
#include "cv.h"
#include "highgui.h"
using namespace xn;

#define _fx 527.96
#define _fy 530.62
#define _cx 315.94
#define _cy 249.10

class KinectCalibration
{

public:
	void Calibrate(BaseBuf*, BaseBuf*, DepthGenerator*);
	POINT3D cvtIPtoGP(POINT3D, DepthGenerator*);		//Convert coordinate :Image point to Global point("L"Frame-based)
	
	//double **getRot(){return;};
	//double *getTran(){return;};
	//void setRot(double **rot){};
	//void setTran(double **tran){};
	void setCalibStage(int n){m_nCalibStage = n;};
public:
	KinectCalibration(void);
	~KinectCalibration(void);

private:
	void calcCalib(BaseBuf*, BaseBuf*, DepthGenerator*);
	void testCalib(BaseBuf*, BaseBuf*, DepthGenerator*);
	POINT3D cvtIPtoCamP(POINT3D, DepthGenerator*);		//Convert coordinate: Image point to Camera point 
	POINT3D cvtCamPtoGP(POINT3D);		//Convert coordinate: Camera point to global point
	void computeRot();			//Compute rotation matrix
	void computeTran();			//Compute tranlation vector
	void identifyIP( POINT3D* );
	double calcDist( POINT3D, POINT3D );
	void swapID( POINT3D*, POINT3D* );
	void segbyColor(BaseBuf*, BaseBuf*, int nColor);
	void findMarkers(IplImage*, POINT3D*);
private:
	POINT3D m_pGP[3];
	ImageProc* m_pImgProc;
	double m_matRot[3][3];			//Rotation Matrix
	double m_matTran[3][1];			//Translation Matrix
	int	m_nCalibStage;				//0 - calculating; 1 - testing

	//for debugging
	int m_value1;
	int m_value2;
	int m_value3;
	int m_rangeV1;
	int m_rangeV2;
	int m_rangeV3;

public:
	int getValue1(){return m_value1;};
	int getValue2(){return m_value2;};
	int getValue3(){return m_value3;};
	int getRangeV1(){return m_rangeV1;};
	int getRangeV2(){return m_rangeV2;};
	int getRangeV3(){return m_rangeV3;};
		void setValue1(int val){
		m_value1 = val; 
		if(m_value1>400)m_value1 = 400;
		if(m_value1<0)m_value1 = 0;
			}

	void setValue2(int val){
		m_value2 = val; 
		if(m_value2>255)m_value2 = 255;
		if(m_value2<0)m_value2 = 0;
			}
	void setValue3(int val){
		m_value3 = val; 
		if(m_value3>255)m_value3 = 400;
		if(m_value3<0)m_value3 = 0;
			}
	void setRangeV1(int val){
		m_rangeV1 = val; 
		if(m_rangeV1>100)m_rangeV1 = 100;
		if(m_rangeV1<0)m_rangeV1 = 0;
			}
	void setRangeV2(int val){
		m_rangeV2 = val; 
		if(m_rangeV2>100)m_rangeV2 = 100;
		if(m_rangeV2<0)m_rangeV2 = 0;
			}
	void setRangeV3(int val){
		m_rangeV3 = val; 
		if(m_rangeV3>100)m_rangeV3 = 100;
		if(m_rangeV3<0)m_rangeV3 = 0;
			}
};

