#pragma once
//Tingfang Created 10/05,2012
#include "MulticastClient.h"
#include "OSFrameData.h"			//Should be a data class from Optitrak
#include "UserConfigs.h"

class KSFrameDataReceiver
{
	OSFrameData m_ReceiverData;		//Should be a data class from Optitrak
	MulticastClient m_Client;

public:
	KSFrameDataReceiver(void);
	~KSFrameDataReceiver(void);

	OSFrameData* getData(){return &m_ReceiverData;};//Should be a data class from Optitrak
	void setData( OSFrameData data ){ m_ReceiverData = data;};//Should be a data class from Optitrak
	void openClient();
	void closeClient();
	void receiveData();
};

