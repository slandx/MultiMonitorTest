// MonitorTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonitorTest.h"
#include "MonitorTestDlg.h"
#include "Monitors.h"
#include "MultiMonitor.h"
#include "MonitorDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorTestDlg dialog

//How to Exploit Multiple Monitor Support in Memphis and Windows NT 5.0
//David Campbell
//http://www.microsoft.com/msj/defaultframe.asp?page=/msj/0697/monitor/monitor.htm&nav=/msj/0697/newnav.htm


CMonitorTestDlg::CMonitorTestDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CMonitorTestDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonitorTestDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorTestDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON_FLASH_ALL, OnBnClickedButtonFlashAll)
END_MESSAGE_MAP()


// CMonitorTestDlg message handlers

BOOL CMonitorTestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    CString s;
    s.Format(L"This machine has %d monitor(s) attached right now", CMonitors::GetMonitorCount());
    
    GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(s);
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMonitorTestDlg::OnPaint()
{
    if(IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

void CMonitorTestDlg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    
    CWnd *pWnd = GetDlgItem(IDC_BUTTON5);
    
    if(!pWnd)
    {
        return;
    }
    
    CRect rect;
    pWnd->GetWindowRect(&rect);
    GetDlgItem(IDC_STATIC_ONSCREEN)->SetWindowText(CMonitors::IsOnScreen(&rect) ? L"yes" : L"no");
    
    CMonitor monitor = CMonitors::GetNearestMonitor(this);
    GetDlgItem(IDC_STATIC_SAME_MONITOR)->SetWindowText(monitor.IsOnMonitor(&rect) ? L"yes" : L"no");
    
    CString s;
    monitor.GetName(s);
    
    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(s);
    GetDlgItem(IDC_STATIC_PRIMARY)->SetWindowText(monitor.IsPrimaryMonitor() ? L"yes" : L"no");
    
    s.Format(L"%d", monitor.GetBitsPerPixel());
    GetDlgItem(IDC_STATIC_BPP)->SetWindowText(s);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMonitorTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//these methods all use the dialog window as the source
//of the monitor handle - the same concepts apply using
//points or rectangles
void CMonitorTestDlg::OnBnClickedButton1()
{
    CMonitor monitor = CMonitors::GetNearestMonitor(this);
    FlashMonitor(monitor);
}


void CMonitorTestDlg::OnBnClickedButton2()
{
    CRect rect;
    CMonitors::GetVirtualDesktopRect(&rect);
    
    FlashRect(rect);
}

void CMonitorTestDlg::OnBnClickedButton3()
{
    CMonitor monitor = CMonitors::GetNearestMonitor(this);
    FlashMonitor(monitor, true);
}


void CMonitorTestDlg::OnBnClickedButton4()
{
    CMonitor monitor = CMonitors::GetNearestMonitor(this);
    monitor.CenterWindowToMonitor(this);
}


void CMonitorTestDlg::OnBnClickedButton6()
{
    CMonitor monitor = CMonitors::GetPrimaryMonitor();
    FlashMonitor(monitor);
}

void CMonitorTestDlg::OnBnClickedButtonFlashAll()
{
    CMonitor monitor;
    CMonitors monitors;
    
    for(int i = 0; i < monitors.GetCount(); i++)
    {
        monitor = monitors.GetMonitor(i);
        FlashMonitor(monitor);
        Sleep(500);
    }
}

void CMonitorTestDlg::FlashMonitor(CMonitor &monitor, bool WorkSpace)
{
    CRect rect;
    
    if(WorkSpace)
        //get the work area rect of the monitor this window is on
    {
        monitor.GetWorkAreaRect(&rect);
    }
    else
    {
        monitor.GetMonitorRect(&rect);
    }
    
    CMonitorDC dc(&monitor);
    dc.Rectangle(&rect);
    
    Sleep(1000);
    
    ::InvalidateRect(NULL, &rect, TRUE);
}

void CMonitorTestDlg::FlashRect(CRect &rect)
{
    //paint the entire rect
    HDC hDC = ::GetDC(NULL);
    ::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
    ::ReleaseDC(NULL, hDC);
    
    //pause briefly
    Sleep(1000);
    
    //then redraw the rect
    ::InvalidateRect(NULL, &rect, TRUE);
}
