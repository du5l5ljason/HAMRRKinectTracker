#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "UserConfigs.h"
using namespace std;


class KalmanFilter{
public:
	//Constructor
	KalmanFilter(void){
		//initialize Xk, Zk, Pk
		for(int i=0;i<4;++i)
			for(int j=0;j<1;++j)
				Xk[i][j] = 0.0f;
		for(int i=0;i<2;++i)
			for(int j=0;j<1;++j)
				Zk[i][j] = 0.0f;
		for(int i=0;i<4;++i)
			for(int j=0;j<4;++j)
				Pk[i][j] = 0.0f;
		for(int i=0;i<2;++i)
			Vk[i][0] = 0.0f;
	};
	~KalmanFilter(void){};
private:
	//Properties
	float Xk[4][1];
	float Zk[2][1];
	float Pk[4][4];
	float Vk[2][1];
public:
	//Interface
	void predict();
	void correct();
	//void updateVelocity;
	float randn();
	void updateVelocity();
	//
	void setXk(float x, float y, float vx, float vy)
	{
		Xk[0][0] = x;
		Xk[1][0] = y;
		Xk[2][0] = vx;
		Xk[3][0] = vy;
	}

	void setZk(float x, float y)
	{
		Zk[0][0] = x;
		Zk[1][0] = y;
	}

	void setVk( float x, float y )
	{
		Vk[0][0] = x;
		Vk[1][0] = y;
	}

	float getXkAt(int row, int col){return Xk[row][col];}
	float getZkAt(int row, int col){return Zk[row][col];}
	float getPkAt(int row, int col){return Pk[row][col];}
	float getVkAt(int row, int col){return Vk[row][col];};

	//methods & Utils
	
};