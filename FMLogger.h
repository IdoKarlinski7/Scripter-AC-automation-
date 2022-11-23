#pragma once



#define DEFULT_LOG CFMLogger cfmLogger(CString(__FUNCTION__));
#define LOG2FILE(thestring) cfmLogger.LogToFile(thestring);	

#define LOG4DEBUG

#ifdef LOG4DEBUG
#define DEBUGLOG2FILE(thestring) cfmLogger.LogToFile(thestring);	
#define DEBUGDEFULT_LOG CFMLogger cfmLogger(CString(__FUNCTION__));
#else
#define DEBUGDEFULT_LOG ;
#define DEBUGLOG2FILE(thestring) ;	
#endif



/*
static ULONGLONG ullBase = 0;
static ULONGLONG ullCurrent = 0;
static ULONGLONG ullDelta = 0;

#define TIMESTAMP ullCurrent = gettickcou
*/

class CFMLogger
{
public:

	// methods
	CFMLogger(); // default constructor - should never be used
	CFMLogger(CString csMethodName);	// constructor for function without parameters
	CFMLogger(CString csMethodName, CString csMethodParameters); // constructor for function with parameters

	~CFMLogger(); // defult destructor

protected:
	// methods
	void LogMethodOpen(); // log to file the method related data on entering method
	void LogMethodClose();// log to file the method name on exiting method

public:
	// static methods
	static void SetFilePath(CString csFilePath){ m_csFilePath = csFilePath; };  
	static CString GetFilePath(){ return (m_csFilePath); };
	static void SetDebugMode(bool bDebug) { m_bDebug = bDebug; };
	static bool GetDebugMode() { return (m_bDebug); };
	static void LogToFile(CString csLine, bool bAlways = true);
	static CString GenerateNewLogFileName(CString csFolder);

protected:
	// static methods
	static void SetFileOpen(bool bFileOpen) { m_bFileOpen = bFileOpen; };
	static bool GetFileOpen() { return (m_bFileOpen); };
	static void OpenFile();
	static void CloseFile();
	
protected:
	// static members
	static CString m_csFilePath;		// string to the log file path
	static CStdioFile m_stdioLogFile;	// the handle to the file
	static bool m_bFileOpen;			// true if file is open
	static bool m_bDebug;				// true if we are in debug mode

	// members
	CString m_csMethodName;				// the name of the method logging is done in
	CString m_csMethodParameters;		// the parameters of the method logging is done in
	ULONGLONG m_ullStartTicks = 0;
	int m_iDelta = 0;
};

