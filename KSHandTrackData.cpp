#include "StdAfx.h"
#include "KSHandTrackData.h"
#include <iostream>

//void KSHandTrackData::update(BaseBuf* rgbImg, BaseBuf* depthImg, BaseBuf* rgbBG, BaseBuf* depthBG, int cx, int cy, int width, int height )
//{	
//	Rect ROI;
//	//1. Set Scan Window
//	setScanWindow(cx, cy, width, height, ROI);	
//	//2. Set/Remove Background
//	m_pBG->removeBG(rgbImg, depthImg, rgbBG, depthBG,  m_img,  ROI, m_fRestPosDepth );
//	m_pImgProc->cvtRGB2HSV(m_img, m_img, ROI, RGB2HSV );	//潜在问题，如何判定meanshift算出来的位置是否在ROI中，if the convergent point is not in the ROI, return false
//	//3. Hand region segmentation and locate the hand position
//	updateHandPos(m_img, NULL, ROI);		//pass the new located rect value to ROI
//	//4. Update Window
//	updateWindow(ROI);
//}

void KSHandTrackData::update(BaseBuf* rgbImg, BaseBuf* depthImg, int cx, int cy, int width, int height )
{	
	Rect ROI;
	//1. Set Scan Window
	setScanWindow(cx, cy, width, height, ROI);	
	//2. Set/Remove Background
	m_pBG->removeBG(rgbImg, m_img, depthImg, ROI, m_fRestPosDepth );
	m_pImgProc->cvtRGB2HSV(m_img, m_img, ROI, RGB2HSV );	//潜在问题，如何判定meanshift算出来的位置是否在ROI中，if the convergent point is not in the ROI, return false
	//3. Hand region segmentation and locate the hand position
	updateHandPos(m_img, NULL, ROI);		//pass the new located rect value to ROI
	//4. Update Window
	updateWindow(ROI);
}

void KSHandTrackData::setScanWindow(int x, int y, int width, int height, Rect &rect)
{

	rect.x = x - 1*width;
	rect.y = y - 1*height;
	rect.width = 2*width;
	rect.height = 2*height;
	int nXRes = m_img->width();
	int nYRes = m_img->height();

	if(rect.width<=0||rect.height<=0)
	{
		rect.x = 0;
		rect.y = 0;
		rect.width = nXRes;
		rect.height = nYRes;
	}
	else if(rect.x>nXRes || rect.y>nYRes)
	{
		rect.x = nXRes;
		rect.y = nYRes;
		rect.width = 0;
		rect.height = 0;
	}

	if(rect.x<0)
		rect.x = 0;
	if((rect.x+rect.width)>nXRes)
		rect.width = nXRes - rect.x;
	if(rect.y<0)
		rect.y = 0;
	if((rect.y+rect.height)>nYRes)
		rect.height = nYRes - rect.y;

}

void KSHandTrackData::updateWindow(Rect rect)
{
	m_pHandPos.x = rect.x+rect.width/2;
	m_pHandPos.y = rect.y+rect.height/2;
	int nXRes = m_img->width();
	int nYRes = m_img->height();
	m_rectWidth = rect.width;
	m_rectHeight = rect.height;

	if((m_pHandPos.x+m_rectWidth/2)>nXRes)
		m_rectWidth = 2*(nXRes - m_pHandPos.x);
	if((m_pHandPos.y+m_rectHeight/2)>nYRes)
		m_rectHeight = 2*(nYRes - m_pHandPos.y);
}