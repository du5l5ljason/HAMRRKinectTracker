#include "StdAfx.h"
#include "KSArchivingData.h"
	

void KSArchivingData::addAFrame(KSFrameData* frameData)
{
	if(frameData == NULL) return;
	if(frameData->getFrameID() < 0 )
		return;

	dataset[nFrames].setFrameID(frameData->getFrameID());
	dataset[nFrames].setTimestamp(frameData->getTimestamp());
	dataset[nFrames].setHandXYZ(frameData->getHandX(), frameData->getHandY(), frameData->getHandZ());
	dataset[nFrames].setTorsoZComp( frameData->getTorsoZComp() );
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
		fprintf( fp, "%d %lf %f %f %f %f\n", dataset[i].getFrameID(), dataset[i].getTimestamp(), dataset[i].getHandX(),dataset[i].getHandY(), dataset[i].getHandZ(), dataset[i].getTorsoZComp() );
	}

	fclose( fp );
}