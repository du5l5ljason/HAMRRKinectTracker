#include "StdAfx.h"
#include "KSHandTrackDataUsingMeanShift.h"
#include <iostream>

using namespace std;
//int gnPointMode;
#define Max(a,b) a>b?a:b
POINT3D last_lastCenter;
POINT3D lastCenter;
bool KSHandTrackDataUsingMeanShift::init(BaseBuf* imgSrc, Rect &ROI)
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
	//April. 11th Kernel we should have four.
	//Kernel* kernelS = new Kernel(MODEL_W-10,MODEL_H-10);
	//Kernel* kernelV = new Kernel(MODEL_W, MODEL_H-10);
	//Kernel* kernelH = new Kernel(MODEL_W-10, MODEL_H);
	/////////////////////////////////////////////////////////////////
	//Read Model Image from local files
	//const char* fileszName = FILE_PATH;
	//cout << "File name is" << fileszName << endl;
	//m_dib->OpenImage(fileszName);	// Create an model Image.
	//if(!=xDim || m_dib->Height()!=yDim) 
	//	return m_bInit;
	//imgModel->copyBuffer(width, height, width*3, m_dib->GetData());
	/////////////////////////////////////////////////////////////////
	//m_pImgProc->cvtRGB2HSV(rgb,rgb, BGR2HSV);
	m_pTargetModelN->updateModel(imgSrc, startPoint.x, startPoint.y, half_size_x, half_size_y, m_pKernel);
//	m_pTargetModelS->updateModel(imgSrc, startPoint.x, startPoint.y, half_size_x-5, half_size_y-5, kernelS);
//  m_pTargetModelV->updateModel(imgSrc, startPoint.x, startPoint.y, half_size_x, half_size_y-5, kernelV);
//	m_pTargetModelH->updateModel(imgSrc, startPoint.x, startPoint.y, half_size_x-5, half_size_y, kernelH);

	m_pHandPos.x = startPoint.x;
	m_pHandPos.y = startPoint.y;
	m_pKF->setXk( startPoint.x, startPoint.y, 0, 0 );
	m_pKF->setZk( startPoint.x, startPoint.y );

	ROI.x = startPoint.x - ROI.width/2;
	ROI.y = startPoint.y - ROI.height/2;
	m_bInit = true;
	return m_bInit;

}
void KSHandTrackDataUsingMeanShift::updateHandPos(BaseBuf* imgSrc, BaseBuf* depth, Rect &ROI)
{

	if(isReady()&&!isInit())
		init(imgSrc, ROI);
	if(isInit())
	{
		int centerX = ROI.x + ROI.width/2;
		int centerY = ROI.y + ROI.height/2;
		int half_size_x = MODEL_W/2;
		int half_size_y = MODEL_H/2;
		int xDim = MODEL_W;
		int yDim = MODEL_H;
		int dx = 0;
		int dy = 0;
		int pdx = 0;
		int pdy = 0;
		int LoopCount = 0;
		bool exit = false;
		//Kalman Filter

		m_pKF->predict();
		int stepX = 0;
		int stepY = 0;
		//Use meanshift to find the measure value Zk.
		//for(stepX=-10;stepX<=0;stepX+=10)
		//{
		//	for(stepY=-10;stepY<=0;stepY+=10)
		//	{
				//m_pKernel = new Kernel(xDim+stepX, yDim+stepY);
				m_Weight = new double*[xDim+stepX];
				for( int i=0;i<xDim+stepX;++i)
					m_Weight[i] = new double[yDim+stepY];
				half_size_x = (xDim+stepX)/2;
				half_size_y = (yDim+stepY)/2;
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
				//每次算完，把meanshift得到的centerX，和centerY的值，更新给Zk

				exit = false;
				//delete m_pKernel;
				for( int i=0;i<xDim+stepX;++i)
					delete [] m_Weight[i];
				delete [] m_Weight;
		//	}

		//}
		//cout << "The observed centerX is " << centerX << ", centerY is " << centerY << endl;
		//m_pKF->setZk((float)centerX, (float)(centerY) );
		
		//m_pKF->correct();

		//update Positions
		//centerX = (int)m_pKF->getXkAt(0,0);
		//centerY = (int)m_pKF->getXkAt(1,0);
		//last_lastCenter.x = lastCenter.x;
		//last_lastCenter.y = lastCenter.y;
		//lastCenter.x = centerX;
		//lastCenter.y = centerY;
		//cout << "The bhaDistance is :" << getBhaDistance() << endl;

		//如果distance 大于0.75，使用estimate的值。estimate的值用centerX加上velocity 值
		ROI.height = ROI_SIZE_H;
		ROI.width = ROI_SIZE_W;
		ROI.x = centerX - ROI.width/2;
		ROI.y = centerY - ROI.height/2;

		//if(getBhaDistance()>0.8)

	}
}

void KSHandTrackDataUsingMeanShift::updateWeight()
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
			//if(xDim == MODEL_W && yDim == MODEL_H)
				R[i] = sqrt( (*m_pTargetModelN)[i]/(*m_pTargetCandidate)[i] ) ;
			//else if(xDim == MODEL_W-10 && yDim == MODEL_H)
			//	R[i] = sqrt( (*m_pTargetModelH)[i]/(*m_pTargetCandidate)[i] ) ;
			//else if(xDim == MODEL_W && yDim == MODEL_H-10)
			//	R[i] = sqrt( (*m_pTargetModelV)[i]/(*m_pTargetCandidate)[i] ) ;
			//else
			//	R[i] = sqrt( (*m_pTargetModelS)[i]/(*m_pTargetCandidate)[i] ) ;
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

void KSHandTrackDataUsingMeanShift::calcDisplacement(int half_x,int half_y, int *dx, int *dy )
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

float KSHandTrackDataUsingMeanShift::getBhaDistance()
{
	float sumN = 0.0f;
	float sumS = 0.0f;
	float sumV = 0.0f;
	float sumH = 0.0f;
	for( int i=0; i< NUMBINS; ++i)
	{
		sumN += sqrt((*m_pTargetModelN)[i]*(*m_pTargetCandidate)[i]);
		//sumS += sqrt((*m_pTargetModelS)[i]*(*m_pTargetCandidate)[i]);
		//sumV += sqrt((*m_pTargetModelV)[i]*(*m_pTargetCandidate)[i]);
		//sumH += sqrt((*m_pTargetModelH)[i]*(*m_pTargetCandidate)[i]);
	}
	//float maxSum = Max(Max(Max(sumN, sumS),sumV),sumH);
	return sqrt(1-sumN);
}

