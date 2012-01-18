#pragma once
// remove background (singleton class)
#include "Buffer.h"
#include "math.h"
class RemoveBackground
{
private:
	BaseBuf* m_rgbBackground;
	BaseBuf* m_depthBackground;

public:
	BaseBuf* getRGBBackground() { return m_rgbBackground;};
	BaseBuf* getDepthBackground() { return m_depthBackground;};
	void setRGBBackground(BaseBuf* rgb) {m_rgbBackground->copyBuffer(*rgb);} ;
	void setDepthBackground(BaseBuf* depth) {m_depthBackground->copyBuffer(*depth);};

	bool removeRGB(BaseBuf* rgb, BYTE threshold, BaseBuf* mask) {
		mask->zeroBuffer();
		for( int i = 0 ; i < rgb->height() ; i ++ ) {
			for( int j = 0 ; j  <rgb->width() ; j++ ) {
				BYTE x[3];
				BYTE y[3];
				m_rgbBackground->getPixelAt(i,j,x);
				rgb->getPixelAt(i,j,y);
				if( abs(x[0]-y[0]) > threshold || abs(x[1]-y[1]) > threshold || abs(x[2]-y[2]) > threshold ) {

				}

			}
		}
		return true;
	}
	bool removeDepth(BaseBuf* rgb, unsigned short threshold, BaseBuf* mask){
		mask->zeroBuffer();
		return true;
	}

public:
	RemoveBackground(void) {
		m_rgbBackground = new Buffer24();
		m_depthBackground = new Buffer16();
	};
	~RemoveBackground(void) {
		delete m_rgbBackground;
		delete m_depthBackground;
	};
};

