#pragma once

class KSFrameData
{
	int frameID;
	double timestamp;
	float lShoulderX;
	float lShoulderY;
	float lShoulderZ;
	float rShoulderX;
	float rShoulderY;
	float rShoulderZ;
	float torsoX;
	float torsoY;
	float torsoZ;
	float handX;
	float handY;
	float handZ;
	float torsoComp;		//-180-180 degree
	float shoulderRot;		//-180-180 degree
	float elbowOpen;        //0-180 degree
	int	  dataStreamStatus;

public:
	KSFrameData(void):frameID(0),timestamp(0.0f),lShoulderX(0.0f),lShoulderY(0.0f),lShoulderZ(0.0f),rShoulderX(0.0f),rShoulderY(0.0f),rShoulderZ(0.0f),torsoX(0.0f),torsoY(0.0f),torsoZ(0.0f),handX(0.0f),handY(0.0f),handZ(0.0f),torsoComp(0.0f),shoulderRot(0.0f),elbowOpen(0.0f),dataStreamStatus(0){};
	~KSFrameData(void){};

	int getFrameID(){return frameID;};
	double getTimestamp(){return timestamp;};
	float getLShoulderX(){return lShoulderX;};
	float getLShoulderY(){return lShoulderY;};
	float getLShoulderZ(){return lShoulderZ;};
	float getRShoulderX(){return rShoulderX;};
	float getRShoulderY(){return rShoulderY;};
	float getRShoulderZ(){return rShoulderZ;};
	float getTorsoX(){return torsoX;};
	float getTorsoY(){return torsoY;};
	float getTorsoZ(){return torsoZ;};
	float getTorsoComp(){return torsoComp;};
	float getShoulderRot(){return shoulderRot;};
	float getElbowOpen(){return elbowOpen;};
	float getHandX(){return handX;};
	float getHandY(){return handY;};
	float getHandZ(){return handZ;};
	int	  getStatus(){return dataStreamStatus;};
	void setFrameID(int id){frameID = id;};
	void setTimestamp(double t){timestamp = t;};
	void setLShoulder(float x, float y, float z){ lShoulderX = x;lShoulderY = y;lShoulderZ = z;};
	void setRShoulder(float x, float y, float z){ rShoulderX = x;rShoulderY = y;rShoulderZ = z;};
	void setTorso(float x, float y, float z){ torsoX = x;torsoY = y;torsoZ = z;};	
	void setTorsoComp(float tc){torsoComp = tc;};
	void setShoulderRot(float sr){shoulderRot = sr;};
	void setElbowOpen(float eo){elbowOpen = eo;};
	void setHand(float x, float y, float z){ handX = x; handY = y; handZ = z;};
	void setDataStreamStatus( int state ) { dataStreamStatus = state;};
	void update(int id, double t, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tc, float sr, float eo, float hx, float hy, float hz, int state ){
			frameID = id;
			timestamp = t;
			lShoulderX = x1;
			lShoulderY = y1;
			lShoulderZ = z1;
			rShoulderX = x2;
			rShoulderY = y2;
			rShoulderZ = z2;
			torsoX = x3;
			torsoY = y3;
			torsoZ = z3;
			torsoComp = tc;
			shoulderRot = sr;
			elbowOpen = eo;
			handX = hx;
			handY = hy;
			handZ = hz;
			dataStreamStatus = state;
	};
};

