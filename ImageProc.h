#pragma once

#include "Buffer.h"
#include "StructDefine.h"

enum CVTMODE{
	RGB2GRAY,
	BGR2GRAY,
	RGB2HSV,
	BGR2HSV,
	RGB2BGR,
};

class ImageProc
{
public:
	void cvtRGB2GRAY(BaseBuf*, BaseBuf*, int);
	void cvtRGB2GRAY(BaseBuf*, BaseBuf*, Rect, int);
	void cvtRGB2HSV(BaseBuf*, BaseBuf*, int);
	void cvtRGB2HSV(BaseBuf*, BaseBuf*, Rect, int);
	 
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