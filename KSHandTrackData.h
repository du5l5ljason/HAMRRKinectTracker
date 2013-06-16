#pragma once
#include "Buffer.h"
#include "UserTypes.h"
#include "ImageProc.h"
#include "Background.h"


class KSHandTrackData
{
protected:
	POINT3D m_pHandPos;
	Rect	m_prevRect;

	ImageProc* m_pImgProc;
	Background* m_pBG;
	BaseBuf* m_img;
	float m_fRestPosDepth;
	bool	m_bIsReady;
public:
	POINT3D	getHandPos() const{return m_pHandPos;};
	Rect	getPrevRect() const{return m_prevRect;};
	void	setHandPos(POINT3D point){
			m_pHandPos.x = point.x;
			m_pHandPos.y = point.y;
			m_pHandPos.z = point.z;
				}
	void	setPrevRect( Rect rectangle ){ 
			if( rectangle.height < 0 )
				rectangle.height = 0;
			if( rectangle.width < 0 )
				rectangle.width = 0;
			m_prevRect = rectangle;
	}
	bool isReady(){return m_bIsReady;};
	void setReady(bool){m_bIsReady = true;};

	float getRestPosDepth(){return m_fRestPosDepth;};
	void setRestPosDepth(float f){ m_fRestPosDepth = f;};
	ImageProc* getImageProc(){return m_pImgProc;};
	Background* getBackground(){return m_pBG;};
	BaseBuf* getImg(){return m_img;};


public:
	//void update(BaseBuf* rgbImg, BaseBuf* depthImg, BaseBuf* rgbBG, BaseBuf* depthBG, int cx, int cy, int width, int height);
	void update(BaseBuf*, BaseBuf*, int, int, int, int );
	virtual void init(BaseBuf*, Rect&) = 0;
	virtual bool track(BaseBuf*, BaseBuf*, Rect&)=0;

private:
	void setScanWindow(int x, int y, int width, int height, Rect& rect);
	void updateWindow(Rect);

public:
	//Edited by Tingfang , 11/12/2011
	KSHandTrackData(void){};
	KSHandTrackData(int width, int height):m_pImgProc(NULL), m_bIsReady(false), m_fRestPosDepth(1500.0f){
		m_pBG = new Background;
		m_pImgProc = new ImageProc;
		m_img = new Buffer24(width,height,width*3);
		m_pHandPos.x = MODEL_StartX;
		m_pHandPos.y = MODEL_StartY;
		m_pHandPos.z =0;
		m_prevRect.x = 0;
		m_prevRect.y = 0;
		m_prevRect.height = height;
		m_prevRect.width = width;
	}
	virtual	~KSHandTrackData(void){};

};

