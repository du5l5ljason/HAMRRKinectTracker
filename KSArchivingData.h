#pragma once
#include "KSFrameData.h"
#include <ctime>

#include "UserConfigs.h"
class KSArchivingData
{
	KSFrameData* dataset;
	int nFrames;
	bool m_bIsArchiving;
public:
	KSArchivingData(void):nFrames(0),m_bIsArchiving(false){
		dataset = new KSFrameData[_MAXARCHIVENUM];
	};
	~KSArchivingData(void){
		if( dataset != NULL )
			delete[] dataset;
	};
	KSFrameData* getData(){return dataset;};
	bool isArchiving(){return m_bIsArchiving;};
	void setIsArchiving(bool flag){m_bIsArchiving = flag;};

	void addAFrame(KSFrameData* frameData);
	void saveArchivingData();

};

