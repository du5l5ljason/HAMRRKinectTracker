#pragma once
#include "imgwnd.h"


class CDrawWnd :
	public CImgWnd
{
public:
	CDrawWnd(void);
	~CDrawWnd(void);

	void showSkeleton(POINT3D* jointPos);
	void showHandJoint(POINT3D RightPos);
	void showHandJoint(POINT3D leftPos, POINT3D RightPos);
};

