#include "StdAfx.h"
#include "KSHandTrackCamShiftTracker.h"

int vmin = VMIN, vmax = VMAX, smin = SMIN;
int nHistBins = 32;
void setVmin( int _vmin)
{
	vmin = VMIN;
}
void setVmax( int _vmax)
{
	vmax = VMAX;
}
void setSmin( int _smin)
{
	smin = SMIN;
}

void KSHandTrackCamShiftTracker::init( BaseBuf *img, Rect & rect )
{
	// the rect is the searching window
	// calculate the first histogram.
	m_pRanges = new float[2];
	m_pRanges[0] = HISTRANGEMIN;
	m_pRanges[1] = HISTRANGEMAX;

	BaseBuf* subImg = new Buffer24( rect.width, rect.height, rect.width*3 );
	m_imgProc.getROI( img, subImg, rect );
	IplImage* tempImg = cvCreateImage( cvSize( rect.width, rect.height ),8, 3 );
	m_imgProc.cvtBuffertoIplImage( subImg, tempImg );

	//test Image copy OK
	//cvShowImage( "ROI Image", tempImg ); 
	
	m_pHSVImg = cvCreateImage( cvGetSize(tempImg), 8,3 );
	m_pHueImg = cvCreateImage( cvGetSize(tempImg), 8,1 );
	m_pMask = cvCreateImage( cvGetSize(tempImg), 8,1 );
	m_pProbImg = cvCreateImage( cvGetSize(tempImg), 8,1 );
	m_pHist = cvCreateHist( 1, &nHistBins, CV_HIST_ARRAY, &m_pRanges, 1 );

	updateHueImage( tempImg );
	updateHist();

	cvReleaseImage( &m_pHSVImg );
	cvReleaseImage( &m_pHueImg );
	cvReleaseImage( &m_pMask );
	cvReleaseImage( &m_pProbImg );

	m_prevRect.height = rect.height;
	m_prevRect.width = rect.width;
	m_prevRect.x = rect.x;
	m_prevRect.y = rect.y;

	m_window.center.x = rect.x + 0.5*rect.width;
	m_window.center.y = rect.y + 0.5*rect.height;
	m_window.size.height = rect.height;
	m_window.size.width = rect.width;

	if( m_window.center.x > img->width() )
		m_window.center.x = img->width();
	if( m_window.center.y > img->height() )
		m_window.center.y = img->height();
	if( m_window.center.x + m_window.size.width > img->width() )
		m_window.size.width = img->width() - m_window.center.x;
	if( m_window.center.y + m_window.size.height > img->height() )
		m_window.size.height = img->height() - m_window.center.y;
	if( m_window.center.x - m_window.size.width < 0 )
		m_window.size.width = m_window.center.x;
	if( m_window.center.y - m_window.size.height < 0 )
		m_window.size.height = m_window.center.y;

	m_window.angle = 0.0f;
	cvReleaseImage( &tempImg );
	if( subImg != 0 )delete subImg;
}


bool KSHandTrackCamShiftTracker::track( BaseBuf *img, BaseBuf *depth, Rect &ROI )
{
	BaseBuf* subImg = new Buffer24( ROI.width, ROI.height, ROI.width*3 );
	m_imgProc.getROI( img, subImg, ROI );
	IplImage* tempImg = cvCreateImage( cvSize( ROI.width, ROI.height ),8, 3 );
	m_imgProc.cvtBuffertoIplImage( subImg, tempImg );	//tempImg size = ROI size
	
	int nOffsetX = ROI.x;
	int nOffsetY = ROI.y;

	//update the location
	CvConnectedComp components;

	//create Images
	m_pHSVImg = cvCreateImage( cvGetSize(tempImg), 8,3 );
	m_pHueImg = cvCreateImage( cvGetSize(tempImg), 8,1 );
	m_pMask = cvCreateImage( cvGetSize(tempImg), 8,1 );
	m_pProbImg = cvCreateImage( cvGetSize(tempImg), 8,1 );
	if( m_pHist == 0 )
	{
		m_pHist = cvCreateHist( 1, &nHistBins, CV_HIST_ARRAY, &m_pRanges, 1 );
		updateHist();
	}

	//updateHueImage
	updateHueImage( tempImg );

	//Create a prob iamge
	cvCalcBackProject( &m_pHueImg, m_pProbImg, m_pHist );
	cvAnd( m_pProbImg, m_pMask, m_pProbImg, 0 );

	//Use Camshift
	m_prevRect.x = m_prevRect.x - nOffsetX;
	m_prevRect.y = m_prevRect.y - nOffsetY;
	cvCamShift( m_pProbImg, m_prevRect,
                cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                &components, &m_window );

	//update location and angle
	m_prevRect = components.rect;
	m_prevRect.x = m_prevRect.x + nOffsetX;
	m_prevRect.y = m_prevRect.y + nOffsetY;
	m_window.angle = -m_window.angle;
	
	ROI.height = m_prevRect.height;
	ROI.width = m_prevRect.width;
	ROI.x = m_prevRect.x;
	ROI.y = m_prevRect.y;
	

	//release Images
	cvReleaseImage( &m_pHSVImg );
	cvReleaseImage( &m_pHueImg );
	cvReleaseImage( &m_pMask );
	cvReleaseImage( &m_pProbImg );
	//cvReleaseHist( &m_pHist );
	delete subImg;
	cvReleaseImage( &tempImg );
	if( components.rect.height == 0 || components.rect.width == 0 )
		return false;
	if( m_prevRect.x < 0 || m_prevRect.x > img->width() || m_prevRect.y < 0 || m_prevRect.y > img->height() 
		|| m_prevRect.width < 0 || m_prevRect.width > img->width() || m_prevRect.height < 0 || m_prevRect.height > img->height() )
		return false;

	return true;
}




void KSHandTrackCamShiftTracker::updateHist( )
{
	//update m_Hist, called every frame
	float maxVal = 0.0f;
	
	cvCalcHist( &m_pHueImg, m_pHist, 0, m_pMask );
	cvGetMinMaxHistValue( m_pHist, 0, &maxVal, 0, 0 );
	cvConvertScale( m_pHist->bins, m_pHist->bins, maxVal?maxVal:0,0);
}

void KSHandTrackCamShiftTracker::updateHueImage( const IplImage *pImg )
{
	//get the hue image of the current frame
	cvCvtColor( pImg, m_pHSVImg, CV_BGR2HSV );
	cvInRangeS( m_pHSVImg, cvScalar( 0, smin, MIN(vmin, vmax), 0 ),
		cvScalar( 180, 256, MAX(vmin, vmax), 0) , m_pMask );
	cvSplit( m_pHSVImg, m_pHueImg , 0, 0, 0 );
}