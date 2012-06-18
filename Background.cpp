#include "StdAfx.h"
#include "Background.h"
#include <iostream>

extern UserGenerator g_UserGenerator;
using namespace std;

void Background::init( BaseBuf* rgb, BaseBuf* depth)
{
	//try to load bg files, if failed, set current frame to the background
	const char* rgbBGFileName = _BGPATH"rgbBG.bmp";
	const char* depthBGFileName = _BGPATH"depthBG.bmp";

	if(!loadBGfromFile(rgbBGFileName, depthBGFileName))
	{
		//setBGFromCurrentFrame
		if(rgb->width()==0 ||rgb->height()==0 ||depth->width()==0 || depth->height()==0)return;
		if(rgb->width()!= m_imgRgbBG->width() ||
		rgb->height()!= m_imgRgbBG->height() ||
		depth->width()!= m_imgDepthBG->width() ||
		depth->height()!= m_imgDepthBG->height() )return;

		setRgbBG(rgb);
		setDepthBG(depth);
	}
	m_bIsBG = true;
}

void Background::removeBG(BaseBuf* rgb, BaseBuf* depth, BaseBuf* rgbBG, BaseBuf* depthBG, BaseBuf* dst, Rect rect, float bkDepthThresh)	 // dst for image to be outputed to the next module
{
	if(rgb->height()!=rgbBG->height() ||
		rgb->widthBytes()!=rgbBG->widthBytes()||
		depth->height()!=depthBG->height()||
		depth->widthBytes()!=depthBG->widthBytes())
		return;

	//Differentiate the current frame and background frame, add a threshold
	BYTE* pRgbSrc = NULL;
	BYTE* pDepthSrc = NULL;
	BYTE* pDepthBG = NULL;
	BYTE* pDst = NULL;

	int offsetx = rect.x;
	int offsety = rect.y;
	int nDepth = 0;
	int nDepthBG = 0;
	dst->zeroBuffer();

	float fbkDepthThreshGrayValue = ( bkDepthThresh>MAX_DEPTH) ? 0 : (255-(int)bkDepthThresh*255/MAX_DEPTH);
	for(int j=0;j< rect.height;j++)
	{
		pRgbSrc = rgb->getData() + (j+offsety)*rgb->widthBytes() + offsetx*3;
		pDst = dst->getData() + (j+offsety)*dst->widthBytes() + offsetx*3;
		for(int i=0;i< rect.width;i++,pRgbSrc+=3,  pDst+=3)
		{
			nDepth = depth->getPixel16At(j+offsety, i+offsetx);
			nDepth = ( nDepth>MAX_DEPTH) ? 0 : 255-nDepth*255/MAX_DEPTH;
			nDepthBG = depthBG->getPixel16At(j+offsety, i+offsetx);
			nDepthBG = ( nDepthBG >MAX_DEPTH) ? 0 : 255-nDepthBG*255/MAX_DEPTH;
			if( abs( nDepth - nDepthBG ) > _DEP_THRESH || nDepthBG > 170) //170 is a parameter. when the hand is rested at the desk, the depth difference is so small that we can't remove the desk as background.
			{
				*(pDst) = 255;
				*(pDst+1) = 255;
				*(pDst+2) = 255;
			}
			if(*(pDst)!=255||(*pDst==255&&(nDepth==255||nDepth<fbkDepthThreshGrayValue)))
			{
				*(pDst) = 0;
				*(pDst+1) = 0;
				*(pDst+2) = 0;
			}
			if((*pDst)==255)
			{
				*pDst = *pRgbSrc;
				*(pDst+1) = *(pRgbSrc+1);
				*(pDst+2) = *(pRgbSrc+2);
			}
			else
			{
				*pDst = 255;
				*(pDst+1) = 255;
				*(pDst+2) = 255;
			}
		}
		
	}
}

bool Background::loadBGfromFile( const char* rgbFileName, const char* depthFileName ){

	IplImage* rgb = cvLoadImage( _BGPATH"rgbBG.bmp", 1 );
	IplImage* depth = cvLoadImage( _BGPATH"depthBG.bmp", 1 );
	if( rgb == NULL || depth == NULL ) return false;
	m_pImgProc.cvtIplImagetoBuffer( rgb, m_imgRgbBG );
	m_pImgProc.cvtIplImagetoBuffer( depth, m_imgDepthBG );

	
	cvReleaseImage( &rgb );
	cvReleaseImage( &depth );
	return true;
}

void Background::removeBG( BaseBuf* rgb,  BaseBuf* dst, BaseBuf* depth, Rect rect, float bkDepthThresh)
{
	SceneMetaData sceneMD;
	g_UserGenerator.GetUserPixels( 0, sceneMD );

	//draw label map
	if(rgb->height()!=sceneMD.FullYRes() ||
		rgb->width()!=sceneMD.FullXRes() )
		return;	

	int offsetx = rect.x;
	int offsety = rect.y;
	BYTE* pRgb = NULL;
	BYTE* pDst = NULL;
	int nDepth = 0;
	const XnUInt16* pLabel = NULL;

	dst->zeroBuffer();
	float fbkDepthThreshGrayValue = ( bkDepthThresh>MAX_DEPTH) ? 0 : (255-(int)bkDepthThresh*255/MAX_DEPTH);
	for( int j = 0; j < rect.height ; j++ )
	{
		pRgb = (BYTE*)( rgb->getData() + (j+rect.y)*rgb->widthBytes() + offsetx*3 );
		pDst = (BYTE*)( dst->getData() + (j+rect.y)*dst->widthBytes() + offsetx*3);
		pLabel = (XnUInt16*)( sceneMD.Data() + (j+rect.y)*sceneMD.FullXRes() + offsetx );
		for( int i=0; i< rect.width; i++, pRgb+=3, pDst +=3, pLabel +=1 )
		{
			nDepth = depth->getPixel16At(j+offsety, i+offsetx);
			nDepth = ( nDepth>MAX_DEPTH) ? 0 : 255-nDepth*255/MAX_DEPTH;
			if( *pLabel != 0 && nDepth>fbkDepthThreshGrayValue)
			{
				*pDst = *pRgb;
				*(pDst + 1) = *(pRgb + 1);
				*(pDst + 2) = *(pRgb + 2);
			}
			else
			{
				*pDst = 255;
				*(pDst + 1) = 255;
				*(pDst + 2) = 255;
			}

		}
	}
}