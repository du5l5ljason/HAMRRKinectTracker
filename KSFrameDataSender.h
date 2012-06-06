#pragma once

#include "MulticastServer.h"
#include "KSFrameData.h"
#include "UserTypes.h"

class KSFrameDataSender
{
	KSFrameData m_SenderData;
	MulticastServer m_Server;

public:
	KSFrameDataSender(void);
	~KSFrameDataSender(void);

	KSFrameData* getData(){return &m_SenderData;};
	void setData( KSFrameData data ) { m_SenderData = data ;};
	void openServer();
	void closeServer();
	void sendData();
};

