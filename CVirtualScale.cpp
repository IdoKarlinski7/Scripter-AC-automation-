#include "pch.h"
#include "CVirtualScale.h"

// constructor / destructor
CVirtualScale::CVirtualScale()
{
}

CVirtualScale::~CVirtualScale()
{
}



// log the sent screen
void CVirtualScale::WriteMeasurment(CString csCommand)
{
    // create the file handle and open it
    CStdioFile stdioLogFile;
    BOOL bRes = stdioLogFile.Open(L"c:\\FUSMobile\\Scripter\\Scale.log", CFile::modeCreate | CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
    if (bRes)
    {
        // if opened, go to the end of the file
        stdioLogFile.SeekToEnd();
        // write the string
        stdioLogFile.WriteString(csCommand + L"\n");

        // Close File
        stdioLogFile.Close();
    }
}
