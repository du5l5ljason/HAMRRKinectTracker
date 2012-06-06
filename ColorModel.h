#pragma once
#include "Buffer.h"
#include "UserTypes.h"
#include "Kernel.h"
//#define MEAN_BLACK	0.0762
//#define VAR_BLACK	0.0027
//#define MEAN_GREEN  0.1127
//#define VAR_GREEN   0.002
//#define MOD_ALPHA	0.02
//#define MOD_BETA	0.001
//#define MOD_WP		0.9
//#define MOD_WB		0.1
#define NUMBINSH 32
#define BINSIZEH 8
#define NUMBINSV 16
#define BINSIZEV 16
#define NUMBINS  512
class ColorModel
{
private:
	double m_Model[NUMBINS];
	bool binsInit;
	unsigned int** bins;		//bin allocation table
	unsigned int m_nXDim, m_nYDim;
	//For test
public:
	double operator[](unsigned int);		//The [] operator indexes into m_model
	unsigned int findBin( unsigned char, unsigned char ) ;
	unsigned int getXDim(){return m_nXDim;};
	unsigned int getYDim(){return m_nYDim;};
	//empties the model for updating
	void clearModel();

	void updateModel(BaseBuf*, int, int, int, int, Kernel*);		//calculate qu, pu
	
	unsigned int theBin(unsigned int, unsigned int);			//returns the bin-number of the pixel at x,y
public:
	ColorModel(void);
	~ColorModel(void);
};



