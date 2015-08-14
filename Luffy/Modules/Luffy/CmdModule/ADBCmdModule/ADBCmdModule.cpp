#include "stdafx.h"
#include "ADBCmdModule.h"
#include "Shlwapi.h"
#include <iostream>

void StringReplace(std::string &str, std::string from, std::string to)
{
	size_t Found = str.find( from );

	while( Found != std::string::npos )
	{
		str.replace( Found, from.size(), to);

		Found = str.find( from, Found + to.size() );
	}
}

/****************************************************************
CADBCmdModule
*****************************************************************/
CADBCmdModule::CADBCmdModule(const char *DeviceSerialNum)
: m_DeviceSerialNum( NULL )
{
    // Set Default.
    m_bConnectDevice = true ;
    m_exeType = exeType_ADB ;

    // Setup Device S/N
	std::string temp( DeviceSerialNum );
	this->m_DeviceSerialNum = new char [ temp.size() + 1 ];
	sprintf_s( this->m_DeviceSerialNum, temp.size() + 1, temp.c_str() );
}

CADBCmdModule::~CADBCmdModule()
{
	this->DisConnectDevice();

	// system command to force kill all adb.exe and fastboot.exe in task manager
    char szCmd1 [] = "taskkill /F /IM adb.exe /T";
    char szCmd2 [] = "taskkill /F /IM fastboot.exe /T";
    CCmdPtr CmdObj1 = new CADBCmdContainer((BYTE *)szCmd1, sizeof szCmd1, false, 5000);
    CCmdPtr CmdObj2 = new CADBCmdContainer((BYTE *)szCmd2, sizeof szCmd2, false, 5000);
    this->ExeSysCmd(CmdObj1, exeType_SYS);
    this->ExeSysCmd(CmdObj2, exeType_SYS);

	if( NULL != this->m_DeviceSerialNum )
	{
		delete this->m_DeviceSerialNum;
		this->m_DeviceSerialNum = NULL;
	}
}

bool CADBCmdModule::ConnectDevice()
{
	return IsConnected();
}

bool CADBCmdModule::DisConnectDevice()
{
	return this->ADBKillServer();
}

bool CADBCmdModule::IsConnected()
{
    if (! m_bConnectDevice)
        return true ;


	char szDevice[512] = {0};
	this->ADBDevices( szDevice, sizeof szDevice );

	std::string strDevice( szDevice );

	//auto detect, ok if found any device
	if( 0 == strcmp( this->m_DeviceSerialNum, "-1") )
	{
		return !strDevice.empty();
	}

	else
	{
		size_t FoundDevice = strDevice.find( this->m_DeviceSerialNum );

		return FoundDevice != std::string::npos;
	}
}

void CADBCmdModule::ShouldConnectDevice(bool bShould)
{
    m_bConnectDevice = bShould ;
}

void CADBCmdModule::SetExecuteType(ADBModuleExeType exeType)
{
    m_exeType = exeType ;
}

bool CADBCmdModule::PreExe()
{
	return true;
}

bool CADBCmdModule::ExeCommand(CCmdPtr &Cmd)
{
	return this->ExeSysCmd(Cmd, m_exeType) ;
}

bool CADBCmdModule::PostExe()
{
	return this->ADBKillServer();
}

