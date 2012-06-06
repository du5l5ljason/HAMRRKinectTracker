#pragma once
#include "imgwnd.h"
#include "UserTypes.h"

class CDrawWnd :
	public CImgWnd
{
public:
	CDrawWnd(void);
	~CDrawWnd(void);

	void showSkeleton(POINT3D* jointPos);
	void showRect(Rect rect);
	void showHandJoint(POINT3D RightPos);
	void showHandJoint(POINT3D leftPos, POINT3D RightPos);
};

