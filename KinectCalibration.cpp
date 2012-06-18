#include "StdAfx.h"
#include "KinectCalibration.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
int gnCounter = 0;  // count the number of frames to be saved.
double tempRot[3][3] = {0};
double tempTran[3][1] = {0};
KinectCalibration::KinectCalibration(void)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			m_matRot[i][j] = 0;
	for(int i=0;i<3;i++)
		for(int j=0;j<1;j++)
			m_matRot[i][j] = 0;
	for( int i=0; i<3; ++i)
	{
		m_pGP[i].x = 0;
		m_pGP[i].y = 0;
		m_pGP[i].z = 0;
	}
	m_pImgProc = new ImageProc;
	m_isSaving = false;
	m_nCalibStage = 0;		//calculating
}

KinectCalibration::~KinectCalibration(void)
{
	delete m_pImgProc;
}

//Interface
void KinectCalibration::startCalib(BaseBuf* imgRGB, BaseBuf* imgDepth, DepthGenerator* pDepthGen)
{
	const char* fileName = _CALIB_FILE_PATH"CalibrationData.txt";
	if(loadCalibrationDatafromFile(fileName)){
		cout << "Already have calibration data, load data from file\n" << endl;
		return;
	}
	
	//segmentation
	segbyColor(imgRGB, imgRGB);

	IplImage* imgTemp = cvCreateImage(cvSize(imgRGB->width(), imgRGB->height()),8,1);
	BYTE* pTemp = NULL;
	BYTE* pSrc = NULL;
	for( int j=0;j<imgRGB->height();j++)
	{
		pTemp = (BYTE*)(imgTemp->imageData+j*imgTemp->widthStep);
		pSrc = (BYTE*)(imgRGB->getData()+j*imgRGB->widthBytes());
		for( int i=0;i<imgRGB->width();i++,pTemp +=1, pSrc += 3)
		{
			*pTemp = *pSrc;
		}
	}

	POINT3D* pImgPtSet = new POINT3D[3];
	findMarkers(imgTemp, pImgPtSet);

	////copy from basebuf to IplImage
	identifyIP( pImgPtSet );		//identify the 3 points according to their position, because the three vertaxes are different in an L-frame
	for(int i=0;i<3;i++)
	{
		m_pGP[i] = cvtIPtoCamP( pImgPtSet[i] , pDepthGen );		
	}

	computeRot();
	computeTran();
	if(isSaving())
	{
		if(gnCounter<=_NFRAMES)
		{
			for(int i = 0; i<3; ++i)
			{
				for(int j = 0; j<3; ++j)
				{
					tempRot[i][j] += m_matRot[i][j];
				}
			}
			for(int i =0; i<3 ; ++i )
				tempTran[i][1] += m_matTran[i][1];
			gnCounter++;
			return;
		}
		//set rotation and translation according to the mean value
		for(int i = 0; i<3; ++i)
		{
				for(int j = 0; j<3; ++j)
				{
					m_matRot[i][j] = tempRot[i][j]/100.0f;
				}
		}
		for(int i =0; i<3 ; ++i )
			m_matTran[i][1] = tempTran[i][1]/100.0f;

		saveCalibrationDatatoFile();
		gnCounter = 0;
		m_isSaving = false;
	}
	delete[] pImgPtSet;
	cvReleaseImage(&imgTemp);
}

void KinectCalibration::startCalib(POINT3D* pImgPtSet, DepthGenerator* pDepthGen)
{
	computeRot();
	computeTran();
}
void KinectCalibration::testCalib(BaseBuf* imgRGB, BaseBuf* imgDepth, DepthGenerator* pDepthGen)
{
	segbyColor(imgRGB, imgRGB);
	IplImage* imgTemp = cvCreateImage(cvSize(imgRGB->width(), imgRGB->height()),8,1);
	BYTE* pTemp = NULL;
	BYTE* pSrc = NULL;
	for( int j=0;j<imgRGB->height();j++)
	{
		pTemp = (BYTE*)(imgTemp->imageData+j*imgTemp->widthStep);
		pSrc = (BYTE*)(imgRGB->getData()+j*imgRGB->widthBytes());
		for( int i=0;i<imgRGB->width();i++,pTemp +=1, pSrc += 3)
		{
			*pTemp = *pSrc;
		}
	}

	POINT3D* pImgPtSet = new POINT3D[3];
	findMarkers(imgTemp, pImgPtSet);

	POINT3D pGlobalPt = cvtIPtoGP(pImgPtSet[0], pDepthGen);
	cout << "The Point in Global Coord is:" << pGlobalPt.x << ", " << pGlobalPt.y << ", " << pGlobalPt.z << ", " << endl;
	delete[] pImgPtSet;
	cvReleaseImage(&imgTemp);
}
POINT3D KinectCalibration::cvtIPtoGP(POINT3D pImgPt, DepthGenerator* pDepthGen)
{
	POINT3D pCamPt = cvtIPtoCamP( pImgPt, pDepthGen );

	//if rotation and translation matrix is ready
	POINT3D pGlobalPt = cvtCamPtoGP( pCamPt ); 

	return pGlobalPt;
}