bool CADBCmdModule::ExeSysCmd(CCmdPtr &Cmd, ADBModuleExeType exeType)
{
	bool ret = true;

	HANDLE hWrite, hRead;
	PROCESS_INFORMATION processAdbInfo;

	memset( &processAdbInfo, 0, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	try
	{
		if (! CreatePipe (&hRead, &hWrite, &saAttr, 0))
			throw "CreatePipe fail";

		STARTUPINFO si = {0};
		si.cb = sizeof (STARTUPINFO);
		si.hStdError = hWrite; //newstdout;
		si.hStdOutput = hWrite; //newstdout;
		si.hStdInput = hRead; //newstdin;
		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		TCHAR szWorkingDir[MAX_PATH], szExePath[MAX_PATH];
		TCHAR szCmdLine[1024];

		// Set WorkPath and ExePath
		GetModuleFileName (NULL, szExePath, MAX_PATH);
		PathRemoveFileSpec (szExePath);
		strcpy_s (szWorkingDir, sizeof szWorkingDir, szExePath);

		SetCurrentDirectory (szWorkingDir);

		if ( exeType_ADB == exeType || exeType_FASTBOOT == exeType)
		{
            if ( exeType_ADB == exeType)
                PathAppend (szExePath, "adb.exe");
            else
                PathAppend (szExePath, "fastboot.exe");
            
            0 == strcmp( this->m_DeviceSerialNum, "-1" ) ? sprintf_s (szCmdLine, sizeof szCmdLine, "\"%s\" %s", szExePath, (char *)Cmd->GetCmd()) : 
                                                           sprintf_s (szCmdLine, sizeof szCmdLine, "\"%s\" -s %s %s", szExePath, this->m_DeviceSerialNum, ( char *)Cmd->GetCmd());
		}
        else if ( exeType_SYS == exeType )
        {
            SetCurrentDirectory (szWorkingDir) ;
            strcpy_s(szCmdLine, sizeof szCmdLine, (char *)Cmd->GetCmd()) ;
        }
        else
            return false ;


		if (! CreateProcess (NULL, //szExePath,		// do not send parameter
							 szCmdLine,		// need enclosing quotation
							 NULL,
							 NULL,
							 TRUE,			// important!
							 CREATE_DEFAULT_ERROR_MODE,
							 NULL,
							 szWorkingDir,	// child process's full path.
							 &si,
							 &processAdbInfo))
		{
			throw "CreateProcess fail";
		}
		
		WaitForSingleObject (processAdbInfo.hProcess, Cmd->GetTimeOut());

		DWORD dwAvail, numread;

		BOOL bRet = PeekNamedPipe (hRead, NULL, 0, NULL, &dwAvail, 0);

		char *szRet = NULL;

		if ( bRet && dwAvail > 0)
		{
			szRet = new char [ dwAvail + 1 ];
			::ZeroMemory( (void *)szRet, dwAvail + 1 );

			::ReadFile (hRead, (void *)szRet, dwAvail, &numread, 0);
		}
        else if ( bRet && 0 == dwAvail )
        {
            szRet = new char [1];
            ::ZeroMemory( (void *)szRet, 1 );
            _stprintf_s(szRet, 1, _T("")) ;
        }

        std::string strTmp(szRet) ;
        if ( NULL != szRet  && std::string::npos != strTmp.find( _T("error: device not found")) )
            ret = false ;

        if ( NULL != szRet  && std::string::npos != strTmp.find( _T("Android Debug Bridge")) )
            ret = false ;

		// if needs return value
		if ( ret && Cmd->HasReturnValue() )
        {
            Cmd->SetResponseBufferSize( dwAvail + 1 );
            ::memcpy((char *)Cmd->GetResponse(), szRet, dwAvail+1);
            sprintf_s( (char *)Cmd->GetResponse(), dwAvail + 1, szRet );

            if ( 0 == strcmp( szRet, _T("") ) )
				ret = false ;
		}

		if ( NULL != szRet )
		{
			delete [] szRet;
			szRet = NULL;
		}
	}

	catch (const TCHAR* szError)
	{
		LPVOID lpMsgBuf;

		FormatMessage (	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, GetLastError(),
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR)&lpMsgBuf,
						0, NULL );

		TCHAR szMessage[1024];
		sprintf_s (szMessage, sizeof szMessage, "%s: %s", szError, lpMsgBuf);

		LocalFree(lpMsgBuf);
		ret = false;
	}

	CloseHandle (hRead);
	CloseHandle (hWrite);
	CloseHandle (processAdbInfo.hProcess); processAdbInfo.hProcess = NULL;
	CloseHandle (processAdbInfo.hThread); processAdbInfo.hThread = NULL;

	return ret;
}

