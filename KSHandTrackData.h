#pragma once
#include "Buffer.h"
#include "UserTypes.h"
#include "ImageProc.h"
#include "Background.h"


class KSHandTrackData
{
protected:
	POINT3D m_pHandPos;
	int		m_rectHeight;
	int		m_rectWidth;
	
	ImageProc* m_pImgProc;
	Background* m_pBG;
	BaseBuf* m_img;
	float m_fRestPosDepth;

public:
	POINT3D	getHandPos() const{return m_pHandPos;};
	int getHeight(){return m_rectHeight;};
	int getWidth(){return m_rectWidth;};
	float getRestPosDepth(){return m_fRestPosDepth;};
	void setRestPosDepth(float f){ m_fRestPosDepth = f;};

	ImageProc* getImageProc(){return m_pImgProc;};
	Background* getBackground(){return m_pBG;};
	BaseBuf* getImg(){return m_img;};
	void setHandPos(POINT3D point){
		m_pHandPos.x = point.x;
		m_pHandPos.y = point.y;
		m_pHandPos.z = point.z;
			}

public:
	//void update(BaseBuf* rgbImg, BaseBuf* depthImg, BaseBuf* rgbBG, BaseBuf* depthBG, int cx, int cy, int width, int height);
	void update(BaseBuf*, BaseBuf*, int, int, int, int );
protected:
	virtual void updateHandPos(BaseBuf*, BaseBuf*, Rect&)=0;

private:
	void setScanWindow(int x, int y, int width, int height, Rect& rect);
	void updateWindow(Rect);

public:
	//Edited by Tingfang , 11/12/2011
	KSHandTrackData(void){};
	KSHandTrackData(int width, int height): m_rectWidth(width), m_rectHeight(height), m_pImgProc(NULL), m_fRestPosDepth(1500.0f){
		m_pBG = new Background;
		m_pImgProc = new ImageProc;
		m_img = new Buffer24(width,height,width*3);
		m_pHandPos.x =MODEL_StartX;
		m_pHandPos.y =MODEL_StartY;
		m_pHandPos.z =0;
	}
	~KSHandTrackData(void){
		if(m_pBG!=NULL)
			delete m_pBG;
		if(m_pImgProc!=NULL)
			delete m_pImgProc;
		if(m_img!=NULL)
			delete m_img;
	};
};

