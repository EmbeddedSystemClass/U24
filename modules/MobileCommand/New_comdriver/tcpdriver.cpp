/*==== DECLARATION CONTROL ==================================================*/

/*==== INCLUDES =============================================================*/
#include "tcpdriver.h"
#include "debug.h"
#include <cstdlib>
#include <string>
#include <sstream>

#include <process.h>

#define SERV_PORT 9600

using namespace std;


HANDLE   m_hThreadKilledEvent; // 
HANDLE   m_hKillThreadEvent; //  

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
	/*string sWorkDir = path.substr(path.rfind('\\'));
	TRACE("working path: %s\n", sWorkDir.c_str());
	SetCurrentDirectoryA(sWorkDir.c_str());*/

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
		CloseHandle( processInfo.hProcess );
		CloseHandle( processInfo.hThread );

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

	m_hThreadKilledEvent = NULL;
	m_hKillThreadEvent=NULL;
	InitializeCriticalSection(&(TcpDrv::CriticalSection));

	TcpBuffer = new CircBuffer(SER_RX_BUF_SIZE); 
	/* Default state is not opened. */
	isOpen = 0;

	if(m_hThreadKilledEvent==NULL)
	{
		m_hThreadKilledEvent   =   ::CreateEvent(NULL,   TRUE,   FALSE,   NULL);
	}
	if(m_hKillThreadEvent==NULL)
	{
		m_hKillThreadEvent   =   ::CreateEvent(NULL,   TRUE,   FALSE,   NULL); 
	}
	Sleep(1000);
}

TcpDrv::~TcpDrv()
{
	TcpDrv::close();
	if (TcpBuffer != NULL)
	{
		delete TcpBuffer;
		TcpBuffer = NULL;
	}	

	if(m_hThreadKilledEvent!=NULL)
	{
		CloseHandle(m_hThreadKilledEvent);
		m_hThreadKilledEvent=NULL;
	}
	if(m_hKillThreadEvent!=NULL)
	{
		CloseHandle(m_hKillThreadEvent);
		m_hKillThreadEvent=NULL;
	}
}

S8 TcpDrv::open(U32 Port, U32 BaudRate, U8 Parity, U8 StopBits, U8 Data)
{
	if (isOpen)
	{
		return 0; /* Socket is already open */
	}

	// open socket here
	if (OpenSocket(Port) == -1) 
	{
		TcpDrv::close();
		return -1; 
	}
	TcpDrv::rxSignal = CreateEvent(NULL,  /* default security attributes */
		FALSE, /* auto-reset event */
		FALSE, /* initial state is signaled */
		NULL); /* object not named */

	if (TcpDrv::configure(BaudRate, Parity, StopBits, Data) != 0)
	{
		return -1;
	}

	TcpDrv::ThreadHndl = CreateThread(NULL,                 /* no security attributes */
		0,                    /* use default stack size */
		TcpDrv::TcpListener,  /* thread function */
		(void*) this,             
		0,                    /* use default creation flags */
		&(TcpDrv::ThreadID));

	if (!TcpDrv::ThreadHndl) 
	{
		TcpDrv::close();
		return -1;
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
	if (!isOpen)
	{
		return 0; /* Socket is already close */
	}

	// add criticial section for TerminateThread
	U32 exitCode;
	DWORD   dwEvent;
	if (TcpDrv::ThreadHndl != NULL)
	{
		::SetEvent(m_hKillThreadEvent); //  
		Sleep(20);
		GetExitCodeThread(TcpDrv::ThreadHndl, &exitCode);
		if (exitCode == STILL_ACTIVE)
		{			
			dwEvent   =   ::WaitForSingleObject(m_hThreadKilledEvent, 3000); //   
			if(dwEvent   !=   WAIT_OBJECT_0)    
			{	 
				EnterCriticalSection(&(TcpDrv::CriticalSection));
				TerminateThread(TcpDrv::ThreadHndl, exitCode);
				LeaveCriticalSection(&(TcpDrv::CriticalSection));
			} 

		}
		::ResetEvent(m_hKillThreadEvent); //  
		::ResetEvent(m_hThreadKilledEvent); //  
		CloseHandle(TcpDrv::ThreadHndl);
		TcpDrv::ThreadHndl = NULL;
	}

	if (TcpDrv::rxSignal != NULL)
	{        
		CloseHandle(TcpDrv::rxSignal);
	}

	// close socket
	if (sockfd != INVALID_SOCKET)
	{        
		shutdown(sockfd,SD_BOTH);
		closesocket(sockfd);
		WSACleanup();
	}

	TcpDrv::rxSignal = NULL;
	TcpDrv::isOpen = 0; /* TCP Closed */
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
		shutdown(sockfd,SD_BOTH);
		closesocket(sockfd);
		WSACleanup();
		//written = Size;
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
			//GetLastError();
			ret = 0;			
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
	U32 ReadCount = 0;
	while (1) {
		ReadCount = 0;
		do {
			if(ReadCount % 32 == 0) {
				DWORD dwEvent = ::WaitForSingleObject(m_hKillThreadEvent, 0);
				if (dwEvent == WAIT_OBJECT_0)
				{
					::SetEvent(m_hThreadKilledEvent);
					_endthread();
					return -1;
				}
				Sleep(0);
			}

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
				ReadCount++;
			}
			// Event driven or using call-back?
			if (drv->CallBack == NULL) {
				// Push the data to the buffer and signal to the upper layer if the threshold is reached.
				EnterCriticalSection(&(drv->CriticalSection));
				drv->TcpBuffer->push(buf, size);
				drv->checkThreshold();
				LeaveCriticalSection(&(drv->CriticalSection));
			}
			else {
				drv->CallBack(buf, size);
			}
		} while (size);
	}
	return -1;
} 


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
	shutdown(socket,SD_BOTH);
	closesocket(socket);
	//WSACleanup();

	Sleep(10); /* Delay added to avoid windows TCP driver error */

	int isOk;
	isOk = OpenSocket();
	if (isOk == -1)
	{
		return -1;
	}
	return 0;
}