POINT3D KinectCalibration::cvtIPtoCamP( const POINT3D pImgPt, DepthGenerator* pDepthGen )
{
	int idx;
	POINT3D pTempPt, pCamPt;
	const XnDepthPixel* pDepthMap = pDepthGen->GetDepthMap() ;	//What is the problem of this sentence
	int nheight = _IMAGE_HEIGHT;
	int nwidth = _IMAGE_WIDTH;
	int nBound = 10;
	pTempPt.x = pImgPt.x; 
	pTempPt.y = pImgPt.y;

	if(pTempPt.x < nBound)
		pTempPt.x = nBound;
	if(pTempPt.x > nwidth - nBound)
		pTempPt.x = nwidth - nBound;
	if(pTempPt.y < nBound )
		pTempPt.y = nBound;
	if(pTempPt.y > (nheight - nBound))
		pTempPt.y = nheight - nBound;

	idx = pTempPt.y * nwidth + pTempPt.x;

	pCamPt.x = ( pTempPt.x - _cx ) * pDepthMap[idx]/ _fx;
	pCamPt.y = ( pTempPt.y - _cy ) * pDepthMap[idx]/ _fy;
	pCamPt.z = pDepthMap[idx];

	//print
	//cout << "Point in IP: (x, y, z) =" << pImgPt.x << ", "  << pImgPt.y << ", " << pImgPt.z << endl;
	//cout << "Point in CamP: (x, y, z) =" << pCamPt.x << ", "  << pCamPt.y << ", " << pCamPt.z << endl;
	return pCamPt;
}

POINT3D KinectCalibration::cvtCamPtoGP( POINT3D pCamPt )
{
	POINT3D ptTemp, pGlobalPt;
	//if() intrinsic and extrinsic are both correct
	//sub, then multi
	ptTemp.x = pCamPt.x - m_matTran[0][0];
	ptTemp.y = pCamPt.y - m_matTran[1][0];
	ptTemp.z = pCamPt.z - m_matTran[2][0];

	//Do matrix multification
	pGlobalPt.x = m_matRot[0][0]*ptTemp.x + m_matRot[0][1]*ptTemp.y + m_matRot[0][2]*ptTemp.z;
	pGlobalPt.y = m_matRot[1][0]*ptTemp.x + m_matRot[1][1]*ptTemp.y + m_matRot[1][2]*ptTemp.z;
	pGlobalPt.z = -(m_matRot[2][0]*ptTemp.x + m_matRot[2][1]*ptTemp.y + m_matRot[2][2]*ptTemp.z);		//z prime should be set reverse due to our need.
	
	return pGlobalPt;
}

