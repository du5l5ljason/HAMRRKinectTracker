#include "StdAfx.h"
#include "math.h"
#include "MarkerModel.h"

bool MarkerModel::create()
{
	if(m_pModPink->height()!= m_pModBlack->height() 
		|| m_pModPink->width()!= m_pModBlack->width())
		return false;

	int height = m_pModPink->height();
	int width = m_pModBlack->width();
	int cx = width/2;
	int cy = height/2;
	int dist = cx*cx+cy*cy;
	BYTE* pPink = NULL;
	BYTE* pBlack = NULL;


	for(int j=0;j<height;j++)
	{
		pPink =  m_pModPink->getData() + j*width;
		pBlack = m_pModBlack->getData() + j*width;
		for(int i=0;i<width;i++, pPink+=1, pBlack+=1)
		{
			//alpha, beta
			*pPink = (int)( 255 * exp((double)( -MOD_ALPHA *( (cy-j)*(cy-j) + (cx-i)*(cx-i) )) ));
			*pBlack = (int)( 255 * exp((double)( -MOD_BETA *( dist - (cy-j)*(cy-j) - (cx-i)*(cx-i))) ));
		}
	}

	m_bIsCreated = true;
	return true;
}

double MarkerModel::matching(BaseBuf* src)
{
	if(src->height()!= m_pModPink->height()
		||src->width()!=m_pModPink->width() 
		||src->widthBytes()!=m_pModPink->widthBytes()*3)
		return -1;
	
	int nHeight = src->height();
	int nWidth = src->width();
	int nWidthBytes = src->widthBytes(); 
	double dsumPink = 0.0;
	double dsumBlack = 0.0;

	BYTE* pPink = NULL;
	BYTE* pBlack = NULL;
	BYTE* pSrc = NULL;

	for(int j=0;j<nHeight;j++)
	{
		pPink = m_pModPink->getData() + j*nWidthBytes/3;
		pBlack = m_pModBlack->getData() + j*nWidthBytes/3;
		pSrc = src->getData() + j*nWidthBytes;
		for(int i=0;i<nWidth;i++,pPink+=1,pBlack+=1,pSrc+=3)
		{
			float x = abs((float)( *(pSrc+2)/255.0f)- MEAN_GREEN ) /sqrt(VAR_GREEN); 
			if(abs(*(pSrc)/255.0f-MEAN_BLACK)< 0.1 || abs(*(pSrc)/255.0f-1)< 0.1)
				x = 20.0f;

			dsumPink = dsumPink + (float)((*pPink)/255.0f) * x;
			x = max( (float)( *(pSrc)/255.0f) - MEAN_BLACK, 0 )/ sqrt(VAR_BLACK);
			dsumBlack = dsumBlack + (float)((*pBlack)/255.0f) * x;
		}
	}

	return (MOD_WP*dsumPink+MOD_WB*dsumBlack);		//weight1 and weight2 can be regarded as two class member. Also alpha beta
}

double MarkerModel::matching(BaseBuf* imgPinkMap, BaseBuf* imgBlackMap)
{
	if(imgPinkMap->height()!=imgBlackMap->height()
		|| imgPinkMap->width()!=imgBlackMap->width()
		|| imgPinkMap->widthBytes()!=imgBlackMap->widthBytes()) 
		return 1000000.0f;
	if(imgPinkMap->height()!=m_pModPink->height()
		|| imgPinkMap->width()!=m_pModPink->width())
		return 1000000.0f;

	int nHeight = imgPinkMap->height();
	int nWidth = imgPinkMap->width();
	int nWidthBytes = imgPinkMap->widthBytes(); 
	double dsumPink = 0.0;
	double dsumBlack = 0.0;

	BYTE* pPink = NULL;
	BYTE* pBlack = NULL;
	BYTE* pPinkMap = NULL;
	BYTE* pBlackMap = NULL;

	for(int j=0;j<nHeight;j++)
	{
		pPink = m_pModPink->getData() + j*nWidthBytes;
		pBlack = m_pModBlack->getData() + j*nWidthBytes;
		pPinkMap = imgPinkMap->getData() + j*nWidthBytes;
		pBlackMap = imgBlackMap->getData() + j*nWidthBytes;
		for(int i=0;i<nWidth;i++,pPink+=1,pBlack+=1,pPinkMap+=1, pBlackMap+=1)
		{
			//pink: H value, mean and var are const
			dsumPink = dsumPink + (float)(*pPink/255.0f) * (*pPinkMap);
			dsumBlack = dsumBlack + (float)(*pBlack/255.0f) * (*pBlackMap);
		}
	}

	return (MOD_WP*dsumPink+MOD_WB*dsumBlack);
}

void MarkerModel::calDistMap(BaseBuf* src, BaseBuf* pinkMap, BaseBuf* blackMap, Rect rect)
{
	if(src->height()<0||src->width()<0||src->widthBytes()!=3*src->width())return;
	if(pinkMap->height()!=blackMap->height()||pinkMap->width()!=pinkMap->width())return;

	int nHeight = rect.height;
	int nWidth = rect.width;
	int nWidthBytes = src->widthBytes();
	int nOffset = rect.y * nWidthBytes + rect.x * 3; 
	BYTE* pSrc = NULL;
	BYTE* pPinkMap = NULL;
	BYTE* pBlackMap = NULL;

	for(int j=0;j<nHeight;j++)
	{
		pSrc = src->getData() + nOffset + j* nWidthBytes;
		pPinkMap = pinkMap->getData() + j*nWidth;
		pBlackMap = blackMap->getData() + j*nWidth;
		for(int i=0;i<nWidth;i++, pSrc+=3, pPinkMap+=1, pBlackMap+=1)
		{
			//For test
			*pPinkMap =  10 * abs(( *(pSrc+2)/255.0f)- MEAN_GREEN ) / sqrt(VAR_GREEN);
			*pBlackMap = 10 * max(( *(pSrc)/255.0f) - MEAN_BLACK, 0 )/ sqrt(VAR_BLACK);
		}

	}


}