int TcpDrv::OpenSocket(int nMultiCtrlPort)
{
	// open adb tcp port
	stringstream ss;
	if( nMultiCtrlPort == 2000 ){
		ss << "forward tcp:" << SERV_PORT << " tcp:" << TARG_PORT; // << " && echo setup ok";		
	}else if( nMultiCtrlPort == 2001 ){
		ss << "-s " << QPHONE_NAME_1 << " forward tcp:" << SERV_PORT_1 << " tcp:" << TARG_PORT;
	}else if( nMultiCtrlPort == 2002 ){
		ss << "-s " << QPHONE_NAME_2 << " forward tcp:" << SERV_PORT_2 << " tcp:" << TARG_PORT;
	}else if( nMultiCtrlPort == 2003 ){
		ss << "-s " << QPHONE_NAME_3 << " forward tcp:" << SERV_PORT_3 << " tcp:" << TARG_PORT;
	}else if( nMultiCtrlPort == 2004 ){
		ss << "-s " << QPHONE_NAME_4 << " forward tcp:" << SERV_PORT_4 << " tcp:" << TARG_PORT;
	}
	string Command = ss.str();
	//TRACE(_T("Forward Command: %s\n"), Command);
	if (ExecAdb(Command)) {
		int port = SERV_PORT + ( nMultiCtrlPort - 2000);
		int retVal;
		// TODO: return error code
		WSADATA wsaData;
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(2, 2);
		retVal = WSAStartup(wVersionRequested, &wsaData);
		if (retVal != 0) {
			return -1;
		}

		// create socket
		sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == sockfd) {
			WSACleanup();
			return -1;
		}

		//Set Time Out
		int timeout = 1;// fail:1000, pass 1,10,100
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
		{
			return WSAGetLastError();
		}

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
			shutdown(sockfd,SD_BOTH);
			closesocket(sockfd);
			WSACleanup();
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

bool TcpDrv::ExecAdb(std::string Command)
{
	string exe = "adb.exe";

	bool isOk = false;
	isOk = Exec(exe, Command, 500, false);

	return isOk && Command.find("error") == string::npos;
}

