#pragma once
#include "Buffer.h"
#include <XnCppWrapper.h>
#include <XnUSB.h>
using namespace xn;

class KinectSensor
{
protected:
	BaseBuf* m_rgbImg;
	BaseBuf* m_depthImg;

public:
	BaseBuf* getRGBImg() {return m_rgbImg;};
	BaseBuf* getDepthImg() {return m_depthImg;};
	

public:
	KinectSensor(void):m_rgbImg(NULL), m_depthImg(NULL){};
	virtual	~KinectSensor(void){
		if( m_rgbImg != NULL) delete m_rgbImg;
		if( m_depthImg != NULL) delete m_depthImg;
	};
public:
	virtual void init()=0;
	virtual void open()=0;
	virtual void update()=0;
	virtual void close()=0;
};


