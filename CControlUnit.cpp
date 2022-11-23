#include "pch.h"
#include "CControlUnit.h"


// constructor / destructor
CControlUnit::CControlUnit()
{
}

CControlUnit::~CControlUnit()
{
}



UINT __cdecl ControlUnitCommandThreadFunction(LPVOID pParam)
{
    static CString csCommand;
    // convert the point to the control unit class
    CControlUnit* pControlUnit = (CControlUnit*)pParam;

    pControlUnit->Connect();

    while (pControlUnit->m_bStayingAlive)
    {
        pControlUnit->m_mtxCommands.Lock();
        int iSize = (int)(pControlUnit->m_arrycsCommands.GetSize());
        if (iSize) // there are message strings waiting to be written
        {
            csCommand = pControlUnit->m_arrycsCommands[0];
            pControlUnit->m_arrycsCommands.RemoveAt(0, 1);
            pControlUnit->m_arrycsCommands.FreeExtra();
        }
        pControlUnit->m_mtxCommands.Unlock();

        // since we got a string, send it to the control unit
        if(iSize)
            pControlUnit->SendStringInternal(csCommand);


        Sleep(50);
    }

    OutputDebugString(L"Exiting Command Thread\n");
    return(1);
}


UINT __cdecl ControlUnitResponseThreadFunction(LPVOID pParam)
{
    static CString csTool; // better make it static instead of keep creating it

    // convert the point to the control unit class
    CControlUnit* pControlUnit = (CControlUnit*)pParam;

    // create an internal CArray to tokenize csTool in case it has more than one line
    CArray<CString, CString> strarrayTool;

    while (pControlUnit->m_bStayingAlive)
    {
        csTool = pControlUnit->GetStringInternal(); // get the string from the port
        pControlUnit->SplitString(csTool, &strarrayTool); // split it into lines per \r\n
        if (strarrayTool.GetSize())
        {
            pControlUnit->m_mtxResponses.Lock();
            pControlUnit->m_arrycsResponses.Append(strarrayTool);
            pControlUnit->m_mtxResponses.Unlock();

            strarrayTool.RemoveAll();
        }

        Sleep(50);
    }

    OutputDebugString(L"Exiting Response Thread\n");
    return(1);
}




// create the thread and 
void CControlUnit::StartControlUnit()
{
    if (m_bTryingtoConnect)
    {
        WriteMessage(L"Already trying to connect");
        return;
    }

    m_bTryingtoConnect = true;
    m_bStayingAlive = true;

    m_pcwinthreadCommandControlUnit = AfxBeginThread(ControlUnitCommandThreadFunction, this);
    m_pcwinthreadResponseControlUnit = AfxBeginThread(ControlUnitResponseThreadFunction, this);
}




// connect to the physical control unit (running in internal thread)
bool CControlUnit::Connect()
{
    bool bRes = OpenPort();
    if (bRes)
    {
        WriteMessage(L"Connected to the Control Unit");
        CString csTool = GetString(); // show version
        WriteMessage(csTool);
    }
    else
    {
        WriteMessage(L"Failed to connect to the Control Unit");
        WriteMessage(m_cstrLastError);
    }
    return(bRes);
}

// connect to the physical control unit (running in internal thread)
bool CControlUnit::Connect(int iPortNum)
{
    bool bRes = SetPortNum( iPortNum );
    if (!bRes)
        return(false);

    bRes = Connect();
    if (bRes)
        WriteMessage(L"Connection Successful");
    else
        WriteMessage(L"Failed to connect");

    return(bRes);
}

void CControlUnit::Disconnect()
{
    if (!m_bStayingAlive)
        return; // no live threads, nothing to do here

    m_bStayingAlive = false; // lower the flag keeping both command and response threads alive
    Sleep(200); // give them 200 miliseconds to finish
    // get the exit code of both threads
    DWORD dwCommandExitCode = 0;
    DWORD dwResponseExitCode = 0;
    bool bResCommand = GetExitCodeThread(m_pcwinthreadCommandControlUnit->m_hThread, &dwCommandExitCode);
    bool bResResponse = GetExitCodeThread(m_pcwinthreadResponseControlUnit->m_hThread, &dwResponseExitCode);
    CString csTool;
    CString csMsg = L"Disconnected.";
    if (bResCommand)
    {
        csTool.Format(L" Command exit code: %d", dwCommandExitCode);
        csMsg += csTool;
    }
    if (bResResponse)
    {
        csTool.Format(L" Response exit code: %d", dwResponseExitCode);
        csMsg += csTool;
    }
    csMsg += L"\n";

    OutputDebugString(csMsg);
    WriteMessage(csMsg);
}

