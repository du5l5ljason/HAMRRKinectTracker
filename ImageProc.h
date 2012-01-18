#pragma once

#include "Buffer.h"
#include "StructDefine.h"

class ImageProc
{
public:
	void cvtRGB2GRAY(BaseBuf*, BaseBuf*, Rect);
	void cvtRGB2HSV(BaseBuf*, BaseBuf*, Rect);
	void rgb2hsv(int, int, int, float&, float&, float&);
	void getROI(BaseBuf*, BaseBuf*, Rect);
	void medianfilter(BaseBuf*, Rect, const int);
	int labelConnectedRegion( BaseBuf*, BaseBuf*, Rect ); 

private:
	void searchNeighbor( BaseBuf*, BaseBuf*, int, int, Queue*);

public:
	ImageProc(void){};
	~ImageProc(void){};
};