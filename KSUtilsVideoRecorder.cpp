#include "StdAfx.h"
#include "KSUtilsVideoRecorder.h"

bool KSUtilsVideoRecorder::init()
{
	
	char recordFileName[128];
	time_t t = time(0);
	strftime( recordFileName, sizeof(recordFileName), _VIDEO_RECORD_FILE_PATH"VIDEO_%Y%m%d_%H%M%S.mpeg", localtime(&t) );

	double fps = 30.0f;
	m_pVideoWriter = cvCreateVideoWriter(
		recordFileName,
		CV_FOURCC( 'M', 'J', 'P', 'G' ),
		fps,
		cvSize( _RECORDVIDEO_WIDTH, _RECORDVIDEO_HEIGHT ),
		1
		);

	return true;
}

void KSUtilsVideoRecorder::record( BaseBuf* img )
{
	if( !isInit() )
		setInit( init() );

	IplImage* imgDst = cvCreateImage(cvSize(img->width(), img->height()), 8, 3 ); // By default we record RGB image sequences.
	m_imgProc.cvtBuffertoIplImage( img, imgDst );
	
	//doDownSampling();
	writeFrameIntoFile( imgDst );
	cvReleaseImage(&imgDst);
}

void KSUtilsVideoRecorder::close()
{
	m_bIsInit = false;
}

void KSUtilsVideoRecorder::doDownSampling( IplImage* imgSrc, IplImage* imgDst, int filter = IPL_GAUSSIAN_5x5 )
{
	assert( imgSrc->width%2 == 0 && imgSrc->height%2 == 0);
	cvPyrDown( imgSrc,imgDst );
}

void KSUtilsVideoRecorder::writeFrameIntoFile( IplImage* img )
{
	cvWriteFrame( m_pVideoWriter, img );
}