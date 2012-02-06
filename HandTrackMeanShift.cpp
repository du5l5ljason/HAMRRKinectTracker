#include "StdAfx.h"
#include "HandTrackMeanShift.h"
#include <iostream>

using namespace std;
int gnPointMode = 1;

bool HandTrackMeanShift::init(BaseBuf* rgb, Rect &ROI)
{
	int centerX = ROI.x+ROI.width/2;
	int centerY = ROI.y+ROI.height/2;
	const int half_size_x = MODEL_W/2;
	const int half_size_y = MODEL_H/2;
	const int xDim = MODEL_W;
	const int yDim = MODEL_H;

	m_pKernel = new Kernel(MODEL_W, MODEL_H);
	cout << "Initializing the weight array" << endl;
	m_Weight = new double*[xDim];
	for( int i=0;i<xDim;++i)
		m_Weight[i] = new double[yDim];

	cout << "Initializing the kernel" << endl;
	m_pKernel->calcKernel(half_size_x,half_size_y);
	
	//Read Model Image from local files
	//const char* fileszName = FILE_PATH;
	//cout << "File name is" << fileszName << endl;
	//m_dib->OpenImage(fileszName);	// Create an model Image.
	
	//if(!=xDim || m_dib->Height()!=yDim) 
	//	return m_bInit;


	//imgModel->copyBuffer(width, height, width*3, m_dib->GetData());
	m_pImgProc->cvtRGB2HSV(rgb,rgb, BGR2HSV);
	
	if(m_pTargetModel!=NULL)
		m_pTargetModel->updateModel(rgb, centerX, centerY, half_size_x, half_size_y, m_pKernel);

	m_bInit = true;
	m_bShouldInit = false;
	return m_bInit;

}
void HandTrackMeanShift::updateHandPos(BaseBuf* rgb, BaseBuf* depth, Rect &ROI)
{
	//1. Convert to HSV 
	m_pImgProc->cvtRGB2HSV(rgb,rgb, ROI, BGR2HSV );	//潜在问题，如何判定meanshift算出来的位置是否在ROI中，if the convergent point is not in the ROI, return false
	//2. Create model
	if(shouldInit())
		init(rgb, ROI);
	if(isInit())
	{
		int centerX = ROI.x + ROI.width/2;
		int centerY = ROI.y + ROI.height/2;
		const int half_size_x = MODEL_W/2;
		const int half_size_y = MODEL_H/2;
		const int xDim = MODEL_W;
		const int yDim = MODEL_H;
		int dx = 0;
		int dy = 0;
		int pdx = 0;
		int pdy = 0;
		int LoopCount = 0;
		bool exit = false;
		while(exit == false)
		{
			pdx = dx;
			pdy = dy;
			LoopCount++;
			m_pTargetCandidate->updateModel(rgb, centerX, centerY, half_size_x, half_size_y, m_pKernel );
			updateWeight();
			calcDisplacement(half_size_x, half_size_y, &dx, &dy);
		
			centerX += dx;
			centerY += dy;

			if (((dx == 0) && (dy == 0)) || (LoopCount > 20) || ( (pdx + dx == 0) && (pdy + dy==0) ))
				exit = true;
		}

		cout << "iTeration times = " << LoopCount << endl;

			ROI.height = ROI_SIZE_H;
			ROI.width = ROI_SIZE_W;
			ROI.x = centerX - ROI.width/2;
			ROI.y = centerY - ROI.height/2;

	}
}

void HandTrackMeanShift::updateWeight()
{
	double R[NUMBINS];

	for( int i = 0; i< NUMBINS; ++i )
	{
		if((*m_pTargetCandidate)[i] == 0)
		{
			R[i] = 0;
		}
		else
		{
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



void HandTrackMeanShift::calcDisplacement(int half_x,int half_y, int *dx, int *dy )
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