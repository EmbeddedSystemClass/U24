/*==== DECLARATION CONTROL ==================================================*/

/*==== INCLUDES =============================================================*/
#include "tcpdriver.h"
#include "debug.h"
#include <cstdlib>
#include <string>
#include <sstream>

#define SERV_PORT 9600

using namespace std;


static bool IsPathExist(const char* path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

static bool Exec(std::string& path, std::string& param, DWORD msTimeout = INFINITE, bool hasResponse = false)
{
	if (!IsPathExist(path.c_str())) {
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	DWORD nPipeSize = 1024;
	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
		return false;
	}

	TCHAR szNowPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szNowPath);
	TRACE("current path: %s\n", szNowPath);
	string sWorkDir = path.substr(path.rfind('\\'));
	TRACE("working path: %s\n", sWorkDir.c_str());
	SetCurrentDirectoryA(sWorkDir.c_str());

	bool isOk = false;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;
	string Command = path;
	if (!param.empty()) {
		Command = Command + " " + param;
	}
	TRACE("exec: %s\n", Command);
	if (::CreateProcessA(NULL, const_cast<LPSTR>(Command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		WaitForSingleObject(processInfo.hProcess, msTimeout);
		isOk = true;

		if (hasResponse) {
			DWORD bytesRead;
			char message[512];
			memset(message, 0, sizeof(message));
			::ReadFile(hRead, message, 512, &bytesRead, NULL);
			if (strlen(message) > 0) {
				path = message;
			}
			else {
				path.clear();
			}
		}
	}
	else {
		isOk = false;
		TRACE("ERROR: Execute fail!\r\ncmd:");
	}
	CloseHandle(hRead);
	CloseHandle(hWrite);
	SetCurrentDirectory(szNowPath);

	return isOk;
}

TcpDrv::TcpDrv(void)
{ 
	TcpDrv::ThreadHndl = NULL;
	TcpDrv::rxSignal = NULL;
	TcpDrv::CallBack = NULL;
	TcpDrv::Threshold = 0;

	/* 2011/05/23 Alex.Chen Modify Start */
	//InitializeCriticalSection(&(TcpDrv::CriticalSection));
    //TcpBuffer = new CircBuffer(SER_RX_BUF_SIZE); 
	/* 2011/05/23 Alex.Chen Modify End */

	/* 2011/05/23 Alex.Chen Modify Start */
	TcpDrv::TcpBuffer = NULL;
	TcpDrv::sockfd = INVALID_SOCKET;
	/* 2011/05/23 Alex.Chen Modify End */

    /* Default state is not opened. */
    isOpen = 0;
}

TcpDrv::~TcpDrv()
{
	/* 2011/05/23 Alex.Chen Modify Start */
	//TcpDrv::close();
    //if (TcpBuffer != NULL)
    //{
    //    delete TcpBuffer;
    //    TcpBuffer = NULL;
    //}
	/* 2011/05/23 Alex.Chen Modify End */
}

S8 TcpDrv::open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{	int nRetCode = 0;

	if (isOpen)
	{
		/* 2011/05/23 Alex.Chen Add Start */
		TcpDrv::close();
		/* 2011/05/23 Alex.Chen Add End */
		/* 110610 Gilpin.Xi delete for Touch Test command Start*/
		//return -2; /* Socket is already open */
		/* 110610 Gilpin.Xi delete for Touch Test command End*/
	}

	/* 2011/05/23 Alex.Chen Add Start */
	InitializeCriticalSection(&(TcpDrv::CriticalSection));
    TcpBuffer = new CircBuffer(SER_RX_BUF_SIZE);

	if (TcpBuffer == NULL) {
		TcpDrv::close();
		return -1;
	}
	/* 2011/05/23 Alex.Chen Add End */

	// open socket here
	if ((nRetCode = OpenSocket()) != 1) 
	{
		TcpDrv::close();
		return nRetCode; 
	}

	TcpDrv::rxSignal = CreateEvent(NULL,  /* default security attributes */
		                           FALSE, /* auto-reset event */
		                           FALSE, /* initial state is signaled */
		                           NULL); /* object not named */

	if (TcpDrv::configure(BaudRate, Parity, StopBits, Data) != 0)
	{
		/* 2011/05/23 Alex.Chen Add Start */
		TcpDrv::close();
		/* 2011/05/23 Alex.Chen Add End */
		return -4;
	}

	TcpDrv::ThreadHndl = CreateThread(NULL,                 /* no security attributes */
		                              0,                    /* use default stack size */
		                              TcpDrv::TcpListener,  /* thread function */
		                              (void*) this,             
		                              0,                    /* use default creation flags */
		                              &(TcpDrv::ThreadID));

	nRetCode = GetLastError();

	if (!TcpDrv::ThreadHndl) 
	{
		TcpDrv::close();
		return nRetCode;
	}
	SetThreadPriority(TcpDrv::ThreadHndl, THREAD_PRIORITY_ABOVE_NORMAL);
	TcpDrv::reset();
	isOpen = 1;
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::close()
+------------------------------------------------------------------------------
| Description : Closing function for the TCP driver. 
|
| Parameters  : void
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 TcpDrv::close(void)
{
	// add criticial section for TerminateThread
	U32 exitCode;

	// close socket
	if (sockfd != INVALID_SOCKET)
	{        
		closesocket(sockfd);
		WSACleanup();
		/* 110901 Gilpin.Xi Add Start */
		sockfd = INVALID_SOCKET;
		/* 110901 Gilpin.Xi Add End */
	}

	if (TcpDrv::ThreadHndl != NULL)
	{
		/* 110901 Gilpin.Xi add Start */
		::WaitForSingleObject(TcpDrv::ThreadHndl, 2000);
		/* 110901 Gilpin.Xi add End */
		GetExitCodeThread(TcpDrv::ThreadHndl, &exitCode);
		if (exitCode == STILL_ACTIVE)
		{
			TerminateThread(TcpDrv::ThreadHndl, exitCode);
		}
		CloseHandle(TcpDrv::ThreadHndl);
		TcpDrv::ThreadHndl = NULL;
	}

	if (TcpDrv::rxSignal != NULL)
	{        
		CloseHandle(TcpDrv::rxSignal);
	}

	TcpDrv::rxSignal = NULL;
	TcpDrv::isOpen = 0; /* TCP Closed */

	/* 2011/05/23 Alex.Chen Add Start */
	DeleteCriticalSection(&(TcpDrv::CriticalSection));

	if (TcpBuffer != NULL) {
		delete TcpBuffer;
		TcpBuffer = NULL;
	}
	/* 2011/05/23 Alex.Chen Add End */

	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::reset()
+------------------------------------------------------------------------------
| Description : TCP Reset Function
|
| Parameters  : None
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 TcpDrv::reset()
{
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::configure()
+------------------------------------------------------------------------------
| Description : TCP Reset Function
|
| Parameters  : 
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 TcpDrv::configure(U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{
	return 0;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::write()
+------------------------------------------------------------------------------
| Description : TCP write Function
|
| Parameters  :
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S32 TcpDrv::write(U8* Buffer, U32 Size)
{
	int written = 0;

	written = send(sockfd, (char *) Buffer, (int) Size, 0);

	if (SOCKET_ERROR == written) {
		// Reading has problem.
		closesocket(sockfd);
		WSACleanup();
		/* 110901 Gilpin.Xi Add Start */
		sockfd = INVALID_SOCKET;
		/* 110901 Gilpin.Xi Add End */
		written = Size;
	}
	return written;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::getRxEventHandle()
+------------------------------------------------------------------------------
| Description : Get RX event handle function
|
| Parameters  : None
|
| Returns     : HANDLE rxSignal
+----------------------------------------------------------------------------*/
HANDLE TcpDrv::getRxEventHandle()
{
    return TcpDrv::rxSignal;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::registerCallback()
+------------------------------------------------------------------------------
| Description : Register Callback Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void TcpDrv::registerCallback(T_SERIAL_CALL_BACK callback)
{
    TcpDrv::CallBack = callback;
    return;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::unregisterCallback()
+------------------------------------------------------------------------------
| Description : Unregister function
|
| Parameters  : None
|
| Returns     : void
+----------------------------------------------------------------------------*/
void TcpDrv::unregisterCallback()
{
    TcpDrv::CallBack = NULL;
    return;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::read()
+------------------------------------------------------------------------------
| Description : TCP Read Function
|
| Parameters  : 
|
| Returns     : S32
+----------------------------------------------------------------------------*/
S32 TcpDrv::read(U8* Buffer, U32 Size, U32 timeout)
{
	S32 ret = 0;
    if (Size == 0) 
    {
        Sleep(timeout);
    }
    else 
    {
        TcpDrv::setEventThreshold(Size);
        // Reading is pending.
        switch(WaitForSingleObject(TcpDrv::rxSignal, timeout)) 
        {
            // Overlapped event has been signaled.
            case WAIT_OBJECT_0:
                /* Store in buffer ... */
                ret = TcpDrv::TcpBuffer->pop(Buffer, Size);
                break;
            case WAIT_TIMEOUT:
                ret = WAIT_TIMEOUT;
                break;
            default:
                ret = GetLastError();
                ExitProcess(0); 
                break;
        }
    }
    return ret;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::setEventThreshold()
+------------------------------------------------------------------------------
| Description : TCP Set Event Threshold Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void TcpDrv::setEventThreshold(U32 num)
{
    EnterCriticalSection(&(TcpDrv::CriticalSection));
    TcpDrv::Threshold = num;

    if (TcpDrv::TcpBuffer->available() >= TcpDrv::Threshold) 
    {
        SetEvent(TcpDrv::rxSignal);
        TcpDrv::Threshold = 0;
    }
    LeaveCriticalSection(&(TcpDrv::CriticalSection));
    return;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::checkThreshold()
+------------------------------------------------------------------------------
| Description : TCP Check Threshold Function
|
| Parameters  : 
|
| Returns     : void
+----------------------------------------------------------------------------*/
void TcpDrv::checkThreshold()
{
    EnterCriticalSection(&(TcpDrv::CriticalSection));

    if (TcpDrv::Threshold != 0) 
    {
        if (TcpDrv::TcpBuffer->available() >= TcpDrv::Threshold) 
        {
            SetEvent(TcpDrv::rxSignal);
            TcpDrv::Threshold = 0;
        }
    }
    LeaveCriticalSection(&(TcpDrv::CriticalSection));
    return;
}


/*-----------------------------------------------------------------------------
| Function    : TcpDrv::TcpListener()
+------------------------------------------------------------------------------
| Description : TCP Check Threshold Function
|
| Parameters  : 
|
| Returns     : unsigned long
+----------------------------------------------------------------------------*/
unsigned long __stdcall TcpDrv::TcpListener(void* lpParam)
{
	U8 buf[64];
	U32 size;

	TcpDrv* drv = (TcpDrv*)lpParam;

	while (1) {
		do {
			/* 2011/05/23 Alex.Chen Add Start */
			Sleep(0);

			if (drv->sockfd == INVALID_SOCKET)
			{
				return 0;
			}
			/* 2011/05/23 Alex.Chen Add End */

			if (recv(drv->sockfd, (char*)buf, 1, 0) == SOCKET_ERROR) {
				size = 0;
				switch (WSAGetLastError())
				{
				case WSAECONNREFUSED:
					drv->reset();
					break;
				default:
					drv->reset();
					break;
				}
			}
			else {
				size = 1;
			}
			// Event driven or using call-back?
			if (drv->CallBack == NULL) {
				// Push the data to the buffer and signal to the upper layer if the threshold is reached.
				EnterCriticalSection(&(drv->CriticalSection));

				/* 2011/05/23 Alex.Chen Modify Start */
				//drv->TcpBuffer->push(buf, size);
				if (drv->TcpBuffer->push(buf, size) == CIRC_BUF_NOT_ENOUGH_SPACE) {
					for (int i = 0; i < 3; i++) {
						Sleep(50);
						if (drv->TcpBuffer->push(buf, size) == CIRC_BUF_NOT_ENOUGH_SPACE) {
							continue;
						} else {
							break;
						}
					}
				}
				/* 2011/05/23 Alex.Chen Modify End */
				LeaveCriticalSection(&(drv->CriticalSection));
				drv->checkThreshold();
			}
			else {
				drv->CallBack(buf, size);
			}
		} while (size);
	}
	return -1;
} 

/*110610 Gilpin.Xi Modify TcpListener API Start*/
//Please do not delete,for future modify use
//unsigned long __stdcall TcpDrv::TcpListener(void* lpParam)
//{
//	U8 buf[64];
//	U32 size;
//	int PayloadSize = 0;
//	int count = 0;
//
//	TcpDrv* drv = (TcpDrv*)lpParam;
//
//	while (1) {
//		do {
//				count = 1;
//
//				if (recv(drv->sockfd, (char*)buf, 4, 0) == SOCKET_ERROR)
//				{
//					continue;
//				}
//				else
//				{
//					size = 4;
//					if (buf[0] != 'B')
//					{
//						continue;
//					}
//
//					if (buf[1] != 'E')
//					{
//						continue;
//					}
//
//					if (buf[2] != 'N')
//					{
//						continue;
//					}
//
//					if (buf[3] != 'Q')
//					{
//						continue;
//					}
//					EnterCriticalSection(&(drv->CriticalSection));
//					drv->TcpBuffer->push(buf, size);
//					drv->checkThreshold();
//					LeaveCriticalSection(&(drv->CriticalSection));
//
//
//				}
//
//				if (recv(drv->sockfd, (char*)buf, 1, 0) == SOCKET_ERROR)
//				{
//					continue;
//				}
//				else
//				{
//					size = 1;
//
//					if(buf[0] == 0x13)
//						count = 0;
//					EnterCriticalSection(&(drv->CriticalSection));
//					drv->TcpBuffer->push(buf, size);
//					drv->checkThreshold();
//					LeaveCriticalSection(&(drv->CriticalSection));
//
//				}
//
//				if (recv(drv->sockfd, (char*)buf, 4, 0) == SOCKET_ERROR)
//				{
//					continue;
//				}
//				else
//				{
//					size = 4;
//					EnterCriticalSection(&(drv->CriticalSection));
//					drv->TcpBuffer->push(buf, size);
//					drv->checkThreshold();
//					LeaveCriticalSection(&(drv->CriticalSection));
//
//				}
//
//				if (recv(drv->sockfd, (char*)buf, 4, 0) == SOCKET_ERROR)
//				{
//					continue;
//				}
//				else
//				{
//					size = 4;
//					PayloadSize = buf[3] + buf[2] * 16 * 16 + buf[1] * 16 * 16 * 16 + buf[0] * 16 * 16 * 16 * 16;
//					EnterCriticalSection(&(drv->CriticalSection));
//					drv->TcpBuffer->push(buf, size);
//					drv->checkThreshold();
//					LeaveCriticalSection(&(drv->CriticalSection));
//
//				}
//
//				if (recv(drv->sockfd, (char*)buf, PayloadSize + 1, 0) == SOCKET_ERROR)
//				{
//					continue;
//				}
//				else
//				{
//					size = PayloadSize + 1;
//					EnterCriticalSection(&(drv->CriticalSection));
//					drv->TcpBuffer->push(buf, size);
//					drv->checkThreshold();
//					LeaveCriticalSection(&(drv->CriticalSection));
//				}
//		} while (count);
//
//		break;
//	}
//	return -1;
//} 
/*110610 Gilpin.Xi Modify TcpListener API End*/

/*-----------------------------------------------------------------------------
| Function    : TcpDrv::ResetSocket()
+------------------------------------------------------------------------------
| Description : TCP Reset Function
|
| Parameters  : 
|
| Returns     : S8
+----------------------------------------------------------------------------*/
S8 TcpDrv::ResetSocket(SOCKET socket)
{    
	closesocket(socket);
	WSACleanup();
	/* 110901 Gilpin.Xi Add Start */
	sockfd = INVALID_SOCKET;
	/* 110901 Gilpin.Xi Add End */

    Sleep(10); /* Delay added to avoid windows TCP driver error */

	int isOk;
    isOk = OpenSocket();

    if (isOk == -1)
    {
        return -1;
    }
    return 0;
}


int TcpDrv::OpenSocket()
{	
	// open adb tcp port
	stringstream ss;
	ss << "forward tcp:" << SERV_PORT << " tcp:" << SERV_PORT; // << " && echo setup ok";
	string Command = ss.str();
	//TRACE(_T("Forward Command: %s\n"), Command);
	if (ExecAdb(Command)) {
		int port = SERV_PORT;
		int retVal;
		// TODO: return error code
		WSADATA wsaData;
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(2, 2);
		retVal = WSAStartup(wVersionRequested, &wsaData);
		if (retVal != 0) {
			return -11;
		}

		// create socket
		sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == sockfd) {
			WSACleanup();
			return -12;
		}

		/* 111206 Gilpin.Xi Add Start */
		/* Set socket net time out */
		int nNetTimeout = 200;//0.2Ãë
		//receive
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		/* 111206 Gilpin.Xi Add End */

		// initialize value in dest
		struct sockaddr_in dest={0};
		dest.sin_family = PF_INET;
		dest.sin_port = htons(port);
		dest.sin_addr.s_addr= inet_addr ("127.0.0.1");

		// connect to server
		retVal = connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
		if (SOCKET_ERROR == retVal) {
			int n = WSAGetLastError();
			//TRACE(_T("SOCKET connect error code: %d\n"), n);
			//msg.Format("# Check Adb status fail: %d\n", n);
			//PrintOut(msg , MSG_ERROR);
			closesocket(sockfd);
			WSACleanup();
			/* 110901 Gilpin.Xi Add Start */
			sockfd = INVALID_SOCKET;
			/* 110901 Gilpin.Xi Add End */
			return n;
		}
		return 1;
	} else {
		return -10;
	}
}

bool TcpDrv::ExecAdb(std::string Command)
{
	char szExePath[MAX_PATH];
	GetModuleFileNameA(NULL, szExePath, MAX_PATH);

	string exe = szExePath;
	exe = exe.substr(0, exe.rfind('\\')) + "\\adb.exe";
	
	bool isOk = false;
	isOk = Exec(exe, Command, 500, false);

	return isOk && Command.find("error") == string::npos;
}

