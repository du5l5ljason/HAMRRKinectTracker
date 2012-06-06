#pragma once

class KSFrameData
{
	int frameID;
	double timestamp;
	float handX;
	float handY;
	float handZ;
	float torsoZComp;
	float elbowOpen;
public:
	KSFrameData(void);
	~KSFrameData(void);

	int getFrameID(){return frameID;};
	double getTimestamp(){return timestamp;};
	float getHandX(){return handX;};
	float getHandY(){return handY;};
	float getHandZ(){return handZ;};
	float getTorsoZComp(){return torsoZComp;};
	float getElbowOpen(){return elbowOpen;};
	void setFrameID(int id){frameID = id;};
	void setTimestamp(double t){timestamp = t;};
	void setHandXYZ(float x, float y, float z){ handX = x;handY = y;handZ = z;};
	void setTorsoZComp(float tc){torsoZComp = tc;};
	void setElbowOpen(float eo){elbowOpen = eo;};
	void update(int id, double t, float x, float y, float z, float tc, float eo){
			frameID = id;
			timestamp = t;
			handX = x;
			handY = y;
			handZ = z;
			
			torsoZComp = tc;
			elbowOpen = eo;
	};
};

