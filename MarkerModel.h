#pragma once
#include "Buffer.h"
#include "StructDefine.h"
	
#define MEAN_BLACK	0.0762
#define VAR_BLACK	0.0027
#define MEAN_GREEN  0.1127
#define VAR_GREEN   0.002
#define MOD_ALPHA	0.02
#define MOD_BETA	0.001
#define MOD_WP		0.9
#define MOD_WB		0.1

class MarkerModel
{
	BaseBuf* m_pModPink;
	BaseBuf* m_pModBlack;
	bool    m_bIsCreated;
	//For test
public:
	BaseBuf* getModPink(){return m_pModPink;};
	BaseBuf* getModBlack(){return m_pModBlack;};
	bool     isCreate() {return m_bIsCreated;};
	bool	 create();
	double	 matching(BaseBuf*);
	double   matching(BaseBuf*, BaseBuf*);
	void	 calDistMap(BaseBuf*, BaseBuf*, BaseBuf*, Rect);

public:
	MarkerModel(void):m_pModPink(NULL),m_pModBlack(NULL){};
	MarkerModel(int width, int height):m_pModPink(NULL),m_pModBlack(NULL), m_bIsCreated(false){
		m_pModPink = new Buffer8(width,height,width*1);
		m_pModBlack = new Buffer8(width,height,width*1);
		
	}
	~MarkerModel(void){
		if(m_pModPink!=NULL)
			delete m_pModPink;
		if(m_pModBlack=NULL)
			delete m_pModBlack;
	}
};



