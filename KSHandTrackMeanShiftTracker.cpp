#include "StdAfx.h"
#include "KSHandTrackMeanshiftTracker.h"
#include <iostream>

using namespace std;
//int gnPointMode;
#define Max(a,b) a>b?a:b
POINT3D last_lastCenter;
POINT3D lastCenter;


void KSHandTrackMeanshiftTracker::init(BaseBuf* imgSrc, Rect &ROI)
{
	POINT3D startPoint;
	startPoint.x = MODEL_StartX;
	startPoint.y = MODEL_StartY;
	startPoint.z = 0;
	const int half_size_x = MODEL_W/2;
	const int half_size_y = MODEL_H/2;
	const int xDim = MODEL_W;
	const int yDim = MODEL_H;

	m_pKernel = new Kernel(MODEL_W, MODEL_H);
	///////////////////////////////////////////////////////////////
	//Read Model Image from local files
	//const char* fileszName = FILE_PATH;
	//cout << "File name is" << fileszName << endl;
	//m_dib->OpenImage(fileszName);	// Create an model Image.
	//if(!=xDim || m_dib->Height()!=yDim) 
	//	return m_bInit;
	//imgModel->copyBuffer(width, height, width*3, m_dib->GetData());
	///////////////////////////////////////////////////////////////
	m_pImgProc->cvtRGB2HSV( imgSrc, imgSrc, ROI, RGB2HSV );
	m_pTargetModel->updateModel(imgSrc, startPoint.x, startPoint.y, half_size_x, half_size_y, m_pKernel);

	m_pHandPos.x = startPoint.x;
	m_pHandPos.y = startPoint.y;
	m_pKF->setXk( startPoint.x, startPoint.y, 0, 0 );
	m_pKF->setZk( startPoint.x, startPoint.y );

	m_prevRect.x = startPoint.x - ROI.width/2;
	m_prevRect.y = startPoint.y - ROI.height/2;
	m_prevRect.height = ROI_SIZE_H;
	m_prevRect.width = ROI_SIZE_W;

}

bool KSHandTrackMeanshiftTracker::track(BaseBuf* imgSrc, BaseBuf* depth, Rect &ROI)
{

	if( !isReady() )
	{	
		init(imgSrc, ROI);
		return true;
	}

	int centerX = ROI.x + ROI.width/2;
	int centerY = ROI.y + ROI.height/2;
	int half_size_x = MODEL_W/2, half_size_y = MODEL_H/2;
	int xDim = MODEL_W, yDim = MODEL_H;
	int dx = 0, dy = 0, pdx = 0, pdy = 0;
	int LoopCount = 0;
	bool exit = false;

	//Kalman Filter
	//m_pImgProc->cvtRGB2HSV( imgSrc, imgSrc, ROI, RGB2HSV );
	m_pKF->predict();
	
	//Use meanshift to find the measure value Zk.
	m_Weight = new double*[xDim];
	for( int i=0;i<xDim;++i)
		m_Weight[i] = new double[yDim];
	half_size_x = (xDim)/2;
	half_size_y = (yDim)/2;
	LoopCount = 0;	

	while(exit == false)
	{
		pdx = dx;
		pdy = dy;
		LoopCount++;
		m_pTargetCandidate->updateModel(imgSrc, centerX, centerY, half_size_x, half_size_y, m_pKernel ); //Boundary
		updateWeight();
		calcDisplacement(half_size_x, half_size_y, &dx, &dy);
		
		centerX += dx;
		centerY += dy;
	
		if (((dx == 0) && (dy == 0)) ||  LoopCount > 13 || ( (pdx + dx == 0) && (pdy + dy==0) ))
			exit = true;

	}

	//cout << "iTeration times = " << LoopCount << endl;
	//cout << "The observed centerX is " << centerX << ", centerY is " << centerY << endl;
	if( centerX < 0 || centerY < 0 || centerX > imgSrc->width() || centerY > imgSrc->height() )
	{
		ROI.x = 0;
		ROI.y = 0;
		ROI.width = imgSrc->width();
		ROI.height = imgSrc->height();

		for( int i=0;i<xDim;++i)
			delete [] m_Weight[i];
		delete [] m_Weight;
		return false;
	}
	
	m_pKF->setZk((float)centerX, (float)(centerY) );
		
	m_pKF->correct();

	//update Positions
	centerX = (int)m_pKF->getXkAt(0,0);
	centerY = (int)m_pKF->getXkAt(1,0);
	//last_lastCenter.x = lastCenter.x;
	//last_lastCenter.y = lastCenter.y;
	//lastCenter.x = centerX;
	//lastCenter.y = centerY;
	//cout << "The bhaDistance is :" << getBhaDistance() << endl;

	ROI.height = MODEL_H;
	ROI.width = MODEL_W;
	ROI.x = centerX - ROI.width/2;
	ROI.y = centerY - ROI.height/2;

	//if(getBhaDistance()>0.8)
	exit = false;
	for( int i=0;i<xDim;++i)
		delete [] m_Weight[i];
	delete [] m_Weight;
	return true;
}

void KSHandTrackMeanshiftTracker::updateWeight()
{
	double R[NUMBINS];
	unsigned int xDim = m_pTargetCandidate->getXDim();
	unsigned int yDim = m_pTargetCandidate->getYDim();

	for( int i = 0; i< NUMBINS; ++i )
	{
		if((*m_pTargetCandidate)[i] == 0)
		{
			R[i] = 0;
		}
		else
		{
			if(xDim == MODEL_W && yDim == MODEL_H)
				R[i] = sqrt( (*m_pTargetModel)[i]/(*m_pTargetCandidate)[i] ) ;

		}
	}
	
	for(unsigned int x=0;x<m_pTargetCandidate->getXDim();++x)
	{
		for(unsigned int y=0;y<m_pTargetCandidate->getYDim();++y)
		{
			m_Weight[x][y] = R[(*m_pTargetCandidate).theBin(x,y)];
		}
	}
}

void KSHandTrackMeanshiftTracker::calcDisplacement(int half_x,int half_y, int *dx, int *dy )
{
	double weight_sum = 0;
	double x_sum = 0, y_sum = 0;
	double curPixelWeight;
	
	for (int x = -half_x;x < half_x;++x)
	{
		for( int y = -half_y;y <half_y;++y)
		{
			curPixelWeight = m_Weight[x+half_x][y+half_y]*(m_pKernel->getGradKernel())[x+half_x][y+half_y];
			weight_sum += curPixelWeight;

			x_sum += x*curPixelWeight;
			y_sum += y*curPixelWeight;
		}
	}
	if(weight_sum != 0)
	{
		*dx = (int)floor(x_sum/weight_sum);
		*dy = (int)floor(y_sum/weight_sum);
	}
	else
	{
		*dx = 0;
		*dy = 0;
	}
}

float KSHandTrackMeanshiftTracker::getBhaDistance()
{
	float sum = 0.0f;

	for( int i=0; i< NUMBINS; ++i)
	{
		sum += sqrt((*m_pTargetModel)[i]*(*m_pTargetCandidate)[i]);
	}
	return sqrt(1-sum);
}

