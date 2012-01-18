#include "StdAfx.h"
#include "Background.h"

#define MAX_DEPTH 4096
void Background::init( BaseBuf* rgb, BaseBuf* depth)
{
	if(rgb->width()==0 ||rgb->height()==0 ||depth->width()==0 || depth->height()==0)return;
	if(rgb->width()!= m_imgRgbBG->width() ||
		rgb->height()!= m_imgRgbBG->height() ||
		depth->width()!= m_imgDepthBG->width() ||
		depth->height()!= m_imgDepthBG->height() )return;

	setRgbBG(rgb);
	setDepthBG(depth);

	m_bIsBG = true;
}

void Background::removeBG(BaseBuf* rgb, BaseBuf* depth, BaseBuf* rgbBG, BaseBuf* depthBG, BaseBuf* dst, Rect rect)	 // dst for image to be outputed to the next module
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

	for(int j=0;j< rect.height;j++)
	{
		pRgbSrc = rgb->getData() + (j+offsety)*rgb->widthBytes() + offsetx*3; 
		pDst = dst->getData() + (j+offsety)*dst->widthBytes() + offsetx*3;
		for(int i=0;i< rect.width;i++,pRgbSrc+=3, pDst+=3)
		{
			nDepth = depth->getPixel16At(j+offsety, i+offsetx);
			nDepth = ( nDepth>MAX_DEPTH) ? 0 : 255-nDepth*255/MAX_DEPTH;
			nDepthBG = depthBG->getPixel16At(j+offsety, i+offsetx);
			nDepthBG = ( nDepthBG >MAX_DEPTH) ? 0 : 255-nDepthBG*255/MAX_DEPTH;
			if( //abs( *(pRgbSrc) - *(pRgbBG) ) < _RGB_THRESH &  
				//abs( *(pRgbSrc+1) - *(pRgbBG+1) ) < _RGB_THRESH & 
				//abs( *(pRgbSrc+2) - *(pRgbBG+2) ) < _RGB_THRESH &
				abs( nDepth - nDepthBG ) > _DEP_THRESH )

			{
				*(pDst) = 255;
				*(pDst+1) = 255;
				*(pDst+2) = 255;
			}
			if(*(pDst)!=255||(*pDst==255&&(nDepth==255||nDepth<100)))
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
