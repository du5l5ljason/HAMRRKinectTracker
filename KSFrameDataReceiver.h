#pragma once
//Tingfang Created 10/05,2012
#include "MulticastClient.h"
#include "KSFrameData.h"			//Should be a data class from Optitrak
#include "UserTypes.h"

class KSFrameDataReceiver
{
	KSFrameData m_ReceiverData;		//Should be a data class from Optitrak
	MulticastClient m_Client;

public:
	KSFrameDataReceiver(void);
	~KSFrameDataReceiver(void);

	KSFrameData* getData(){return &m_ReceiverData;};//Should be a data class from Optitrak
	void setData( KSFrameData data ){ m_ReceiverData = data;};//Should be a data class from Optitrak
	void openServer();
	void closeServer();
	void receiveData();
};

