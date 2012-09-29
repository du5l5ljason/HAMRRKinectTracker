#include "StdAfx.h"
#include "KSHandTrackData.h"
#include <iostream>

void KSHandTrackData::update(BaseBuf* rgbImg, BaseBuf* depthImg, int cx, int cy, int width, int height )
{	
	Rect ROI;
	//1. Set Scan Window
	setScanWindow(cx, cy, width, height, ROI);	
	//2. Set/Remove Background
	m_pBG->removeBG(rgbImg, m_img, depthImg, ROI, m_fRestPosDepth );//潜在问题，如何判定meanshift算出来的位置是否在ROI中，if the convergent point is not in the ROI, return false
	//3. Hand region segmentation and locate the hand position
	bool isTracked = track(m_img, NULL, ROI);		//pass the new located rect value to ROI
	//4. Update Window
	if( !isTracked )
	{	
		ROI.x = 0;
		ROI.y = 0;
		ROI.width = rgbImg->width();
		ROI.height = rgbImg->height();
	}

	updateWindow(ROI);
}

void KSHandTrackData::setScanWindow(int x, int y, int width, int height, Rect &rect)
{

	rect.x = x - width;
	rect.y = y - height;
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
	m_prevRect.width = rect.width;
	m_prevRect.height = rect.height;
	m_prevRect.x = rect.x;
	m_prevRect.y = rect.y;

	if((m_pHandPos.x+m_prevRect.width/2)>nXRes)
		m_prevRect.width  = 2*(nXRes - m_pHandPos.x);
	if((m_pHandPos.y+m_prevRect.height/2)>nYRes)
		m_prevRect.height = 2*(nYRes - m_pHandPos.y);
	if( m_pHandPos.x > nXRes || m_pHandPos.y > nYRes)
	{	
		m_pHandPos.x = 0;
		m_pHandPos.y = 0;
		m_prevRect.height = nYRes;
		m_prevRect.width = nXRes;
	}
	if( m_pHandPos.x < 0 )
		m_pHandPos.x = 0;
	if( m_pHandPos.y < 0 )
		m_pHandPos.y = 0;
}