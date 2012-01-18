#include "StdAfx.h"
#include "ImageProc.h" 

void pushQueue(Queue* queue, int data)
{
	QNode* p = NULL;
	p = (QNode*)malloc(sizeof(QNode));
	p->data = data;

	if(queue->last == NULL)
	{
		queue->first = p;
		queue->last = p;
		p->next = NULL;
	}
	else
	{
		queue->last->next = p;
		p->next = NULL;
		queue->last = p;
	}
}
int popQueue(Queue* queue)
{
	QNode* p = NULL;
	int data;

	if(queue->first == NULL)
		return -1;
	p = queue->first;
	data = p->data;

	if(queue->first->next == NULL)
	{
		queue->first = NULL;
		queue->last = NULL;
	}
	else
	{
		queue->first = p->next;
	}

	free(p);
	return data;

}
void ImageProc::cvtRGB2GRAY(BaseBuf* rgb, BaseBuf* gray, Rect rect)
{
	if(rgb->height() != gray->height() || rgb->width() != gray->width())return;
	if(rgb->widthBytes() != 3* gray->widthBytes() ) return;

	int nOffset, nHeight, nWidth, nWidthBytes;
	BYTE* pSrc = NULL;
	BYTE* pDst = NULL;
	nHeight = rect.height;
	nWidth = rect.width;
	nWidthBytes = rgb->widthBytes();
	nOffset = rect.y * nWidthBytes + rect.x*3;

	for(int j=0;j<nHeight;j++)
	{	
		pSrc = rgb->getData() + nOffset + j*nWidthBytes;
		pDst = gray->getData() + (nOffset + j*nWidthBytes)/3;
		for(int i=0;i<nWidth;i++, pSrc+=3, pDst+=1)
			*pDst = *(pSrc);	
	}
	
}
void ImageProc::cvtRGB2HSV(BaseBuf* rgb, BaseBuf* hsv, Rect rect)
{
	if(hsv->height() != rgb->height() || hsv->width() != rgb->width())return;
	if(hsv->widthBytes() != rgb->widthBytes() ) return;

	int r, g, b;
	float h, s, v;
	int nOffset, nWidth, nHeight, nWidthBytes;
	BYTE* pSrc = NULL;
	BYTE* pDst = NULL;
	nWidth = rect.width;
	nHeight = rect.height;
	nWidthBytes= rgb->widthBytes();
	nOffset = rect.y* rgb->widthBytes() + rect.x*3;

	for(int j=0;j<nHeight;j++)
	{	
		pSrc = rgb->getData() + nOffset + j*nWidthBytes;
		pDst = hsv->getData() + nOffset + j*nWidthBytes;
		for(int i=0;i<nWidth;i++, pSrc+=3, pDst+=3)
		{
			b = *(pSrc);			//
			g = *(pSrc+1);
			r = *(pSrc+2);
			
			rgb2hsv(b,g,r,v,s,h);

			*(pDst) = v;
			*(pDst+1) = s;
			*(pDst+2) = h*(255.0f/360);

		}
		
	}
}
void ImageProc::rgb2hsv(int blue, int green, int red, float& val, float& sat, float& hue )
{
	float max, min, delta, temp;
	temp = max(blue, green);
	max = max(temp, red);
	temp = min(blue, green);
	min = min(temp, red);
	val = max;
	delta = max - min;

	if(max!=0)
		sat = delta/max * 255;
	else
		hue = sat = val =0;
	
	if(delta==0)
	{
		hue = val;
		sat = val;
		return;
	}

	if(red == max)
	{
		hue = (green - blue) / delta;
	}
	else if(green == max)
	{
		hue = 2 + (blue - red) / delta;
	}
	else
		hue = 4 + (red - green) / delta;
	
	hue *= 60;
	
	if(hue<0)
		hue += 360;
	return;
}
void ImageProc::getROI(BaseBuf* src, BaseBuf* dst, Rect rect)
{
	if(src->height() <rect.height ||src->width()<rect.width)return;
	if(dst->height()!=rect.height||dst->width()!=rect.width)return;

	BYTE* pSrc = src->getData();
	BYTE* pDst = dst->getData();
	int nHeight,nWidth,nOffset,nSrcWidthBytes,nDstWidthBytes;
	nHeight = rect.height;
	nWidth = rect.width;
	nSrcWidthBytes = src->widthBytes();
	nDstWidthBytes = dst->widthBytes();
	nOffset = rect.y * nSrcWidthBytes + rect.x * 3;

	for( int j=0; j<nHeight; j++)
	{
		pSrc = src->getData() + nOffset + j*nSrcWidthBytes;
		pDst = dst->getData() + j*nDstWidthBytes;
		memcpy(pDst, pSrc, sizeof(BYTE)*nDstWidthBytes);
		//for(int i=0;i<nWidth;i++, pDst+=3,pSrc+=3)
		//{
		//	*pDst = *pSrc;
		//	*(pDst+1) = *(pSrc+1);
		//	*(pDst+2) = *(pSrc+2);
		//}
	}

}
void ImageProc::medianfilter(BaseBuf* src, Rect rect, int size)
{
	if(size%2==0)return;

	int L = (size-1)/2;
	int* filter = new int[size*size];
	int i,j,m,n,k,l;
	int id =0;
	int offsetx = rect.x;
	int offsety = rect.y;
	int temp=0;
	BYTE* pTemp = NULL;
	int count = 0;

	for(j=5; j<rect.height-5; j++)
	{
		pTemp = src->getData()+ (j+offsety)* src->widthBytes() + (5+offsetx)*3;
		for(i=5; i<rect.width-5; i++,pTemp+=3)
		{
			
			for(m = -L; m<=L; m++)
			{
				for(n = -L; n<=L; n++, id++)
					filter[id] = src->getPixel24At(j+m+offsety, i+n+offsetx);
			}

			for(k=0;k<(id/2+1);k++)
			{
				for(l=id-1-k;l>=0;l--)
				{
					if(filter[l]>filter[id-1-k])
					{
						temp = filter[id-1-k];
						filter[id-1-k] = filter[l];
						filter[l] = temp;
					}
				}
			}

			
			*pTemp = filter[id/2]; 
			*(pTemp+1) = filter[id/2];
			*(pTemp+2) = filter[id/2];
			
			id=0;
		}
	}

	delete[] filter;
}
int ImageProc::labelConnectedRegion(BaseBuf* bin, BaseBuf* labelbin, Rect rect)
{
	if((bin->width()!=labelbin->width())||(bin->height()!=labelbin->height()))
		return -1;

	int index = 0;
	int popIndex = 0;
	int labelIndex = 0;
	int pixelIndex = 0;
	int width, height;
	Queue* queue = NULL;
	queue = (Queue*)malloc(sizeof(Queue));
	queue->first = NULL;
	queue->last = NULL;
	
	width = bin->width();
	height = bin->height();

	BYTE* pBin = NULL;
	BYTE* pLabelBin = NULL;
	//set label map to zero
	memset(labelbin->getData(), 0, sizeof(BYTE)*width*height);

	for( int j=5; j< rect.height - 5; j++)
	{
		
		pBin = bin->getData() + (j+rect.y)*bin->widthBytes() + rect.x;
		pLabelBin = labelbin->getData() + (j+rect.y)*labelbin->widthBytes() + rect.x;
		for( int i=5; i< rect.width - 5; i++, pBin++, pLabelBin++)
		{
			if( (*pBin == 255)&&(*pLabelBin == 0) )
			{
				//temp = m_counter; test OK
				labelIndex++;

				pixelIndex = (j+rect.y)*bin->widthBytes()+(i+rect.x);
				searchNeighbor(bin, labelbin, labelIndex, pixelIndex, queue);
				popIndex = popQueue(queue);
				while(popIndex > -1)
				{
					searchNeighbor(bin, labelbin, labelIndex, popIndex, queue);
					popIndex = popQueue(queue);
				}
			}
		}
	}

	free(queue);


	return labelIndex;
	
}
void ImageProc::searchNeighbor(BaseBuf* bin, BaseBuf* labelbin, int labelIndex, int pixelIndex, Queue* queue )
{
	int searchIndex;
	BYTE* pLabelBin = NULL;
	BYTE* pBin = NULL;
	int nNeighborDirection[8][2] = {
		{0,1}, {1,1}, {1,0}, {1,-1},
		{0,-1}, {-1,-1}, {-1,0},{-1,1}};

	pLabelBin = labelbin->getData() + pixelIndex;
	pBin = bin->getData();
	*pLabelBin = labelIndex;

	pLabelBin = labelbin->getData();

	for( int i=0;i<8;i++)
	{
		searchIndex = pixelIndex + nNeighborDirection[i][0]*bin->widthBytes() + nNeighborDirection[i][1];
		pBin = bin->getData() + searchIndex;
		pLabelBin = labelbin->getData() + searchIndex;
		
		if(searchIndex > 0 && searchIndex < bin->widthBytes()*bin->height() &&
			*(pBin) == 255 && *(pLabelBin) == 0)
		{
			*pLabelBin = labelIndex;
			
			pushQueue(queue, searchIndex);
		}
	}

}


/*
{
	int height = buf->height();
	int widthSteps = buf->widthBytes();
	if(img->height!=height || img->widthStep!=widthSteps) return;

	memcpy(img->imageData, buf->getData(), sizeof(BYTE)*height*widthSteps);
}




{
	CvRect rect;
	if(x<=0||y<=0||w<=0||h<=0)return;
	
	rect.x = x;
	rect.y = y;
	rect.width = w;
	rect.height = h;

	if(x<0)
		rect.x = 0;
	if(x+w>640)
		rect.width = 640 - rect.x;
	if(y<0)
		rect.y = 0;
	if(y+h>0)
		rect.height = 480 - rect.y;
	cvSetImageROI(img, rect);
}

*/

