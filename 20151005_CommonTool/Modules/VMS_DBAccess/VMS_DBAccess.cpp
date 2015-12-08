// VMS_DBAccess.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "VMS_DBAccess.h"

#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


_ConnectionPtr       m_p_connection;       // _ConnectionPtr接口
CCriticalSection     m_obj_loadADBFileCritSection;     //single lock
CString              m_str_serverName;     // 服务器名
CString              m_str_dbName;         // 数据库名
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}


CVMS_DBAccess::CVMS_DBAccess()
{
	::CoInitialize(NULL);
}

CVMS_DBAccess::~CVMS_DBAccess()
{
	::CoUninitialize();
}

int CVMS_DBAccess::ReadIniFile(void)
{
	/* Check ini file exist */

	TCHAR szModulePath[260] = {0};
	TCHAR szVMS_DBConfigPath[260] = {0};

	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);

	_stprintf(szVMS_DBConfigPath, _T("%s\\DB\\INI\\VMS_DBConfig.ini"), szModulePath ) ;

	CString str_iniPath;
	str_iniPath.Format(_T("%s"), szVMS_DBConfigPath);

	if (_taccess(str_iniPath, 0) != 0) {
		MessageBox(NULL, str_iniPath.GetBuffer(0), "warring", NULL);
		//AfxMessageBox("can't access fonfig"); 
		//AfxMessageBox(str_iniPath.GetBuffer(0));
		
		return NG;
	}

	/* Server Name */
	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("ServerName"), _T(""), sz_buffer, sizeof(sz_buffer), szVMS_DBConfigPath);
	m_str_serverName = sz_buffer;

	/* DB Name */
	memset(sz_buffer, 0, sizeof(sz_buffer));
//	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("DBName"), _T(""), sz_buffer, sizeof(sz_buffer), szVMS_DBConfigPath);
	m_str_dbName = sz_buffer;

	return OK;
}


/*****************************************************************************
* Function name: ExecuteSQL      
* Summary      : Execute create, insert, update, delete.
* Parameters   : str_sql : sql语句
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
int ExecuteSQL(CString str_sql)
{
    CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	/*LOG*/
	DBLOG(str_sql);

	/* Check Input */
	if (str_sql == _T("")) {
		DBLOG("str_sql is empty");
		return NG;
	}

	_variant_t RecordsAffected;
	/* _ConnectionPtr Execute */
	try {
		m_p_connection->Execute((_bstr_t)str_sql, &RecordsAffected, adCmdText);
	} catch (_com_error e) {
		CString str_log;
		str_log.Format(_T("ExecuteSQL fail: %s"), LPCTSTR(e.Description()));
		DBLOG(str_log);
		/*Sunlight add retry when fail, 20111221*/
		try {
			if (!VMS_ConnectDB())
			{
				DBLOG("reconnect DB fail.");
				return NG;
			}
			DBLOG(str_sql);
			m_p_connection->Execute((_bstr_t)str_sql, &RecordsAffected, adCmdText);
		} catch (_com_error e) {
			str_log.Format("Re ExecuteSQL fail: %s", LPCTSTR(e.Description()));
			DBLOG(str_log);
			return NG;
		}
    }

	DBLOG(_T("ExecuteSQL Succeed."));
	return OK;
}


/*****************************************************************************
* Function name: GetQueryResult      
* Summary      : Execute select.
* Parameters   : str_sql     : select sql语句
*                p_recordSet : select 结果集
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/08/22          First version             
*****************************************************************************/
int GetQueryResult(CString str_sql, _RecordsetPtr & p_recordSet)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	_RecordsetPtr p_recorSet2;
	/* Check Input */
	if (str_sql == _T("")) {
		DBLOG("sql is empty");
		return NG;
	}

	CString str_log;
	str_log.Format(_T("Query DB: %s"),str_sql);
    DBLOG(str_log);

	HRESULT hr = NULL;
	hr = p_recorSet2.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr)) {
		DBLOG(_T("Query DB fail."));
		return NG;
	}

	/* 如果记录集打开的话，把它关了 */
	if (p_recorSet2->State) {
		p_recorSet2.Release();
		p_recorSet2->Close();
	}

	/* _RecordsetPtr Open */
	try {
		p_recorSet2->Open(_variant_t(str_sql),           // 查询表中所有字段
		_variant_t((IDispatch *)m_p_connection,true),
		adOpenDynamic,                                 
		adLockReadOnly,          
		adCmdText);                                        // adCmdText：文本查询语句
	} catch (_com_error e) {
		str_log.Format(_T("Query DB fail: %s"), LPCTSTR(e.Description()));
		DBLOG(str_log);
		/*Sunlight add 20111122, for fail retry*/
		//return NG;
		hr = NULL;
		if (!VMS_ConnectDB())
		{
			DBLOG("reconnect DB fail.");
			return NG;
		}
		hr = p_recorSet2.CreateInstance(_T("ADODB.Recordset"));
		if (FAILED(hr)) {
			DBLOG("Re CreateInstance ADODB.Recordset fail.");
			return NG;
		}
		try {
			p_recorSet2->Open(_variant_t(str_sql),      
				_variant_t((IDispatch *)m_p_connection,true),
				adOpenDynamic,                                 
				adLockReadOnly,          
				adCmdText);                                   
		} catch (_com_error e) {
			str_log.Empty();
			str_log.Format("Re Query DB fail: %s", (char*)(e.Description()));
			DBLOG(str_log);
			return NG;
		}
	}
	/*Sunlight add end*/

	p_recordSet = p_recorSet2;
    
//	DBLOG(_T("Query DB Succeed")); //nancy delete on 20130510
	return OK;
}


/*****************************************************************************
* Function name: DBLOG      
* Summary      : Write log.
* Parameters   : void
* Return       : void
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/22          First version             
*****************************************************************************/
void DBLOG(CString str_message)
{
	/* Check Input valid */
	if (str_message == _T("")) {
		return;
	}

	/* Get current time */
	SYSTEMTIME time;
	::GetLocalTime(&time);

	/* Get current app directory */
	TCHAR sz_currentDir[256];
	memset(sz_currentDir, 0, sizeof(sz_currentDir));
	::GetCurrentDirectory(sizeof(sz_currentDir), sz_currentDir);
	CString str_currentPath;
	str_currentPath.Format(_T("%s"), sz_currentDir);

	/* Check LOG\\DB_Log exist and create */
	CString str_logDir;
//	str_logDir = str_currentPath + _T("\\DB");
	str_logDir = "D:\\LOG";

	WIN32_FIND_DATA fd_fileInformation;
	HANDLE h_searchFind;
	h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
    if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		FindClose(h_searchFind); 
    } else {
		FindClose(h_searchFind ); 
		if (CreateDirectory(str_logDir, NULL) == 0) {
			return;
		} 
	} 

	str_logDir = str_logDir + _T("\\VMS_Log");
	h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
    if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		FindClose(h_searchFind); 
    } else {
		FindClose(h_searchFind ); 
		if (CreateDirectory(str_logDir, NULL) == 0) {
			return;
		} 
	} 

	/* Open log file */
	CStdioFile obj_file;
    CString str_logFile;
	CString str_temp;

	str_temp.Format(_T("[%02d:%02d:%02d:%03d]:  %s "),time.wHour, time.wMinute, time.wSecond,time.wMilliseconds,str_message);
	str_logFile.Format(_T("%s\\%04d_%02d_%02d.log"),str_logDir, time.wYear, time.wMonth, time.wDay);

	if (obj_file.Open(str_logFile, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL)) {
		obj_file.SeekToEnd();
		obj_file.WriteString(str_temp);
		obj_file.WriteString(_T("\r\n"));
		obj_file.Close();
	}

	return;
}


