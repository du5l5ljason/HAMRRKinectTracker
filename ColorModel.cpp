#include "StdAfx.h"
#include "math.h"
#include "ColorModel.h"
#include <iostream>

using namespace std;
ColorModel::ColorModel()
{
	//model must be initialized to 0
	memset( m_Model, '\0', sizeof(m_Model));
	binsInit = false;
}

ColorModel::~ColorModel()
{
	if( binsInit )
	{
		for( int i=0;i<m_nXDim;++i )
			delete[] bins[i];
		delete[] bins;
	}
}

double ColorModel::operator[](unsigned int bin)
{
	if( bin >= NUMBINS )
	{
		cerr << "ERROR!Tried to access a model bin that doesn't exit" << endl;
		return 0;
	}
	return m_Model[bin];
}

unsigned int ColorModel::findBin( unsigned char H, unsigned char V )
{
	//return the color index.
	//Range - 0-31
	unsigned int colorId; 
	unsigned int hId = (unsigned int)floor( (float)(H/BINSIZEH) );
	unsigned int vId = (unsigned int)floor( (float)(V/BINSIZEV) );

	return (hId*16+vId);
}

void ColorModel::updateModel(BaseBuf* imgHSV, int cx, int cy, int half_x, int half_y, Kernel* kernel)
{
	
	clearModel();
	int nOffsetX, nOffsetY;
	//bin table: xaxis - ColorId ; yaxis - No. of Points
	if(!binsInit)
	{
		m_nXDim = 2*half_x;
		m_nYDim = 2*half_y;
		bins = new unsigned int*[m_nXDim];
		for(int i=0;i<m_nXDim; i++)
			bins[i] = new unsigned int[m_nYDim];
		binsInit = true;
	}
	BYTE* pImg = new BYTE;
	unsigned char Hval;
	unsigned char Vval;

	if(m_nXDim!=2*half_x||m_nYDim!=2*half_y)
	{
		for(int i=0;i<m_nXDim; i++)
			delete [] bins[i];
		delete [] bins;
				m_nXDim = 2*half_x;
		m_nYDim = 2*half_y;
		bins = new unsigned int*[m_nXDim];
		for(int i=0;i<m_nXDim; i++)
			bins[i] = new unsigned int[m_nYDim];

	}
	//update a bin allocation given the current frame of HSV image
	for(int j = -half_y;j<half_y;j++)	
	{	
		nOffsetY = j + cy;
		for(int i = -half_x;i< half_x;i++)
		{
			nOffsetX = i + cx;
			//pImg = imgHSV->getData() + nOffsetY*imgHSV->widthBytes() + nOffsetX*3;
			imgHSV->getPixelAt(nOffsetY, nOffsetX, pImg);
			Hval = pImg[2];
			Vval = pImg[0];
			bins[i+half_x][j+half_y] = findBin(Hval, Vval);
			m_Model[bins[i+half_x][j+half_y]] += (kernel->getKernel())[i+half_x][j+half_y];
		}
	}
	//normalization
	double total = 0;
	for( int i=0;i<NUMBINS;i++ )
	{
		total += m_Model[i];
	}
	for( int i=0;i<NUMBINS;i++ )
	{
		m_Model[i] /= total;
	}
	//if(pImg!=NULL)
	//	delete pImg;
}

unsigned int ColorModel::theBin(unsigned int x, unsigned int y)
{
	if((x > m_nXDim) || (y > m_nYDim) || (!binsInit))
	{
		cerr << "ERROR! Attempted to access a pixel out of the window!" << endl;
		return (0);
	}
	else
	{
		return(bins[x][y]);
	}
	
}
void ColorModel::clearModel()
{
	memset( m_Model, '\0', sizeof(m_Model) );
}

