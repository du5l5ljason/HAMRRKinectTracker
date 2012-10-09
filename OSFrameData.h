#pragma once
class OSFrameData
{
	int state;
public:
	int getState(){return state;};
	void setState( int id ){ state = id; };
	void update( int id ){
		state = id;
	};
public:
	OSFrameData(void):state(-1){};
	~OSFrameData(void){};
	
};

