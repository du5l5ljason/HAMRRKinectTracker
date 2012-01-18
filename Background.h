#pragma once
#include "Buffer.h"
#include "StructDefine.h"
#include "ImageProc.h"
#define _RGB_THRESH 40
#define _DEP_THRESH 10

class Background
{
private:
	BaseBuf* m_imgRgbBG;
	BaseBuf* m_imgDepthBG;
	ImageProc m_pImgProc;
	bool m_bIsBG;
	
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
	void init(BaseBuf*, BaseBuf*);
	void removeBG(BaseBuf*, BaseBuf*, BaseBuf*, BaseBuf*,  BaseBuf*, Rect);

public:
	Background(void):m_imgRgbBG(NULL), m_imgDepthBG(NULL),m_bIsBG(false){};
	Background(int w, int h):m_bIsBG(false){
		m_imgRgbBG = new Buffer24(w, h, w*3);
		m_imgDepthBG = new Buffer16(w, h, w*2);
	}
	~Background(void){
		delete m_imgRgbBG,m_imgDepthBG;
	}
};

