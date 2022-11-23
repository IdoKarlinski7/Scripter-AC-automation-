#pragma once


#define ARDUINO_PORT_BUFFER_SIZE 255
#define ARDUINO_WAIT_TIME 2000


class CControlUnit
{
public:
    // constructor / destructor
    CControlUnit();
    ~CControlUnit();

    bool Connect(); // connect to the physical control unit
    bool Connect(int iPortNum); // connect to the physical control unit

    void Disconnect();

    bool SetPortNum(int iPortNum);
    int GetPortNum() { return(m_iPortNum); };
    CString GetPortString() { return(m_csPortNum); };
    bool OpenPort();
    bool ClosePort();
    bool UpdateData();
    bool IsPortOpen() { return(m_bPortOpen); };
    bool GetPortHandle();    // open the port and get the port handle
    bool SetupOpenedPort();     // setup the port
        
    int m_iPortNum = -1; // the control unit port number
    CString m_csPortNum = L""; // the control unit port string
    bool m_bPortOpen = false; // true if the port is open
    HANDLE m_hndPort = NULL; // port handle
    CString m_cstrLastError = L""; // last communicaiton error

    void SendString(CString csText); // Sending a string to the control unit
    CString GetString(); // Getting a string from the control unit

    void SetMessagesListBox(CListBox* pcListBox) { m_pcListBox = pcListBox; };
    CListBox* m_pcListBox = NULL;

    // threading and syncronization
    void StartControlUnit(); // create the thread and 
    CArray<CString, CString> m_arrycsCommands;
    CArray<CString, CString> m_arrycsResponses;
    CArray<CString, CString> m_arrycsMessages;
    CMutex m_mtxCommands;
    CMutex m_mtxResponses;
    CMutex m_mtxMessages;

    bool m_bTryingtoConnect = false;
    bool m_bStayingAlive = false;
    CWinThread* m_pcwinthreadCommandControlUnit = NULL;
    CWinThread* m_pcwinthreadResponseControlUnit = NULL;

    bool SendStringInternal(CString csText); // Sending a string to the control unit from the internal thread
    CString GetStringInternal(); // Getting a string from the control unit from the internal thread
    void SplitString(CString csInString, CArray<CString, CString>* parrycsSplitString); // spliting the csInString into the CArray

    // sonication related methods and variables
    bool Sonicate();
    bool Sonicate(int iDuration, int iEnergy, double fFocalDepth);
    void SetDuration(int iDuration) { m_iDuration = iDuration; };
    int GetDuration() { return(m_iDuration); }
    void SetEnergy(int iEnergy) { m_iEnergy = iEnergy; };
    int GetEnergy() { return(m_iEnergy); }
    void SetPower(double fPower) { m_fPower = fPower; };
    double GetPower() { return(m_fPower); }
    void SetFocalDepth(double fFocalDepth) { m_fFocalDepth = fFocalDepth; };
    double GetFocalDepth() { return(m_fFocalDepth); }
    int m_iDuration = 0;
    int m_iEnergy = 0;
    double m_fPower = 0.0;
    double m_fFocalDepth = 0.0;

    // called to update internal active parameter based on control unit ones
    bool UpdateActiveSonicationParameters();
    int GetActiveDuration() { return(m_iActiveDuration); }
    int GetActiveEnergy() { return(m_iActiveEnergy); }
    double GetActivePower() { return(m_fActivePower); }
    int m_iActiveDuration = 0;
    int m_iActiveEnergy = 0;
    double m_fActivePower = 0;

    // safe write of message to the imaging workstation screen
    void WriteMessage(CString csMsg);
    // flush all the message to the control unit
    void FlushMessages();

    // log the sent string
    void LogString(CString csCommand);
};

