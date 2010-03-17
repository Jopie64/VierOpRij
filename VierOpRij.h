
// VierOpRij.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Threading.h"

// CVierOpRijApp:
// See VierOpRij.cpp for the implementation of this class
//

class CVierOpRijApp : public CWinAppEx
{
public:
	CVierOpRijApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVierOpRijApp theApp;

class CMainTd : public Threading::CWinMlHook, public Threading::CSingleton<CMainTd>
{
public:
};