/*****************************************************************************
* Function name: ConnectDB      
* Summary      : Connect DB.
* Parameters   : void
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
bool VMS_ConnectDB()
{
	CVMS_DBAccess obj_dbAccess;
	/* Read ini File */
	if (obj_dbAccess.ReadIniFile() != OK) {
		CString str_log;
		str_log.Format(_T("Read %s fail QQ."), DBCONFIGFILE);
		DBLOG(str_log);
		return false;
	}

	HRESULT hr = NULL;
	try {
		// 创建连接对象实例
		hr = m_p_connection.CreateInstance(_T("ADODB.Connection"));
		if (SUCCEEDED(hr)) {
			CString str_connect;
			str_connect  = _T("Provider=SQLOLEDB.1;");
            str_connect += _T("Persist Security Info=True;");
            str_connect += _T("Initial Catalog=") + m_str_dbName + _T(";");
			str_connect += _T("User ID=qtyrd;");
			str_connect += _T("Password=qtyrd;");
			str_connect += _T("Data Source=") + m_str_serverName + _T(",1433");

			/*  设置超时时间为15秒  */
			m_p_connection->ConnectionTimeout = 15;

			DBLOG("****************************************** S T A R T ******************************************************************");
			DBLOG(m_str_serverName);

    		hr = m_p_connection->Open(_bstr_t(str_connect),_T(""), _T(""), adModeUnknown);
			if (FAILED(hr)) {
				m_p_connection->Close();
				DBLOG(_T("Connect DB fail"));
				return false;
			}
		} else {
			DBLOG(_T("Connect DB fail"));
			return false;
		}
	} catch (_com_error e) {
		CString str_log;
		str_log.Format(_T("Connect DB fail: %s"), LPCTSTR(e.Description()));
		DBLOG(str_log);
		return false;
	} 

	DBLOG(_T("Connect DB Succeed."));
	return true;
}


