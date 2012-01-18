#pragma once
#include "StructDefine.h"

#define MAXJOINT 25

class KinectSkeleton
{
protected:
	int m_jointCount;
	POINT3D m_jointPos[MAXJOINT];
	float m_jointConf[MAXJOINT];

public:
	int getJointCount() {return m_jointCount;};
	POINT3D* getJointPos(){return m_jointPos;};
	POINT3D getJointPosAt(int idx) {return m_jointPos[idx];};
	float getJointConfAt(int idx) {return m_jointConf[idx];};

	void setJointCount(int count) {m_jointCount = count;};
	void setJointPosAt(int idx, POINT3D pt, int conf) {m_jointPos[idx] = pt; m_jointConf[idx]=conf;};

	virtual POINT2D convertJoint2DAt(int idx)=0;
	virtual bool update() = 0;
public:

	KinectSkeleton(void):m_jointCount(0) {};
	KinectSkeleton(int count):m_jointCount(count) {};
	~KinectSkeleton(void){};
};

