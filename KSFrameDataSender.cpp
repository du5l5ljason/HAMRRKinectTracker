#include "StdAfx.h"
#include "KSFrameDataSender.h"

KSFrameDataSender::KSFrameDataSender(void)
{
	memset( &m_SenderData, 0, sizeof( KSFrameData ) );
}

KSFrameDataSender::~KSFrameDataSender(void)
{
}

void KSFrameDataSender::openServer()
{
	m_Server.open( KINECTSERVER, KINECTSERVERPORT );
}

void KSFrameDataSender::closeServer()
{
	m_Server.close();
}

void KSFrameDataSender::sendData()
{
	m_Server.beginBuffer();

	m_Server.packBuffer( m_SenderData.getFrameID() );
	m_Server.packBuffer( m_SenderData.getTimestamp() );
	m_Server.packBuffer( m_SenderData.getHandX() );
	m_Server.packBuffer( m_SenderData.getHandY() );
	m_Server.packBuffer( m_SenderData.getHandZ() );
	m_Server.packBuffer( m_SenderData.getTorsoZComp() );
	m_Server.packBuffer( m_SenderData.getElbowOpen() );

	m_Server.send();
}