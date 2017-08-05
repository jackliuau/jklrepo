// jkl6plus.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "jkl6plus.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cjkl6plusApp

BEGIN_MESSAGE_MAP(Cjkl6plusApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &Cjkl6plusApp::OnAppAbout)
    ON_COMMAND(ID_APP_BEZIER, &Cjkl6plusApp::OnAppBezier)
    ON_COMMAND(ID_APP_BSPLINE, &Cjkl6plusApp::OnAppBSpline)
    ON_COMMAND(ID_APP_OFFSET, &Cjkl6plusApp::OnAppOffset)
    ON_COMMAND(ID_APP_CUTBACK, &Cjkl6plusApp::OnAppCutback)
    ON_COMMAND(ID_APP_OPEN, &Cjkl6plusApp::OnAppOpen)
    ON_COMMAND(ID_APP_SAVE, &Cjkl6plusApp::OnAppSave)
    ON_COMMAND(ID_APP_SAVE_AS, &Cjkl6plusApp::OnAppSaveAs)
    ON_COMMAND(ID_APP_EXIT, &Cjkl6plusApp::OnAppExit)
END_MESSAGE_MAP()

// Cjkl6plusApp construction
Cjkl6plusApp theApp;

int g_idx;
long g_num;
const int THREAD_NUM = 18;
HANDLE  g_hdls[THREAD_NUM];
HANDLE  g_hdl_0;
HANDLE  g_hdl_1;
CRITICAL_SECTION g_cs_0;
CRITICAL_SECTION g_cs_1;

void thOutput(const char* szFormat, ...)
{
    char szBuff[128];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugString(szBuff);
}

unsigned int __stdcall Cjkl6plusApp::thFun(void *pPM)
{
    int th_id = *(int*)pPM;
    SetEvent(g_hdl_0);

    //::WaitForSingleObject(g_hdl_1, INFINITE);
    //g_mtx1.Lock();
    EnterCriticalSection(&g_cs_1);
    DWORD dwTick0 = GetTickCount();

    Sleep(50);
    g_num++;
    Sleep(0);
    thOutput("tick:%d~%d -- th_id:%d, g_num:%d\r\n", dwTick0, GetTickCount(), th_id, g_num);

    //SetEvent(g_hdl_1);
    //g_mtx1.Unlock();
    LeaveCriticalSection(&g_cs_1);

    return 0;
}

int Cjkl6plusApp::mainn()
{
    g_hdl_0 = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hdl_1 = CreateEvent(NULL, FALSE, FALSE, NULL);
    InitializeCriticalSection(&g_cs_0);
    InitializeCriticalSection(&g_cs_1);


    //for (int ii = 0; ii < 8; ii++)
    {
        g_num = 0;
        g_idx = 0;

        while (g_idx < THREAD_NUM)
        {
            g_hdls[g_idx] = (HANDLE)_beginthreadex(NULL, 0, &Cjkl6plusApp::thFun, &g_idx, 0, NULL);
            ::WaitForSingleObject(g_hdl_0, INFINITE);
            //g_mtx0.Lock();
            g_idx++;
        }

        WaitForMultipleObjects(THREAD_NUM, g_hdls, TRUE, INFINITE);
    }

    CloseHandle(g_hdl_0);
    CloseHandle(g_hdl_1);
    DeleteCriticalSection(&g_cs_0);
    DeleteCriticalSection(&g_cs_1);

    return 0;
}

Cjkl6plusApp::Cjkl6plusApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    mainn();
}

// The one and only Cjkl6plusApp object

// Cjkl6plusApp initialization

BOOL Cjkl6plusApp::InitInstance()
{
    CWinApp::InitInstance();

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // To create the main window, this code creates a new frame window
    // object and then sets it as the application's main window object
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame)
        return FALSE;
    m_pMainWnd = pMainFrame;

    // create and load the frame with its resources
    pMainFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

    // The one and only window has been initialized, so show and update it
    pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
    pMainFrame->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand
    return TRUE;
}

int Cjkl6plusApp::ExitInstance()
{
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    return CWinApp::ExitInstance();
}

// Cjkl6plusApp message handlers

void Cjkl6plusApp::OnAppAbout()
{
    CDialog dlg(IDD_ABOUTBOX);
    dlg.DoModal();
}

void Cjkl6plusApp::OnAppBezier()
{
    ((CMainFrame*)m_pMainWnd)->m_ChildView.SwitchBezier();
}

void Cjkl6plusApp::OnAppBSpline()
{
    ((CMainFrame*)m_pMainWnd)->m_ChildView.SwitchBSpline();
}

void Cjkl6plusApp::OnAppOffset()
{
    ((CMainFrame*)m_pMainWnd)->m_ChildView.SwitchOffset();
}

void Cjkl6plusApp::OnAppCutback()
{
    ((CMainFrame*)m_pMainWnd)->m_ChildView.Cutback();
}

void Cjkl6plusApp::OnAppOpen()
{
    if (((CMainFrame*)m_pMainWnd)->m_ChildView.HasCGSChanged())
    {
        if (AfxMessageBox("Do you want to save current change?", MB_YESNO) == IDYES)
        {
            OnAppSave();
        }
    }

    CFileDialog fdlg(TRUE, "jkl6");
    if (fdlg.DoModal() == IDOK)
    {
        ((CMainFrame*)m_pMainWnd)->m_ChildView.LoadCGSFromFile(std::string(fdlg.GetPathName()));
    }
}

void Cjkl6plusApp::OnAppSave()
{
    std::string strFileName = ((CMainFrame*)m_pMainWnd)->m_ChildView.GetFileName();
    if (strFileName.length() >= 4)
    {
        ((CMainFrame*)m_pMainWnd)->m_ChildView.SaveCGSIntoFile(strFileName);
        AfxMessageBox("Saved.");
    }
    else // invalid strFileName.
    {
        OnAppSaveAs();
    }
}

void Cjkl6plusApp::OnAppSaveAs()
{
    CFileDialog fdlg(FALSE, "jkl6");
    if (fdlg.DoModal() == IDOK)
    {
        ((CMainFrame*)m_pMainWnd)->m_ChildView.SaveCGSIntoFile(std::string(fdlg.GetPathName()));
    }
}

void Cjkl6plusApp::OnAppExit()
{
    if (((CMainFrame*)m_pMainWnd)->m_ChildView.HasCGSChanged())
    {
        if (AfxMessageBox("Do you want to save current change?", MB_YESNO) == IDYES)
        {
            OnAppSave();
        }
    }

    CWinApp::OnAppExit();
}