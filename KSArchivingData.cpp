#include "StdAfx.h"
#include "KSArchivingData.h"
	

void KSArchivingData::addAFrame(KSFrameData* frameData)
{
	if(frameData == NULL) return;
	if(frameData->getFrameID() < 0 )
		return;

	dataset[nFrames].setFrameID(frameData->getFrameID());
	dataset[nFrames].setTimestamp(frameData->getTimestamp());
	dataset[nFrames].setLShoulder(frameData->getLShoulderX(), frameData->getLShoulderY(), frameData->getLShoulderZ());
	dataset[nFrames].setRShoulder(frameData->getRShoulderX(), frameData->getRShoulderY(), frameData->getRShoulderZ());
	dataset[nFrames].setTorso(frameData->getTorsoX(), frameData->getTorsoY(), frameData->getTorsoZ());
	dataset[nFrames].setTorsoComp( frameData->getTorsoComp());
	dataset[nFrames].setShoulderRot( frameData->getShoulderRot());
	dataset[nFrames].setElbowOpen( frameData->getElbowOpen());
	dataset[nFrames].setHand( frameData->getHandX(), frameData->getHandY(), frameData->getHandZ());
	nFrames = nFrames + 1;


	if( nFrames >= _MAXARCHIVENUM ) 
		nFrames = nFrames - _MAXARCHIVENUM;
}

void KSArchivingData::saveArchivingData()
{
	FILE* fp;
	time_t t = time(0);
	char fileName[128];
	strftime( fileName, sizeof(fileName), _DATA_ARCHIVEFILE_PATH"KSData_%Y%m%d_%H%M%S.txt", localtime(&t) );
	if((fp = fopen(fileName, "w+"))==NULL)
		return;
	
	for( int i = 0 ; i< nFrames; ++i)
	{
		fprintf( fp, "%d %lf %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", dataset[i].getFrameID(), dataset[i].getTimestamp(), dataset[i].getLShoulderX(),dataset[i].getLShoulderY(), dataset[i].getLShoulderZ(), dataset[i].getRShoulderX(),dataset[i].getRShoulderY(),dataset[i].getRShoulderZ(), dataset[i].getTorsoX(),dataset[i].getTorsoY(),dataset[i].getTorsoZ(),dataset[i].getTorsoComp(),dataset[i].getShoulderRot(),dataset[i].getElbowOpen(), dataset[i].getHandX(),dataset[i].getHandY(),dataset[i].getHandZ() );
	}

	fclose( fp );
}