bool CControlUnit::SetPortNum(int iPortNum) //(running in main process)
{
    m_iPortNum = iPortNum;

    // sanity check on port
    if (m_iPortNum < 1 || m_iPortNum > 40)
        return (false);

    // create and store text port number
    CString csTool;
    if (m_iPortNum < 10)
    {
        csTool = L"COM";
    }
    else
    {
        csTool = L"\\\\.\\COM";
    }
    m_csPortNum.Format(L"%s%d", csTool, m_iPortNum);

    return(true);
}


bool CControlUnit::OpenPort() // (running in internal thread)
{
    m_bPortOpen = false;

    m_bPortOpen = GetPortHandle();
    if (m_bPortOpen)
    {
        m_bPortOpen = SetupOpenedPort();
        if (m_bPortOpen)
        {
            //bRes = SendUARTMessage(); //EDIT: May not need this... Based on UARTTest Code...
            return (m_bPortOpen);
        }
    }
    return (m_bPortOpen);
}


bool CControlUnit::ClosePort() // (running in internal thread)
{
    bool bRes = (bool)(CloseHandle(m_hndPort));
    return(bRes);
}

bool CControlUnit::UpdateData()
{
    return(true);
}

bool CControlUnit::GetPortHandle()    // open the port and get the port handle (running in internal thread)
{
    // temp tools
    DWORD dwRes;

    // open existing port
    m_hndPort = CreateFile(m_csPortNum, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hndPort == INVALID_HANDLE_VALUE)
    {
        dwRes = GetLastError();
        CString csErrorNum;
        csErrorNum.Format(L"Error num %d", dwRes);
        //AfxMessageBox(csErrorNum);
        m_cstrLastError = csErrorNum;
        return (false);
    }

    BOOL bRes = SetupComm(m_hndPort, 250, 250);
    if (bRes)
        return(true);
    else
        return(false);
}
bool CControlUnit::SetupOpenedPort()     // setup the port (running in internal thread)
{
    DCB dcbBase;

    // get relevant data from the open port
    BOOL bRes = GetCommState(m_hndPort, &dcbBase);
    if (bRes)
    {
        dcbBase.BaudRate = 9600; //EDIT: Because I noticed an issue with the sample code (Original Value: 115200);
        dcbBase.ByteSize = 8;
        dcbBase.StopBits = ONESTOPBIT;
        dcbBase.fParity = NOPARITY;
        dcbBase.fDtrControl = DTR_CONTROL_ENABLE; 
    }
    else
    {
        //AfxMessageBox(L"Failed to get Port Data");
        m_cstrLastError = L"Failed to get Port Data";
        return (false);
    }

    // Set relevant data from the open port
    bRes = SetCommState(m_hndPort, &dcbBase);
    if (!bRes)
    {
        m_cstrLastError = L"Failed to set Port Data";
        //AfxMessageBox(L"Failed to set Port Data");
        return (false);
    }

    PurgeComm(m_hndPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(ARDUINO_WAIT_TIME);

    // do some house cleaning
    DWORD dwErrorFlags;
    COMSTAT Comstat;
    ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);

    COMMTIMEOUTS comTimeOut;
    comTimeOut.ReadIntervalTimeout = 0;			//EDIT: Allow read operation to just go without checks. Was: 5
    comTimeOut.ReadTotalTimeoutMultiplier = 0;	//EDIT: Allow read operation to just go without checks. Was: 10
    comTimeOut.ReadTotalTimeoutConstant = 70;	//EDIT: Reduce the total timeout time. Was: 100
    comTimeOut.WriteTotalTimeoutMultiplier = 5;
    comTimeOut.WriteTotalTimeoutConstant = 5;
    bRes = SetCommTimeouts(m_hndPort, &comTimeOut);

    if (bRes)
        return (true);
    else
        return (true);
}


