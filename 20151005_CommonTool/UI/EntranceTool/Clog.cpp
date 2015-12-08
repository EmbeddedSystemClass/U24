
#include "stdafx.h"
#include "afx.h"
#include "atltime.h" 
#include "Clog.h"
#include "Shlwapi.h"
Clog::Clog(void)
{
	memset(m_sz_path, 0, MAX_PATH);
	m_str_LogName = _T("");
	Setpath(m_str_LogName);
}

Clog::~Clog(void)
{

}


/*****************************************************************************
* Function name: CheckFileExist(CString str_path)      
* Summary      : Verify the file path.
* Parameters   : IN   CString str_path file path.
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Tom.Li        2011/06/28          First version             
*****************************************************************************/
bool Clog::CheckFileExist(CString str_path)
{
	CFileFind obj_fileFind;
	if(!obj_fileFind.FindFile(str_path))
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* function name:  OpenFile () 
* summary      :  Open file.
*        
* Parameters   :  NULL       
*                          
* Return       :  Return success if succed, else return false 	    		                                            
* Version       Author        Date                Abstract                 
* 1.0           Tom Li      2012/03/14           First version           
****************************************************************************/
bool Clog::OpenFile (TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	if(m_str_LogName.IsEmpty())
	{
		CString str_ErrCode = _T("Clog Error");
		CString str_ErrMsg = _T("EntranceTool LOG文件路径D:\\LOG\\EntranceTool_Log为空");
		strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;
	}
	else
	{
		return true;
	}
}

/*****************************************************************************
* function name:  WriteLog (CString str_logContent, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)    
* summary      :  The WriteLog function will write the content of "logcontent"
*                 into files "chLogFile".
* Parameters   :  chLogFile    :(IN)    Log File name         
*                 logcontent   :(IN)    Content writed into Log File            
* Return       :  Return success if record succed, else return false 	    		                                            
* Version       Author        Date                Abstract                 
* 1.0           Tom Li      2011/06/28           First version           
****************************************************************************/
bool Clog::WriteLog (CString str_logContent, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	/* Input Check */
	if (str_logContent.IsEmpty())
	{
		/*CString str_ErrCode = _T("Clog Error");
		CString str_ErrMsg = _T("Log 内容为空");
		strcpy_s(sz_ErrCode,512,str_ErrCode.GetBuffer());
		strcpy_s(sz_ErrMsg,512,str_ErrMsg.GetBuffer());
		str_ErrCode.ReleaseBuffer();
		str_ErrMsg.ReleaseBuffer();
		return false;*/
		return true; //Nancy.Yang changed
	}


	bool b_res = false;
	CString str_data = _T("");
	str_data = GetcurrentTime();
	str_logContent = str_data + _T("      ") + str_logContent;
    //_T("D:\\LOG\\EntranceTool_Log");
	CStdioFile flog;
	CFileException fileException;
    
    if (PathIsDirectory(_T("D:\\LOG\\EntranceTool_Log")))
    {
		if(CheckFileExist(m_str_LogName))
		{
			if(flog.Open (m_str_LogName, CFile::typeText|CFile::modeReadWrite|CFile::modeNoTruncate, &fileException))
			{
				flog.SeekToEnd();
				flog.WriteString(str_logContent);
				flog.WriteString("\n");
				flog.Close();
				b_res = true;
			}
			else
			{
				TRACE("Can't open file, error = %u\n",fileException.m_cause);
			}
		}
		else
		{
			if(flog.Open (m_str_LogName, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
			{
				flog.WriteString(str_logContent);
				flog.WriteString("\n");
				flog.Close();
				b_res = true;
			}
			else
			{
				TRACE("Can't open file, error = %u\n",fileException.m_cause);
			}
		}
    }
	else
	{
		if   (CreateDirectory(_T("D:\\LOG\\EntranceTool_Log"),NULL))   
		{   
            
			if(CheckFileExist(m_str_LogName))
			{   
				
				if(flog.Open (m_str_LogName, CFile::typeText|CFile::modeReadWrite|CFile::modeNoTruncate, &fileException))
				{
					flog.SeekToEnd();
					flog.WriteString(str_logContent);
					flog.WriteString("\n");
					flog.Close();
					b_res = true;
				}
				else
				{
					TRACE("Can't open file, error = %u\n",fileException.m_cause);
				}
			}
			else
			{
				if(flog.Open (m_str_LogName, CFile::typeText|CFile::modeReadWrite|CFile::modeCreate, &fileException))
				{
					flog.WriteString(str_logContent);
					flog.WriteString("\n");
					flog.Close();
					b_res = true;
				}
				else
				{
					TRACE("Can't open file, error = %u\n",fileException.m_cause);
				}
			}
		}
		else
		{
          ::MessageBox(NULL,_T("创建LOG路径失败"),_T("提示"),MB_OK);
		}


	}
	return b_res;
}

/*****************************************************************************
* function name:  Setpath(CString& str_path)
* summary      :  Get current path
* Parameters   :  str_path    :(OUT)    Current path 
* Return       :  void
* Version       Author        Date                Abstract                 
* 1.0           Tom Li      2011/06/28           First version      
*****************************************************************************/
void Clog::Setpath(CString& str_path)
{
	::GetCurrentDirectory(MAX_PATH, m_sz_path);
	CTime dt_currenttm;
	CString str_tm;
	dt_currenttm = CTime::GetCurrentTime();
	str_tm.Format("%d_%02d_%02d_%02d_%02d_%02d",dt_currenttm.GetYear(),dt_currenttm.GetMonth(),dt_currenttm.GetDay(),dt_currenttm.GetHour(),dt_currenttm.GetMinute(),dt_currenttm.GetSecond());
	str_tm = str_tm + _T("_");
	str_tm = str_tm + _T("EntranceTool.log");
	str_path = _T("D:\\LOG\\EntranceTool_Log");
	str_path = str_path +  _T("\\") + str_tm;
}



/*****************************************************************************
* function name:  GetcurrentTime()
* summary      :  Get current system time
* Parameters   :  NULL
* Return       :  System time
* Version       Author        Date                Abstract                 
* 1.0           Tom Li      2011/06/28           First version      
*****************************************************************************/
CString Clog::GetcurrentTime()
{
	CTime dt_currenttm;
	CString str_tm;
	dt_currenttm = CTime::GetCurrentTime();
	str_tm.Format("%d-%02d-%02d %02d:%02d:%02d",dt_currenttm.GetYear(),dt_currenttm.GetMonth(),dt_currenttm.GetDay(),dt_currenttm.GetHour(),dt_currenttm.GetMinute(),dt_currenttm.GetSecond());
	return str_tm;
}


void Clog::LogDispose_syslog(CString str_logInfo)
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
