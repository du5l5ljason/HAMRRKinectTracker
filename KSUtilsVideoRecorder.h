#pragma once

#include <ctime>
#include <cv.h>
#include <highgui.h>
#include "UserConfigs.h"
#include "ImageProc.h"

class KSUtilsVideoRecorder
{
	CvVideoWriter* m_pVideoWriter;
	ImageProc m_imgProc;
	bool	m_bIsRun;
	bool    m_bIsInit;
public:
	KSUtilsVideoRecorder(void):m_bIsRun(false), m_bIsInit(false){};
	~KSUtilsVideoRecorder(void){
		if( m_bIsRun != false) m_bIsRun = false;
		if( m_bIsInit != false) m_bIsInit = false;
		cvReleaseVideoWriter( &m_pVideoWriter );};

public:
	bool isRun(){return m_bIsRun;};
	void setIsRun(bool flag){m_bIsRun = flag;};
	bool isInit(){return m_bIsInit;};
	void setInit(bool flag){m_bIsInit = flag;};
	bool init();
	void record( BaseBuf* img );
	void close();

private:
	void doDownSampling( IplImage* , IplImage*,  int );
	void writeFrameIntoFile( IplImage* );

};

