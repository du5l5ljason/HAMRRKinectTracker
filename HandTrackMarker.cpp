#include "StdAfx.h"
#include "HandTrackMarker.h"


int gnPointMode = 1;
void HandTrackMarker::relocateHandPos(BaseBuf* rgb, BaseBuf* depth, Rect &ROI)
{
	//1. Convert to HSV 
	m_pImgProc->cvtRGB2HSV(rgb,rgb, ROI);

	//segColorBased(rgb, rgb, ROI);
	//2. Create model
	if(!m_pModel->isCreate())
		m_pModel->create();

	//3. call Template matching function
	findMatchWindow( rgb, ROI );
}

void HandTrackMarker::findMatchWindow(BaseBuf* imgRgb, Rect& ROI)
{

	if(ROI.x + ROI.width > imgRgb->width())return;
	if(ROI.y + ROI.height > imgRgb->height())return;

	int offsetx = ROI.x;
	int offsety = ROI.y;
	int nModWidth = m_pModel->getModPink()->width();		//template size
	int nModHeight = m_pModel->getModPink()->height();

	int row = ROI.height - nModHeight;					//sliding window range
	int col = ROI.width - nModWidth;						//sliding window range
	if(col<0 || row<0 || col > imgRgb->width() || row > imgRgb->height())	//Do nothing, just use the positions by skeleton tracking.
		return;

	//BaseBuf* imgPinkMap = new Buffer8(ROI.width, ROI.height, ROI.width);
	//BaseBuf* imgBlackMap = new Buffer8(ROI.width, ROI.height, ROI.width);
	//BaseBuf* imgSubPink = new Buffer8(nModWidth, nModHeight, nModWidth);
	//BaseBuf* imgSubBlack = new Buffer8(nModWidth, nModHeight, nModWidth);
	BaseBuf* imgSub = new Buffer24(nModWidth, nModHeight, nModWidth*3);

	Rect nslideRect;
	double min = 1000000.0f;
	double score = 0.0f ;
	double thresh = 1800.0f;
	nslideRect.x = 0;									//Initialize slide window
	nslideRect.y = 0;
	nslideRect.width = nModWidth;
	nslideRect.height = nModHeight;

	//m_pModel->calDistMap( imgRgb, imgPinkMap, imgBlackMap, ROI);

	for( int j=0; j< row; j+=4)
	{
		for( int i=0; i<col; i+=4)
		{
			nslideRect.x = i + offsetx;
			nslideRect.y = j + offsety;
			//m_pImgProc->getROI(imgPinkMap, imgSubPink, nslideRect);
			//m_pImgProc->getROI(imgBlackMap, imgSubBlack, nslideRect);
			m_pImgProc->getROI(imgRgb, imgSub, nslideRect);
			score = m_pModel->matching(imgSub);
			
			//score = m_pModel->matching(imgSubPink, imgSubBlack);
			if(min > score)
			{
				min = score;
				ROI.x = nslideRect.x;
				ROI.y = nslideRect.y;
			}
		}
	}

	if(gnPointMode == 1 && min < thresh)
		gnPointMode = 2;
	if(gnPointMode == 2 && min > 3000.0f)
		gnPointMode = 1;

	ROI.width = nModWidth;
	ROI.height = nModHeight;

	if(imgSub!=NULL)
		delete imgSub;
	//if(imgSubPink!=NULL)
	//delete imgSubPink;
	//if(imgSubBlack!=NULL)
	//delete imgSubBlack;
	//if(imgPinkMap!=NULL)
	//delete imgPinkMap;
	//if(imgBlackMap!=NULL)
	//delete imgBlackMap;
}

	//Edited by Tingfang, 11/12, segmentation method
	//if(m_colorMode == _CM_NONE)return;
	//int nConnectRegion = 0;
	//BaseBuf* bin = new Buffer8(rgb->width(), rgb->height(), rgb->widthBytes()/3);
	//BaseBuf* labelBin = new Buffer8(bin->width(), bin->height(), bin->widthBytes());
	//sliding window, setROI，在ImgProc写一个这个函数，每次得到一个ROI的图
	//Edited by Tingfang, 11/12, segmentation method
	//segColorBased(rgb,rgb, ROI);
	//m_pImgProc->cvtRGB2GRAY(rgb, bin, ROI);

	//nConnectRegion = m_pImgProc->labelConnectedRegion(bin, labelBin, ROI);	
	//get bounding box of the largest connected region as the new ROI value.
	//findMaxBoundingRect(labelBin, nConnectRegion, ROI);
	
	//delete labelBin;
	//delete bin;
