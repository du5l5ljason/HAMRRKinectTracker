#include "StdAfx.h"
#include "KSUtilsVideoRecorder.h"

bool KSUtilsVideoRecorder::init()
{
	
	char recordFileName[128];
	time_t t = time(0);
	strftime( recordFileName, sizeof(recordFileName), _VIDEO_RECORD_FILE_PATH"VIDEO_%Y%m%d_%H%M%S.mpeg", localtime(&t) );

	double fps = 20.0f;

	m_pVideoWriter = cvCreateVideoWriter(
		recordFileName,
		CV_FOURCC('P', 'I', 'M', '1'),
		fps,
		cvSize( _RECORDVIDEO_WIDTH/2, _RECORDVIDEO_HEIGHT/2 ),
		1
		);

	return true;
}

void KSUtilsVideoRecorder::record( BaseBuf* img )
{

	IplImage* imgDst = cvCreateImage(cvSize(img->width(), img->height()), 8, 3 ); // By default we record RGB image sequences.
	m_imgProc.cvtBuffertoIplImage( img, imgDst );
	IplImage* imgDSampledDst = cvCreateImage(cvSize(img->width()/2, img->height()/2), 8,3);
	doDownSampling(imgDst, imgDSampledDst,IPL_GAUSSIAN_5x5);
	writeFrameIntoFile( imgDSampledDst );
	cvReleaseImage(&imgDst);
	cvReleaseImage(&imgDSampledDst);
}

void KSUtilsVideoRecorder::close()
{
	cvReleaseVideoWriter( & m_pVideoWriter );
}

void KSUtilsVideoRecorder::doDownSampling( IplImage* imgSrc, IplImage* imgDst, int filter = IPL_GAUSSIAN_5x5 )
{
	assert( imgSrc->width%2 == 0 && imgSrc->height%2 == 0);
	cvPyrDown( imgSrc,imgDst, filter );
}

void KSUtilsVideoRecorder::writeFrameIntoFile( IplImage* img )
{
	cvWriteFrame( m_pVideoWriter, img );
}