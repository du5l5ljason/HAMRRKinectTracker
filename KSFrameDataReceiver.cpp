#include "StdAfx.h"
#include "KSFrameDataReceiver.h"

//Tingfang Created 10/05,2012
KSFrameDataReceiver::KSFrameDataReceiver(void)
{
	memset( &m_ReceiverData, 0, sizeof( KSFrameData ) );
}


KSFrameDataReceiver::~KSFrameDataReceiver(void)
{
}

void KSFrameDataReceiver::openServer()
{
	m_Client.open( KINECTSERVER, KINECTSERVERPORT );
}

void KSFrameDataReceiver::closeServer()
{
	m_Client.close();
}

void KSFrameDataReceiver::receiveData()
{
	int n = m_Client.read();
	m_Client.beginUnpack();

	if( n> 0 )
	{
		int id = m_Client.unpackInt();
		double time = m_Client.unpackDouble();
		//receive data from Optitrak.

	}
}