#pragma once

#include "KinectSensor.h"

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
