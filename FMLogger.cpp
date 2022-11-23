#include "pch.h"
#include "FMLogger.h"


// creation of the static variables
CString CFMLogger::m_csFilePath = L"NULL";		// string to the log file path
CStdioFile CFMLogger::m_stdioLogFile;			// the handle to the file
bool CFMLogger::m_bFileOpen = false;			// true if file is open
bool CFMLogger::m_bDebug = true;				// true if we are in debug mode


// default constructor - should never be used
CFMLogger::CFMLogger()
{
	m_csMethodName = L"No Method";
	m_csMethodParameters.Empty();
	LogMethodOpen();
}


// constructor to be used in methods without parameters
CFMLogger::CFMLogger(CString csMethodName)
{
	m_csMethodName = csMethodName;
	m_csMethodParameters = L"";
	LogMethodOpen();
}

// constructor to be used in methods with parameters
CFMLogger::CFMLogger(CString csMethodName, CString csMethodParameters)
{
	m_csMethodName = csMethodName;
	m_csMethodParameters = csMethodParameters;
	LogMethodOpen();
}

// default destructor
CFMLogger::~CFMLogger()
{
	LogMethodClose();
}


// Generate new log file name file - static function.
CString CFMLogger::GenerateNewLogFileName(CString csFolder)
{
	CTime tCurrentTime = CTime::GetCurrentTime();   
	// %d abriviated month, %b day of the month, %Y Year with century, %H Hour in 24-hour, %M Minute, %S Second
	CString csDateAndTime = tCurrentTime.Format(L"%d%b%Y%H%M%S"); 
    
    CString csFilepath = csFolder + csDateAndTime + L".log";
	return (csFilepath);
}

// open file - static function.
// all other functions would call this one and not access file directly
// to speed things up error handling is internal with ASSERT and no value is returned
void CFMLogger::OpenFile()
{

	// do nothing if file is already open
	if (m_bFileOpen)
		return;

	// try to open and ASSERT on failure
	BOOL bRes = m_stdioLogFile.Open(m_csFilePath, CFile::modeCreate | CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);

    m_stdioLogFile.SeekToEnd();
	if (bRes)
		m_bFileOpen = true;
	else
		ASSERT(FALSE);
}

// close file - static function.
// all other functions would call this one and not access file directly
void CFMLogger::CloseFile()
{
	// do nothing if file is already closed
	if (!m_bFileOpen)
		return;

	// Close file
	m_stdioLogFile.Close();
	m_bFileOpen = false;
}


// Log string to file - static function.
// all other functions would call this one and not access file directly
void CFMLogger::LogToFile(CString csLine, bool bAlways)
{
	if (bAlways)
	{
		OpenFile();
		CTime tCurrentTime = CTime::GetCurrentTime();
		// %d abriviated month, %b day of the month, %Y Year with century, %H Hour in 24-hour, %M Minute, %S Second
		CString csTime = tCurrentTime.Format(L"%H:%M:%S: ");
		m_stdioLogFile.WriteString(csTime + csLine + L"\n");
		CloseFile();
	}
	else
	{
		if (m_bDebug)
		{
			OpenFile();
			m_stdioLogFile.WriteString(csLine);
			CloseFile();
		}
	}
    // make sure it is written to the file
    m_stdioLogFile.Flush();
}


// Log method and parameters strings to file when entring a function.
void CFMLogger::LogMethodOpen()
{
	// tick counts when method started
	m_ullStartTicks = GetTickCount64();

	if (m_csMethodParameters.IsEmpty())
	{
		CString csLine = L"In: " + m_csMethodName;
		LogToFile(csLine, true);
	}
	else
	{
		CString csLine = L"In: " + m_csMethodName + L", Param: " + m_csMethodParameters;
		LogToFile(csLine, true);
	}
}



// Log method string when Exiting a function.
void CFMLogger::LogMethodClose()
{
	CString csLine = L"Out: " + m_csMethodName;

	if (m_bDebug)
	{
		ULONGLONG ullCurrent = GetTickCount64(); // current tick counts
		int iDelta = (int)(ullCurrent - m_ullStartTicks); // should not overflow
		CString csTool; csTool.Format(L", Ticks: (%4d)", iDelta);
		csLine += csTool;
	}

	LogToFile(csLine, true);
}
