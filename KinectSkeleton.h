#pragma once
#include "UserTypes.h"
#include "UserConfigs.h"


class KinectSkeleton
{
protected:
	int m_jointCount;
	POINT3D m_jointPos[MAXJOINT];
	POINT3D m_joint3DPos[MAXJOINT];
	float m_jointConf[MAXJOINT];

public:
	int getJointCount() {return m_jointCount;};
	POINT3D* getJointPos(){return m_jointPos;};
	POINT3D* getJoint3DPos(){return m_joint3DPos;};
	POINT3D getJointPosAt(int idx) {return m_jointPos[idx];};
	POINT3D getJoint3DPosAt(int idx) {return m_joint3DPos[idx];};
	float getJointConfAt(int idx) {return m_jointConf[idx];};

	void setJointCount(int count) {m_jointCount = count;};
	void setJointPosAt(int idx, POINT3D pt, float conf) {m_jointPos[idx] = pt; m_jointConf[idx]=conf;};
	void setJoint3DPosAt(int idx, POINT3D pt, float conf){ m_joint3DPos[idx] = pt; m_jointConf[idx] = conf;};
	virtual bool update() = 0;
public:

	KinectSkeleton(void):m_jointCount(0) {};
	KinectSkeleton(int count):m_jointCount(count) {};
	~KinectSkeleton(void){};
};