void KinectCalibration::computeRot()
{
	POINT3D ptA, ptB, ptC;
	
	ptA.x = m_pGP[1].x-m_pGP[2].x;
	ptA.y = m_pGP[1].y-m_pGP[2].y;
	ptA.z = m_pGP[1].z-m_pGP[2].z;
	double norm = sqrt(ptA.x*ptA.x+ptA.y*ptA.y+ptA.z*ptA.z);
	if(norm != 0)
	{
		ptA.x = ptA.x/norm;
		ptA.y = ptA.y/norm;
		ptA.z = ptA.z/norm;
	}

	ptB.x = m_pGP[0].x-m_pGP[2].x;
	ptB.y = m_pGP[0].y-m_pGP[2].y;
	ptB.z = m_pGP[0].z-m_pGP[2].z;
	norm = sqrt(ptB.x*ptB.x+ptB.y*ptB.y+ptB.z*ptB.z);
	
	if(norm != 0)
	{
		ptB.x = ptB.x/norm;
		ptB.y = ptB.y/norm;
		ptB.z = ptB.z/norm;
	}

	//do cross product
	//A*B = [a2b3-a3b2,a3b1-a1b3,a1b2-a2b1]
	ptC.x = ptA.y*ptB.z - ptA.z*ptB.y;
	ptC.y = ptA.z*ptB.x - ptA.x*ptB.z;
	ptC.z = ptA.x*ptB.y - ptA.y*ptB.x;
	ptB.x = ptC.y*ptA.z - ptC.z*ptA.y;
	ptB.y = ptC.z*ptA.x - ptC.x*ptA.z;
	ptB.z = ptC.x*ptA.y - ptC.y*ptA.x;

	//We compute the Transpose of the Rotation Matrix: pGlobalPt = RotT * (pCamPt - Tran)
	m_matRot[0][0] = ptA.x;
	m_matRot[0][1] = ptA.y;
	m_matRot[0][2] = ptA.z;
	m_matRot[1][0] = ptB.x;
	m_matRot[1][1] = ptB.y;
	m_matRot[1][2] = ptB.z;
	m_matRot[2][0] = ptC.x;
	m_matRot[2][1] = ptC.y;
	m_matRot[2][2] = ptC.z;

	//print
	//cout << "Rotation Matrix is : " << m_matRot[0][0] << ", " << m_matRot[0][1] << ", " << m_matRot[0][2] << endl;
	//cout << m_matRot[1][0] << ", " << m_matRot[1][1] << ", " << m_matRot[1][2] << endl;
	//cout << m_matRot[2][0] << ", " << m_matRot[2][1] << ", " << m_matRot[2][2] << endl;

}

void KinectCalibration::computeTran()
{
	m_matTran[0][0] = m_pGP[2].x;
	m_matTran[1][0] = m_pGP[2].y;
	m_matTran[2][0] = m_pGP[2].z;
	//print
	/*cout << "Translation Vector is : " << m_matTran[0][0] << ", " << m_matTran[1][0] << ", " << m_matTran[2][0] << endl;  */
}

void KinectCalibration::saveCalibration()
{
	m_isSaving = true;
}

void KinectCalibration::saveCalibrationDatatoFile()
{
	FILE* fp;
	const char* fileName = _CALIB_FILE_PATH"calibrationData.txt";
	if((fp = fopen(fileName, "w+"))==NULL)
		return;

	for( int i = 0 ; i< 3; ++i)
	{
		fprintf( fp, "%lf, %lf, %lf\n", m_matRot[i][0], m_matRot[i][1], m_matRot[i][2] );
	}

	fprintf( fp, "%lf, %lf, %lf\n", m_matTran[0][0], m_matTran[1][0], m_matTran[2][0] );
	fclose( fp );
}

bool KinectCalibration::loadCalibrationDatafromFile(const char* fileName)
{
	FILE* fp;
	if((fp = fopen(fileName, "r")) == NULL)
		return false;
	else
	{
		for( int i = 0 ; i< 3; ++i)
		{
			fscanf_s( fp, "%lf, %lf, %lf", &m_matRot[i][0], &m_matRot[i][1], &m_matRot[i][2] );
		}
		fscanf_s( fp, "%lf, %lf, %lf", &m_matTran[0][0], &m_matTran[1][0], &m_matTran[2][0] );
		
		fclose( fp );
		return true;
	}

}
//
void KinectCalibration::identifyIP( POINT3D* pImgPtSet )
{
	double distance12 = calcDist(pImgPtSet[0], pImgPtSet[1]);
	double distance13 = calcDist(pImgPtSet[0], pImgPtSet[2]);
	double distance23 = calcDist(pImgPtSet[1], pImgPtSet[2]);

	if(distance12>distance13)
	{
		if(distance13>distance23)
		{

		}
		else if(distance12>distance23)
		{
			//Swap 1,2
			swapID(&pImgPtSet[0],&pImgPtSet[1]);
		}
		else
		{
			//Swap (1,2), Swap (2,3)
			swapID(&pImgPtSet[0],&pImgPtSet[1]);
			swapID(&pImgPtSet[1],&pImgPtSet[2]);
		}
	}
	else
	{
		if(distance12>distance23)
		{
			//Swap (2,3)
			swapID(&pImgPtSet[1],&pImgPtSet[2]);
		}
		else if(distance13>distance23)
		{
			//Swap(1,3), Swap(2,3)
			swapID(&pImgPtSet[0],&pImgPtSet[2]);
			swapID(&pImgPtSet[1],&pImgPtSet[2]);
		}
		else
		{
			//Swap (1,3)
			swapID(&pImgPtSet[0],&pImgPtSet[2]);
		}
	}

}