void CControlUnit::WriteMessage(CString csMsg) // add a message to the message queue (runs in both internal thread and main process)
{
    m_mtxMessages.Lock();
    m_arrycsMessages.Add(csMsg); // add this message at the end
    m_mtxMessages.Unlock();
}


// send all messages in the message array to the screen and empty the array (runs in main process)
void CControlUnit::FlushMessages()
{
    m_mtxMessages.Lock();
    int iSize = (int)(m_arrycsMessages.GetSize());
    if (iSize) // there are message strings waiting to be written
    {
        for (int i = 0; i < iSize; i++)
        {
            m_pcListBox->AddString(m_arrycsMessages[i]);
        }
        m_arrycsMessages.RemoveAll(); // all messages sent to the screen, empty the array
    }
    m_mtxMessages.Unlock();

    // scroll the list box
    if (iSize) // we did something
    {
        int iMax, iMin;
        m_pcListBox->GetScrollRange(SB_VERT, &iMin, &iMax);
        m_pcListBox->SetScrollPos(SB_VERT, iMax, TRUE);
        m_pcListBox->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
        m_pcListBox->UpdateWindow();
    }
}

// Sending a string to the control unit from the internal thread (runs in thread)
bool CControlUnit::SendStringInternal(CString csText)
{
    //OutputDebugString(L"Sending command: ");
    //OutputDebugString(csText);
    //OutputDebugString(L"\n");

    unsigned int uiLen = csText.GetLength();
    LPWSTR csBuff = csText.GetBuffer();

    char* cNewBuffer = new char[uiLen + 3]; // the +3 is to enable carrige return, new line and terminating 0
    for (unsigned int ui = 0; ui < uiLen; ui++)
    {
        cNewBuffer[ui] = (char)(csBuff[ui]);
    }
    // add the carrige return and terminating zero
    cNewBuffer[uiLen] = '\r';   // carrige return (ASCI 13)
    cNewBuffer[uiLen + 1] = '\n'; // carrige return (ASCI 13)
    cNewBuffer[uiLen + 2] = 0;    // Need this only for debug, not sent to CU

    // clear the server buffer
    PurgeComm(m_hndPort, PURGE_RXCLEAR | PURGE_TXCLEAR);

    // Send the string to the server
    DWORD iR;
    OVERLAPPED stcWriteStatus = { 0 };
    BOOL bRes = WriteFile(m_hndPort, cNewBuffer, uiLen + 1, &iR, &stcWriteStatus);
    if (!bRes)
    {
        DWORD dwErrorFlags;
        COMSTAT Comstat;
        ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
        CString csTool;
        csTool.Format(L"Error num %d", dwErrorFlags);
        WriteMessage(csTool);
    }
    else
    {
        CString csTool;
        csTool.Format(L"Message sent: %s", csText);
        WriteMessage(csTool);
    }

    delete[] cNewBuffer;

    return(bRes);
}


// Sending a string to the control unit (runs in main process)
void CControlUnit::SendString(CString csText)
{
    m_mtxCommands.Lock();
    m_arrycsCommands.Add(csText);
    //OutputDebugString(L"Adding to command array: ");
    //OutputDebugString(csText);
    //OutputDebugString(L"\n");
    m_mtxCommands.Unlock();
    // log the sent screen
    LogString(csText);

}


