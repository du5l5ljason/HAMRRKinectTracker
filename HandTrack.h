#pragma once
#include "Buffer.h"
#include "StructDefine.h"
#include "ImageProc.h"
#include "Background.h"

#define ROI_SIZE_W 80
#define ROI_SIZE_H 80
class HandTrack
{
protected:
	POINT3D m_pHandPos;
	int		m_rectHeight;
	int		m_rectWidth;
	ImageProc* m_pImgProc;
	Background* m_pBG;
	BaseBuf* m_img;

public:
	POINT3D	getHandPos() const{return m_pHandPos;};
	int getHeight(){return m_rectHeight;};
	int getWidth(){return m_rectWidth;};
	ImageProc* getImageProc(){return m_pImgProc;};
	Background* getBackground(){return m_pBG;};
	BaseBuf* getImg(){return m_img;};

public:
	void track(BaseBuf* rgbImg, BaseBuf* depthImg, BaseBuf* rgbBG, BaseBuf* depthBG, int x, int y, int width, int height);
protected:
	virtual void updateHandPos(BaseBuf*, BaseBuf*, Rect&)=0;

private:
	void setScanWindow(int x, int y, int width, int height, Rect& rect);
	void updateWindow(Rect);

public:
	//Edited by Tingfang , 11/12/2011
	HandTrack(void){};
	HandTrack(int width, int height): m_rectWidth(width), m_rectHeight(height), m_pImgProc(NULL){
		m_pBG = new Background;
		m_pImgProc = new ImageProc;
		m_img = new Buffer24(width,height,width*3);
		m_pHandPos.x =0;
		m_pHandPos.y =0;
		m_pHandPos.z =0;
	}
	~HandTrack(void){
		if(m_pBG!=NULL)
			delete m_pBG;
		if(m_pImgProc!=NULL)
			delete m_pImgProc;
		if(m_img!=NULL)
			delete m_img;
	};
};

