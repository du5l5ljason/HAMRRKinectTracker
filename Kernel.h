#pragma once
class Kernel
{

private:
	double **m_pKlArr;			//Kernel, the size of the array should be set to 640*480
	double **m_pKlGradArr;		//The Gradient of Kernel
	int		m_height;
	int		m_width;
public:
	int	 Height(){return m_height;};
	int  Width(){return m_width;};
	void setHeight(int h){m_height = h;};
	void setWidth(int w){m_width = w;};
	void calcKernel( int, int);
	//void calcGradKernel( int, int);
	double **getKernel(){return m_pKlGradArr;};
	double **getGradKernel(){return m_pKlGradArr;};
	double getKernelAt(int, int, int, int);
public:
	Kernel(void){};
	Kernel(int w, int h):m_height(h),m_width(w){
	m_pKlArr = new double*[w];
	for(int i=0;i<w;++i)
		m_pKlArr[i] = new double[h];

	m_pKlGradArr = new double*[w];
	for(int i=0;i<w;++i)
		m_pKlGradArr[i] = new double[h];
	}
	~Kernel(void){
		for(int i=0;i<m_width;++i)
		{	
			delete[] m_pKlArr[i];
			delete[] m_pKlGradArr[i];
		}
		delete[] m_pKlArr;
		delete[] m_pKlGradArr;
	};
};	

