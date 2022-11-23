
// ScripterDlg.h : header file
//

#pragma once

#include "CControlUnit.h"
#include "CVirtualScale.h"


// CScripterDlg dialog
class CScripterDlg : public CDialogEx
{
// Construction
public:
	CScripterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CArray <CString, CString> m_strarryScript;
	CControlUnit m_cControlUnit;
	CVirtualScale m_cScale;
	int m_iAutoSonicationDuration = 10;
	int m_iAutoSonicationRepetition = 1;
	int m_iAutoSonicationPause = 3;
	enum AutoSonicationStatus {e10w, e15w, e20w, e25w, e30w, e35w, e40w, eOff, eStart};
	AutoSonicationStatus m_eAutoSonicationStatus = eOff;
	void AutoSonicationLoop();
	void StopSonicationLoop();


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonSendCommand();
	CListBox m_clistResponse;
	CListBox m_clistCommands;
	CEdit m_ceditCommand;
	afx_msg void OnBnClickedButtonConnect();
	CComboBox m_ccomboPort;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox m_clistMessages;
	CEdit m_ceditTiming;
	afx_msg void OnBnClickedButtonStop();
	CButton m_checkLoop;
	afx_msg void OnBnClickedButtonGetCalibration();
	CEdit m_ceditCalibration;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_checkShowScale;
	afx_msg void OnBnClickedButtonSonicOff();
	afx_msg void OnBnClickedButtonSonic10();
	afx_msg void OnBnClickedButtonSonic15();
	afx_msg void OnBnClickedButtonSonic20();
	afx_msg void OnBnClickedButtonSonic25();
	afx_msg void OnBnClickedButtonSonic30();
	afx_msg void OnBnClickedButtonSonic35();
	afx_msg void OnBnClickedButtonSonic40();
	afx_msg void OnBnClickedButtonSonicAll();
	CComboBox m_comboDuration;
	CComboBox m_comboRepetition;
	CComboBox m_comboPause;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
