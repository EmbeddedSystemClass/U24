
#include "stdafx.h"
#include "LogDispose.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LogDispose::LogDispose()
{

}

LogDispose::~LogDispose()
{

}

void LogDispose::LogDispose_syslog(CString str_logInfo)
{
	DWORD dw_writeNumber;                  // the writed byte size 
	HANDLE h_client;                       // a pipe handle
	int i_length;                          // the total length
	CString str_loginformation;            // the log content
	
	dw_writeNumber = 0;
	h_client = 0;
	i_length = 0;
	/* check the str_logInfo whether is null */
	if( str_logInfo.IsEmpty() != 0 ) {
		/* the input is null */
		return;
	}
	i_length = str_logInfo.GetLength();
	/* wait the connecting from the server */
	if ( WaitNamedPipe( NAMEDPIPE , NMPWAIT_USE_DEFAULT_WAIT ) == FALSE ) {
		return;
	}
	/* create a pipe */
	h_client = CreateFile( NAMEDPIPE , GENERIC_WRITE|GENERIC_READ,0,NULL,
			   OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	/* create the pipe failurely */
	if( h_client == INVALID_HANDLE_VALUE ) {
		return;	
	}
	/* write the data into the pipe */
	 WriteFile( h_client, str_logInfo , i_length , &dw_writeNumber, NULL );
	/* close the pipe */
	CloseHandle( h_client );
}

void LogDispose::LogDispose_writeLog(CString str_logContent)
{
	if( str_logContent.IsEmpty() != 0 ) {
		/* the input is null */
		return;
	}
	try {
		/* move the file pointer to end position of the file */
		obj_file.SeekToEnd();
		/* write the log to file */
		obj_file.Write( str_logContent , str_logContent.GetLength() );
	} catch ( CFileException *e ) {
		/* catch the file exception opening the file */
		e->Delete();
		return;
	}	
}

void LogDispose::LogDispose_init()
{
	CString str_logPathName;
	int i_existentResult;                 // the existent sign of the file 
	i_existentResult = 0;   
	SYSTEMTIME st_systemTime;         // the system time
	CString str_currentTime;

	WIN32_FIND_DATA fd_fileInformation;  // WIN32_FIND_DATA structure
	HANDLE h_searchFind;                 // a search handle
	/* find the directory and return a search handle */
	h_searchFind = FindFirstFile( LOG_FILEPATH, &fd_fileInformation );
	/* the handle is invalid and the attribute is directory */
    if( ( h_searchFind != INVALID_HANDLE_VALUE ) && ( 
		  fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
		/* find success and close the specified search handle */
		FindClose( h_searchFind ); 
    } else {
		/* find failure and close the specified search handle */
		FindClose( h_searchFind ); 
		/* create a null directory */
		if( CreateDirectory( LOG_FILEPATH , NULL ) == 0 ) {
			/* fail to create */
			return;
		} 
	} 
	/* get the system current time */
	GetLocalTime( &st_systemTime );
	str_currentTime.Format( "%4d%02d%02d", st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay);
	str_logPathName.Format("%s\\%s_%s.log",LOG_FILEPATH,LOG_FILENAME,str_currentTime);
	if ( !obj_file.Open( str_logPathName, CFile::modeReadWrite|CFile::modeCreate
		|CFile::modeNoTruncate|CFile::shareDenyNone ,NULL ) ) {
		return;
	}
}

UINT LogDispose::LogDispose_startupLogServer(LPVOID lpParam)
{
	DWORD dw_readNum;                 // the readed number
    HANDLE h_server;                  // the pipe handle of the server
	DWORD dw_errerNo;                 // the errorNo
	CString str_errNo;                // the errNo(CString)
	LogDispose obj_logDispose;   // declare a class object
	int i_circleNumber;               // circle number

	dw_readNum = 0;
	h_server = 0 ;
	dw_errerNo = 0;
	i_circleNumber = 0;
	/* save the readed data from pipe */
	char sz_buffer[MAXSIZE];
	/* init the server */
	obj_logDispose.LogDispose_init();
	/* create a pipe and return a handle */
	h_server = CreateNamedPipe( NAMEDPIPE , PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED ,
        PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT , 
		100,  1024 , 1024 , 1000 , NULL );
	/* create the pip failure */
	if ( h_server == INVALID_HANDLE_VALUE ) {
		/* create a pipe again */
		h_server = CreateNamedPipe ( NAMEDPIPE , 
			PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED ,
            PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT , 100 , 1024 , 1024,
								     1000, NULL );
		if ( h_server == INVALID_HANDLE_VALUE ) {
			/* get the error cause */
			dw_errerNo = GetLastError();
			CString str_errorContent;
			str_errorContent = "Creating the pipe fails when the system startup the log server !";
			str_errNo.Format( "%s[errNo=%ld]" , str_errorContent , 
				dw_errerNo );
			/* write the error cause to the errlog file */
			obj_logDispose.LogDispose_writeLog( str_errNo );
			return NG;
		}
	}
	while(1) {
		/* listen the pipe and wait the connection from the client */
		if( ConnectNamedPipe( h_server, NULL) ) {
			/* listened the client and read the content from buffer */
			/* init the array content 0 */
			memset( sz_buffer, 0 ,sizeof( sz_buffer ) );
			if( ReadFile( h_server,sz_buffer,sizeof( sz_buffer ),
				&dw_readNum,NULL ) == FALSE ) {
				/* get the error cause */
				dw_errerNo = GetLastError();
				CString str_errorContent;
				str_errorContent = "Reading the data fails when the system get the data from the pipe !";
				str_errNo.Format( "%s[errNo=%ld]" , 
					str_errorContent , dw_errerNo );
				/* write the error cause to the errlog file */
				obj_logDispose.LogDispose_writeLog( str_errNo );
			} else {
				obj_logDispose.LogDispose_writeLog( sz_buffer );
			}
			/* close the connection */
			DisconnectNamedPipe( h_server );
		} else {
			/* create to connect again */
			i_circleNumber++;
			if( i_circleNumber > TRY_COUNT ) {
				i_circleNumber = 0;
				/* stop one second */
				Sleep( 1000 );
			}
		}	
	}
	/* close the handle */
    CloseHandle( h_server );
	return OK;
}

