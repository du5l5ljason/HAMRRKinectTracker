#include "StdAfx.h"
#include "KSFrameDataReceiver.h"

//Tingfang Created 10/05,2012
KSFrameDataReceiver::KSFrameDataReceiver(void)
{
	memset( &m_ReceiverData, 0, sizeof( OSFrameData ) );
}


KSFrameDataReceiver::~KSFrameDataReceiver(void)
{
}

void KSFrameDataReceiver::openClient()
{
	m_Client.open( DASHSERVER, DASHSERVERPORT );
}

void KSFrameDataReceiver::closeClient()
{
	m_Client.close();
}

void KSFrameDataReceiver::receiveData()
{
	int n = m_Client.read();
	m_Client.beginUnpack();

	if( n> 0 )
	{
		int dashState = m_Client.unpackInt();			
		//receive data from Optitrak.
		m_ReceiverData.setState( dashState );
	}
}