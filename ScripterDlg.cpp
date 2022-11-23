
// ScripterDlg.cpp : implementation file
//

#include <stdio.h>
#include "pch.h"
#include "framework.h"
#include "Scripter.h"
#include "ScripterDlg.h"
#include "afxdialogex.h"
#include "FMLogger.h"
#include <mmsystem.h> // for the play sound function


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScripterDlg dialog



CScripterDlg::CScripterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCRIPTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScripterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESPONSE, m_clistResponse);
	DDX_Control(pDX, IDC_LIST_COMMANDS, m_clistCommands);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_ceditCommand);
	DDX_Control(pDX, IDC_COMBO_PORT, m_ccomboPort);
	DDX_Control(pDX, IDC_LIST_MESSAGES, m_clistMessages);
	DDX_Control(pDX, IDC_EDIT_TIME, m_ceditTiming);
	DDX_Control(pDX, IDC_CHECK1, m_checkLoop);
	DDX_Control(pDX, IDC_EDIT_CALIBRATION, m_ceditCalibration);
	DDX_Control(pDX, IDC_CHECK_SCALE, m_checkShowScale);
	DDX_Control(pDX, IDC_COMBO_DURATION, m_comboDuration);
	DDX_Control(pDX, IDC_COMBO_REPETITION, m_comboRepetition);
	DDX_Control(pDX, IDC_COMBO_PAUSE, m_comboPause);
}