// log the sent screen
void CControlUnit::LogString(CString csCommand)
{
    // create the file handle and open it
    CStdioFile stdioLogFile;
    BOOL bRes = stdioLogFile.Open(L"c:\\FUSMobile\\Scripter\\Session.log", CFile::modeCreate | CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
    if (bRes)
    {
        // if opened, go to the end of the file
        stdioLogFile.SeekToEnd();

        // write the command string with a time stamp
        CTime tCurrentTime = CTime::GetCurrentTime();
        // %d abriviated month, %b day of the month, %Y Year with century, %H Hour in 24-hour, %M Minute, %S Second
        CString csTime = tCurrentTime.Format(L"%H:%M:%S: ");
        stdioLogFile.WriteString(csTime + csCommand + L"\n");

        // Close File
        stdioLogFile.Close();
    }
}




/*

// Getting a string from the control unit from the internal thread (runs in internal thread)
CString CControlUnit::GetStringInternal()
{
    CString csResponse;

    // initilize variables
    DWORD bytesRead{};
    unsigned int uiBytesToRead = 0;

    DWORD dwErrorFlags;
    COMSTAT Comstat;
    BOOL bRes = false;
    bRes = ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
    if (Comstat.cbInQue == 0)    // buffer is empty, give it a second and try again
    {
        Sleep(1000);
        bRes = ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
    }

    if (Comstat.cbInQue > 0)
    {
        if (Comstat.cbInQue > ARDUINO_PORT_BUFFER_SIZE)
        {
            uiBytesToRead = ARDUINO_PORT_BUFFER_SIZE;
        }
        else
        {
            uiBytesToRead = Comstat.cbInQue;
        }
    }
    else  // zero bytes are waiting in the buffer, just return an empty string
    {
        return(csResponse);
    }

    // read from the port
    DWORD dwBytesRead;
    char pcharReadBuffer[ARDUINO_PORT_BUFFER_SIZE + 1];	// buffer used to read from the port
    bRes = ReadFile(m_hndPort, pcharReadBuffer, uiBytesToRead, &dwBytesRead, NULL);
    if (dwBytesRead)
        pcharReadBuffer[dwBytesRead - 1] = 0; // as this is a string
    //else
    //    pcharReadBuffer[dwBytesRead] = 0; // as this is a string

    // if read was ok then do the following
    if (bRes)
    {
        // got something, now copy it to the CArray
        csResponse = CString(pcharReadBuffer);
    }
    else
    {
        // there was some Error Reading Bytes
        DWORD dwRes = GetLastError();
        if (dwRes == ERROR_IO_PENDING)
        {
            DWORD dwErrorFlags;
            COMSTAT Comstat;
            ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
            WriteMessage(L"ERROR_IO_PENDING");
            // not data in buffer, return with true. Data will be based on old buffer
        }
        else
        {
            CString csErrorNum;
            csErrorNum.Format(L"Error num %d \n", dwRes);
            WriteMessage(csErrorNum);
        }
    }
    OutputDebugString(csResponse);
    return(csResponse); // if there was an error the response would still be an empty string
}


*/





// Getting a string from the control unit from the internal thread (runs in internal thread)
CString CControlUnit::GetStringInternal()
{
    CString csResponse;

    // initilize variables
    DWORD bytesRead{};
    unsigned int uiBytesToRead = 0;
    DWORD dwErrorFlags;
    COMSTAT Comstat;

    // try to read from the port
    BOOL bRes = ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);

    while (bRes)
    {
        // buffer is empty, exit and return whatever string we have
        if (Comstat.cbInQue == 0)    
            return(csResponse);

        // there is something in the buffer, get ready to read a full or partial frame
        if (Comstat.cbInQue > ARDUINO_PORT_BUFFER_SIZE)
            uiBytesToRead = ARDUINO_PORT_BUFFER_SIZE;
        else
            uiBytesToRead = Comstat.cbInQue;

        // read from the port
        DWORD dwBytesRead;
        char pcharReadBuffer[ARDUINO_PORT_BUFFER_SIZE + 1];	// buffer used to read from the port
        bool bReadOK = ReadFile(m_hndPort, pcharReadBuffer, uiBytesToRead, &dwBytesRead, NULL);

        // if read ok copy what was read into the csResponse string
        if (bReadOK)
        {
            if (dwBytesRead)
            {   // got something, now make it a string and and concate to previous if needed
                pcharReadBuffer[dwBytesRead] = 0; // make this a string
                csResponse += CString(pcharReadBuffer); // add to csResponse
                OutputDebugString(csResponse);
                // check is there is more to ready and set the Comstat.cbInQue to the number of bytes waiting
                bRes = ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
            }
        }
        else
        {
            // there was some Error Reading Bytes
            DWORD dwRes = GetLastError();
            if (dwRes == ERROR_IO_PENDING)
            {
                DWORD dwErrorFlags;
                COMSTAT Comstat;
                ClearCommError(m_hndPort, &dwErrorFlags, &Comstat);
                WriteMessage(L"ERROR_IO_PENDING");
                // not data in buffer, return with true. Data will be based on old buffer
            }
            else
            {
                CString csErrorNum;
                csErrorNum.Format(L"Error num %d \n", dwRes);
                WriteMessage(csErrorNum);
            } 

        } // end of read bReadOK

    } // end of while

    // we are out of the while, return the csResponse
    return(csResponse);

} // end of function



