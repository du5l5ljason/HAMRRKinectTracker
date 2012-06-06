#include "StdAfx.h"
#include "KalmanFilter.h"

float A[4][4] = {	{1,0,1,0},
					{0,1,0,1},
					{0,0,1,0},
					{0,0,0,1}};
float Q[4][4] = {	{0.5,0,0,0},
					{0,0.5,0,0},
					{0,0,0.5,0},
					{0,0,0,0.5}};
float H[2][4] = {	{1,0,0,0},
					{0,1,0,0}};
float R[2][2] = {	{1,0},
					{0,1}};

float KalmanFilter::randn(){
	
	//The Box-Muller transform is used to generating normal distributed numbers.
	//http://en.wikipedia.org/wiki/Box_Muller_transform
	float rA, rB;
	rA = (float)(rand()%100000)/100000;
	rB = (float)(rand()%100000)/100000;
	if(rA == 0)
		rA = 1e-9;
	if(rB == 0)
		rB = 1e-9;
	
	return sqrt(-2*log(rA))*cos(2*PI*rB);
}

void KalmanFilter::predict()
{
	//predict Xk' according to Xk-1
	//AXk 
	float Xtemp[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for(int i = 0;i<4;++i)
		for(int j=0;j<4;++j)
			Xtemp[i] += A[i][j]*Xk[j][0];
	//randn()
	for( int i=0; i<4; ++i)
		Xk[i][0] = Xtemp[i];

	//predict Pk' according to Pk-1
	//A*Pk-1*AT + Q
	float Ptemp[4][4] = {	{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0}};
	float Ptemp2[4][4] = {	{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0}};
	for(int i = 0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			for(int k=0;k<4;++k)
				Ptemp[i][j] += A[i][k]*Pk[k][j];
		}
	}

	float At[4][4] = {	{1,0,0,0},
						{0,1,0,0},
						{1,0,1,0},
						{0,1,0,1}};
	for(int i = 0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			for(int k=0;k<4;++k)
				Ptemp2[i][j] += Ptemp[i][k]*At[k][j];
			Pk[i][j] = Ptemp2[i][j] + Q[i][j];
		}
	}

}

void KalmanFilter::correct()
{
	//Correct the measurement with Kalman filter, compute the revision matrix and 
	//renew poster state estimation as well as posteriori estimate covariance
	//Correct Kk
	float Ht[4][2] = {	{1,0},
						{0,1},
						{0,0},
						{0,0}};
	float Kk[4][2] = {	{0,0},
						{0,0},
						{0,0},
						{0,0}};
	float PHtemp[4][2] = {	{0,0},
							{0,0},
							{0,0},
							{0,0}};
	float PHtemp2[4][2] = {	{0,0},
							{0,0},
							{0,0},
							{0,0}};
	for( int i = 0;i<4;++i )
		for( int j=0;j<2;++j )
			for( int k=0;k<4;++k )
				PHtemp[i][j] += Pk[i][k]*Ht[k][j];

	float Ktemp[2][2] = {{0},{0}};
	for( int i = 0; i<2;++i)
	{
		for( int j=0; j<2;++j )
		{	
			for( int k=0; k<4;++k )
				Ktemp[i][j] += H[i][k]*PHtemp[k][j];
			
			Ktemp[i][j] += R[i][j];
			if(Ktemp[i][j] == 0)
				Ktemp[i][j] = 0.0f;
			else
				Ktemp[i][j] = 1/Ktemp[i][j];
		}
	}

	for( int i = 0; i<4; ++i )
		for( int j = 0; j<2; ++j )
		{
			for( int k =0; k<2; ++k)
			{
				PHtemp2[i][j] += PHtemp[i][k]* Ktemp[k][j];
			}
			Kk[i][j] = PHtemp2[i][j];
		}

	//Correct Xk

	//randn()
	float Xtemp[4][1] = {{0},{0},{0},{0}};
	float Xtemp2[2][1] = {{0},{0}};
	for( int i = 0; i<2; ++i)
		for( int j = 0; j<4; ++j)
			Xtemp2[i][0] += H[i][j]*Xk[j][0];

	for( int i = 0; i<4; ++i)
		for( int j =0; j<2; ++j)
			Xtemp[i][0] += Kk[i][j]*(Zk[j][0]-Xtemp2[j][0]);

	for( int i = 0; i< 4; ++i )
		Xk[i][0] += Xtemp[i][0];

	//Correct Pk
	float PTemp[4][4] = {	{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0}};

	for( int i=0; i<4; ++i )
		for( int j=0; j<4; ++j )
		{
			for( int k=0; k<2; ++k )
				PTemp[i][j] += -Kk[i][k]*H[k][j];
			if(i == j)
				PTemp[i][j] = 1 + PTemp[i][j];
		}


	float PTemp2[4][4] = {	{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,0}};

	for( int i=0; i<4 ; ++i )
		for( int j=0; j<4 ; ++j )
			for( int k=0; k<4; ++k )
				PTemp2[i][j] += PTemp[i][k] * Pk[k][j];

	for( int i=0; i<4; ++i)
		for( int j=0; j<4; ++j )
			Pk[i][j] = PTemp2[i][j];
}		

void KalmanFilter::updateVelocity()
{
	setVk( Xk[2][0], Xk[3][0] );
}