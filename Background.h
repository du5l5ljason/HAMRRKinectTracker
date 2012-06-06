#pragma once
#include "Buffer.h"
#include "UserTypes.h"
#include "ImageProc.h"
#include <cv.h>
#include <highgui.h>
#include <XnCppWrapper.h>
#include "UserConfigs.h"


using namespace xn;

class Background
{
private:
	BaseBuf* m_imgRgbBG;
	BaseBuf* m_imgDepthBG;
	ImageProc m_pImgProc;
	bool m_bIsBG;

	unsigned int m_nDepthThresh;
public:
	BaseBuf* getRgbBG() const{ return m_imgRgbBG;};
	BaseBuf* getDepthBG() const{ return m_imgDepthBG;};
	void setRgbBG( BaseBuf* buf) { 
		m_imgRgbBG->copyBuffer(buf->getData());
	}
	void setDepthBG( BaseBuf* buf ) {
		m_imgDepthBG->copyBuffer(buf->getData());
	}
	bool isBG(){return m_bIsBG; }
	
	unsigned int getDepthThresh(){return m_nDepthThresh;};
	void setDepthThresh(unsigned int n){m_nDepthThresh = n;};
	void init(BaseBuf*, BaseBuf*);
	void removeBG(BaseBuf*, BaseBuf*, BaseBuf*, BaseBuf*,  BaseBuf*, Rect, float);
	void removeBG( BaseBuf*, BaseBuf*, BaseBuf*, Rect, float  ); 
public:
	Background(void):m_imgRgbBG(NULL), m_imgDepthBG(NULL),m_bIsBG(false){

	};
	Background(int w, int h):m_bIsBG(false),m_nDepthThresh(0){
		m_imgRgbBG = new Buffer24(w, h, w*3);
		m_imgDepthBG = new Buffer16(w, h, w*2);
	}
	~Background(void){
		delete m_imgRgbBG;
		delete m_imgDepthBG;

	}

private:
	bool loadBGfromFile(const char*, const char*);
};