// spliting the csInString into the CArray (runs in get string thread)
void CControlUnit::SplitString(CString csInString, CArray<CString, CString>* parrycsSplitString)
{
    CString Seperator = _T("\r\n");
    int Position = 0;
    CString Token;

    Token = csInString.Tokenize(Seperator, Position);
    while (!Token.IsEmpty())
    {
        parrycsSplitString->Add(Token);
        // Get next token.
        Token = csInString.Tokenize(Seperator, Position);
    }
}



// Getting a string from the control unit (runs in main process)
CString CControlUnit::GetString()
{
    CString csResponse;
    m_mtxResponses.Lock();
    int iSize = (int)(m_arrycsResponses.GetSize());
    if (iSize) // there are responses strings waiting to be read
    {
        csResponse = m_arrycsResponses[0];
        m_arrycsResponses.RemoveAt(0, 1);
        m_arrycsResponses.FreeExtra();
    }
    m_mtxResponses.Unlock();

    // return either the empty string of the latest response from the control unit
    return(csResponse);
}

bool CControlUnit::Sonicate()
{
    WriteMessage(L"Sonicating");

    m_iActiveDuration =  m_iDuration;
    m_iActiveEnergy = 0;

    // get the power code for the temporary interface
    CString csPower;
    double fPower = m_fPower + 2.5;
    int iPowerCode = ((int)((fPower / 5.0) - 1.0));
    csPower.Format(L"%d", iPowerCode);
    if (iPowerCode == 10)
        csPower == "A";
    if (iPowerCode < 1)
        csPower == "1";
    if (iPowerCode > 10)
        csPower == "B";

    // get the steering code for the temporary interface
    CString csDepth;
    int iDepth = (int)(m_fFocalDepth / 5.0 - 5.15);
    switch (iDepth) 
    {
    case 1:
        csDepth = "a";
        break;
    case 2:
        csDepth = "b";
        break;
    case 3:
        csDepth = "c";
        break;
    case 4:
        csDepth = "d";
        break;
    case 5:
        csDepth = "e";
        break;
    case 6:
        csDepth = "f";
        break;
    case 7:
        csDepth = "g";
        break;
    case 8:
        csDepth = "h";
        break;
    case 9:
        csDepth = "i";
        break;
    case 10:
        csDepth = "j";
        break;
    case 11:
        csDepth = "k";
        break;
    default:
        csDepth = "S";
        break;
    }

    // send the command strings to the control unit to start sonication
    SendString(csDepth);
    SendString(csPower);
    SendString(L"E"); // start energy output
 
    return(true);
}

bool CControlUnit::Sonicate(int iDuration, int iEnergy, double fFocalDepth)
{
    SetDuration(iDuration);
    SetEnergy(iEnergy);
    double fPower = ((double)iEnergy) / ((double)iDuration);
    SetPower(fPower);
    SetFocalDepth(fFocalDepth);
    bool bRes = Sonicate();
    return(bRes);
}

// called to update internal active parameter based on control unit ones
bool CControlUnit::UpdateActiveSonicationParameters()
{
    if (m_iActiveDuration > 0)
    {
        m_iActiveDuration--;
        m_iActiveEnergy += (m_iEnergy / m_iDuration);
        m_fActivePower = m_fPower;

        // check if we just finished sonication, if yes, send disable to the power
        if (m_iActiveDuration == 0)
            SendString(L"D");
    }

    return (true);
}
