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
	m_Server.packBuffer( m_SenderData.getLShoulderX() );
	m_Server.packBuffer( m_SenderData.getLShoulderY() );
	m_Server.packBuffer( m_SenderData.getLShoulderZ() );
	m_Server.packBuffer( m_SenderData.getRShoulderX() );
	m_Server.packBuffer( m_SenderData.getRShoulderY() );
	m_Server.packBuffer( m_SenderData.getRShoulderZ() );
	m_Server.packBuffer( m_SenderData.getTorsoX() );
	m_Server.packBuffer( m_SenderData.getTorsoY() );
	m_Server.packBuffer( m_SenderData.getTorsoZ() );
	m_Server.packBuffer( m_SenderData.getTorsoComp() );
	m_Server.packBuffer( m_SenderData.getShoulderRot() );
	m_Server.packBuffer( m_SenderData.getElbowOpen() );

	m_Server.send();
}