/*****************************************************************************
* Function name: CloseDB      
* Summary      : Close DB.
* Parameters   : void
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
void VMS_CloseDB(void)
{
	try {
		if (m_p_connection->State) {
			m_p_connection->Close();
			m_p_connection.Release();
		}
		m_p_connection = NULL;

	} catch (_com_error e) {
		CString str_log;
		str_log.Format(_T("Close DB connection fail: %s"), LPCTSTR(e.Description()));
		DBLOG(str_log);
	}

    DBLOG(_T("Close DB success"));
	return;
}

/*****************************************************************************
* Function name: VMS_QueryImageStatus
* Summary      : Query image version status
* Parameters   : input/Model, imageVer; output/status, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
bool VMS_QueryFactoryImageStatus(char * sz_Model, char * sz_ImageVer,char * sz_Status, char * sz_starttime, char * sz_endtime,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_SW = sz_ImageVer;
	CString str_Temp = _T("");
	CString str_modelID =_T("");


	char sz_modelID[10] = {0};
	char sz_projID[10] = {0};
	if (!VMS_QueryModelID(sz_Model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	str_modelID = sz_modelID;
	/* Execute DB*/
    str_sql = _T("select * from image_version_control where img_version = '"+str_SW+"' and model_id like '%"+str_modelID+"%'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query image version fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("status"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_Status, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("start_time"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_starttime, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("end_time"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_endtime, _T("%s"), str_Temp);
		}

        if( (strcmp(sz_Status,_T("2")) ==0)  || (strcmp(sz_Status,_T("3"))==0) )
        {
			strcpy(sz_ErrCode, ERROR_SWSTATUS_INVALID);
			sprintf(sz_ErrMsg,_T("Image status is invalid\n软件版本%s对应的状态%s无效"),str_SW, sz_Status);
			DBLOG(sz_ErrMsg);
			p_recordSet->Close();
			return false;
        }
		else
		{
			p_recordSet->Close();
			return true;
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTSW_EMPTY);
		sprintf(sz_ErrMsg,("Invalid SW image \n软件版本:%s对应%s无效,请确认"),str_SW,sz_Model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryToolStatus
* Summary      : Query tool status
* Parameters   : input model/tool ver; output/image ver, status
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version   
* 1.1          Sunlight.Qian     2012/04/24          Second version  , for unify_UI model query 
*****************************************************************************/
bool VMS_QueryToolStatus(char * sz_Model, char * sz_ToolVer,char * sz_ImageVer,char * sz_Status, char * sz_Reserve1,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_ToolVer = sz_ToolVer;
	CString str_Temp = _T("");
	CString str_modelID =_T("");

	char sz_modelID[10] = {0};
	char sz_projID[10] = {0};

	if (!VMS_QueryModelID(sz_Model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}
	
	str_modelID = sz_modelID;
	str_sql = _T("select * from tool_version_control where tool_version = '"+str_ToolVer+"' and model_ids like '%"+str_modelID+"%'");
	/* Execute DB*/
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query image version fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("status"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_Status, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("img_versions"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_ImageVer, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("reserve1_value"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_Reserve1, _T("%s"), str_Temp);
		}

        if( (strcmp(sz_Status,_T("2")) ==0)  || (strcmp(sz_Status,_T("3"))==0) )
        {
			strcpy(sz_ErrCode, ERROR_TOOLSTATUS_INVALID);
			sprintf(sz_ErrMsg,_T("Image status is invalid\nTOOL版本:%s对应的状态%s无效"),str_ToolVer, sz_Status);
			DBLOG(sz_ErrMsg);
			p_recordSet->Close();
			return false;
        }
		else
		{
			p_recordSet->Close();
			return true;
		}

	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTTOOL_EMPTY);
		sprintf(sz_ErrMsg,("Invalid TOOL version \nTOOL版本:%s对应%s无记录,请确认"),str_ToolVer,sz_Model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QuerySQNinfo
* Summary      : Check image SQN status
* Parameters   : input model/part no; output/SQN_id, startTime, endTime
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
bool VMS_QuerySQNinfo(char * sz_Model, char * sz_Partno, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_Partno = sz_Partno;
	CString str_model = sz_Model;

	CString str_Temp = _T("");
	CString str_SQNid = _T("");

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

    /*************************** Execute DB, query SQN id, SQN start time and end time by part no***************************/
	str_sql = _T("select * from SQN_PART where part_no='"+str_Partno+"' order by SQN_id desc");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query SQN_part fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect(_T("SQN_id"));
		if (thevalue.vt != VT_NULL)
		{
			str_SQNid = (char *)_bstr_t(thevalue);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("start_datetime"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(st_PNinfo.SQN_starttime, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("end_datetime"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(st_PNinfo.SQN_endtime, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYPN_INSQN);
		sprintf(sz_ErrMsg,_T("Query SQN_part by :%s fail\n查询此料号对应的SQN信息失败"),str_Partno);
		DBLOG(sz_ErrMsg);
		return false;
	}

    /*************************** query SQN detail info by SQN_id **********************************/
	str_sql = _T("select * from SQN where SQN_id = '"+str_SQNid+"' and model='"+str_model+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query SQN fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		DBLOG(sz_ErrMsg);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("SW_version"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(st_PNinfo.SW_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("variant_no"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.variant_no, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("region"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.region, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("OP_channel"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.OP_channel, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("SD_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.SD_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("user_tool_name"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.user_tool_name, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("RC_check"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.RC_check, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("RC_filename"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.RC_filename, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("modem_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.modem_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("OS_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.OS_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("touch_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.touch_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("gauge_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.gauge_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("DFI_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.DFI_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("lock_filename"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.lock_filename, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("FW_version"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.FW_version, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("SQN_release_status"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			st_PNinfo.SQN_status = atoi(str_Temp);
		}

	    if (st_PNinfo.SQN_status == 3)
		{
			strcpy(sz_ErrCode, ERROR_SQNSTATUS_INVALID);
			sprintf(sz_ErrMsg,_T("SQN status is invalid\n此SQNID:%s对应的SQN状态%d无效"),str_SQNid, st_PNinfo.SQN_status);
			DBLOG(sz_ErrMsg);
			p_recordSet->Close();
			return false;
		}

	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTSQNID_EMPTY);
		sprintf(sz_ErrMsg,("Invalid SQN id\n此SQNID:%s无SQN记录,请确认"),str_SQNid);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* **************************query 9J PN info *****************************/
	str_sql = _T("select * from pn_profile where pn = '"+str_Partno+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query partno profile fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("band"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(st_PNinfo.band, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("SIM_lock"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.SIM_lock, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("SIM_included"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.SIM_inclueded, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("IMEI_tag"));	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(st_PNinfo.IMEI_tag, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTPN_EMPTY);
		sprintf(sz_ErrMsg,("Invalid PN\n此PN:%s无profile记录,请确认"),str_Partno);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}


	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}



/*****************************************************************************
* Function name: VMS_Query5Einfo
* Summary      : Query the info about 5E from 5E_control table
* Parameters   : input model/part no; output/sz_5Einfo
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
bool VMS_Query5Einfo(char * sz_Model, char * sz_partNo, char * sz_count,char * sz_5Einfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	std::map<CString, CString>  map_value;             // 5X reserve value
	std::map<CString, CString>  map_matching;         // 5X reserve Matching Table
	std::map<CString,CString>::iterator Itr;
	std::map<CString,CString>::iterator Itrm;

	int i_vcount = 0, i_mcount = 0,i_dot = 0;
	CString str_Temp = _T("");
	CString str_Info = _T("");

	/* Query model ID by model */
	char sz_modelID [5];
	char sz_projID [5];
	if (!VMS_QueryModelID(sz_Model,sz_modelID,sz_projID,sz_ErrCode,sz_ErrMsg))
	{
		return false;
	}

	/* select 5E_reserve table to get matching table  */
	if(!VMS_QueryReserveTable(sz_projID, "5E_reserve", map_matching ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

    /* query 5E control detail info  */
	if (!VMS_QueryControlTable(sz_partNo,sz_modelID,"5E_control",map_value ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	//sunlight mark 20111226, VMS_QueryControlTable add the query condition status=1
	/*if (map_value[_T("status")]  == _T("2"))
	{
		strcpy(sz_ErrCode, ERROR_5ESTATUS_INVALID);
		sprintf(sz_ErrMsg,_T("5E PN status is invalid\n此PN:%s对应的状态%s无效"),sz_partNo,map_value[_T("status")]);
		DBLOG(sz_ErrMsg);
		return false;
	}*/

	/* deal with data  */
	for(Itr = map_value.begin(),i_vcount=1; Itr != map_value.end(),i_vcount <= (int)map_value.size(); Itr++,i_vcount++)
	{
		i_dot = 0;

        str_Temp = Itr->first;

		i_dot = str_Temp.Find(_T("_value"));
	    if(i_dot != -1)
		{
			str_Temp = str_Temp.Left(i_dot);
			if (map_matching[str_Temp] != _T(""))
			{
				str_Info += map_matching[str_Temp];
			}
			else
			{
				str_Info += Itr->first;
			}
		}
		else
		{
            str_Info += Itr->first;
		}

		str_Info += ",";

		str_Info += Itr->second;
		if (i_vcount != map_value.size())
		{
			str_Info += ";";
		}
	}
    sprintf(sz_count,"%d",i_vcount-1);
	sprintf(sz_5Einfo,"%s",str_Info);
	return true;
}

/*****************************************************************************
* Function name: VMS_Query5Ginfo
* Summary      : Query the info about 5G from 5G_control table
* Parameters   : input model/part no; output/sz_5Ginfo
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/22          First version             
*****************************************************************************/
bool VMS_Query5Ginfo(char * sz_model, char * sz_partNo, char * sz_count, char * sz_5Ginfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	std::map<CString, CString>  map_value;             // 5X reserve value
	std::map<CString, CString>  map_matching;         // 5X reserve Matching Table
	std::map<CString,CString>::iterator Itr;
	std::map<CString,CString>::iterator Itrm;

	int i_vcount = 0, i_mcount = 0,i_dot = 0;
	CString str_Temp = _T("");
	CString str_Info = _T("");

	/* Query model ID by model */
	char sz_modelID [5];
	char sz_projID [5];
	if (!VMS_QueryModelID(sz_model,sz_modelID,sz_projID,sz_ErrCode,sz_ErrMsg))
	{
		return false;
	}

	/* select 5G_reserve table to get matching table  */
	if(!VMS_QueryReserveTable(sz_projID, "5G_reserve", map_matching ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	/* query 5G_control detail info  */
	if (!VMS_QueryControlTable(sz_partNo,sz_modelID,"5G_control",map_value ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	//sunlight mark 20111226, VMS_QueryControlTable add the query condition status=1
	//if (map_value[_T("status")]  == _T("2"))
	//{
	//	strcpy(sz_ErrCode, ERROR_5GSTATUS_INVALID);
	//	sprintf(sz_ErrMsg,_T("5G PN status is invalid\n此PN:%s对应的状态%s无效"),sz_partNo,map_value[_T("status")]);
	//	DBLOG(sz_ErrMsg);
	//	return false;
	//}

	/* deal with data  */
	for(Itr = map_value.begin(),i_vcount=1; Itr != map_value.end(),i_vcount <= (int)map_value.size(); Itr++,i_vcount++)
	{
		i_dot = 0;

		str_Temp = Itr->first;

		i_dot = str_Temp.Find(_T("_value"));
		if(i_dot != -1)
		{
			str_Temp = str_Temp.Left(i_dot);
			if (map_matching[str_Temp] != _T(""))
			{
				str_Info += map_matching[str_Temp];
			}
			else
			{
				str_Info += Itr->first;
			}
		}
		else
		{
			str_Info += Itr->first;
		}

		str_Info += ",";

		str_Info += Itr->second;
		if (i_vcount != map_value.size())
		{
			str_Info += ";";
		}
	}
	sprintf(sz_count,"%d",i_vcount-1);
	sprintf(sz_5Ginfo,"%s",str_Info);
	return true;
}



/*****************************************************************************
* Function name: VMS_QueryModelID
* Summary      : Query modelid by model name
* Parameters   : input/Model, output/modelid, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/26        First version             
*****************************************************************************/
bool VMS_QueryModelID(char * sz_model, char * sz_modelID, char * sz_projID,char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_model = sz_model;
	CString str_Temp = _T("");


	str_sql = _T("select * from model_list where model_name = '"+str_model+"'");

	/* Execute DB*/
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("model_id"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_modelID, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("proj_id"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_projID, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYMODELID_EMPTY);
		sprintf(sz_ErrMsg,("Can't found model ID for this model\n机种:%s找不到其对应的ID,请确认"),str_model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: VMS_QueryControlTabel
* Summary      : Query control table info
* Parameters   : input/ModelID,part no, table; output/map_value, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/26        First version
* 1.1          Sunlihgt.Qian     2011/12/27        Second version , if no record still return true
*****************************************************************************/
bool VMS_QueryControlTable(char * sz_partNo, char * sz_modelID, char * sz_table,std::map<CString, CString> & map_value ,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0; 

	CString str_sql;
	CString str_partNo = sz_partNo;
	CString str_modelID = sz_modelID;
    CString str_table = sz_table;
	CString str_Temp = _T("");

	/*************************** query SQN detail info by SQN_id **********************************/
	str_sql = _T("select * from ["+str_table+"] where PN = '"+str_partNo+"' and model_id='"+str_modelID+"' and status=1");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		ColCount = 0;
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			DBLOG(sz_ErrMsg);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{			
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集//中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_value[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
	}
	else
	{
//		strcpy(sz_ErrCode, ERROR_PNCONTROL_EMPTY);
		sprintf(sz_ErrMsg,("Invalid PN, SQL:\n此料号:%s对应的控制信息无记录,请确认"),str_sql,str_partNo);
		DBLOG(sz_ErrMsg);
//		p_recordSet->Close();
//		return false;
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryReserveTabel
* Summary      : Query 5E/5G reserve table info
* Parameters   : input/proj id, table; output/map_value, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/08/26        First version 
* 1.1          Sunlihgt.Qian     2011/12/27        Second version , if no record still return true
*****************************************************************************/
bool VMS_QueryReserveTable(char * sz_projID, char * sz_table, std::map<CString, CString> & map_matching ,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue, filedvalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0; 

	CString str_sql;
	CString str_table = sz_table;
	CString str_projID = sz_projID;
	CString str_Temp = _T("");

	/*************************** query SQN detail info by SQN_id **********************************/
	str_sql = _T("select * from ["+str_table+"] where proj_id = '"+str_projID+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query reserve matching fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}
	else if (!p_recordSet->GetadoEOF())
	{
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get 5E reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_matching[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针

	}
	else
	{ 
//		strcpy(sz_ErrCode, ERROR_RESERVE_EMPTY);
		sprintf(sz_ErrMsg,_T("Query reserve record is empty,SQL:%s \n查不到机种:%s对应的栏位定义记录"),str_sql,str_projID);
		DBLOG(sz_ErrMsg);
//		p_recordSet->Close();
//		return false;  
	}

	p_recordSet->Close();
	return true;

}




/*****************************************************************************
* Function name: VMS_QueryProjectName
* Summary      : Query Project Name by model name
* Parameters   : input/Model name, output/project name, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/09/6        First version             
*****************************************************************************/
bool VMS_QueryProjectName(char * sz_model, char * sz_proj,char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_model = sz_model;
	CString str_Temp = _T("");
	CString str_ProjID = _T("");

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	/* query project ID */
	str_sql = _T("SELECT * FROM proj_list INNER JOIN model_list ON model_list.proj_id = proj_list.proj_id WHERE model_list.model_name= '"+str_model+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("proj_name"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_proj, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYPROJ_EMPTY);
		sprintf(sz_ErrMsg,("Can't found project name for this model\n此机种:%s找不到其对应的项目,请确认"),str_model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryPartNumber
* Summary      : Query part number list by model name
* Parameters   : input/Model name, output/part number list,PN count error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/09/9        First version             
*****************************************************************************/
bool VMS_QueryPartNumber(char * sz_model, char * sz_partnumber,char * sz_Count,char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_model = sz_model;
	CString str_Temp = _T("");
	CString str_PN = _T("");
	int i_count = 0;

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	/* query project ID */
	str_sql = _T("SELECT * FROM sqn_part INNER JOIN sqn ON sqn.sqn_id = sqn_part.sqn_id WHERE sqn.model= '"+str_model+"' and sqn.SQN_release_status <> 3");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query part number fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	while( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("part_no"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
		}
         
		str_PN += str_Temp;
		str_PN += ",";
		i_count ++;

		p_recordSet->MoveNext();

	}
    
	sprintf(sz_partnumber,"%s",str_PN);
	sprintf(sz_Count,"%d",i_count);

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QuerySQNinfo_V2
* Summary      : Check image SQN status by model+PN+SW version
* Parameters   : input model/part no/sw ver; output/SQN_id, startTime, endTime
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/09/21          First version 
* 1.1          Sunlight.Qian     2012/06/29         Second version, add the starttime and endtime in select cofigurate
*****************************************************************************/
bool VMS_QuerySQNinfo_V2(char * sz_Model, char * sz_Partno, char * sz_SWver, st_9JPNData & st_PNinfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_Partno = sz_Partno;
	CString str_model = sz_Model;
	CString str_swVer = sz_SWver;

	CString str_Temp = _T("");
	CString str_SQNid = _T("");
	CString str_CurrTime = _T("");

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	/*************************** Execute DB, query SQN id, SQN start time and end time by part no***************************/
	str_sql = _T("select * from SQN, SQN_part WHERE  SQN.SQN_id=SQN_part.SQN_id and SQN_part.part_no='"+str_Partno+"' and SQN.model='"+str_model+"' and SQN.SW_version='"+str_swVer+"' and SQN_part.start_datetime <=GETDATE() and SQN_part.end_datetime >=GETDATE() order by SQN.sqn_id DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query SQN_part fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	/*SYSTEMTIME time;
	::GetLocalTime(&time);

	str_CurrTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);*/

	COleDateTime tm_begin, tm_end, tm_now;
	tm_now = COleDateTime::GetCurrentTime();


	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("start_datetime"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(st_PNinfo.SQN_starttime, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("end_datetime"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(st_PNinfo.SQN_endtime, _T("%s"), str_Temp);
			}

			tm_begin.ParseDateTime(st_PNinfo.SQN_starttime);
			tm_end.ParseDateTime(st_PNinfo.SQN_endtime);

			if (tm_now > tm_end || tm_now < tm_begin)
			{
				strcpy(sz_ErrCode, ERROR_SQN_OUTTIME);
				sprintf(sz_ErrMsg,_T("Check SQN term of validity fail\nSQN已经过期%s~%s，请确认"),st_PNinfo.SQN_starttime,st_PNinfo.SQN_endtime);
				return false;
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("SW_version"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(st_PNinfo.SW_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("variant_no"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.variant_no, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("region"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.region, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("OP_channel"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.OP_channel, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("SD_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.SD_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("user_tool_name"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.user_tool_name, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("RC_check"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.RC_check, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("RC_filename"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.RC_filename, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("modem_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.modem_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("OS_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.OS_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("touch_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.touch_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("gauge_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.gauge_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("DFI_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.DFI_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("lock_filename"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.lock_filename, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("FW_version"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.FW_version, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("SQN_release_status"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				st_PNinfo.SQN_status = atoi(str_Temp);
			}
		}catch (_com_error e) {
			strcpy(sz_ErrCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_ErrMsg,("Get SQN,SQN_part filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_ErrMsg);
			return false;
		}

		if (st_PNinfo.SQN_status == 3)
		{
			strcpy(sz_ErrCode, ERROR_SQNSTATUS_INVALID);
			sprintf(sz_ErrMsg,_T("SQN status is invalid\n此SQNID:%s对应的SQN状态%d无效"),str_SQNid, st_PNinfo.SQN_status);
			DBLOG(sz_ErrMsg);
			p_recordSet->Close();
			return false;
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTSQNID_EMPTY);
		sprintf(sz_ErrMsg,("Invalid SQN, SQL=%s \n查询无此SQN记录,请确认"),str_sql);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* **************************query 9J PN info *****************************/
	str_sql = _T("select * from pn_profile where pn = '"+str_Partno+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query partno profile fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("band"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(st_PNinfo.band, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("SIM_lock"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.SIM_lock, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("SIM_included"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.SIM_inclueded, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("IMEI_tag"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.IMEI_tag, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("CERT"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(st_PNinfo.CERT, _T("%s"), str_Temp);
			}

		}catch (_com_error e) {
			strcpy(sz_ErrCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_ErrMsg,("Get pn_profile filed value fail:%s\n栏位值抓取异常"),LPCTSTR(e.Description()));
			DBLOG(sz_ErrMsg);
			return false;
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTPN_EMPTY);
		sprintf(sz_ErrMsg,("Invalid PN\n此料号:%s无profile记录,请确认"),str_Partno);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}


	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}



/*****************************************************************************
* Function name: VMS_Query9Jinfo
* Summary      : Query the info about 9J/CS from pn_control table
* Parameters   : input model/part no; output/sz_9Jinfo
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/12/26          First version   
* 1.1          Sunlight.Qian     2012/06/29        Third version, add pnID for select
*****************************************************************************/
bool VMS_Query9Jinfo(char * sz_model, char * sz_partNo, char * sz_pnID,char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	std::map<CString, CString>  map_value;             // 9X reserve value
	std::map<CString, CString>  map_matching;         // 9X reserve Matching Table
	std::map<CString,CString>::iterator Itr;
	std::map<CString,CString>::iterator Itrm;

	int i_vcount = 0, i_mcount = 0,i_dot = 0;
	CString str_Temp = _T("");
	CString str_Info = _T("");

	/* Query model ID by model */
	char sz_modelID [5];
	char sz_projID [5];
	if (!VMS_QueryModelID(sz_model,sz_modelID,sz_projID,sz_ErrCode,sz_ErrMsg))
	{
		return false;
	}

	/* select pn_reserve table to get matching table  */
	if(!VMS_QueryPNReserveTable(sz_pnID, "pn_reserve",sz_partNo ,map_matching ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	/* query pn_control detail info  */
	if (!VMS_QueryPNControlTable(sz_pnID,sz_modelID,"pn_control",map_value ,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	/* deal with data  */
	for(Itr = map_value.begin(),i_vcount=1; Itr != map_value.end(),i_vcount <= (int)map_value.size(); Itr++,i_vcount++)
	{
		i_dot = 0;

		str_Temp = Itr->first;

		i_dot = str_Temp.Find(_T("_value"));
		if(i_dot != -1)
		{
			str_Temp = str_Temp.Left(i_dot);
			if (map_matching[str_Temp] != _T(""))
			{
				str_Info += map_matching[str_Temp];
			}
			else
			{
				str_Info += Itr->first;
			}
		}
		else
		{
			str_Info += Itr->first;
		}

		str_Info += ",";

		str_Info += Itr->second;
		if (i_vcount != map_value.size())
		{
			str_Info += ";";
		}
	}
	sprintf(sz_count,"%d",i_vcount-1);
	sprintf(sz_9Jinfo,"%s",str_Info);
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryPNReserveTabel
* Summary      : Query PN reserve table info
* Parameters   : input/proj id, table; output/map_value, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/12/26        First version      
* 1.1          Sunlihgt.Qian     2011/12/27        Second version , if no record still return true
* 1.2          Sunlight.Qian     2012/06/29        Third version, change projID to pnID for select
*****************************************************************************/
bool VMS_QueryPNReserveTable(char * sz_pnID, char * sz_table,char * sz_PN, std::map<CString, CString> & map_matching ,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue, filedvalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0; 

	CString str_sql;
	CString str_table = sz_table;
	CString str_pnID = sz_pnID;
	CString str_pn = sz_PN;
	CString str_Temp = _T("");

	/*************************** query SQN detail info by SQN_id **********************************/
	str_sql = _T("select * from ["+str_table+"] where PN_id = '"+str_pnID+"' and pn='"+str_pn+"' and status=1");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query PN reserve matching fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}
	else if (!p_recordSet->GetadoEOF())
	{
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get PN reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_matching[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针

	}
	else
	{ 
//		strcpy(sz_ErrCode, ERROR_RESERVE_EMPTY);
		sprintf(sz_ErrMsg,_T("Query reserve record is empty,SQL:%s \n查询不到PN:%s对应的栏位定义记录"),str_sql,str_pn);
		DBLOG(sz_ErrMsg);
//		p_recordSet->Close();
//		return false;
	}

	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: VMS_QueryPNControlTabel
* Summary      : Query control table info
* Parameters   : input/ModelID,part no, table; output/map_value, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/12/27        First version       
* 1.1          Sunlihgt.Qian     2011/12/27        Second version , if no record still return true
*****************************************************************************/
bool VMS_QueryPNControlTable(char * sz_partID, char * sz_modelID, char * sz_table,std::map<CString, CString> & map_value ,char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0; 

	CString str_sql;
	CString str_partID = sz_partID;
	CString str_modelID = sz_modelID;
    CString str_table = sz_table;
	CString str_Temp = _T("");

	/*************************** query SQN detail info by SQN_id **********************************/
	str_sql = _T("select * from ["+str_table+"] where PN_id = '"+str_partID+"' and model_id='"+str_modelID+"' and status=1");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		ColCount = 0;
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			DBLOG(sz_ErrMsg);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{			
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集//中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_value[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
	}
	else
	{
//		strcpy(sz_ErrCode, ERROR_PNCONTROL_EMPTY);
		sprintf(sz_ErrMsg,("Invalid PN, SQL:\n此料号PN:%s无控制信息记录,请确认"),str_sql,str_partID);
		DBLOG(sz_ErrMsg);
//		p_recordSet->Close();
//		return false;
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QuerySQNinfo_All
* Summary      : Check image SQN status by model+PN+SW version
* Parameters   : input model/part no/sw ver; output/SQN_id, startTime, endTime
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/12/26         First version   
* 1.1          Sunlight.Qian     2012/06/29         Second version, add the starttime and endtime in select cofigurate
*****************************************************************************/
bool VMS_QuerySQNinfo_All(char * sz_Model, char * sz_Partno, char * sz_SWver, char * sz_count, char * sz_9Jinfo, char * sz_ErrCode, char * sz_ErrMsg )
{
	CString str_sql;
	CString str_Partno = sz_Partno;
	CString str_model = sz_Model;
	CString str_swVer = sz_SWver;

	CString str_Temp = _T("");
	CString str_TempInfo = _T("");

	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue, filedvalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0;
	int           i_totalcount=0; 

	std::map<CString, CString>  map_value; 
	std::map<CString,CString>::iterator Itr;

	char sz_tempCount[10] ={0};
	char sz_tempInfo[4096] = {0};
	char sz_pnID[10] = {0};

	/*************************** Execute DB, query SQN id, SQN start time and end time by part no***************************/
	str_sql = _T("select * from SQN, SQN_part WHERE  SQN.SQN_id=SQN_part.SQN_id and SQN_part.part_no='"+str_Partno+"' and SQN.model='"+str_model+"' and SQN.SW_version='"+str_swVer+"' and SQN_part.start_datetime <=GETDATE() and SQN_part.end_datetime >=GETDATE() and SQN.SQN_release_status<>3 order by SQN.sqn_id DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query SQN_part fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}
	else if (!p_recordSet->GetadoEOF())
	{
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get PN reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_value[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
	}
	else
	{ 
		strcpy(sz_ErrCode, ERROR_INPUTSQNID_EMPTY);
		sprintf(sz_ErrMsg,("Invalid SQN, SQL=%s \n查询无此SQN记录,请确认"),str_sql);
		DBLOG(sz_ErrMsg);
		return false;
	}


	//COleDateTime tm_begin, tm_end, tm_now;
	//tm_now = COleDateTime::GetCurrentTime();

	//tm_begin.ParseDateTime(map_value[_T("start_datetime")]);
	//tm_end.ParseDateTime(map_value[_T("end_datetime")]);

	//if (tm_now > tm_end || tm_now < tm_begin)
	//{
	//	strcpy(sz_ErrCode, ERROR_SQN_OUTTIME);
	//	sprintf(sz_ErrMsg,_T("Check SQN term of validity fail\nSQN已经过期%s~%s，请确认"),map_value[_T("start_datetime")],map_value[_T("end_datetime")]);
	//	return false;
	//}

	//if (atoi(map_value[_T("SQN_release_status")]) == 3)
	//{
	//	strcpy(sz_ErrCode, ERROR_SQNSTATUS_INVALID);
	//	sprintf(sz_ErrMsg,_T("SQN status is invalid\n此软件版本:%s对应的SQN状态无效"),sz_SWver);
	//	DBLOG(sz_ErrMsg);
	//	p_recordSet->Close();
	//	return false;
	//}

    i_totalcount = ColCount;
	str_TempInfo.Empty();
	for(Itr = map_value.begin(); Itr != map_value.end(); Itr++)
	{
		str_TempInfo += Itr->first;
		str_TempInfo += ",";
		str_TempInfo += Itr->second;
		str_TempInfo += ";";
	}

	map_value.clear();

	/* **************************query 9J PN_profile *****************************/
	ColCount=0;
	str_sql = _T("select * from pn_profile where pn = '"+str_Partno+"' and status='approved' order by id DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query partno profile fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}
	else if ( !p_recordSet -> GetadoEOF() )
	{
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get PN reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				map_value[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTPN_EMPTY);
		sprintf(sz_ErrMsg,("Invalid PN\n此料号:%s无profile记录,请确认"),str_Partno);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	i_totalcount += ColCount;

	for(Itr = map_value.begin(); Itr != map_value.end(); Itr++)
	{
		str_TempInfo += Itr->first;
		str_TempInfo += ",";
		str_TempInfo += Itr->second;
		str_TempInfo += ";";
	}

	sprintf(sz_pnID,"%s",map_value[_T("id")]);
	/* ************************query 9J PN info*****************************/
	if (!VMS_Query9Jinfo(sz_Model,sz_Partno, sz_pnID,sz_tempCount, sz_tempInfo,sz_ErrCode,sz_ErrMsg ))
	{
		return false;
	}
	else
	{
		i_totalcount += atoi(sz_tempCount);
		sprintf(sz_count,"%d", i_totalcount);
		sprintf(sz_9Jinfo,"%s%s",str_TempInfo,sz_tempInfo);
	}
	/* Disconnect DB */
	p_recordSet->Close();

	return true;

}



/*****************************************************************************
* Function name: VMS_QueryCommonToolVersion
* Summary      : Query common tool version
* Parameters   : input model; output/tool ver, status
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2012/03/16        First version    
* 1.1          Sunlight.Qian     2012/04/25        Second version, common tool version get under by SO type             
*****************************************************************************/
bool VMS_QueryCommonToolVersion(char * sz_Model, char * sz_CRtype,char * sz_ToolVer,char * sz_Status, char * sz_ErrCode, char * sz_ErrMsg )
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp = _T("");
	CString str_modelID =_T("");

	char sz_modelID[10] = {0};
	char sz_projID[10] = {0};

	if (!VMS_QueryModelID(sz_Model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}
	
	str_modelID = sz_modelID;

	if (strncmp(sz_CRtype,"",1) == 0)
	{
		str_sql = _T("select * from tool_version_control where model_ids like '%"+str_modelID+"%' and status=1 order by tool_version desc");
	}
	else
	     str_sql = _T("select * from tool_version_control where model_ids like '%"+str_modelID+"%' and status<>2 and status <>3 order by tool_version desc");
	
	/* Execute DB*/
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query image version fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("status"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_Status, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("tool_version"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_ToolVer, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_COMMONTOOL_EMPTY);
		sprintf(sz_ErrMsg,("Invalid TOOL version \n%s没有有效的TOOL版本记录,请确认"),sz_ToolVer,sz_Model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryModelProjectList
* Summary      : Query model and project list from VMS
* Parameters   : output/model_project, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2014/04/24        First version             
*****************************************************************************/
bool VMS_QueryModelProjectList(char * sz_modelprojList,char * sz_count,char * sz_ErrCode, char * sz_ErrMsg)
{
	CString str_sql = _T("");
	CString str_Temp = _T("");
	CString str_ListTemp = _T("");

	int i_count = 0;


	str_sql = _T("select proj_name, model_name from proj_list,model_list where model_list.proj_id=proj_list.proj_id");

	/* Execute DB*/
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("model_name"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
		}

		str_ListTemp += str_Temp;
		str_ListTemp += _T("_");

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("proj_name"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
		}
		str_ListTemp += str_Temp;
		str_ListTemp += _T(",");

		i_count++;

		p_recordSet->MoveNext();
	}

	sprintf(sz_count,"%d",i_count);
    sprintf(sz_modelprojList,"%s",str_ListTemp);
	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryCRImageListByModel
* Summary      : Query CR image lsit from VMS
* Parameters   : output/CR image list, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          luffy.liu     2012/06/11            First version             
*****************************************************************************/
bool VMS_QueryCRImageList(char * sz_model,char * sz_imageList,bool b_CR, char * sz_ErrCode, char * sz_ErrMsg)
{
	CString str_sql = _T("");
	CString str_sql0 = _T("");
	CString str_sql1 = _T("");
	CString str_Temp = _T("");
	CString str_ListTemp = _T("");
	CString str_model = _T("");
	str_model = sz_model;

	str_sql = _T("select distinct img_version from image_version_control where proj_id in (select proj_id from model_list where model_name='" + str_model + "') and status =" );

	//verify
	str_sql0 = str_sql + _T("'0'");

	//released
	str_sql1 = str_sql + _T("'1'");
	/* Execute DB*/
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	//get CR image list if so is CR
	if (b_CR)
	{
		if(!GetQueryResult(str_sql0, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		while (!p_recordSet -> GetadoEOF())
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("img_version"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				str_ListTemp += str_Temp;
				str_ListTemp += _T(",");
			}	
			p_recordSet->MoveNext();
		}
	}

	if (!b_CR || (b_CR && str_ListTemp.IsEmpty()))
	{
		//get all released image list
		if(!GetQueryResult(str_sql1, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		while (!p_recordSet -> GetadoEOF())
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect(_T("img_version"));
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				str_ListTemp += str_Temp;
				str_ListTemp += _T(",");
			}	
			p_recordSet->MoveNext();
		}
	}

	if (str_ListTemp.Right(1) == _T(","))
	{
		str_ListTemp = str_ListTemp.Left(str_ListTemp.GetLength()-1);
	}

	sprintf(sz_imageList,"%s",str_ListTemp);
	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: VMS_QueryNormalImageListByModel
* Summary      : Query Normal image lsit from VMS
* Parameters   : output/Normal image list, error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          luffy.liu     2012/09/13            First version             
*****************************************************************************/
bool VMS_QueryNormalImageList(char * sz_model,char * sz_imageList, char * sz_ErrCode, char * sz_ErrMsg)
{
	CString strModel =  sz_model;
	CString strSql = _T("select model_id from model_list where model_name='") + strModel + _T("'");
	CString strTemp = _T("");
	CString strSql2 = _T("");
	CString strListTemp = _T("");

	/* Execute DB*/
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	if(!GetQueryResult(strSql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),strSql);
		return false;
	}

	if (!p_recordSet -> GetadoEOF())
	{
		thevalue=p_recordSet->GetCollect(_T("model_id"));
		if (thevalue.vt != VT_NULL)
		{
			strTemp=(char *)_bstr_t(thevalue);
			strSql2.Format(_T("select distinct img_version from image_version_control where model_id like '%%%s%%' and status='1'"),strTemp);
		}	
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYMODELID_EMPTY);
		sprintf(sz_ErrMsg,("Can't found model ID for this model\n机种:%s找不到其对应的ID,请确认"),strModel);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	//get all released image list
	if(!GetQueryResult(strSql2, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query model ID fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),strSql2);
		return false;
	}

	while (!p_recordSet -> GetadoEOF())
	{
		strTemp.Empty();
		thevalue=p_recordSet->GetCollect(_T("img_version"));
		if (thevalue.vt != VT_NULL)
		{
			strTemp=(char *)_bstr_t(thevalue);
			strListTemp += strTemp;
			strListTemp += _T(",");
		}	
		p_recordSet->MoveNext();
	}


	if (strListTemp.Right(1) == _T(","))
	{
		strListTemp = strListTemp.Left(strListTemp.GetLength()-1);
	}

	sprintf(sz_imageList,"%s",strListTemp);

	strTemp.Format("Get released factory image list from image_version_control: %s",strListTemp);
	DBLOG(strTemp);

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryImagePNToolVersionStatus
* Summary      : Query Image(OS/Factory) PN ToolVersion from VMS VRCM table
* Parameters   : input/PN, Image(OS/Factory),ToolVersion,output error code, error message ,status
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Fox.     2012/09/13            First version             
*****************************************************************************/
bool VMS_QueryImagePNToolVersionStatus(char * sz_model,char * sz_PN,char * sz_FactoryImageVer,char * sz_OSImageVer, char * sz_ToolVerison,char * sz_status ,char * sz_Record_status,char * sz_ErrCode, char * sz_ErrMsg)
{ 

  CString str_PN = sz_PN;
  CString str_FactoryImageVer = sz_FactoryImageVer;
  CString str_OSImageVer = sz_OSImageVer;
  CString str_ToolVerison = sz_ToolVerison;
  CString strTemp = _T("");
  CString str_PN_type = _T("");

  str_PN_type = str_PN.Left(2);
  
  CString str_sql = _T("");
  
  char sz_modelID[10] = {0};
  char sz_projID[10] = {0};

  if (!VMS_QueryModelID(sz_model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
  {
	  return false;
  }
  
  CString str_model_id = sz_modelID;
  _RecordsetPtr p_recordSet;
  _variant_t thevalue;

  str_sql = _T("select * from VRCM where model_id = '") + str_model_id + _T("'");
  
  if(!GetQueryResult(str_sql, p_recordSet))
  {
	  strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	  sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
	  return false;
  }
  
  if (p_recordSet -> GetadoEOF())
  {  
      strcpy(sz_ErrCode, _T("NoRecord"));
	  return false;
  }

  if (str_PN_type.MakeUpper()==_T("5E")|| str_PN_type.MakeUpper()==_T("6K")|| str_PN_type.MakeUpper()==_T("5G"))
  {
	  if (str_PN_type.MakeUpper()==_T("5E") || str_PN_type.MakeUpper()==_T("6K"))
	  {
//		  str_sql = _T("select * from VRCM where img_version ='")+str_FactoryImageVer+_T("' and [5e_PN] ='") +str_PN+_T("'and tool_version ='")+ str_ToolVerison+_T("' order by timestamp desc");
		  str_sql = _T("select * from VRCM where model_id='") +str_model_id+ _T("' and img_version ='")+str_FactoryImageVer+_T("' and [5e_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' and status='1' and record_status='2' order by version desc");
	  }
	  else
	  {
//		  str_sql = _T("select * from VRCM where img_version ='")+str_FactoryImageVer+_T("' and [5g_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' order by timestamp desc");
		  str_sql = _T("select * from VRCM where model_id='") +str_model_id+ _T("' and img_version ='")+str_FactoryImageVer+_T("' and [5g_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' and status='1' and record_status='2' order by version desc");
	  }
   
  }
  else if (str_PN_type.MakeUpper()==_T("CS")||str_PN_type.MakeUpper()==_T("9J")||str_PN_type.MakeUpper()==_T("5J") ||str_PN_type.MakeUpper()==_T("9B"))
  {
//	  str_sql = _T("select * from VRCM where img_version ='")+str_FactoryImageVer +_T("' and OS_version ='") +str_OSImageVer +_T("' and [9JCS_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' order by timestamp desc");
 	  str_sql = _T("select * from VRCM where model_id='") +str_model_id+ _T("' and img_version ='")+str_FactoryImageVer +_T("' and OS_version ='") +str_OSImageVer +_T("' and [9JCS_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' and status='1' and record_status='2' order by version desc");
  }
  else
  {   
	  return false;
  }


  if(!GetQueryResult(str_sql, p_recordSet))
  {
	  strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	  sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
	  return false;
  }

  if(p_recordSet->GetadoEOF())
  {
	  if(str_PN_type.MakeUpper()==_T("6K")) //6K in ALine
	  {
		  str_sql = _T("select * from VRCM where model_id='") +str_model_id+ _T("' and img_version ='")+str_FactoryImageVer+_T("' and [5g_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison+_T("' and status='1' and record_status='2' order by version desc");
		  if(!GetQueryResult(str_sql, p_recordSet))
		  {
			  strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			  sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			  return false;
		  }
	  }
  }

  if (!p_recordSet -> GetadoEOF())
  {
	  thevalue=p_recordSet->GetCollect(_T("status"));
	  if (thevalue.vt != VT_NULL)
	  {
		  strTemp=(char *)_bstr_t(thevalue);
		  sprintf(sz_status,"%s",strTemp);
	  }	
	  strTemp.Empty();
	  thevalue = p_recordSet->GetCollect(_T("record_status"));
	  if(thevalue.vt != VT_NULL)
	  {
		  strTemp=(char *)_bstr_t(thevalue);
          sprintf(sz_Record_status,"%s",strTemp);
	  }
  }
  else
  {
	  strcpy(sz_ErrCode, ERROR_QUERYVRCM_EMPTY);
	  sprintf(sz_ErrMsg,("Can't found Record from vrcm table \nVRCM表中找不到对应的记录请确认"));
	  DBLOG(sz_ErrMsg);
	  p_recordSet->Close();
	  return false;
  }
  
  /* Disconnect DB */
  p_recordSet->Close();

  return true;

}

/*****************************************************************************
* Function name: VMS_QueryImageVersionReserveValue
* Summary      : Query image version Reserve1 Reserve2
* Parameters   : input/Model, imageVer; output/Reserve1, Reserve2,error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Fox.Yan     2013/04/18          First version             
*****************************************************************************/

bool VMS_QueryImageVersionReserveValue(char * sz_model,char * sz_ImageVer ,char * sz_reserve1_value,char * sz_reserve2_value,char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_SW = sz_ImageVer;
	CString str_Temp = _T("");
	CString str_modelID =_T("");


	char sz_modelID[10] = {0};
	char sz_projID[10] = {0};
	if (!VMS_QueryModelID(sz_model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	str_modelID = sz_modelID;
	/* Execute DB*/
	str_sql = _T("select * from image_version_control where img_version = '"+str_SW+"' and model_id like '%"+str_modelID+"%' and status != '2' order by release_date desc");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query image version fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("reserve1_value"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_reserve1_value, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect(_T("reserve2_value"));
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_reserve2_value, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INPUTSW_EMPTY);
		sprintf(sz_ErrMsg,("Invalid SW image \n软件版本:%s对应%d无效,请确认"),str_SW,sz_model);
		DBLOG(sz_ErrMsg);
		p_recordSet->Close();
		return false;
	}

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: VMS_QueryVRCMRImageList
* Summary      : Query Release Image from VRCM which linked with defined tool version
* Parameters   : input/PN, ToolVersion,output image version list,error code, error message
* Return       : true/false
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Nancy.Yang     2013/06/17            First version             
*****************************************************************************/
bool VMS_QueryVRCMRImageList(char * sz_model,char * sz_PN,char * sz_ToolVerison,char * sz_FactoryImageList,char * sz_ErrCode, char * sz_ErrMsg)
{ 
	CString strListTemp = _T("");

	CString str_PN = sz_PN;
	CString str_ToolVerison = sz_ToolVerison;
	CString strTemp = _T("");
	CString str_PN_type = _T("");

	str_PN_type = str_PN.Left(2);

	CString str_sql = _T("");

	char sz_modelID[10] = {0};
	char sz_projID[10] = {0};

	if (!VMS_QueryModelID(sz_model, sz_modelID, sz_projID,sz_ErrCode, sz_ErrMsg ))
	{
		return false;
	}

	CString str_model_id = sz_modelID;
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	str_sql = _T("select * from VRCM where model_id = '") + str_model_id + _T("'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}

	if (p_recordSet -> GetadoEOF())
	{  
		strcpy(sz_ErrCode, _T("NoRecord"));
		return false;
	}

	/*if (p_recordSet -> GetadoEOF())
	{  
		strcpy(sz_ErrCode, ERROR_QUERYVRCM_EMPTY);
		strcpy(sz_ErrMsg, _T("No record of this model in VRCM!"));
		return false;
	}*/

	if (str_PN_type.MakeUpper()==_T("5E")||str_PN_type.MakeUpper()==_T("6K")||str_PN_type.MakeUpper()==_T("5G"))
	{
		if (str_PN_type.MakeUpper()==_T("5E") || str_PN_type.MakeUpper()==_T("6K"))
		{
			str_sql = _T("select distinct img_version from VRCM where [5e_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison + _T("' and status='1' and record_status='2'");
		}
		else
		{
			str_sql = _T("select distinct img_version from VRCM where [5g_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison + _T("' and status='1' and record_status='2'");
		}

	}
	else if (str_PN_type.MakeUpper()==_T("CS")||str_PN_type.MakeUpper()==_T("9J")||str_PN_type.MakeUpper()==_T("5J")||str_PN_type.MakeUpper()==_T("9B"))
	{
		str_sql = _T("select distinct img_version from VRCM where [9JCS_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison + _T("' and status='1' and record_status='2'");
	}
	else
	{   
		return false;
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
		return false;
	}
	
	if(p_recordSet->GetadoEOF())
	{
		if(str_PN_type.MakeUpper()==_T("6K")) //6K in ALine
		{
			str_sql = _T("select distinct img_version from VRCM where [5g_PN] ='") +str_PN+_T("' and tool_version ='")+ str_ToolVerison + _T("' and status='1' and record_status='2'");
			if(!GetQueryResult(str_sql, p_recordSet))
			{
				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
				sprintf(sz_ErrMsg,_T("Query Record Fail From VRCM, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
				return false;
			}
		}
	}


	while (!p_recordSet -> GetadoEOF())
	{
		strTemp.Empty();
		thevalue=p_recordSet->GetCollect(_T("img_version"));
		if (thevalue.vt != VT_NULL)
		{
			strTemp=(char *)_bstr_t(thevalue);
			strListTemp += strTemp;
			strListTemp += _T(",");
		}	
		p_recordSet->MoveNext();
	}

	if (strListTemp.Right(1) == _T(","))
	{
		strListTemp = strListTemp.Left(strListTemp.GetLength()-1);
	}

	sprintf(sz_FactoryImageList,"%s",strListTemp);

	strTemp.Format("Get released factory image list from VRCM: %s",sz_FactoryImageList);
	DBLOG(strTemp);

	/* Disconnect DB */
	p_recordSet->Close();

	return true;

}