BEGIN_MESSAGE_MAP(CScripterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CScripterDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CScripterDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SEND_COMMAND, &CScripterDlg::OnBnClickedButtonSendCommand)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CScripterDlg::OnBnClickedButtonConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CScripterDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_GET_CALIBRATION, &CScripterDlg::OnBnClickedButtonGetCalibration)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_OFF, &CScripterDlg::OnBnClickedButtonSonicOff)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_10, &CScripterDlg::OnBnClickedButtonSonic10)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_15, &CScripterDlg::OnBnClickedButtonSonic15)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_20, &CScripterDlg::OnBnClickedButtonSonic20)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_25, &CScripterDlg::OnBnClickedButtonSonic25)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_30, &CScripterDlg::OnBnClickedButtonSonic30)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_35, &CScripterDlg::OnBnClickedButtonSonic35)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_40, &CScripterDlg::OnBnClickedButtonSonic40)
	ON_BN_CLICKED(IDC_BUTTON_SONIC_ALL, &CScripterDlg::OnBnClickedButtonSonicAll)
	ON_BN_CLICKED(IDOK, &CScripterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CScripterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CScripterDlg message handlers

BOOL CScripterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ceditTiming.SetWindowTextW(L"10");
	m_ceditCalibration.SetWindowTextW(L"850");
	m_comboDuration.SetCurSel(0);
	m_comboRepetition.SetCurSel(0);
	m_comboPause.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScripterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScripterDlg::OnPaint()
{
	if (IsIconic())
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScripterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScripterDlg::OnBnClickedButtonLoad()
{
	DEFULT_LOG;
	// Create an instance First
	CFileDialog cfOpenDlg(TRUE);

	// Initializes m_pOFN structure
	cfOpenDlg.m_pOFN->lpstrTitle = L"Load Script";

	// variable to store file path
	CString csFilePath;
	
	// Call DoModal
	if (cfOpenDlg.DoModal() == IDOK)
	{
		OutputDebugString(L"File open\n");
		csFilePath = cfOpenDlg.GetPathName();
	}
	else
	{
		OutputDebugString(L"Failed to load file\n");
		AfxMessageBox(L"Failed to load file\n");
		return;
	}

	// load all the commands into the listbox and array
	CString cstrLine;
	CStdioFile stdfileScript;
	BOOL bRes = stdfileScript.Open(csFilePath, CFile::modeRead | CFile::typeText);
	if(bRes)
		bRes = stdfileScript.ReadString(cstrLine);
	while (bRes)
	{
		m_strarryScript.Add(cstrLine);
		m_clistCommands.AddString(cstrLine.GetString());
		bRes = stdfileScript.ReadString(cstrLine);
	}

	stdfileScript.Close();
		
	int iLines = (int)(m_strarryScript.GetSize());
	CString csTool;
	csTool.Format(L"Loaded %d Lines from %s", iLines, csFilePath.GetString());
	AfxMessageBox(csTool, MB_ICONINFORMATION);
}


void CScripterDlg::OnBnClickedButtonRun()
{
	CString csTiming;
	m_ceditTiming.GetWindowTextW(csTiming);
	int iMiliseconds = _wtoi(csTiming);

	// starting script timer
	int iLines = (int)(m_strarryScript.GetSize());
	if (iLines)
		SetTimer(SCRIPT_TIMER, iMiliseconds, NULL);
	else
		AfxMessageBox(L"No loaded commands");
}


void CScripterDlg::OnBnClickedButtonSendCommand()
{
	if (m_cControlUnit.IsPortOpen())
	{
		// get the command string
		CString csTool;
		m_ceditCommand.GetWindowTextW(csTool);
		
		// if there is a string, send it to the control unit
		if(csTool.GetLength()) 
			m_cControlUnit.SendString(csTool);
	}
	else
		AfxMessageBox(L"Not connected to control unit");
}


void CScripterDlg::OnBnClickedButtonConnect()
{
	CString csTool;
	m_ccomboPort.GetWindowTextW(csTool);
	int iPort = _wtoi(csTool);

	// make sure it is more than 0 and than connect
	if (iPort)
	{
		m_cControlUnit.SetMessagesListBox(&m_clistMessages);
		m_cControlUnit.SetPortNum(iPort);
		m_cControlUnit.StartControlUnit();
		SetTimer(CONTROL_UNIT_TIMER, 100, NULL);
	}
	else
		AfxMessageBox(L"Illegal port number");
}


void CScripterDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int iCommandIndex = 0;
	static int iTime = 2;

	if (nIDEvent == CONTROL_UNIT_TIMER)
	{
		// get all the messages and write them to the screen
		m_cControlUnit.FlushMessages();
		// loop on all the responses and either act on this or write them to the screen
		bool bGotSomething = false;
		CString csResponse = m_cControlUnit.GetString();
		while (!csResponse.IsEmpty())
		{
			bGotSomething = true;
			m_clistResponse.AddString(csResponse);
			csResponse = m_cControlUnit.GetString();
		}

		if (bGotSomething)
		{
			int iMin = 0;
			int iMax = 0;
			m_clistResponse.GetScrollRange(SB_VERT, &iMin, &iMax);
			m_clistResponse.SetScrollPos(SB_VERT, iMax, TRUE);
			m_clistResponse.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
			m_clistResponse.UpdateWindow();
		}
	}

	if (nIDEvent == SCRIPT_TIMER)
	{
		// if timer settings were changed, mostly handles sleep command
		if (iTime != 2) // ((iCommandIndex > 0) && (m_strarryScript[iCommandIndex - 1].Left(6) == L"Sleep="))
		{
			iTime = 2;
			KillTimer(SCRIPT_TIMER);
			SetTimer(SCRIPT_TIMER, iTime, NULL);
		}
		int iCommands = (int)(m_strarryScript.GetSize());
		if (iCommandIndex < iCommands)
		{
			CString csCommand = m_strarryScript[iCommandIndex];
			// check if this is a comment
			if ((csCommand.GetLength() > 2) && (csCommand.Left(2) == L"//"))
			{
				// this is a comment, do nothing
			}
			else
			{
				if ((csCommand.GetLength() > 6) && (csCommand.Left(6) == L"Sleep="))
				{
					int iDurationStart = csCommand.GetLength() - 6;
					iTime = _wtoi(csCommand.Right(iDurationStart));
					if (iTime > 0)
					{
						KillTimer(SCRIPT_TIMER);
						SetTimer(SCRIPT_TIMER, iTime, NULL);
					}
				}
				else if ((csCommand.GetLength() > 4) && (csCommand.Left(4) == L"Log:"))
				{
					int iCommandStart = csCommand.GetLength() - 4;
					CFMLogger::LogToFile(csCommand.Right(iCommandStart));
				}
				else
				{
					if ((csCommand.GetLength() > 4) && (csCommand.Left(2) == L"RF"))
					{
						int iRequestedAcouticPower = _wtoi(csCommand.Right(2));
						CString csCalibration;
						m_ceditCalibration.GetWindowTextW(csCalibration);
						int iCalibration = _wtoi(csCalibration);
						int iElectricalPower = iRequestedAcouticPower * 1000 / iCalibration;
						CString csCorrectedString;
						csCorrectedString.Format(L"%s=%d", csCommand.Left(3), iElectricalPower);
						m_cControlUnit.SendString(csCorrectedString);
					}
					else
					{
						// just send the string as is
						m_cControlUnit.SendString(csCommand);

					}
				}
			}
			iCommandIndex++;
		}
		else
		{	// reset for next loop
			iCommandIndex = 0;
			// if looped is checked, do nothing and second loop would start, otherwise kill the timer
			if(!m_checkLoop.GetCheck())
				KillTimer(SCRIPT_TIMER);
			
		}
	}

	if (nIDEvent == RFB_TIMER)
	{
		AutoSonicationLoop();
	}
		

	CDialogEx::OnTimer(nIDEvent);
}

void CScripterDlg::StopSonicationLoop()
{
	// kill the timer
	KillTimer(RFB_TIMER);
	// time to stop the loop
	PlaySound(L"c:\\FUSMobile\\Scripter\\AutoDone.wav", NULL, SND_ASYNC);
	Sleep(1000);
	PlaySound(NULL, NULL, SND_SYNC);
}

void CScripterDlg::AutoSonicationLoop()
{
	static bool bPause = false;
	static bool bDirectionUp = true;

	int iPauseDuration = m_iAutoSonicationPause;

	switch (m_eAutoSonicationStatus)
	{

	case CScripterDlg::eStart:
		KillTimer(RFB_TIMER);
		if (bDirectionUp)
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			OnBnClickedButtonSonic10();
			m_eAutoSonicationStatus = e10w;
		}
		else
		{
			bDirectionUp = true;
			bPause = false;
			m_iAutoSonicationRepetition--;
			if (m_iAutoSonicationRepetition < 1)
			{ // time to stop the loop
				StopSonicationLoop();
			}
			else
			{
				SetTimer(RFB_TIMER, m_iAutoSonicationPause, NULL);
			}
		}
		break;

	case CScripterDlg::e10w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic15();
				m_eAutoSonicationStatus = e15w;
			}
			else
			{
				OnBnClickedButtonSonicOff();
				m_eAutoSonicationStatus = eStart;
			}
		}
		break;

	case CScripterDlg::e15w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic20();
				m_eAutoSonicationStatus = e20w;
			}
			else
			{
				OnBnClickedButtonSonic10();
				m_eAutoSonicationStatus = e10w;
			}
		}
		break;

	case CScripterDlg::e20w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic25();
				m_eAutoSonicationStatus = e25w;
			}
			else
			{
				OnBnClickedButtonSonic15();
				m_eAutoSonicationStatus = e15w;
			}
		}
		break;

	case CScripterDlg::e25w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic30();
				m_eAutoSonicationStatus = e30w;
			}
			else
			{
				OnBnClickedButtonSonic20();
				m_eAutoSonicationStatus = e20w;
			}
		}
		break;

	case CScripterDlg::e30w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic35();
				m_eAutoSonicationStatus = e35w;
			}
			else
			{
				OnBnClickedButtonSonic25();
				m_eAutoSonicationStatus = e25w;
			}
		}
		break;

	case CScripterDlg::e35w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonic40();
				m_eAutoSonicationStatus = e40w;
			}
			else
			{
				OnBnClickedButtonSonic30();
				m_eAutoSonicationStatus = e30w;
			}
		}
		break;

	case CScripterDlg::e40w:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			OnBnClickedButtonSonicOff();
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				OnBnClickedButtonSonicOff();
				m_eAutoSonicationStatus = eOff;
			}
			else
			{
				OnBnClickedButtonSonic35();
				m_eAutoSonicationStatus = e35w;
			}
		}
		break;
	case CScripterDlg::eOff:
		KillTimer(RFB_TIMER);
		if (bPause)
		{
			SetTimer(RFB_TIMER, iPauseDuration, NULL);
			bPause = false;
		}
		else
		{
			SetTimer(RFB_TIMER, m_iAutoSonicationDuration, NULL);
			bPause = true;
			if (bDirectionUp)
			{
				m_iAutoSonicationRepetition--;
				if (m_iAutoSonicationRepetition < 1)
				{ // time to stop the loop
					StopSonicationLoop();
				}
				else
				{
					OnBnClickedButtonSonic40();
					m_eAutoSonicationStatus = e40w;
					bDirectionUp = false;
				}
			}
		}
		break;
	default:
		break;
	}

	// auto scroll the text on the responses list box
	int iMin = 0;
	int iMax = 0;
	m_clistResponse.GetScrollRange(SB_VERT, &iMin, &iMax);
	m_clistResponse.SetScrollPos(SB_VERT, iMax, TRUE);
	m_clistResponse.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
	m_clistResponse.UpdateWindow();
}