bool CADBCmdModule::ADBRoot()
{
	char Cmd[] = "root";

    CCmdPtr CmdObj = new CADBCmdContainer( (BYTE *)Cmd, sizeof Cmd, true, INFINITE );

	bool ret = false, success = false;

	ret = this->ExeDiag( CmdObj );

	std::string strRet( ( char * )CmdObj->GetResponse() );

	if(ret)
		success = strRet.find( "adbd is already running as root" ) != std::string::npos;

	return ret && success;
}

bool CADBCmdModule::ADBKillServer()
{
	char Cmd[] = "kill-server";

	CCmdPtr CmdObj = new CADBCmdContainer( (BYTE *)Cmd, sizeof Cmd, false, INFINITE );
    this->SetExecuteType(exeType_ADB) ;

	return this->ExeCommand( CmdObj );
}

bool CADBCmdModule::ADBDevices(char *szDeviceList, size_t BufferSize)
{
	char Cmd[] = "devices";

    CCmdPtr CmdObj = new CADBCmdContainer( (BYTE *)Cmd, sizeof Cmd, true, INFINITE );

	bool ret = this->ExeCommand( CmdObj );

	if( ret )
	{
		std::string strRet( ( char * )CmdObj->GetResponse() );

		char szKey[] = "List of devices attached";

		size_t FoundKey = strRet.find( szKey );

		if( FoundKey != std::string::npos )
		{
			FoundKey += sizeof szKey + 2;//including \r\n

			std::string strDevice( ( char * )( CmdObj->GetResponse() + FoundKey ) );

			StringReplace( strDevice, "\t", ";");
			StringReplace( strDevice, "device", "" );
			StringReplace( strDevice, "\r\n", "" );

			sprintf_s(szDeviceList, BufferSize, strDevice.c_str() );
		}
		
		else
		{
			ret = false;
		}
	}

	return ret;
}

/****************************************************************
CADBCmdContainer
*****************************************************************/
CADBCmdContainer::CADBCmdContainer(BYTE *Cmd, UINT CmdSize, bool HasReturnValue, UINT TimeOut)
:	m_Cmd( NULL ),
	m_Response( NULL ),
	m_TimeOut( TimeOut ),
	m_HasReturnValue( HasReturnValue ),
	m_CmdSize( CmdSize )
{
	if( 0 < this->m_CmdSize )
	{
		this->m_Cmd = new BYTE [this->m_CmdSize];
        ::ZeroMemory(this->m_Cmd, this->m_CmdSize) ;
		memcpy((void *)this->m_Cmd, (void *)Cmd, this->m_CmdSize);
	}
}

CADBCmdContainer::~CADBCmdContainer()
{
	if(NULL != this->m_Cmd)
	{
		delete this->m_Cmd;
		this->m_Cmd = NULL;
	}

	if(NULL != this->m_Response)
	{
		delete this->m_Response;
		this->m_Response = NULL;
	}
}

const BYTE *CADBCmdContainer::GetCmd() const
{
	return this->m_Cmd;
}

const BYTE *CADBCmdContainer::GetResponse() const
{
	return this->m_Response;
}

const UINT CADBCmdContainer::GetCmdSize() const
{
	return this->m_CmdSize;
}

bool CADBCmdContainer::HasReturnValue() const
{
	return this->m_HasReturnValue;
}

UINT CADBCmdContainer::GetTimeOut() const
{
	return this->m_TimeOut;
}

void CADBCmdContainer::SetResponseBufferSize(UINT size)
{
	if( NULL != this->m_Response )
	{
		delete this->m_Response;
		this->m_Response = NULL;
	}

	this->m_Response = new BYTE [size];
	::ZeroMemory((void *)this->m_Response, size);
}

UINT CADBCmdContainer::GetResponseSize() const
{
    return sizeof this->m_Response ;
}