//void HandTrackMarker::segColorBased(BaseBuf* src, BaseBuf* dst, Rect rect)
//{
//	if(dst->height() != src->height() || dst->width() != src->width())return;
//	if(dst->widthBytes() != src->widthBytes() ) return;
//
//	BYTE* pSrc = NULL;
//	BYTE* pDst = NULL;
//
//	int va1,va2,va3;
//	for(int j=0;j<rect.height;j++)
//	{	
//		pSrc = src->getData() + (j+rect.y)*src->widthBytes() + rect.x*3;
//		pDst = dst->getData() + (j+rect.y)*dst->widthBytes() + rect.x*3;
//		for(int i=0;i<rect.width;i++, pSrc+=3, pDst+=3)
//		{
//			va3 = *(pSrc);
//			va2 = *(pSrc+1);
//			va1 = *(pSrc+2);
//
//			if( abs(va1-m_value1)<m_rangeV1 &&
//				abs(va2-m_value2)<m_rangeV2 &&
//				abs(va3-m_value3)<m_rangeV3 )
//			{	
// 			   *(pDst) = 255;
//				*(pDst+1) = 255;
//				*(pDst+2) = 255;
//			}
//			else
//			{
//				*(pDst) = 0;
//				*(pDst+1) = 0;
//				*(pDst+2) = 0;
//			}
//
//		}
//
//	}
//
//	m_pImgProc->medianfilter(dst, rect, 3);
//}
//
//void HandTrackMarker::findMaxBoundingRect(BaseBuf* imgLabel, int nConReg, Rect& rect)
//{
//
//	if(!nConReg)return;
//
//	int* nCounter = new int[nConReg+1];	//nCounter[0] = 0; no meaning
//	for( int i = 0; i< nConReg; i++)
//		nCounter[i] = 0;
//	int nThreshSize = 50;
//
//	BYTE* pLabel = imgLabel->getData();
//	for( int j = 0; j< rect.height; j++)
//	{
//		for( int i = 0; i< rect.width; i++)
//		{
//			pLabel = imgLabel->getData() + (j+rect.y)* imgLabel->widthBytes() + (i + rect.x);
//			if(*(pLabel)>0)
//				nCounter[*(pLabel)]++;
//		}
//	}
//
//	int max = 0;
//	int nMaxId = 1;
//
//	if(nConReg>1)
//	{
//		for(int i = 1; i< nConReg; i++)
//		{	
//			if(nCounter[i]> max)
//			{	
//				max = nCounter[i];
//				nMaxId = i;
//			}
//		}
//	}
//	
//	if(max<nThreshSize)
//		return;
//
//	int xTopleft = 640;
//	int yTopleft = 480;
//	int xBotRight = 0;
//	int yBotRight = 0;
//	for( int j = 0; j< rect.height; j++)
//	{
//		for( int i = 0; i< rect.width; i++)
//		{
//			pLabel = imgLabel->getData() + (j+rect.y)* imgLabel->widthBytes() + (i + rect.x);
//			if(*(pLabel)==nMaxId)
//			{
//				if((i+rect.x)<xTopleft)
//					xTopleft = i+rect.x;
//				if((i+rect.x)>xBotRight)
//					xBotRight = i+rect.x;
//				if((j+rect.y)<yTopleft)
//					yTopleft = j + rect.y;
//				if((j+rect.y)>yBotRight)
//					yBotRight = j + rect.y;
//			}
//		}
//	}
//
//	rect.x = xTopleft;
//	rect.y = yTopleft;
//	rect.width = (xBotRight - xTopleft)>0?(xBotRight - xTopleft):0;
//	rect.height = (yBotRight - yTopleft)>0?(yBotRight - yTopleft):0;
//
//	delete[] nCounter;
//}