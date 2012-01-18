
// MRRKinect.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMRRKinectApp:
// See MRRKinect.cpp for the implementation of this class
//

class CMRRKinectApp : public CWinApp
{
public:
	CMRRKinectApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMRRKinectApp theApp;