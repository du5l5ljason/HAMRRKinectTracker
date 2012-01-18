#pragma once
#include "Buffer.h"
#include <XnCppWrapper.h>

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
	~KinectSensor(void){
		if( m_rgbImg != NULL) delete m_rgbImg;
		if( m_depthImg != NULL) delete m_depthImg;
	};
public:
	virtual void init()=0;
	virtual void open()=0;
	virtual void update()=0;
	virtual void close()=0;
};

class KinectOpenNI: public KinectSensor {
private:
	Context				m_Context;
	DepthGenerator		m_depthGen;
	ImageGenerator		m_rgbGen;
public:
	Context* getContext() {return &m_Context;};
	DepthGenerator* getDepthGenerator() {return &m_depthGen;};
public:
	KinectOpenNI(void) : KinectSensor() {
	}
	void init();
	void open();
	void update();
	void close();
};

