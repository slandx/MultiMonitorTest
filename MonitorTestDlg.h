// MonitorTestDlg.h : header file
//

#pragma once

class CMonitor;

// CMonitorTestDlg dialog
class CMonitorTestDlg : public CDialog
{
// Construction
public:
	CMonitorTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MONITORTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();

private:
	void FlashMonitor( CMonitor& monitor, bool WorkSpace = false );
	void FlashRect(CRect& rect );
public:
	afx_msg void OnBnClickedButtonFlashAll();
};
