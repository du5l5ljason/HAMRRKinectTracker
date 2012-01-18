#include "StdAfx.h"
#include "KinectSensor.h"

//-------------------------------------------------------------------------
const XnChar* SAMPLE_XML_PATH = "C:\\Program Files\\OpenNI\\Data\\SamplesConfig.xml";

void checkOpenNIError( XnStatus eResult, char* sStatus)
{
	//if( eResult != XN_STATUS_OK )
	//	cerr << sStatus << " Error : " << xnGetStatusString( eResult ) << endl;
}
//-------------------------------------------------------------------------


void KinectOpenNI::init(){
	//Initialize context
	checkOpenNIError( m_Context.InitFromXmlFile(SAMPLE_XML_PATH), "initialize context");

	//Create generator
	checkOpenNIError( m_depthGen.Create( m_Context ), "Create depth generator" );
	checkOpenNIError( m_rgbGen.Create( m_Context ), "Create image generator" );


	//Set Map Output Mode
	XnMapOutputMode mapMode;
	m_depthGen.GetMapOutputMode(mapMode);
 	m_depthGen.GetAlternativeViewPointCap().SetViewPoint( m_rgbGen );

	m_rgbImg = new Buffer24(mapMode.nXRes, mapMode.nYRes, ((mapMode.nXRes*3+3)/4)*4);
	m_depthImg = new Buffer16(mapMode.nXRes, mapMode.nYRes, ((mapMode.nXRes*2+3)/4)*4);
}

void KinectOpenNI::open() {
	checkOpenNIError( m_Context.StartGeneratingAll(), "Start DataStream" );

	//m_rgbImg->copyBuffer((BYTE*)m_rgbGen.GetImageMap());
	//m_depthImg->copyBuffer((BYTE*)m_depthGen.GetDepthMap());
}
void KinectOpenNI::update(){
	checkOpenNIError( m_Context.WaitAnyUpdateAll(), "Update DataStream" );
	m_rgbImg->copyBuffer((BYTE*)m_rgbGen.GetImageMap());
	m_depthImg->copyBuffer((BYTE*)m_depthGen.GetDepthMap());
}

void KinectOpenNI::close(){
	checkOpenNIError( m_Context.StopGeneratingAll(), "Stop DataStream" );
	m_Context.Release();
}
