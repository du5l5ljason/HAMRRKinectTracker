#pragma once

#include "Buffer.h"
#include "UserTypes.h"
#include "UserConfigs.h"
#include <cv.h>
#include <highgui.h>


class ImageProc
{
public:
	void cvtIplImagetoBuffer( IplImage*, BaseBuf* );
	void cvtBuffertoIplImage( BaseBuf*, IplImage* );
	void loadImageFromFile( const char*, BaseBuf* );
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