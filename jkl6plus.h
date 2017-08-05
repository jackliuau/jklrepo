// jkl6plus.h : main header file for the jkl6plus application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cjkl6plusApp:
// See jkl6plus.cpp for the implementation of this class
//

class Cjkl6plusApp : public CWinApp
{
public:
    Cjkl6plusApp();

    ULONG_PTR m_gdiplusToken;

    CMutex g_mtx1;

    static unsigned int __stdcall thFun(void *pPM);
    int mainn();

// Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

// Implementation

public:
    afx_msg void OnAppAbout();
    afx_msg void OnAppBezier();
    afx_msg void OnAppBSpline();
    afx_msg void OnAppOffset();
    afx_msg void OnAppCutback();
    afx_msg void OnAppOpen();
    afx_msg void OnAppSave();
    afx_msg void OnAppSaveAs();
    afx_msg void OnAppExit();
    DECLARE_MESSAGE_MAP()
};

extern Cjkl6plusApp theApp;
