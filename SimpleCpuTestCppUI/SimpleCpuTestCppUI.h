
// SimpleCpuTestCppUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSimpleCpuTestCppUIApp:
// See SimpleCpuTestCppUI.cpp for the implementation of this class
//

class CSimpleCpuTestCppUIApp : public CWinApp
{
public:
	CSimpleCpuTestCppUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSimpleCpuTestCppUIApp theApp;
