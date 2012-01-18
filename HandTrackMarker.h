#pragma once
#include <cv.h>
#include "handtrack.h"
#include "MarkerModel.h"


enum COLORMODE{
	_CM_NONE = 0,
	_CM_PINK,
	_CM_BLACK,
};

#define MODEL_WIDTH 40
#define MODEL_HEIGHT 40 

class HandTrackMarker :
	public HandTrack
{
private:

	int m_colorMode;
	MarkerModel* m_pModel;

public:

	MarkerModel* getModel(){return m_pModel;};
	int getColorMode() const{return m_colorMode;};
	void setColorMode(int cm) {m_colorMode = cm;};
	
public:
	HandTrackMarker(void);
	HandTrackMarker(int width, int height):HandTrack(width, height), m_colorMode(0){
		m_pModel = new MarkerModel(MODEL_WIDTH,MODEL_HEIGHT); 
	}
	~HandTrackMarker(void){
		if(m_pModel!=NULL)
			delete m_pModel;
		};

private:
	void relocateHandPos(BaseBuf*, BaseBuf*, Rect&);
	void findMatchWindow(BaseBuf*, Rect&);
	void segColorBased(BaseBuf*, BaseBuf*, Rect );
	void findMaxBoundingRect(BaseBuf*, int, Rect&);
};

