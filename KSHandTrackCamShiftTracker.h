// KSHandTrackCamShiftTracker.h 
// Based on the camshift_wrapper from Robin Hewitt, 20077
// http://www.cognotics.com/opencv/downloads/camshift_wrapper


#pragma once
#include "KSHandTrackData.h"
#include <cv.h>
#include <highgui.h>
#include "ImageProc.h"
#include <iostream>

using namespace std;
#define nHISTBINS 32
#define VMIN 0
#define VMAX 100
#define SMIN 20
#define HISTRANGEMIN 0
#define HISTRANGEMAX 180



class KSHandTrackCamShiftTracker : public KSHandTrackData
{
public:
	KSHandTrackCamShiftTracker(void);
	KSHandTrackCamShiftTracker( int w, int h ): KSHandTrackData(w,h){};
	~KSHandTrackCamShiftTracker(void){};
public:
	void init( BaseBuf*, Rect& );
	bool track(BaseBuf*, BaseBuf*, Rect&);

public:
	IplImage* getHSVImg() const{ return m_pHSVImg;};
	IplImage* getHueImg() const{ return m_pHueImg;};
	IplImage* getMaskImg() const{ return m_pMask;};
	IplImage* getProbImg() const{ return m_pProbImg;};
	CvHistogram *getHist() const{return m_pHist;};
	CvRect getPrevRect() const{return m_prevRect;};
	CvBox2D getCurWin() const{ return m_window;};

private:
	IplImage *m_pHSVImg;	// the input image converted to HSV color mode
	IplImage *m_pHueImg;	// the Hue channel of the HSV image
	IplImage *m_pMask;		// this image is used for masking pixels
	IplImage *m_pProbImg;   // the probability estimates for each pixel
	CvHistogram *m_pHist;   // histogram of hue in the origin model image
	ImageProc m_imgProc;
	float *m_pRanges;		// value range
	CvRect m_prevRect;		// location of the object in previous frame
	CvBox2D m_window;		// current object location estimate

private:
	void updateHueImage( const IplImage *pImg );
	void updateHist();
};