void CScripterDlg::OnBnClickedButtonStop()
{
	if (m_cControlUnit.IsPortOpen())
	{
		m_cControlUnit.SendString(L"D");
	}
	else
		AfxMessageBox(L"Not connected to control unit");

	KillTimer(SCRIPT_TIMER);
}


void CScripterDlg::OnBnClickedButtonGetCalibration()
{
	if (m_cControlUnit.IsPortOpen())
	{
		// check calibration value
		m_cControlUnit.SendString(L"CAL?");

		// wait one 1 second
		Sleep(1000);

		// loop on all the responses 
		bool bGotSomething = false;
		CString csFirstResponse = m_cControlUnit.GetString();
		CString csResponse = csFirstResponse;
		while (!csResponse.IsEmpty())
		{
			bGotSomething = true;
			m_clistResponse.AddString(csResponse);
			csResponse = m_cControlUnit.GetString();
		}

		// use the last response (CALD=XYZ) as calibration value
		CString csCalibration = csFirstResponse.Right(3);
		m_ceditCalibration.SetWindowTextW(csCalibration);
	}
	else
		AfxMessageBox(L"Not connected to control unit");
}


BOOL CScripterDlg::PreTranslateMessage(MSG* pMsg)
{
	// a local string to store inputs from the scale
	static CString csKeys;
		
	if (pMsg->message == WM_KEYDOWN)
	{
		// get key code
		WPARAM wKeyCode = pMsg->wParam;
		//CString csForDisplay; csForDisplay.Format(L"Got from scale %d\n", wKeyCode);
		//m_clistResponse.AddString(csForDisplay);
		
		if (wKeyCode == 71 || wKeyCode == 103) // g or G was sent
		{
			// check if string is long enough
			int iLength = csKeys.GetLength();
			//send commend to screen
			// check is last key was i or G
			if (iLength > 7)
			{
				//m_clistResponse.AddString(L"Length more then 7");

				if (csKeys.Left(3) == L"SUI" || csKeys.Left(3) == L"sui")
				{
					// create and add the time stamp (%d abriviated month, %b day of the month, %Y Year with century, %H Hour in 24-hour, %M Minute, %S Second)
					CTime tCurrentTime = CTime::GetCurrentTime();
					CString csTime = tCurrentTime.Format(L"%H:%M:%S, ");
					// create the measurment
					CString csTool = csKeys.Mid(3, iLength - 3);
					double fVal = _wtof(csTool);
					csTool; csTool.Format(L"%3.3f", fVal);
					// combine both strings and write to file and screen
					CString csMessage = csTime + csTool;
					m_cScale.WriteMeasurment(csMessage);
					if (m_checkShowScale.GetCheck())
						m_clistResponse.AddString(csMessage);
				}
				csKeys.Empty(); // clean screen no matter what
		
				return(true); // kill all keystrokes
			}
		}

		if (wKeyCode > 47 && wKeyCode < 58) // a number, add it to the string
		{
			//m_clistResponse.AddString(L"Got number");
			char cKey = (char)wKeyCode;
			CString csTool; csTool = cKey;
			csKeys += csTool;
		}

		if (wKeyCode > 64 && wKeyCode < 91) // an upper case letter, add it to the string
		{
			//m_clistResponse.AddString(L"Got upper case");
			char cKey = (char)wKeyCode;
			CString csTool; csTool = cKey;
			csKeys += csTool;
		}

		if (wKeyCode > 96 && wKeyCode < 123) // a lower case letter, add it to the string
		{
			//m_clistResponse.AddString(L"Got lower case");
			char cKey = (char)wKeyCode;
			CString csTool; csTool = cKey;
			csKeys += csTool;
		}

		if (wKeyCode == 189) // a minus sign, add to the string
			csKeys += L"-";
		
		if (wKeyCode == 190) // a decimal point, add to the string
			csKeys += L".";

		// kill all keystrokes
		return(true);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CScripterDlg::OnBnClickedButtonSonicOff()
{
	OutputDebugString(L"Off\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,OFF");
}


void CScripterDlg::OnBnClickedButtonSonic10()
{
	OutputDebugString(L"10\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,10");
}


void CScripterDlg::OnBnClickedButtonSonic15()
{
	OutputDebugString(L"15\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,15");
}


void CScripterDlg::OnBnClickedButtonSonic20()
{
	OutputDebugString(L"20\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,20");
}


void CScripterDlg::OnBnClickedButtonSonic25()
{
	OutputDebugString(L"25\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,25");
}


void CScripterDlg::OnBnClickedButtonSonic30()
{
	OutputDebugString(L"30\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,30");
}


void CScripterDlg::OnBnClickedButtonSonic35()
{
	OutputDebugString(L"35\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,35");
}


void CScripterDlg::OnBnClickedButtonSonic40()
{
	OutputDebugString(L"40\n");
	if (m_cControlUnit.IsPortOpen())
		m_cControlUnit.SendString(L"SONIC,ON,40");
}


void CScripterDlg::OnBnClickedButtonSonicAll()
{
	// get requested duration and repitition
	CString csTool;
	m_comboDuration.GetWindowTextW(csTool);
	m_iAutoSonicationDuration = _wtoi(csTool) *1000;

	m_comboRepetition.GetWindowTextW(csTool);
	m_iAutoSonicationRepetition = _wtoi(csTool);

	m_comboPause.GetWindowTextW(csTool);
	m_iAutoSonicationPause = _wtoi(csTool) *1000;

	if (m_iAutoSonicationDuration < 1 || m_iAutoSonicationRepetition < 1 || m_iAutoSonicationPause < 1)
	{
		AfxMessageBox(L"Duration, Pause or Repitition are not set");
		return;
	}

	csTool.Format(L"Duration: %d, Pause: %d, Repitition: %d\n", m_iAutoSonicationDuration, m_iAutoSonicationPause, m_iAutoSonicationRepetition);
	OutputDebugString(csTool);

	// start the autosonication timer
	SetTimer(RFB_TIMER, 100, NULL);
	m_eAutoSonicationStatus = eStart;
}


void CScripterDlg::OnBnClickedOk()
{
	OnBnClickedButtonSonicOff();
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CScripterDlg::OnBnClickedCancel()
{
	OnBnClickedButtonSonicOff();
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