double KinectCalibration::calcDist( POINT3D point1, POINT3D point2 )	
{
	if(abs(point1.z)<0.1)
		point1.z = 0;
	if(abs(point2.z)<0.1)
		point2.z = 0;
	return (sqrt((point1.x-point2.x)*(point1.x-point2.x)+ 
	(point1.y-point2.y)*(point1.y-point2.y)+ 
	(point1.z-point2.z)*(point1.z-point2.z)));
}

void KinectCalibration::swapID( POINT3D *point1, POINT3D *point2 )
{
	POINT3D temp = *point1;
	*point1 = *point2;
	*point2 = temp;
}

void KinectCalibration::segbyColor( BaseBuf* imgSrc, BaseBuf* imgDst )
{
	if(imgDst->height() != imgSrc->height() || imgDst->width() != imgSrc->width())return;
	if(imgDst->widthBytes() != imgSrc->widthBytes() ) return;

	BYTE* pSrc = NULL;
	BYTE* pDst = NULL;

	int va1,va2,va3;

	//m_value1 = 186;
	//m_value2 = 132;
	//m_value3 = 22;
	m_rangeV1 = 40;
	m_rangeV2 = 40;
	m_rangeV3 = 100;

	for(int j=0;j<imgSrc->height();j++)
	{	
		pSrc = imgSrc->getData() + j*imgSrc->widthBytes();
		pDst = imgDst->getData() + j*imgDst->widthBytes();
		for(int i=0;i<imgSrc->width();i++, pSrc+=3, pDst+=3)
		{
			va3 = *(pSrc);   //V channel
			va2 = *(pSrc+1); //S channel
			va1 = *(pSrc+2); //H channel

			if( abs(va1-m_value1)<m_rangeV1 &&
				abs(va2-m_value2)<m_rangeV2 &&
				abs(va3-m_value3)<m_rangeV3 )
			{	
 				*(pDst) = 255;
				*(pDst+1) = 255;
				*(pDst+2) = 255;
			}
			else
			{
				*(pDst) = 0;
				*(pDst+1) = 0;
				*(pDst+2) = 0;
			}

		}

	}
	Rect rect;
	rect.x = 0;
	rect.y = 0; 
	rect.width = _IMAGE_WIDTH;
	rect.height = _IMAGE_HEIGHT;

	m_pImgProc->medianfilter(imgDst, rect, 3);
}

void KinectCalibration::findMarkers( IplImage* imgBin, POINT3D* pImgPtSet )
{
	CvMemStorage *memStorage = cvCreateMemStorage(0);			//Problem: memory leak
	CvSeq *contours = NULL;
	//CvSeq *tempcontours = NULL;
	double area;
	CvRect CirBoundingBox;
	CvPoint BoundingCtr;
	int idx = 0;

	cvFindContours( imgBin,
		memStorage,
		&contours,
		sizeof(CvContour),
		CV_RETR_CCOMP,
		CV_CHAIN_APPROX_NONE
		);

	for(;contours;contours=contours->h_next)
	{
		//1.Get the boundingBox for each contour
		CirBoundingBox = cvBoundingRect( contours, 0 );
		BoundingCtr = cvPoint((int)(CirBoundingBox.x+CirBoundingBox.width/2),(int)(CirBoundingBox.y+CirBoundingBox.height/2)); 
		area = CirBoundingBox.width * CirBoundingBox.height;

		//2.Filter the noise contour
		if( CirBoundingBox.x >100 && CirBoundingBox.x < 500)
		{
			if( area>30 && area<5000 && idx<3 )			//Problem: why it can't be replaced as m_nMarkerNum
			{
				//3.Draw the contours, Boundingboxes, and centers
				//DrawMarkerRegions(CirBoundingBox, BoundingCtr, IplRawImage);
				pImgPtSet[idx].x = BoundingCtr.x;
				pImgPtSet[idx].y = BoundingCtr.y;
				pImgPtSet[idx].z = 0;
				idx++;
			
			}
		}
	}
	//cout << "The point position is " << m_pMarkerPos[0].x << " " << m_pMarkerPos[0].y << " " << m_pMarkerPos[0].z  <<endl;

	cvClearMemStorage(memStorage);

}