#include "StdAfx.h"
#include "Kernel.h"
#include <math.h>

void Kernel::calcKernel(int half_x, int half_y)
{
	double euclideanDistance;
	for( int x = -half_x; x<half_x; x++ )
		for( int y = -half_y; y<half_y; y++ )
		{
			euclideanDistance = getKernelAt(x, y, half_x, half_y);
			m_pKlArr[x+half_x][y+half_y] = euclideanDistance;
			m_pKlGradArr[x+half_x][y+half_y] = euclideanDistance;		//g(x) = - k'(x), k = 1-x
		}
}

double Kernel::getKernelAt(int x, int y, int half_x, int half_y)
{
	double euclideanDistance = sqrt( (float)x/half_x *(float)x/half_x + (float)y/half_y*(float)y/half_y );
	if(euclideanDistance > 1 )
		return 0.0;
	else
		return (1.0 - euclideanDistance * euclideanDistance);
}