
#include "stdafx.h"
#include "DBAccess.h"

#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_ConnectionPtr   m_p_connection;       // _ConnectionPtr接口
CString          m_str_serverName;     // 服务器名
CString          m_str_dbName;         // 数据库名
CString          m_str_userName;       // 登录用户名
CString          m_str_userPassword;   // 登录密码

CCriticalSection  m_obj_loadADBFileCritSection;

std::map<CString, CString>  m_mapPICS;             // PICS数据
std::map<CString, CString>  m_map_matchingTable;   // Matching Table


int G_ErrNumber = 0;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC
	int nRetCode = 0;
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else {
	}
	return nRetCode;
}

CDBAccess::CDBAccess()
{
	::CoInitialize(NULL);

}

CDBAccess::~CDBAccess()
{
//	Clear_PICSData();
	::CoUninitialize();
}

/*****************************************************************************
* Function name: ReadIniFile      
* Summary      : Read .\\DBConfig.ini.
* Parameters   : void
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/03/22          First version             
*****************************************************************************/
/**
* @brief ReadIniFile
* Read .\\DBConfig.ini.
*/
int CDBAccess::ReadIniFile(void)
{
	/* Check ini file exist */
	if (_taccess(DBCONFIGFILE, 0) != 0) {
		return NG;
	}

	/* Server Name */
	TCHAR sz_buffer[128];
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("ServerName"), _T(""), sz_buffer, sizeof(sz_buffer), DBCONFIGFILE);
	m_str_serverName = sz_buffer;

	/* DB Name */
	memset(sz_buffer, 0, sizeof(sz_buffer));
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("DBName"), _T(""), sz_buffer, sizeof(sz_buffer), DBCONFIGFILE);
	m_str_dbName = sz_buffer;

    /* sunlight cancel, 20110426, 测试结束，安全起见将ID，password 隐于code	
	/* User Name *
	memset(sz_buffer, 0, sizeof(sz_buffer));
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("UserName"), _T(""), sz_buffer, sizeof(sz_buffer), DBCONFIGFILE);
	m_str_userName = sz_buffer;

	/* User Password *
	memset(sz_buffer, 0, sizeof(sz_buffer));
	memset(sz_buffer, 0, sizeof(sz_buffer));
	::GetPrivateProfileString(_T("DB_SET"), _T("UserPassword"), _T(""), sz_buffer, sizeof(sz_buffer), DBCONFIGFILE);
	m_str_userPassword = sz_buffer;*/
	return OK;
}


/*****************************************************************************
* Function name: ConnectDB      
* Summary      : Connect DB.
* Parameters   : void
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/03/22          First version   
* 1.1          Sunlight.Qian     2011/1128           Second version , add retry
*****************************************************************************/
bool ConnectDB(void)
{
	CDBAccess obj_dbAccess;
	CString str_connect = _T("");
	/* Read ini File */

	if (obj_dbAccess.ReadIniFile() != OK) {
		CString str_log;
		str_log.Format("Read %s fail.", DBCONFIGFILE);
		DBLOG(str_log);
		return false;
	}

	HRESULT hr = NULL;
	try {
		// 创建连接对象实例
		hr = m_p_connection.CreateInstance(_T("ADODB.Connection"));
		if (SUCCEEDED(hr)) {
			str_connect  = "Provider=SQLOLEDB.1;";
            str_connect += "Persist Security Info=True;";
            str_connect += "Initial Catalog=" + m_str_dbName + ";";

            /*sunlight cancel, 20110426, 测试结束，安全起见将ID，password 隐于code	
			str_connect += "User ID=" + m_str_userName + ";";
			str_connect += "Password=" + m_str_userPassword + ";";
            */
			str_connect += "User ID=pics;";
			str_connect += "Password=pics#2011;";
			str_connect += "Data Source=" + m_str_serverName + ",1433";

			/*  设置超时时间为15秒  */
			m_p_connection->ConnectionTimeout = 15;

			///* Sunlight add for debug 20110622 */
			DBLOG("****************************************** S T A R T ******************************************************************");
			DBLOG(m_str_serverName);
			///* Sunlight add end 20110622 */
    		hr = m_p_connection->Open(_bstr_t(str_connect),"", "", adModeUnknown);
			if (FAILED(hr)) 
			{
				hr = NULL;
				hr = m_p_connection->Open(_bstr_t(str_connect),"", "", adModeUnknown);
				if (FAILED(hr)) 
				{
				m_p_connection->Close();
				DBLOG("Re Connect DB fail");
				return false;
			    }
			}
		} else {
			DBLOG("Connect DB fail");
			return false;
		}
	} catch (_com_error e) {
		CString str_log;
		str_log.Format("Connect DB fail: %s", LPCTSTR(e.Description()));
		DBLOG(str_log);
		try
		{
             hr = m_p_connection->Open(_bstr_t(str_connect),"", "", adModeUnknown);
		}catch(_com_error e){
			CString str_log;
			str_log.Format("Re Connect DB fail: %s", LPCTSTR(e.Description()));
			DBLOG(str_log);
			return false;
		}
	} 

	DBLOG("Connect DB Succeed.");
	return true;
}


/*****************************************************************************
* Function name: ExecuteSQL      
* Summary      : Execute create, insert, update, delete.
* Parameters   : str_sql : sql语句
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/03/22          First version             
*****************************************************************************/
/**
* @brief ExecuteSQL
*
* ####Execute SQL command, and there is no return####
*
* @param str_sql input, sql cmd
*/
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

	/* _ConnectionPtr Execute */
	_variant_t RecordsAffected;
	try {
		m_p_connection->Execute((_bstr_t)str_sql, &RecordsAffected, adCmdText);
	} catch (_com_error e) {
		CString str_log;
		str_log.Format("ExecuteSQL fail: %s", LPCTSTR(e.Description()));
		DBLOG(str_log);
		/*Sunlight add retry when fail, 20111221*/
		try {
			if (!ConnectDB())
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

	DBLOG("ExecuteSQL Succeed.");
	return OK;
}


/*****************************************************************************
* Function name: GetQueryResult      
* Summary      : Execute select.
* Parameters   : str_sql     : select sql语句
*                p_recordSet : selcet结果集
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/03/22          First version             
* 1.1          Sunlight.Qian 2011/11/22          Second version add retry when fail, and modify error log
*****************************************************************************/
/**
* @brief GetQueryResult
*
* ####Execute SQL command, and return the p_recordSet(Read Only) which keeping the result from cmd execution####
* 
* @param str_sql input, sql cmd
* @param p_recordSet input, a _RecordsetPtr pointer
*/
int GetQueryResult(CString str_sql, _RecordsetPtr & p_recordSet)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	_RecordsetPtr p_recorSet2;
	/* Check Input */
	if (str_sql == _T("")) {
		DBLOG("Query DB SQL is empty");
		return NG;
	}

	CString str_log;
	str_log.Format("Query DB: %s",str_sql);
    DBLOG(str_log);

	HRESULT hr = NULL;
	hr = p_recorSet2.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr)) {
		DBLOG("CreateInstance ADODB.Recordset fail.");
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
		str_log.Format("Query DB fail: %s", LPCTSTR(e.Description()));
		DBLOG(str_log);
		/*Sunlight add 20111122, for fail retry*/
		//return NG;
		hr = NULL;
		if (!ConnectDB())
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
//	DBLOG("Query DB Succeed");//nancy deleted
	return OK;
}

/*****************************************************************************
* Function name: GetQueryResult_RW      
* Summary      : Execute select, and have read/write table permissions 
* Parameters   : str_sql     : select sql语句
*                p_recordSet : selcet结果集
* Return       : OK / NG
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Karen.Wang     2014/07/25          First version             
*****************************************************************************/
/**
* @brief GetQueryResult_RW
*
* ####1. Execute SQL command, and return the p_recordSet which keeping the result from cmd execution####
* ####2. The p_recordSet is ability to Read/Write and it will let us to insert/update record more efficient####
* @param str_sql input, sql cmd
* @param p_recordSet input, a _RecordsetPtr pointer
*/
int GetQueryResult_RW(CString str_sql, _RecordsetPtr & p_recordSet)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	_RecordsetPtr p_recorSet2;
	/* Check Input */
	if (str_sql == _T("")) {
		DBLOG("Query DB SQL is empty");
		return NG;
	}

	CString str_log;
	str_log.Format("Query DB: %s",str_sql);
	DBLOG(str_log);

	HRESULT hr = NULL;
	hr = p_recorSet2.CreateInstance(_T("ADODB.Recordset"));
	if (FAILED(hr)) {
		DBLOG("CreateInstance ADODB.Recordset fail.");
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
			adLockOptimistic,          
			adCmdText);                                        // adCmdText：文本查询语句
	} catch (_com_error e) {
		str_log.Format("Query DB fail: %s", LPCTSTR(e.Description()));
		DBLOG(str_log);
		/*Sunlight add 20111122, for fail retry*/
		//return NG;
		hr = NULL;
		if (!ConnectDB())
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
				adLockOptimistic,          
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
	//	DBLOG("Query DB Succeed");//nancy deleted
	return OK;
}

/*****************************************************************************
* Function name: CloseDB      
* Summary      : Close DB.
* Parameters   : void
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Sunlight.Qian     2011/03/22          First version             
*****************************************************************************/
void CloseDB(void)
{
//	try {
//		if (m_p_recordSet->State) {
//			m_p_recordSet->Close();
//			m_p_recordSet.Release();
//		}
//		m_p_recordSet = NULL;
//
//	} catch (_com_error e) {
//		CString str_log;
//		str_log.Format("Close DB recordSet fail: %s", LPCTSTR(e.Description()));
//		DBLOG(str_log);
////		return;
//	}

	try {
		if (m_p_connection->State) {
			m_p_connection->Close();
			m_p_connection.Release();
		}
		m_p_connection = NULL;

	} catch (_com_error e) {
		CString str_log;
		str_log.Format("Close DB connection fail: %s", LPCTSTR(e.Description()));
		DBLOG(str_log);
//		return;
	}

//	Clear_PICSData();
    DBLOG("Close DB success");
	return;
}



/*****************************************************************************
* Function name: DBLOG      
* Summary      : Write log.
* Parameters   : void
* Return       : void
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/04/06          First version         
* 1.1          Sunlight.Qian      2012/04/03          Second version    auto create .\db\db_log\     
*****************************************************************************/
/**
* @brief DBLOG
*
* ####Log will be created in D:\\Log\\DBLOG and name the file by date####
* @param str_message input, the message intent to be recorded
*/
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
	char sz_currentDir[256];
	memset(sz_currentDir, 0, sizeof(sz_currentDir));
	::GetCurrentDirectory(sizeof(sz_currentDir), sz_currentDir);
	CString str_currentPath;
	str_currentPath.Format("%s", sz_currentDir);

	/* Check LOG\\DB_Log exist and create */
	CString str_logDir;
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

	str_logDir = str_logDir + "\\DB_Log";
	h_searchFind = FindFirstFile(str_logDir, &fd_fileInformation);
    if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		FindClose(h_searchFind); 
    } else {
		FindClose(h_searchFind ); 
		if (CreateDirectory(str_logDir, NULL) == 0) {
			return;
		} 
	} 

	/* sunlight add20120403,for ErrorInfo.ini, auto create .\db\db_log\ */
	CString str_ErrorInfoDir = str_currentPath + "\\DB";
	h_searchFind = FindFirstFile(str_ErrorInfoDir, &fd_fileInformation);
	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		FindClose(h_searchFind); 
	} else {
		FindClose(h_searchFind ); 
		if (CreateDirectory(str_ErrorInfoDir, NULL) == 0) {
			return;
		} 
	} 

	str_ErrorInfoDir = str_ErrorInfoDir + "\\DB_Log";
	h_searchFind = FindFirstFile(str_ErrorInfoDir, &fd_fileInformation);
	if ((h_searchFind != INVALID_HANDLE_VALUE) && (fd_fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		FindClose(h_searchFind); 
	} else {
		FindClose(h_searchFind ); 
		if (CreateDirectory(str_ErrorInfoDir, NULL) == 0) {
			return;
		} 
	} 
	/* Sunlight add end,20120403*/

	/* Open log file */
	CStdioFile obj_file;
    CString str_logFile;
	CString str_temp;
	str_temp.Format(_T("[%02d:%02d:%02d:%03d]:  %s "),time.wHour, time.wMinute, time.wSecond,time.wMilliseconds,str_message);
	str_logFile.Format("%s\\%04d_%02d_%02d.log",str_logDir, time.wYear, time.wMonth, time.wDay);

	if (obj_file.Open(str_logFile, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL)) {
		obj_file.SeekToEnd();
		obj_file.WriteString(str_temp);
		obj_file.WriteString(_T("\r\n"));
		obj_file.Close();
	}

	return;
}


/*****************************************************************************
* Function name: ReadXMLFile      
* Summary      : Read xml file(matching table)
* Parameters   : void
* Return       : true / false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/04/06          First version             
*****************************************************************************/
/**
* @brief ReadXMLFile
*
* ####1. Read .\\DB\\XML\\[Pics.xml] ####
* ####2. Assign all item to a map(m_map_matchingTable) ####
* ####3. The [Pics.xml] must include "ColType" or return fail
* @param str_xmlFileName input, the [Pics.xml] full path
*/
bool ReadXMLFile(CString str_xmlFileName)
{
	m_map_matchingTable.clear();

	CString str_log =_T("");

	/* Check xml file exist */
	if (_taccess(str_xmlFileName, 0) != 0) {
		str_log.Empty();
		str_log.Format("%s xml file is not exist.", str_xmlFileName);
		DBLOG(str_log);
		return false;
	}

	/* Read XML file */
	CMyMSXML obj_xml;
	if (obj_xml.Load(str_xmlFileName) != 0) {
		str_log.Empty();
		str_log.Format("Load %s fail.", str_xmlFileName);
		DBLOG(str_log);
		return false;
	}

	/* Get version type, for example: [ColType,V1] */
	XMLNode node;
	node = obj_xml.SearchNode("//PICS/ColType");
	if (node == NULL) {
		DBLOG("Get //PICS/ColType fail.");
		return false;
	}

	CString str_colType;
	str_colType = obj_xml.GetNodeText(node);

	if (str_colType.GetLength() == 0)
	{
		str_log.Empty();
		str_log.Format("ColType:%s is null.", str_xmlFileName);
		DBLOG(str_log);
	}
	m_map_matchingTable["ColType"] = str_colType;
	m_mapPICS["coltype"] = str_colType;

	/* Get column map, for example: [PICASSO,L1] */
	CString str_LNumber;
	CString str_LValue;
	/*CString str_temp;
	for(int i = 1; i > 0; i++) {
		str_LNumber.Format(_T("L%d"), i);
		str_temp.Format(_T("//PICS/%s"), str_LNumber);
		node = obj_xml.SearchNode(str_temp);
		if (node == NULL) {
			break;
		} else {
			str_LValue = obj_xml.GetNodeText(node);
			if(str_LValue.GetLength() > COLNAME_LENGTH)
			{
				str_log.Empty();
				str_log.Format("The test item name is more than %d character",COLNAME_LENGTH);
				DBLOG(str_log);
				return false;
			}
			m_map_matchingTable[str_LValue] = str_LNumber;
		}
	}*/

	
	XMLNode nodeBase;
	nodeBase = obj_xml.SearchNode("//PICS");
	if (nodeBase == NULL) {
		DBLOG("Get node PICS fail.");
		return false;
	}

	XMLNode pItem;
	pItem = obj_xml.GetFirstChild(nodeBase);

	while(pItem) 
	{
		str_LValue = obj_xml.GetNodeText(pItem);
		str_LNumber = (char *)(pItem->GetbaseName());

		if(str_LValue.GetLength() > COLNAME_LENGTH)
		{
			str_log.Empty();
			str_log.Format("The test item name is more than %d character",COLNAME_LENGTH);
			DBLOG(str_log);
			return false;
		}

		if (str_LNumber.Left(1) == "L")
		{
			m_map_matchingTable[str_LValue] = str_LNumber;
		}

		pItem = pItem->GetnextSibling();
	}


	/* Check the value of node valid */
	if (m_map_matchingTable.size() <= 1) {
		DBLOG("Get node PICS Ln fail.");
		return false;
	}

    str_log.Empty();
	str_log.Format("%s xml file Success.", str_xmlFileName);
	DBLOG(str_log);
	return true;
}


/*****************************************************************************
* Function name: CheckMatchTable      
* Summary      : 检查MatchingTabel中，是否存在XML中的ColType
* Parameters   : void
* Return       : true / false
* Exception    : 
* Version       Author              Date                Abstract                 
* 1.0          Sunlight.Qian       2011/04/06          First version         
* 1.1          Sunlight.Qian       2012/04/09          Second version ,common tool XML file name define as process.XML, read MODEL+PROCESS.XML first
* 1.2          Sunlight.Qian       2012/04/12          Third version ,common tool XML file name define as process.XML, cancel read MODEL+PROCESS.XML first
*****************************************************************************/
/**
* @brief CheckMatchTable
*
* ####1. 检查MatchingTabel中，是否存在XML中的ColType####
* ####2. Is ColType exist in the MachingPT table:####
*    - Not Exist
*         - Using GetMatchTableInsertSql to get the sql cmd and insert a new record in the table
*    - Exist
*         - Do nothing and return true
*  .
*
*      DB Table: MatchingPT
*      SQL: select * from MatchingPT where ColType = =str_colType and and Model =sz_type
*      Related Function: ReadXMLFile, GetMatchTableInsertSql
*/
bool CheckMatchTable(void)
{
	/* Get Model and Process */
	CString str_model;
	str_model = m_mapPICS[_T("model")];
	CString str_process;
	str_process = m_mapPICS[_T("process")];

	/* Get xml filename */
	CString str_xmlFileName;
	str_xmlFileName.Empty();

	/*Sunlight add 20120409, common tool XML file name define as process.XML*/
	if(str_process == _T("FALINK") || 
		str_process == _T("FALINK2") ||
		str_process == _T("VI") || 
		str_process == _T("FQC") || 
		str_process == _T("RUN_IN") ||
		str_process == _T("SHIPPINGOS") ||
		str_process == _T("OOB") ||
		str_process == _T("PPIDLINK")||
		str_process == _T("P_VI")	||
		str_process == _T("AOI"))
	{
		str_xmlFileName = XMLPATHFILE + str_process + _T(".xml");
	}
	/*Sunlight add 20120409 end*/
	else
	{
		str_xmlFileName = str_model + str_process + _T(".xml");
		str_xmlFileName = XMLPATHFILE + str_xmlFileName;
	}

	/* Read XML */
	if (!ReadXMLFile(str_xmlFileName)) 
	{
		 DBLOG("[CheckMatchTable] Read xml file fail.");
		 return false; 
	}

	CString str_colType;
	str_colType = m_map_matchingTable["ColType"];

	/* Get ColType from DB */
	//CDBAccess obj_dbAccess;
   /* sunlight cancel, 20110426 
	if (obj_dbAccess.ConnectDB() != OK) {
		DBLOG("[CheckMatchTable] Connect DB fail.");
		return false;
	}*/

	CString str_sql;
	str_sql = "select * from MatchingPT where ColType = '" + str_colType + "' and Model = '" + str_model + "' and Process = '" + str_process + "'";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
    if(!GetQueryResult(str_sql, p_recordSet)) {
//		CloseDB();
		DBLOG("[CheckMatchTable] GetQueryResult fail.");
		return false;
	}

	if (p_recordSet->GetadoEOF()) {
		/* Not exist and Insert Data */
		CString str_sql;
		str_sql = GetMatchTableInsertSql();
		if (!ExecuteSQL(str_sql)) {
//			CloseDB();
			DBLOG("[CheckMatchTable] ExecuteSQL fail.");
			return false;
		}
		
	} else {
	}

	p_recordSet->Close();
//  CloseDB();
    
	return true;
}


/*****************************************************************************
* Function name: GetMatchTableInsertSql      
* Summary      : 生成matching table的insert sql语句
* Parameters   : void
* Return       : sql语句
* Exception    : 
* Version       Author              Date                Abstract                 
* 1.0          Sunlight.Qian       2011/04/06          First version             
*****************************************************************************/
/**
* @brief GetMatchTableInsertSql
*
* ####生成matching table的insert sql语句####
*
*     DB Table: MatchingPT
*     SQL: insert into MatchingPT ...
*/
CString GetMatchTableInsertSql(void)
{
	CString str_sql;
	CString str_key;
	CString str_value;
	str_sql = "insert into MatchingPT(";
	str_value = "values(";
	str_key = str_key + 
		      "ColType" + "," +
			  "Model" + "," +
			  "Process" + "," +
			  "Flag" + "," +
			  "Muser" + "," +
			  "Mdate" + "," +
			  "Mtime" + "," +
			  "Remark" + ","; 
	str_value= str_value +
		       "'" + m_map_matchingTable["ColType"] + "',"  +
			   "'" + m_mapPICS["model"] + "',"  +
			   "'" + m_mapPICS["process"] + "',"  +
			   "'" + "1" + "',"  +
			   "'" + m_mapPICS["workstation"] + "',"  +
			   "'" + m_mapPICS["tstdate"] + "',"  +
			   "'" + m_mapPICS["tsttime"] + "',"  +
			   "'" + m_mapPICS["remark"] + "',";

	std::map<CString, CString>::iterator  itr;
	unsigned int i_count = 0;
	for (itr = m_map_matchingTable.begin(),i_count=1; itr != m_map_matchingTable.end(),i_count <= m_map_matchingTable.size(); itr++,i_count++) {
		if (itr->first == "ColType") {
			continue;
		}
		str_key += itr->second;
		str_value += "'" + itr->first +"'";
		if (i_count != m_map_matchingTable.size()) {
		   str_key += ",";
		   str_value += ",";
		}
	}
	str_key += ")";
	str_value += ")";

	str_sql += str_key + str_value;

	return str_sql;
}

/*****************************************************************************
* Function name: GetPreTableFixtureItems     
* Summary      : 确认是否是pretable 必要的15个栏位值，方便
* Parameters   : void
* Return       : sql语句
* Exception    : 
* Version       Author              Date                Abstract                 
* 1.0          Sunlight.Qian       2011/04/06          First version             
*****************************************************************************/
bool GetPreTableFixtureItems (CString str_ID)
{
	if (str_ID == "id" 
		|| str_ID == "line" 
		|| str_ID == "segment" 
		|| str_ID == "process" 
		|| str_ID == "workstation" 
		|| str_ID == "tstdate" 
		|| str_ID == "tsttime" 
		|| str_ID == "so" 
		|| str_ID == "model" 
		|| str_ID == "partno"
		|| str_ID == "sotype"
		|| str_ID == "employee"
		|| str_ID == "okng"
		|| str_ID == "errorcode"
		|| str_ID == "coltype"
		|| str_ID == "sw"          
		|| str_ID == "hw"
		|| str_ID == "mal"
		|| str_ID == "lineid"
		|| str_ID == "picasso")    //sunlight add 20120112, add sw,hw,mal,lineid,picasso for IMEI process
	{     
		return true;
	}
	else
	{
		return false;
	}
}


/**
* @brief QuerySO_Info
*
* ####1. Query SO info from SO_Info table, and check the length is valid####
* ####2. Global map m_mapPICS will keep so, sotype, partno, model values####
*
*     DB Table: SO_Info
*     SQL: select * from SO_INFO where sord = sz_so
*/
bool QuerySO_Info(
		char* sz_so, ///< so number, input
		char* sz_model, ///< model name, output
		char* sz_partNo, ///< partnumber, output
		char* sz_soType, ///< so type, output
		char* sz_errorCode, ///< Error code, output
		char* sz_errorMsg ///< Error message, output
		)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	/* Check input */
	if (strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) {
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number \n输入的工单不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */


	/* Connect DB */
    /* sunlight cancel, 20110426
	if (ConnectDB() != OK) {
		strcpy(sz_errorCode, ERROR_CONNECTDB_FAIL);
		sprintf(sz_errorMsg,("Connect to DB Fail."));
		return false;
	}*/
	

	CString str_sql;
	CString so=sz_so;
	CString str_Temp = "";
	str_sql = _T("select * from SO_INFO where sord = '"+so+"'");

    /* Execute DB*/
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO fail \n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
 //   	CloseDB();
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("PN");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_partNo, _T("%s"), str_Temp);
		}


		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("Model");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_model, _T("%s"), str_Temp);
		}


		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("Rework");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)_bstr_t(thevalue);
			_stprintf(sz_soType, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number \n此工单:%s无效,请确认后重新输入"),sz_so);
		p_recordSet->Close();
//		CloseDB();
		return false;
	}
	
	/* Disconnect DB */
	p_recordSet->Close();
//    CloseDB();

	/*Check the data is valid*/
	if ( strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits /n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}

	if ( strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits /n机种名:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}

	if ( strlen(sz_soType) !=1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO tyPe:%s is not equal to %d digits /n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	m_mapPICS[_T("so")]=so;
	m_mapPICS[_T("sotype")]=sz_soType;
	m_mapPICS[_T("partno")]=sz_partNo;
	m_mapPICS[_T("model")]=sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType);
	DBLOG(str_Temp);
	return true;
}

/**
* @brief QueryStation_Info
*
* ####1. Query station info from picsline table####
* ####2. Global map m_mapPICS will keep workstation, line, process, segment values####
*
*     DB Table: SO_Info
*     SQL: select * from picsline where wrkstation = PCname
*/
bool QueryStation_Info(
		char* PCname, ///< PC name, input
		char* sz_line, /// Line name, output
		char* sz_process, /// Station name, output
		char* sz_segment, ///< Segment name (TLINE, FA or PACKAGE), output
		char* sz_errorCode, ///< Error code, output
		char* sz_errorMsg ///< Error Message, output
		)
{

//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	CDBAccess obj_dbAccess;
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_PC = "";
	CString str_Temp = "";

   /*char HostName[20] = {0};
	unsigned long nLen=20;
	GetComputerName(HostName,&nLen);*/

	/* Connect DB */
    /*sunlight cancel, 20110426
	if (ConnectDB() != OK) {
		strcpy(sz_errorCode, ERROR_CONNECTDB_FAIL);
		sprintf(sz_errorMsg,("Connect to DB Fail."));
		return false;
	}*/
	


	/* Query Line */
	str_PC=PCname;
	str_sql = _T("select * from picsline where wrkstation =  '"+str_PC+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYLINE_FAIL);
		sprintf(sz_errorMsg,("Query workstation:%s from picsline fail \n数据库操作异常，请尝试重新测试或者重启程式"),str_PC);
//		CloseDB();
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("line");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_line, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("process");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_process, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("segment");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_segment, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_errorCode, ERROR_QUERYLINE_FAIL);
		sprintf(sz_errorMsg,("Get workstation:%s from picsline fail \n查不到此电脑名字对应的线体信息，请找工程师解决"),str_PC);
		p_recordSet->Close();
//		CloseDB();
		return false;

	}

	p_recordSet->Close();
//	CloseDB();
	/* check the data is valid*/
	if ( strlen(sz_line) == 0)
	{
		strcpy(sz_errorCode, ERROR_LINE_EMPTY);
		sprintf(sz_errorMsg,("The Line:%s is null \n 此电脑名字对应的线体信息为空，请找工程师解决"),sz_line);
		return false;
	}

	if (strlen(sz_process) == 0)
	{
		strcpy(sz_errorCode, ERROR_PROCESS_EMPTY);
		sprintf(sz_errorMsg,("The process:%s is null \n此电脑名字对应的站别信息为空，请找工程师解决"),sz_process);
		return false;
	}

	if (strlen(sz_segment) == 0)
	{
		strcpy(sz_errorCode, ERROR_SEGMENT_EMPTY);
		sprintf(sz_errorMsg,("The Segment:%s is null \n此电脑名字对应的段信息为空，请找工程师解决"),sz_segment);
		return false;
	}

	m_mapPICS[_T("workstation")]=str_PC;
	m_mapPICS[_T("line")]=sz_line;
	m_mapPICS[_T("process")]=sz_process;
	m_mapPICS[_T("segment")]=sz_segment;
	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_line,sz_process,sz_segment);
	DBLOG(str_Temp);
	return true;
}

/**
* @brief InsertFAIL_data
*
* ####1. Check error code have defined in PICSECD table, if not, return false and do not insert any record to DB####
* ####2. Insert fail record to [PTStation] table####
* ####3. Check if fail times > re-test times? if yes, then insert the record to TS table####
*
*     DB Table: SO_Info
*     SQL: select * from PICSECD where code = sz_inputErrorCode
*     Related Function: CheckMatchTable, GetInsert_String, CheckRetestCount_new, GetInsertTS_String
*
*     NOTE: In UI v0.99 will change rule 1, if error code was not defined in PICSECD table, the fail record would still be inserted
*/
bool InsertFAIL_data(
		char* sz_id, ///< Picasso or IMEI, input
		char* sz_so, ///< So number, input
		char* sz_employId, ///< Employee id, input, but in fact it did not be used
		char* sz_inputErrorCode, ///< Test result error code, input
		char* sz_errorCode, ///< Error code, output
		char* sz_errorMsg ///< Error message, output
		)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	//	if (strlen(sz_id) != 10 )
	if (strlen(sz_id) < 7 )  //sunlight 20110707 ,for IMEI station , service tag or IMEI
	{
		strcpy(sz_errorCode, ERROR_INPUTID_INVALID);
		sprintf(sz_errorMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_id);
		return false;
	}

	/* Sunlight add end  */
	if (strlen(sz_employId) < 7)
	{
		strcpy(sz_errorCode, ERROR_INPUTEMPLOYEE_INVALID);
		sprintf(sz_errorMsg,("The Employee id you input:%s is null \n员工工号长度不符合要求"),sz_employId);
		return false;
	}

	m_mapPICS[_T("id")]=sz_id;
//	m_mapPICS[_T("employee")]=sz_employId;
	m_mapPICS[_T("okng")]=_T("N");
	m_mapPICS[_T("errorcode")]=sz_inputErrorCode;

	//Luffy modify get local time to get server time if get server time fail, use local instead
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		m_mapPICS[_T("tstdate")] = datetime.date;
		m_mapPICS[_T("tsttime")] = datetime.time;
		
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		m_mapPICS[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		m_mapPICS[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}
	

	/**/

	/* Connect DB */
    /* Sunlight cancel, 20110426
	if (ConnectDB() != OK) {
		strcpy(sz_errorCode, ERROR_CONNECTDB_FAIL);
		sprintf(sz_errorMsg,("Connect to DB Fail."));
		return false;
	}*/


	/* Check matching table */
	if (CheckMatchTable() != true) {
		strcpy(sz_errorCode, ERROR_CHECKMATCHTABEL_FAIL);
		sprintf(sz_errorMsg,("Check matching table fail.\nXML文件不符合要求， 请联系工程师确认"));
//		CloseDB();
		return false;
	}

	CDBAccess obj_dbAccess;
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_Input = "";

/*	char HostName[20] = {0};
	unsigned long nLen=20;
	GetComputerName(HostName,&nLen);*/


	/* Query Line */
	str_Input=sz_inputErrorCode;
	str_sql = _T("select * from PICSECD where code =  '"+str_Input+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYLINE_FAIL);
		sprintf(sz_errorMsg,("Query error code:%s from picsecd fail \n数据库操作异常，请尝试重新测试或者重新启动程式"),str_Input);
//		CloseDB();
		return false;
	}

	if ( p_recordSet -> adoEOF )
	{
		strcpy(sz_errorCode, ERROR_ERRCODE_INVAILD);
		sprintf(sz_errorMsg,("Get error code:%s from picsecd fail, but the record would still be inserted\n系统中未找到此错误代码的信息，请联系工程师"),str_Input);
		DBLOG(sz_errorMsg);
//		CloseDB();
//		return false;
	}

	p_recordSet -> Close();
	/*Insert NG recode to pre-table*/
    str_sql = GetInsert_String();
    if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_errorCode, ERROR_INSERTNG_FAIL);
		sprintf(sz_errorMsg,("Insert pre-table a NG record fail, ID:%s, SQL: %s \n插入此次测试数据失败"),sz_id,str_sql);
//		CloseDB(); 
//		Clear_PICSData();
		return false;
	}
	/*Insert NG recode to TS table*/
	char sz_process[15] = {0};
	strcpy(sz_process,m_mapPICS[_T("process")]);

    /* Sunlight add 20110711 IMEI station input IMEI, but check retest count should be PICASSO*/
	if (strcmp(sz_process,"IMEI") == 0)
	{
		if (m_mapPICS[_T("PICASSO")].GetLength() != 10)
		{
			strcpy(sz_errorCode, ERROR_INPUTID_INVALID);
			sprintf(sz_errorMsg,("[IMEI]The PICASSO you set:%s is null \nPicasso的长度不符合要求"),sz_id);
			return false;
		}
		sprintf(sz_id,"%s",m_mapPICS[_T("PICASSO")]);
	}
	/*  Sunlight add 20110711 end */

	if(!CheckRetestCount_new(sz_id,sz_process, sz_errorCode,sz_errorMsg))
	{
		if (strncmp(sz_errorCode,ERROR_CHECK_RETESTCOUNT_FAIL,6)==0 )
		{
		   str_sql = GetInsertTS_String();
		   if(!ExecuteSQL(str_sql))  //TempMagan
//		   if(0) //TempMagan
		   {
				strcpy(sz_errorCode, ERROR_INSERTNGTS_FAIL);
				sprintf(sz_errorMsg,("Insert TS table a NG record fail, ID:%s,\n向TS表插入此次测试数据失败"),sz_id);
//				CloseDB();
//				Clear_PICSData();
				return false;
		   }
		}
		else
		{
			return false;
		}
	}

//	CloseDB();
//	Clear_PICSData();
	return true;
}

/**
* @brief InsertPASS_data
*
* ####1. Check if sz_id length >= 7, sz_employId length >= 7, if not return false####
* ####2. Insert pass record to [PTStation] table####
*
*     Related Function: CheckMatchTable, GetInsert_String
*/
bool InsertPASS_data(
		char* sz_id, ///< Picasso or IMEI, input
		char* sz_so, ///< so number, input
		char* sz_employId, ///< Employee id, input
		char* sz_errorCode, ///< Error code, output
		char* sz_errorMsg ///< Error message, output
		)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

//	if (strlen(sz_id) != 10 )
	if (strlen(sz_id) < 7 )  //sunlight 20110707 ,for IMEI station , service tag or IMEI
	{
		strcpy(sz_errorCode, ERROR_INPUTID_INVALID);
		sprintf(sz_errorMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_id);
		DBLOG(sz_errorMsg);
		return false;
	}

	if (strlen(sz_employId) < 7)
	{
		strcpy(sz_errorCode, ERROR_INPUTEMPLOYEE_INVALID);
		sprintf(sz_errorMsg,("The Employee id you input:%s is null \n员工工号的长度不符合要求"),sz_employId);
		DBLOG(sz_errorMsg);
		return false;
	}

	m_mapPICS[_T("id")]=sz_id;
//	m_mapPICS[_T("employee")]=sz_employId;
	m_mapPICS[_T("okng")]=_T("Y");
	m_mapPICS[_T("errorcode")]=_T("");  //ITS要求PASS的errorcode为空，20110503

	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		m_mapPICS[_T("tstdate")] = datetime.date;
		m_mapPICS[_T("tsttime")] = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		m_mapPICS[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		m_mapPICS[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}
	

	/* Connect DB */
    /* Sunlight cancel, 20110426
	if (ConnectDB() != OK) {
		strcpy(sz_errorCode, ERROR_CONNECTDB_FAIL);
		sprintf(sz_errorMsg,("Connect to DB Fail."));
		return false;
	}*/


	/* Check matching table */
	if (CheckMatchTable() != true) {
		strcpy(sz_errorCode, ERROR_CHECKMATCHTABEL_FAIL);
		sprintf(sz_errorMsg,("Check matching table fail.\nXML 文件不符合要求， 请联系工程师确认"));
//		CloseDB();
		return false;
	}


	CString str_sql = _T("");

/*	char HostName[20] = {0};
	unsigned long nLen=20;
	GetComputerName(HostName,&nLen);*/

	str_sql = GetInsert_String();
	
    if(!ExecuteSQL(str_sql)) //TempMagan
//	if(0)  // TempMagan
	{
		strcpy(sz_errorCode, ERROR_INSERTOK_FAIL);
		sprintf(sz_errorMsg,("Insert pretable a OK record fail, ID:%s,\n插入此次测试数据失败"),sz_id);
//		CloseDB();
//		Clear_PICSData();
		return false;
	}

//	CloseDB();
//	Clear_PICSData();
	return true;
}

/**
* @brief Insertdata_Common
*
* ####Insert data to DB for common tool (Looks like it just for OOB?)####
*
*     Related Function: ReadXMLFile, GetCommon_String
*/
bool Insertdata_Common(
		char * sz_XMLname, ///< The [Pics.xml] file name, input
		char * sz_tablename, ///< 
		char* sz_errorCode, 
		char* sz_errorMsg
		)
{
    CString str_sql = _T("");

	/* Get xml filename */
	CString str_xmlFileName;
	CString str_tablename;

	str_tablename = sz_tablename;
	str_xmlFileName = sz_XMLname;
	str_xmlFileName += _T(".xml");
	str_xmlFileName = XMLPATHFILE + str_xmlFileName;

	/* Read XML */
	if (!ReadXMLFile(str_xmlFileName)) {
		DBLOG("[CheckMatchTable] Read xml file fail.");
		strcpy(sz_errorCode, ERROR_CHECKMATCHTABEL_FAIL);
		sprintf(sz_errorMsg,("Check matching table fail.\nXML文件不符合要求， 请联系工程师确认"));
		return false;
	}

	/* Connect DB */
//	CDBAccess obj_dbAccess;

/*  Sunlight cancel, 20110426
	if (ConnectDB() != OK) {
		strcpy(sz_errorCode, ERROR_CONNECTDB_FAIL);
		sprintf(sz_errorMsg,("Connect to DB Fail."));
		return false;
	}
*/

	str_sql = GetCommon_String(sz_tablename);
	
    if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_errorCode, ERROR_INSERTOK_FAIL);
		sprintf(sz_errorMsg,("Insert %s a record fail, SQL: %s \n插入此次测试数据失败"),sz_tablename,str_sql);
//		CloseDB();
//		Clear_PICSData();
		return false;
	}
//	CloseDB();
//	Clear_PICSData();
	return true;
}

/**
* @brief InsertOOB_data Insert OOB data to DB
*
* ####1. If sz_result is fail then used GetErrCodeDetail_QA to get error detail message (sz_NGType, sz_NGDetail, sz_MajorMin, sz_desp) ####
* ####2. If sz_result is pass, [Pics.xml] use QA.xml and insert the record to oba_accessorycheck table####
* ####3. If sz_result is fail, [Pics.xml] use QANG.xml and insert the record to OBACheckNGInfo table####
*
*     Related Function: Insertdata_Common, GetErrCodeDetail_QA
*/
bool InsertOOB_data(
		char* sz_result, ///< test result, input
		char* sz_InputErrCode, ///< test error code, input
		char* sz_errorCode, ///< error code, output
		char* sz_errorMsg ///< error message,  output
		)
{
    CString str_result = sz_result;
	int i_result = atoi(str_result);

    bool DBreturn=false; 
	char sz_NGType[20] = {0};
	char sz_NGDetail[100] = {0};
	char sz_MajorMin[15] = {0};
	char sz_desp[15] = {0};
	
    DBreturn = Insertdata_Common("QA","oba_accessorycheck",sz_errorCode,sz_errorMsg);
    if(!DBreturn)
    {
		DBLOG(sz_errorMsg);
		return false;
    }

	if (i_result == 0)
	{
		DBreturn = GetErrCodeDetail_QA(sz_InputErrCode,  sz_NGType,  sz_NGDetail, sz_MajorMin, sz_desp);
		if(!DBreturn)
		{
			DBLOG("Query error code detail fail");
			strcpy(sz_errorCode, ERROR_QAERRORCODE_INVLID);
			sprintf(sz_errorMsg,("Check QA error code information table fail.\nQA错误代码信息找不到"));
			return false;
		}
		SetValue("Symptom",sz_NGType);
		SetValue("NGDetail",sz_NGDetail);
		SetValue("MajorMinor",sz_MajorMin);
		SetValue("ResDep",sz_desp);

		DBreturn = Insertdata_Common("QANG","OBACheckNGInfo", sz_errorCode,sz_errorMsg);
		if(!DBreturn)
		{
			DBLOG(sz_errorMsg);
			return false;
		}
	}
	return true;
}


bool SetValue(const char * sz_index, const char* sz_value)
{
	m_mapPICS[sz_index] = sz_value;

	return true;
}

bool Clear_PICSData()
{
	m_mapPICS.clear();                    //清空数据
	m_map_matchingTable.clear();

	return true;
}

CString GetInsert_String()
{
	std::map<CString,CString>::iterator Itr;
	CString str_Value = _T("(");
	CString str_ID = _T("(");
	CString str_Temp = _T("");
	CString str_sql = _T("");
	unsigned int i_count = 0;

	CString str_IDTemp = _T("");

	for(Itr = m_mapPICS.begin(),i_count=1; Itr != m_mapPICS.end(),i_count <= m_mapPICS.size(); Itr++,i_count++)
	{
        str_IDTemp = m_map_matchingTable[Itr->first];
		if (GetPreTableFixtureItems(Itr->first))
		{
           str_ID += Itr->first;
		}
		else if ( str_IDTemp == "")
		{ 
			str_Temp.Format("(Warning)Found Index:(%s) which not define in matching table %s",Itr->first,m_mapPICS["coltype"]);
			DBLOG(str_Temp);
			continue;
		}
		else
		{
           	str_ID += m_map_matchingTable[Itr->first];
		}


		str_Value += "'"+Itr->second+"'";

		if (i_count != m_mapPICS.size())
		{
		   str_ID += ",";
		   str_Value += ",";
		}
	}
	str_ID += ")";
	str_Value += ")";

	str_Temp = "pt" + m_mapPICS["process"];
	str_sql = "Insert into " + str_Temp;
	str_sql += str_ID;
	str_sql += " values " + str_Value;

	return str_sql;

}

CString GetInsertTS_String()
{

	CString str_sql = _T("");

    if (strcmp(m_mapPICS["process"],"IMEI") == 0)
    {
		str_sql = "Insert into TS (id, model, ngprocess, ngwrkstation, ngdate, ngtime, nguser) values ('"+m_mapPICS[_T("PICASSO")]+"','"+m_mapPICS[_T("model")]+"','"+m_mapPICS[_T("process")]+"','"+m_mapPICS[_T("workstation")]+"','"+m_mapPICS[_T("tstdate")]+"','"+m_mapPICS[_T("tsttime")]+"','"+m_mapPICS[_T("employee")]+"')";
	}
	else
	{
        str_sql = "Insert into TS (id, model, ngprocess, ngwrkstation, ngdate, ngtime, nguser) values ('"+m_mapPICS[_T("id")]+"','"+m_mapPICS[_T("model")]+"','"+m_mapPICS[_T("process")]+"','"+m_mapPICS[_T("workstation")]+"','"+m_mapPICS[_T("tstdate")]+"','"+m_mapPICS[_T("tsttime")]+"','"+m_mapPICS[_T("employee")]+"')";
	}
	
	return str_sql;

}


CString GetCommon_String(CString str_tablename)
{
	CString str_Value = _T("(");
	CString str_ID = _T("(");
	CString str_Temp = _T("");
	CString str_sql = _T("");
	unsigned int i_count = 0;
	CString str_IDTemp = _T("");

	std::map<CString,CString>::iterator Itr;

	for(Itr = m_mapPICS.begin(),i_count=1; Itr != m_mapPICS.end(),i_count <= m_mapPICS.size(); Itr++,i_count++)
	{
		str_IDTemp = m_map_matchingTable[Itr->first];
        if ( str_IDTemp == "")
		{ 
			str_Temp.Format("(Warning)Found Index:(%s) which not define in matching table %s",Itr->first,m_mapPICS["coltype"]);
			DBLOG(str_Temp);
			continue;
		}
		else
		{
			str_ID += Itr->first;
		}

		str_Value += "'"+Itr->second+"'";

		if (i_count != m_mapPICS.size())
		{
			str_ID += ",";
			str_Value += ",";
		}
	}

    int i_IDlength = str_ID.GetLength();
	int i_Valuelength = str_Value.GetLength();

	str_ID = str_ID.Left(i_IDlength-1);
	str_Value = str_Value.Left(i_Valuelength-1);

	str_ID += ")";
	str_Value += ")";

	str_Temp = str_tablename;
	str_sql = "Insert into " + str_Temp;
	str_sql += str_ID;
	str_sql += " values " + str_Value;

	return str_sql;
   
}

bool CheckProcess(int i_checkrule,char * sz_id, char * sz_model, char * sz_process , char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_model = sz_model;
	CString str_process = sz_process;
    CString str_PCSSeq = "";
	CString str_SUBSeq = "";

	CString str_Fixdate = "0";
	CString str_Fixtime = "0";
	CString str_Returnpro = "";
	CString str_TXModel = "";
	CString str_id = sz_id;

	int i_ReturnPro = 0;

   /* V1.0.1.1 Sunlight 20110608  * 
	Get the ng return process sequence number to check the tstdate/tsttime of the process which after ngretrurnprocess
	the last tstdate/tsttime must be last than fixdate/fixtime */
	/*V1.0.5.5,Sunlight modify 20120507,for CSD model*/
	if (strcmp(sz_model,"GORACS1")==0)
	{
		str_sql = _T("SELECT * FROM ts WHERE id='"+str_id+"' and status='F' and model='"+str_model+"' ORDER BY fixdate DESC, fixtime DESC");
	}
	else
        str_sql = _T("SELECT * FROM ts WHERE id='"+str_id+"' and status='F' ORDER BY fixdate DESC, fixtime DESC");


	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information from ts fail, SQL:%s "),str_sql);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{

		thevalue=p_recordSet->GetCollect("fixdate");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Fixdate=(char *)(_bstr_t)(thevalue);
		}


		thevalue=p_recordSet->GetCollect("fixtime");	  //Segment sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Fixtime=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("rtnprocess");	  //TS ng return process
		if (thevalue.vt != VT_NULL)
		{
			str_Returnpro=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("model");	  //TS ng model
		if (thevalue.vt != VT_NULL)
		{
			str_TXModel=(char *)(_bstr_t)(thevalue);
		}

		if(str_Returnpro.GetLength() < 2)
		{
			strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
			sprintf(sz_ErrMsg,("The nrtprocess is empty for id:%s in TS \n此ID在TS系统中维修记录缺少回流站别定义，请联系工程师解决"),str_id);
			return false;
		}

        /* Get the return process sequence */
//		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and process= '"+str_Returnpro+"'");
		str_sql = _T("select * from picsrouting where model = '"+str_TXModel+"' and process= '"+str_Returnpro+"'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
			sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_PCSSeq=(char *)(_bstr_t)(thevalue);
			}

			thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
			if (thevalue.vt != VT_NULL)
			{
				str_SUBSeq=(char *)(_bstr_t)(thevalue);
			}

			i_ReturnPro = atoi(str_SUBSeq)*10 + atoi(str_PCSSeq);
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
			sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_Returnpro,str_model,str_Returnpro);
			return false;
		}

	}

    /* Get the current process sequence number*/
	str_SUBSeq.Empty();
	str_PCSSeq.Empty();
	str_sql = _T("select * from picsrouting where model = '"+str_model+"' and process= '"+str_process+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
		sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_PCSSeq=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
		if (thevalue.vt != VT_NULL)
		{
			str_SUBSeq=(char *)(_bstr_t)(thevalue);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
		sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_process,str_model,str_process);
		return false;
	}

/* check the routing */
	if (atoi(str_SUBSeq) < 1 || atoi(str_PCSSeq) < 1)
	{
		strcpy(sz_ErrCode, ERROR_PCS_SUB_SEQ_INVALID);
		sprintf(sz_ErrMsg,("Process sequence invalid , SQL:%s \n系统里面没有此process顺序异常，请联系工程师解决"),str_sql);
		return false;
	}
	if (atoi(str_SUBSeq) == 1 && atoi(str_PCSSeq) == 1)
	{
		return true;
	}

//	if(atoi(str_SUBSeq) > 1) // > T-Line
//	{
//		//first check T-Line Query T-Line Model Magan 20140714
//		CString strRefModel = _T("");
//		str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'TLINE'");
//		if(!GetQueryResult(str_sql, p_recordSet))
//		{
//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
//			return false;
//		}
//
//		if(!p_recordSet -> GetadoEOF())  
//		{
//			thevalue=p_recordSet->GetCollect("refmodel");	
//			if (thevalue.vt != VT_NULL)
//			{
//				strRefModel=(char *)(_bstr_t)(thevalue);
//			}
//		}
//		else // no record used str_model
//			strRefModel = str_model;
//
//		str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '1' and process <> 'OOB' order by pcsseq");  //check t-line all process
//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//		{
//			strRefModel.ReleaseBuffer();
//			return false;
//		}
//		strRefModel.ReleaseBuffer();
//		if(	atoi(str_SUBSeq) > 2) // > A-Line
//		{
//			str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'FA'");
//			if(!GetQueryResult(str_sql, p_recordSet))
//			{
//				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//				sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
//				return false;
//			}
//
//			if(!p_recordSet -> GetadoEOF())  
//			{
//				thevalue=p_recordSet->GetCollect("refmodel");	
//				if (thevalue.vt != VT_NULL)
//				{
//					strRefModel=(char *)(_bstr_t)(thevalue);
//				}
//			}
//			else // no record used str_model
//				strRefModel = str_model;
//			
//			str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '2' and process <> 'OOB' order by pcsseq");  //check FA all process
//			if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//			{
//				strRefModel.ReleaseBuffer();
//				return false;
//			}
//			strRefModel.ReleaseBuffer();
//		}
//
//		if(	atoi(str_SUBSeq) > 3) // > FQC
//		{
//			str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'FQC'");
//			if(!GetQueryResult(str_sql, p_recordSet))
//			{
//				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//				sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
//				return false;
//			}
//
//			if(!p_recordSet -> GetadoEOF())  
//			{
//				thevalue=p_recordSet->GetCollect("refmodel");	
//				if (thevalue.vt != VT_NULL)
//				{
//					strRefModel=(char *)(_bstr_t)(thevalue);
//				}
//			}
//			else // no record used str_model
//				strRefModel = str_model;
//			
//			str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '3' and process <> 'OOB' order by pcsseq");  //check FA all process
//			if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//			{
//				strRefModel.ReleaseBuffer();
//				return false;
//			}
//			strRefModel.ReleaseBuffer();
//		}	
//
//		if(	atoi(str_SUBSeq) > 4) // > RUN_IN
//		{
//			str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'RUN_IN'");
//			if(!GetQueryResult(str_sql, p_recordSet))
//			{
//				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//				sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
//				return false;
//			}
//
//			if(!p_recordSet -> GetadoEOF())  
//			{
//				thevalue=p_recordSet->GetCollect("refmodel");	
//				if (thevalue.vt != VT_NULL)
//				{
//					strRefModel=(char *)(_bstr_t)(thevalue);
//				}
//			}
//			else // no record used str_model
//				strRefModel = str_model;
//			
//			str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '4' and process <> 'OOB' order by pcsseq");  //check FA all process
//			if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//			{
//				strRefModel.ReleaseBuffer();
//				return false;
//			}
//			strRefModel.ReleaseBuffer();
//		}
//
//		if(	atoi(str_SUBSeq) > 5) // > PACKAGE
//		{
//			str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'PACKAGE'");
//			if(!GetQueryResult(str_sql, p_recordSet))
//			{
//				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//				sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
//				return false;
//			}
//
//			if(!p_recordSet -> GetadoEOF())  
//			{
//				thevalue=p_recordSet->GetCollect("refmodel");	
//				if (thevalue.vt != VT_NULL)
//				{
//					strRefModel=(char *)(_bstr_t)(thevalue);
//				}
//			}
//			else // no record used str_model
//				strRefModel = str_model;
//			
//			str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '5' and process <> 'OOB' order by pcsseq");  //check FA all process
//			if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//			{
//				strRefModel.ReleaseBuffer();
//				return false;
//			}
//			strRefModel.ReleaseBuffer();
//		}
//	}
//	if (atoi(str_PCSSeq) > 1 )
//	{
////		if (i_checkrule == 1)
//		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process <> 'OOB' order by pcsseq");  //sunlight add 20120612, 新机种FQC加入正常process卡关机制
////		else
////		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process<> 'FQC' and process <> 'OOB' order by pcsseq");
//		
//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_model,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
//		{
//			return false;
//		}
//	}


	if (atoi(str_PCSSeq) > 1 )
	{
//		if (i_checkrule == 1)
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process <> 'OOB' order by pcsseq");  //sunlight add 20120612, 新机种FQC加入正常process卡关机制
//		else
//		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process<> 'FQC' and process <> 'OOB' order by pcsseq");
		
		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_model,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
		{
			return false;
		}
	}
	if (atoi(str_SUBSeq) > 1)
	{
//		if (i_checkrule == 1)
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq < "+str_SUBSeq+" and process <> 'OOB' order by subseq,pcsseq");   //sunlight add 20120612, 新机种FQC加入正常process卡关机制
//		else
//		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq < "+str_SUBSeq+" and process<> 'FQC' and process <> 'OOB' order by subseq,pcsseq");
		
		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_model,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
		{
			return false;
		}
	}

    return true;
}


bool CheckProcess_V2(int i_checkrule,char * sz_id, char * sz_model, char * sz_process , char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_model = sz_model;
	CString str_process = sz_process;
    CString str_PCSSeq = "";
	CString str_SUBSeq = "";

	CString str_Fixdate = "0";
	CString str_Fixtime = "0";
	CString str_ngDate = "0";
	CString str_ngTime = "0";

	CString str_Returnpro = "";
	CString str_id = sz_id;
	CString str_ngModel;

	CString str_msg;

	int i_ReturnPro = 0;

	bool b_CSDSpecial = false;
	bool b_CSDSpecial1 = false;
	bool b_CSDSpecialA = false;

	char sz_newModel[10]={0};
	strcpy(sz_newModel,sz_model);

    str_sql = _T("SELECT * FROM ts WHERE id='"+str_id+"'ORDER BY fixdate DESC, fixtime DESC");


	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information from ts fail, SQL:%s "),str_sql);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		thevalue=p_recordSet->GetCollect("fixdate");
		if (thevalue.vt != VT_NULL)
		{
			str_Fixdate=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("fixtime");
		if (thevalue.vt != VT_NULL)
		{
			str_Fixtime=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("ngdate");
		if (thevalue.vt != VT_NULL)
		{
			str_ngDate=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("ngtime");
		if (thevalue.vt != VT_NULL)
		{
			str_ngTime=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("rtnprocess");	  //TS ng return process
		if (thevalue.vt != VT_NULL)
		{
			str_Returnpro=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("model");	  //TS ng model
		if (thevalue.vt != VT_NULL)
		{
			str_ngModel=(char *)(_bstr_t)(thevalue);
		}
	}

	if(str_Returnpro != _T("")) //have TS record
	{
		if (str_Fixdate==""|| str_Fixdate=="0")
		{
			str_Fixdate = str_ngDate;
			str_Fixtime = str_ngTime;
		}
		if(str_ngModel.Mid(4,2).CompareNoCase(_T("CS")) == 0)
		{ // CS must check from T-Line firset station
			char sz_firstProcess[20]={0};
			if(str_model == str_ngModel) // current CS need get normal model first station
			{
				CString strTemp;
				strTemp.Format(_T("%sB%sA"), str_ngModel.Left(4) ,str_ngModel.Mid(5,1));
//				str_model = strTemp;
				sprintf(sz_newModel, "%s",strTemp);
			}
			if(QueryFirstProcess(sz_newModel,sz_firstProcess,sz_ErrCode,sz_ErrMsg))
			{
				str_Returnpro = sz_firstProcess;

				str_msg.Format("=== Current model=%s, TS model=%s, should change return process as normal model first station=%s to check routing",str_model,str_ngModel,sz_firstProcess);
				DBLOG(str_msg);
				sprintf(sz_newModel, "%s",str_model);

			}
			else
			{
				return false;
			}
		}
		else // ng return process
		{
			if((str_model.Left(1).CompareNoCase(_T("S")) == 0) || (str_model.Mid(4,2).CompareNoCase(_T("CS")) == 0) )
			{
				CString strTemp;
				str_sql = _T("SELECT * FROM picsrouting WHERE model='"+str_ngModel+"' and process ='"+str_Returnpro+"'");
				if(!GetQueryResult(str_sql, p_recordSet))
				{
					strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
					sprintf(sz_ErrMsg,("Query segment from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
					return false;
				}

				if ( !p_recordSet -> GetadoEOF() )
				{
					thevalue=p_recordSet->GetCollect("segment");	  //process sequence
					if (thevalue.vt != VT_NULL)
					{
						strTemp=(char *)(_bstr_t)(thevalue);
					}
				}
				else
				{
					strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
					sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get segment from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的线别记录，请联系工程师解决"),str_ngModel,str_Returnpro,str_ngModel,str_Returnpro);
					return false;
				}

				if(strTemp != _T("TLINE")) //SX + CS ignore A-Line/P-LIne
				{
					str_msg.Format("=== Current model=%s, TS model=%s, ReturnPro=%s is in A-line, ignore TS record",str_model,str_ngModel,str_Returnpro);
					DBLOG(str_msg);
					str_Fixdate = "0";
					str_Fixtime = "0";
					str_ngDate = "0";
					str_ngTime = "0";
					str_Returnpro = "";
					i_ReturnPro = 0;
				}
			}
		}
	}
//	else  //have No TS Record
//	{
//	}
		//CSD model first station need check normal model PRe-Stage
		//CS -- > T-Line
		//SX -- > T-Line
		//TX -- > T-Line, A-Line

	/* Get the return process sequence */
	if (str_Returnpro!="")
	{
		CString strTemp;
		if ((str_model.Mid(4,2).CompareNoCase(_T("CS")) == 0))//CSD model
		{
			strTemp.Format(_T("%sB%sA"), str_model.Left(4) ,str_model.Right(1));
		}
		else
			strTemp = str_model;
		DBLOG("=== Get the return process sequence");
		str_sql = _T("select * from picsrouting where model = '"+strTemp+"' and process= '"+str_Returnpro+"'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
			sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_PCSSeq=(char *)(_bstr_t)(thevalue);
			}

			thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
			if (thevalue.vt != VT_NULL)
			{
				str_SUBSeq=(char *)(_bstr_t)(thevalue);
			}

			i_ReturnPro = atoi(str_SUBSeq)*10 + atoi(str_PCSSeq);
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
			sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_Returnpro,str_model,str_Returnpro);
			return false;
		}
	}

	DBLOG("=== Get current process sequence");
	str_SUBSeq.Empty();
	str_PCSSeq.Empty();
	str_sql = _T("select * from picsrouting where model = '"+str_model+"' and process= '"+str_process+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
		sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_PCSSeq=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
		if (thevalue.vt != VT_NULL)
		{
			str_SUBSeq=(char *)(_bstr_t)(thevalue);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
		sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_process,str_model,str_process);
		return false;
	}

	if((str_model.Mid(4,2).CompareNoCase(_T("CS")) == 0) )//CSD model check Normal T-Line
	{
		str_msg.Format("=== csd model and in csd first station,assign to check T-line routing using normal model name");
		DBLOG(str_msg);
		str_SUBSeq = "2";
		str_PCSSeq = "1";
		CString strTemp;
		strTemp.Format(_T("%sB%sA"), str_model.Left(4) ,str_model.Right(1));
		str_model = strTemp;
		sprintf(sz_newModel,"%s",strTemp);
	}
	else if((str_model.Left(1).CompareNoCase(_T("S")) == 0))
	{
		str_msg.Format("=== csd model and in csd first station,assign to check T-line routing using normal model name");
		DBLOG(str_msg);
		str_SUBSeq = "2";
		str_PCSSeq = "1";
		str_model.SetAt(0, _T('G'));
		sprintf(sz_newModel,"%s",str_model);
	}
	else if((str_model.Left(1).CompareNoCase(_T("T")) == 0))
	{
		str_msg.Format("=== csd model and in csd first station,assign to check T-line/A-Line routing using normal model name");
		DBLOG(str_msg);
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and segment= 'PACKAGE'and pcsseq = '1'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
			sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_PCSSeq=(char *)(_bstr_t)(thevalue);
			}

			thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
			if (thevalue.vt != VT_NULL)
			{
				str_SUBSeq=(char *)(_bstr_t)(thevalue);
			}
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
			sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_process,str_model,str_process);
			return false;
		}
		str_model.SetAt(0, _T('G'));
		sprintf(sz_newModel,"%s",str_model);
	}


/* check the routing */
	if (atoi(str_SUBSeq) < 1 || atoi(str_PCSSeq) < 1)
	{
		strcpy(sz_ErrCode, ERROR_PCS_SUB_SEQ_INVALID);
		sprintf(sz_ErrMsg,("Process sequence invalid , SQL:%s \n系统里面没有此process顺序异常，请联系工程师解决"),str_sql);
		return false;
	}
	if (atoi(str_SUBSeq) == 1 && atoi(str_PCSSeq) == 1)
	{
		return true;
	}

	if (atoi(str_SUBSeq) > 1)
	{
		//		if (i_checkrule == 1)
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq < "+str_SUBSeq+" and process <> 'OOB' order by subseq,pcsseq");   //sunlight add 20120612, 新机种FQC加入正常process卡关机制
		//		else
		//		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq < "+str_SUBSeq+" and process<> 'FQC' and process <> 'OOB' order by subseq,pcsseq");

		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_newModel,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
		{
			return false;
		}
	}
	//if(atoi(str_SUBSeq) > 1) // > T-Line
	//{
	//	//first check T-Line Query T-Line Model Magan 20140714
	//	CString strRefModel = _T("");
	//	str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'TLINE'");
	//	if(!GetQueryResult(str_sql, p_recordSet))
	//	{
	//		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//		sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
	//		return false;
	//	}

	//	if(!p_recordSet -> GetadoEOF())  
	//	{
	//		thevalue=p_recordSet->GetCollect("refmodel");	
	//		if (thevalue.vt != VT_NULL)
	//		{
	//			strRefModel=(char *)(_bstr_t)(thevalue);
	//		}
	//	}
	//	else // no record used str_model
	//		strRefModel = sz_newModel;

	//	str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '1' and process <> 'OOB' order by pcsseq");  //check t-line all process
	//	if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
	//	{
	//		strRefModel.ReleaseBuffer();
	//			return false;
	//	}
	//	strRefModel.ReleaseBuffer();

	//	if(	atoi(str_SUBSeq) > 2) // > A-Line
	//	{
	//		str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'FA'");
	//		if(!GetQueryResult(str_sql, p_recordSet))
	//		{
	//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
	//			return false;
	//		}

	//		if(!p_recordSet -> GetadoEOF())  
	//		{
	//			thevalue=p_recordSet->GetCollect("refmodel");	
	//			if (thevalue.vt != VT_NULL)
	//			{
	//				strRefModel=(char *)(_bstr_t)(thevalue);
	//			}
	//		}
	//		else // no record used str_model
	//			strRefModel = sz_newModel;
	//		
	//		str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '2' and process <> 'OOB' order by pcsseq");  //check FA all process
	//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
	//		{
	//			strRefModel.ReleaseBuffer();
	//			return false;
	//		}
	//		strRefModel.ReleaseBuffer();
	//	}

	//	if(	atoi(str_SUBSeq) > 3) // > FQC
	//	{
	//		str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'FQC'");
	//		if(!GetQueryResult(str_sql, p_recordSet))
	//		{
	//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
	//			return false;
	//		}

	//		if(!p_recordSet -> GetadoEOF())  
	//		{
	//			thevalue=p_recordSet->GetCollect("refmodel");	
	//			if (thevalue.vt != VT_NULL)
	//			{
	//				strRefModel=(char *)(_bstr_t)(thevalue);
	//			}
	//		}
	//		else // no record used str_model
	//			strRefModel = sz_newModel;
	//		
	//		str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '3' and process <> 'OOB' order by pcsseq");  //check FA all process
	//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
	//		{
	//			strRefModel.ReleaseBuffer();
	//			return false;
	//		}
	//		strRefModel.ReleaseBuffer();
	//	}	

	//	if(	atoi(str_SUBSeq) > 4) // > RUN_IN
	//	{
	//		str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'RUN_IN'");
	//		if(!GetQueryResult(str_sql, p_recordSet))
	//		{
	//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
	//			return false;
	//		}

	//		if(!p_recordSet -> GetadoEOF())  
	//		{
	//			thevalue=p_recordSet->GetCollect("refmodel");	
	//			if (thevalue.vt != VT_NULL)
	//			{
	//				strRefModel=(char *)(_bstr_t)(thevalue);
	//			}
	//		}
	//		else // no record used str_model
	//			strRefModel = sz_newModel;
	//		
	//		str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '4' and process <> 'OOB' order by pcsseq");  //check FA all process
	//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
	//		{
	//			strRefModel.ReleaseBuffer();
	//			return false;
	//		}
	//		strRefModel.ReleaseBuffer();
	//	}

	//	if(	atoi(str_SUBSeq) > 5) // > PACKAGE
	//	{
	//		str_sql = _T("select * from picsmdlinseg where model = '"+str_model+"' and segment = 'PACKAGE'");
	//		if(!GetQueryResult(str_sql, p_recordSet))
	//		{
	//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
	//			return false;
	//		}

	//		if(!p_recordSet -> GetadoEOF())  
	//		{
	//			thevalue=p_recordSet->GetCollect("refmodel");	
	//			if (thevalue.vt != VT_NULL)
	//			{
	//				strRefModel=(char *)(_bstr_t)(thevalue);
	//			}
	//		}
	//		else // no record used str_model
	//			strRefModel = sz_newModel;
	//		
	//		str_sql = _T("select * from picsrouting where model = '"+strRefModel+"' and subseq = '5' and process <> 'OOB' order by pcsseq");  //check FA all process
	//		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, strRefModel.GetBuffer(),str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
	//		{
	//			strRefModel.ReleaseBuffer();
	//			return false;
	//		}
	//		strRefModel.ReleaseBuffer();
	//	}
	//}
	if (atoi(str_PCSSeq) > 1 )
	{
//		if (i_checkrule == 1)
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process <> 'OOB' order by pcsseq");  //sunlight add 20120612, 新机种FQC加入正常process卡关机制
//		else
//		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process<> 'FQC' and process <> 'OOB' order by pcsseq");
		
		if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_newModel,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
		{
			return false;
		}
	}

	str_model = sz_model;
	if((str_model.Left(1) .CompareNoCase(_T("G")) != 0) || (str_model.Mid(4,2).CompareNoCase(_T("CS")) == 0) )
	{
		str_msg.Format("=== Current model=%s,ReturnPro=%s is in T-line,current process=%s, check routing as CSD model",sz_model,str_Returnpro,str_process);
		DBLOG(str_msg);

		strcpy(sz_newModel,sz_model);
		str_model = sz_model;

		DBLOG("=== Get current process sequence");
		str_SUBSeq.Empty();
		str_PCSSeq.Empty();
		str_sql = _T("select * from picsrouting where model = '"+str_model+"' and process= '"+str_process+"'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
			sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("pcsseq");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_PCSSeq=(char *)(_bstr_t)(thevalue);
			}

			thevalue=p_recordSet->GetCollect("subseq");	  //Segment sequence
			if (thevalue.vt != VT_NULL)
			{
				str_SUBSeq=(char *)(_bstr_t)(thevalue);
			}
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_INVALIDROUTING_SEQ);
			sprintf(sz_ErrMsg,("The model=%s,process=%s,can't get process sequence from picsrouting fail \n针对机种：%s系统里面没有此站别：%s的顺序记录，请联系工程师解决"),str_model,str_process,str_model,str_process);
			return false;
		}	

		if(str_PCSSeq.CompareNoCase(_T("1")) != 0) //not first station check csd routing for csd owner process
		{
			//str_Fixdate = "0";
			//str_Fixtime = "0";
			//str_ngDate = "0";
			//str_ngTime = "0";
			//str_Returnpro = "";
			//i_ReturnPro = 0;

			/* check the routing */
			if (atoi(str_SUBSeq) < 1 || atoi(str_PCSSeq) < 1)
			{
				strcpy(sz_ErrCode, ERROR_PCS_SUB_SEQ_INVALID);
				sprintf(sz_ErrMsg,("Process sequence invalid , SQL:%s \n系统里面没有此process顺序异常，请联系工程师解决"),str_sql);
				return false;
			}

			if (atoi(str_PCSSeq) > 1 )
			{
				//		if (i_checkrule == 1)
				str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process <> 'OOB' order by pcsseq");  //sunlight add 20120612, 新机种FQC加入正常process卡关机制
				//		else
				//		    str_sql = _T("select * from picsrouting where model = '"+str_model+"' and subseq = "+str_SUBSeq+" and pcsseq < "+str_PCSSeq+" and process<> 'FQC' and process <> 'OOB' order by pcsseq");

				if(!GetPreProcessResult(i_checkrule,str_sql, sz_id, sz_newModel,str_Fixdate,str_Fixtime,i_ReturnPro,sz_ErrCode, sz_ErrMsg))
				{
					return false;
				}
			}
		}
	}
	
    return true;
}


/*****************************************************************************
* Function name: GetProcessResult
* Summary      : Check the test result of this process, 最后一次PASS 就算PASS
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/05/03         First version             
*****************************************************************************/
bool GetProcessResult(char * sz_id, char * sz_model,char * sz_process)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model =sz_model;

	str_table = _T("PT");
	str_table += sz_process;

//Eg: select * from ptbbtest where id = 'AB12345678' order by tstdate desc,tsttime desc
	
	/* Sunlight add 20110706 for SSS */
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	}
	else
	/*  Sunlight add 20110706 for SSS */
		str_sql = "select * from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_result.Empty();
		thevalue = p_recordSet->GetCollect("okng");
		if (thevalue.vt != VT_NULL)
		{
		   str_result = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_result,"Y",1) ==0 )
		{
			return true;
		}
		else
		{
			return false;  //The result is NG
		}
	}
	else
	{
		if (str_table == _T("PTIQT"))//for IQT sampling inspection, used in warehouse tool,(Nancy.Yang 20140715)
		{
			return true;
		}
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: GetProcessResult_Ngreturn
* Summary      : Check all the test result which process is before the current process,最后一次PASS 就算PASS
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/05/03         First version             
*****************************************************************************/
bool GetProcessResult_NgReturn(char * sz_id, char * sz_model, char * sz_process, const CString str_Fixdate, const CString str_Fixtime)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;

	str_table = _T("PT");
	str_table += sz_process;

//Eg: select * from ptbbtest where id = 'AB12345678' order by tstdate desc,tsttime desc
	/* Sunlight add 20110706 for SSS */
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";
	}
	else
		/*  Sunlight add 20110706 for SSS */
	    str_sql = "select * from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";   
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_result.Empty();
		thevalue = p_recordSet->GetCollect("okng");
		if (thevalue.vt != VT_NULL)
		{
		   str_result = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_result,"Y",1) ==0 )
		{
			return true;
		}
		else
		{
			return false;  //The result is NG
		}
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: GetErrorCode  
* Summary      : Calculate the error code base on process sequecce and segment sequence
* Parameters   : process sequence, segment sequence
* Return       : error code
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/05/03         First version             
*****************************************************************************/
CString GetErrorCode(int pcsseq, int subseq)
{
	int ErrNumber =0;
	char ErrCode[10] = {0};

	ErrNumber = 1100 + subseq*10 + pcsseq;  // Eg: 1121 mean check process fail at segment 2 process 1
    G_ErrNumber = ErrNumber;
    sprintf(ErrCode, "DB%d", ErrNumber);

    return ErrCode;
}

bool CheckTSUnFixedData(char * sz_id, char * sz_ngprocss,char * sz_ErrCode,char * sz_TSModelName)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Temp = _T("");
	CString str_id = sz_id;

	str_sql = "select * from TS where id = '"+str_id+"' and status ='W'";  //W: waiting for fixed, F: Fixed
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("ngprocess");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)(_bstr_t)(thevalue);
			sprintf(sz_ngprocss,"%s",str_Temp);
		}

		thevalue=p_recordSet->GetCollect("model");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_TSModelName,"%s",str_Temp);
		}    
	    strcpy(sz_ErrCode , ERROR_TESTFAIL_UNFIXED);
		return false;
	}

	p_recordSet->Close();
	return true;
}

/*Sunlight,连续的fail才计入retest count*/
bool CheckRetestCount(char * sz_id, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_id = sz_id;
	CString str_Table = "";
	CString str_fixdate = "0";
	CString str_fixtime = "0";
	int i_RetestCount = 0;
	int i_TestCount = 0;
	
	SYSTEMTIME time;
	::GetLocalTime(&time);

	m_mapPICS[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
	m_mapPICS[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);

	/* Get Retest Count */
	i_RetestCount = GetPrivateProfileInt(_T("SETTING"), _T("RetestCount"), 2, DBCONFIGFILE);

	/* Sunlight add 20110627 */
	if (i_RetestCount <= 0)
	{
		strcpy(sz_ErrCode , ERROR_RETESTCOUNT_INVALID);
		sprintf(sz_ErrMsg,("Retest count=%d define is invalid.\n重测试次数=d%, 定义不合法"),i_RetestCount,i_RetestCount);
		return false;
	}
	/* Sunlight add end */

    str_Table = "PT";
	str_Table += sz_process;

	str_sql = _T("SELECT * FROM ts WHERE id='"+str_id+"' and status='F' ORDER BY fixdate DESC, fixtime DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information from ts fail, SQL:%s "),str_sql);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		thevalue=p_recordSet->GetCollect("fixdate");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_fixdate=(char *)(_bstr_t)(thevalue);
		}
	
		thevalue=p_recordSet->GetCollect("fixtime");	  //Segment sequence
		if (thevalue.vt != VT_NULL)
		{
			str_fixtime=(char *)(_bstr_t)(thevalue);
		}    
        /* Sunlight add 20110711 , for IMEI process, id=IMEI, l1=Picasso*/
		if (strcmp(sz_process,"IMEI") == 0)
		{
			str_sql = "SELECT * FROM "+str_Table+" WHERE l1 = '"+str_id+"' AND (( tstdate = "+str_fixdate+" AND tsttime > "+str_fixtime+") OR tstdate > "+str_fixdate+") ORDER BY tstdate DESC, tsttime DESC";			
		}
		else
		/* Sunlight end 20110711, for IMEI process, id=IMEI, l1=Picasso */

        /* Sunlight modify 20110627, because of retest count calculate change  */
//		str_sql = "select count(*) from "+str_Table+" where id = '"+str_id+"' and okng ='N' and (( tstdate = "+str_fixdate+" and tsttime > "+str_fixtime+") or tstdate > "+str_fixdate+")";	
		   str_sql = "SELECT * FROM "+str_Table+" WHERE id = '"+str_id+"' AND (( tstdate = "+str_fixdate+" AND tsttime > "+str_fixtime+") OR tstdate > "+str_fixdate+") ORDER BY tstdate DESC, tsttime DESC";			
	    /*  Sunlight modify and 20110627*/
	}
	else
	{
		/* Sunlight add 20110711 , for IMEI process, id=IMEI, l1=Picasso*/
		if (strcmp(sz_process,"IMEI") == 0)
		{
			str_sql = "SELECT * FROM "+str_Table+" WHERE l1 = '"+str_id+"' ORDER BY tstdate DESC, tsttime DESC";		
		}
		else
		/* Sunlight end 20110711, for IMEI process, id=IMEI, l1=Picasso */
		/* Sunlight modify 20110627, because of retest count calculate change  */
//      str_sql = "select count(*) from "+str_Table+" where id = '"+str_id+"' and okng ='N'";
		str_sql = "SELECT * FROM "+str_Table+" WHERE id = '"+str_id+"' ORDER BY tstdate DESC, tsttime DESC";
	    /*  Sunlight modify and */
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode , ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Get test count from %s fail, SQL:%s"),str_Table,str_sql);
		return false;
	}

    if( !p_recordSet -> GetadoEOF() )
	{
		/* Sunlight modify 20110627, because of retest count calculate change  */
		//str_Temp.Empty();
		//thevalue=p_recordSet->GetCollect("");	  //count
		//str_Temp=(char *)(_bstr_t)(thevalue);
		//i_TestCount = atoi(str_Temp);
		//if (i_TestCount >= i_RetestCount)
		//{
		//	strcpy(sz_ErrCode , ERROR_CHECK_RETESTCOUNT_FAIL);
		//	sprintf(sz_ErrMsg,("Test fail count %d is more than the target %d \n此次测试已经超过重测次数的限定，请送TS 维修"),i_TestCount,i_RetestCount);
		//	return false;
		//}
		for(i_TestCount=0;(i_TestCount<i_RetestCount) && (!p_recordSet -> GetadoEOF() ) ;i_TestCount++)
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("okng");	  //count
			str_Temp=(char *)(_bstr_t)(thevalue);
			if (strcmp(str_Temp,"Y") == 0)
			{
				p_recordSet ->Close();
				return true;
			}
			p_recordSet ->MoveNext();
		}

		if (i_TestCount >= i_RetestCount)
		{
			strcpy(sz_ErrCode , ERROR_CHECK_RETESTCOUNT_FAIL);
			sprintf(sz_ErrMsg,("Test fail count %d is more than the target %d \n此次测试已经超过重测次数的限定，请送TS 维修"),i_TestCount,i_RetestCount);
			p_recordSet ->Close();
			return false;
		}
		/* Sunlight modify 20110627 end  */
	}

	p_recordSet ->Close();
	return true;

}

/*Sunlight, 20111010, 只要是fail,都计入retest count*/
bool CheckRetestCount_new(char * sz_id, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_id = sz_id;
	CString str_Table = "";
	CString str_fixdate = "0";
	CString str_fixtime = "0";
	int i_RetestCount = 0;
	int i_TestCount = 0;
	CString str_DBmsg = "";

	SYSTEMTIME time;
	::GetLocalTime(&time);

	m_mapPICS[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
	m_mapPICS[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);

	/* Get Retest Count */
	i_RetestCount = GetPrivateProfileInt(_T("SETTING"), _T("RetestCount"), 2, DBCONFIGFILE);

	str_DBmsg.Format("Retest count read from file is: %d ",i_RetestCount);
	DBLOG(str_DBmsg);

	/* Sunlight add 20110627 */
	if (i_RetestCount <= 0)
	{
		strcpy(sz_ErrCode , ERROR_RETESTCOUNT_INVALID);
		sprintf(sz_ErrMsg,("Retest count=%d define is invalid.\n重测试次数=d%, 定义不合法"),i_RetestCount,i_RetestCount);
		DBLOG(sz_ErrMsg);
		return false;
	}
	/* Sunlight add end */

	str_Table = "PT";
	str_Table += sz_process;

	str_sql = _T("SELECT * FROM ts WHERE id='"+str_id+"' and status='F' ORDER BY fixdate DESC, fixtime DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information from ts fail, SQL:%s\n数据库操作异常，请联系工程师"),str_sql);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		thevalue=p_recordSet->GetCollect("fixdate");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_fixdate=(char *)(_bstr_t)(thevalue);
		}

		thevalue=p_recordSet->GetCollect("fixtime");	  //Segment sequence
		if (thevalue.vt != VT_NULL)
		{
			str_fixtime=(char *)(_bstr_t)(thevalue);
		}    
		/* IMEI process, id=IMEI, l1=Picasso*/
		if (strcmp(sz_process,"IMEI") == 0)
		{
			str_sql = "select count(*) from "+str_Table+" where l1 = '"+str_id+"' and okng ='N' and (( tstdate = "+str_fixdate+" and tsttime > "+str_fixtime+") or tstdate > "+str_fixdate+")";		
		}
		else
			str_sql = "select count(*) from "+str_Table+" where id = '"+str_id+"' and okng ='N' and (( tstdate = "+str_fixdate+" and tsttime > "+str_fixtime+") or tstdate > "+str_fixdate+")";			
	}
	else
	{
		/* IMEI process, id=IMEI, l1=Picasso*/
		if (strcmp(sz_process,"IMEI") == 0)
		{
			str_sql = "select count(*) from "+str_Table+" where l1 = '"+str_id+"' and okng ='N'";		
		}
		else
			str_sql = "select count(*) from "+str_Table+" where id = '"+str_id+"' and okng ='N'";
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode , ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Get test count from %s fail, SQL:%s\n数据库操作异常，请联系工程师"),str_Table,str_sql);
		return false;
	}

	if( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("");	  //count
		str_Temp=(char *)(_bstr_t)(thevalue);
		i_TestCount = atoi(str_Temp);

		str_DBmsg.Format("NG count is:%d",i_TestCount);
		DBLOG(str_DBmsg);

		if (i_TestCount >= i_RetestCount)
		{
			strcpy(sz_ErrCode , ERROR_CHECK_RETESTCOUNT_FAIL);
			sprintf(sz_ErrMsg,("Test fail count %d is more than the target %d \n此次测试已经超过重测次数的限定，请送TS 维修"),i_TestCount,i_RetestCount);
			DBLOG(sz_ErrMsg);
			p_recordSet ->Close();
			return false;
		}
	}

	p_recordSet ->Close();
	return true;

}

bool GetPreProcessResult(int i_checkrule,CString str_sql, char * sz_id, char * sz_model,const CString str_Fixdate, const CString str_Fixtime, const int i_ReturnPro, char * sz_ErrCode, char * sz_ErrMsg)
{ 
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_Temp = "";
	CString str_model = sz_model;
	char sz_TempPro[50] = {0};

	int i_PCSTemp = 0;
	int i_SUBTemp = 0;
	int i_CurrentPro = 0;
	bool b_DBresult = false;

	int i_special = 1;

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
		sprintf(sz_ErrMsg,("Query process sequence from picsrouting fail, SQL:%s \n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	while(!p_recordSet->GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("process");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_TempPro,_T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("pcsseq");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			i_PCSTemp = atoi(str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("subseq");
		if (thevalue.vt != VT_NULL)
		{
		   str_Temp = (char *)_bstr_t(thevalue);
		   i_SUBTemp = atoi(str_Temp);
		}

        i_CurrentPro = i_SUBTemp * 10 + i_PCSTemp;

		if ((i_SUBTemp == 1) && (str_model != "GLUNB1A") && (str_model.Find("GLUN",0) != -1))   //Luna TL 不判断model
		{
			if ((i_CurrentPro < i_ReturnPro) || (i_ReturnPro == 0))
			{
				if (!GetProcessResult_Luna(sz_id,sz_model,sz_TempPro))
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
			else
			{
				if (!GetProcessResult_NgReturn_Luna(sz_id,sz_model,sz_TempPro, str_Fixdate, str_Fixtime))
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s 大于维修时间的PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
		}
		/* Sunlight add 20120504, for GORACS1, TL 数据用GORAB1A */
		else if ((i_SUBTemp == 1) && (str_model == "GORACS1"))
		{
			if ((i_CurrentPro < i_ReturnPro) || (i_ReturnPro == 0))
			{
				if (!GetProcessResult_CSD(sz_id,sz_model,sz_TempPro))
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
			else
			{
				if (!GetProcessResult_NgReturn_CSD(sz_id,sz_model,sz_TempPro, str_Fixdate, str_Fixtime))
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s 大于维修时间的PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
		}/* Sunlight add 20120504 end */
		else  //其他Project 照常规rule check
		{
			if ((i_CurrentPro < i_ReturnPro) || (i_ReturnPro == 0))
			{
				/* Sunlight add 20111221, TL/TRIGGER;FA/FALINK,VI, RUN_IN;PACKAGE still use old rule*/
                i_special = 1; 
				if ( (strcmp(sz_TempPro ,"IMEI")==0) || (strcmp(sz_TempPro ,"FCHECK")==0) || (strcmp(sz_TempPro ,"OS_DL")==0)|| (strcmp(sz_TempPro ,"SHIPPINGOS")==0) || (strcmp(sz_TempPro ,"NAL_SCAN")==0) || (strcmp(sz_TempPro ,"PPIDLINK")==0)|| (strcmp(sz_TempPro ,"FQC")==0)|| (strcmp(sz_TempPro ,"VI")==0)|| (strcmp(sz_TempPro ,"FALINK")==0) || (strcmp(sz_TempPro ,"FALINK2")==0) ||(strcmp(sz_TempPro ,"FAPRINT")==0) ||(strcmp(sz_TempPro ,"CSD_LINK")==0) ||(strcmp(sz_TempPro ,"CSD_PRINT")==0) ||(strcmp(sz_TempPro ,"RUN_IN")==0) || (strcmp(sz_TempPro ,"TRIGGER")==0) || (strcmp(sz_TempPro ,"DL")==0)|| (strcmp(sz_TempPro ,"P_VI")==0) || (strcmp(sz_TempPro, "MMI_EM") == 0) )
				{
                    i_special = 0;
				}
				/*Sunlight add temp end */

				if ( (i_checkrule == 1) && (i_special == 1) ) //new rule,如果fail后，连续2次PASS 才算PASS
				    b_DBresult = GetProcessResult_new(sz_id,sz_model,sz_TempPro);
				else                   //old rule,最后一次PASS 即PASS
                    b_DBresult = GetProcessResult(sz_id,sz_model,sz_TempPro);

				if (!b_DBresult)
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
			else
			{
				/* Sunlight add 20120503, TL/TRIGGER;FA/FALINK,VI, RUN_IN;PACKAGE still use old rule*/
				i_special = 1; 
				if ( (strcmp(sz_TempPro ,"IMEI")==0) || (strcmp(sz_TempPro ,"FCHECK")==0) || (strcmp(sz_TempPro ,"OS_DL")==0)|| (strcmp(sz_TempPro ,"SHIPPINGOS")==0) || (strcmp(sz_TempPro ,"NAL_SCAN")==0) || (strcmp(sz_TempPro ,"PPIDLINK")==0)|| (strcmp(sz_TempPro ,"FQC")==0)|| (strcmp(sz_TempPro ,"VI")==0)|| (strcmp(sz_TempPro ,"P_VI")==0)|| (strcmp(sz_TempPro ,"FALINK")==0) || (strcmp(sz_TempPro ,"FALINK2")==0) ||(strcmp(sz_TempPro ,"FAPRINT")==0) ||(strcmp(sz_TempPro ,"CSD_LINK")==0) ||(strcmp(sz_TempPro ,"CSD_PRINT")==0) ||(strcmp(sz_TempPro ,"RUN_IN")==0) || (strcmp(sz_TempPro ,"TRIGGER")==0) || (strcmp(sz_TempPro ,"DL")==0) || (strcmp(sz_TempPro, "MMI_EM") == 0))
				{
					i_special = 0;
				}
				/*Sunlight add temp end */

				if ( (i_checkrule == 1) && (i_special == 1) ) //new rule,如果fail后，连续2次PASS 才算PASS
					b_DBresult = GetProcessResult_NgReturn_new(sz_id,sz_model,sz_TempPro, str_Fixdate, str_Fixtime);
				else                   //old rule,最后一次PASS 即PASS
					b_DBresult = GetProcessResult_NgReturn(sz_id,sz_model,sz_TempPro, str_Fixdate, str_Fixtime);
				
				if ( !b_DBresult)
				{
					str_Temp.Empty();
					str_Temp = GetErrorCode(i_PCSTemp, i_SUBTemp);
					sprintf(sz_ErrCode,"%s",str_Temp);
					sprintf(sz_ErrMsg,("Check id=%s, process=%s test data fail \n找不到在站别:%s 大于维修时间的PASS记录，请回到此站重新测试"),sz_id,sz_TempPro,sz_TempPro);
					return false;
				}
			}
		}
		p_recordSet->MoveNext();

	}

    p_recordSet->Close();

	return true;
}

bool CheckRouting(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
    CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	char c_ngprocess[20] = {0};
	char c_ngErrCode[20] = {0};
	char c_ngErrMsg[512] = {0};
	char c_ngModel[20] = {0};

	/* Check Picasso, Sunlight add 20110701 */ 
	if (strlen(sz_id) != 10 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_id);
		return false;
	}
	/*  Sunlight add end  */

	/* Check input ID, Sunlight add 20110726 */ 
	if (strlen(sz_model) < 3 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The model you input:%s is null \n机种名的长度不符合要求"),sz_model);
		return false;
	}

	if (strlen(sz_process) < 2 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The process you input:%s is null \n站别名的长度不符合要求"),sz_process);
		return false;
	}
	/*  Sunlight add end  */

	/* Check is there TS unfixed data */
	if (!CheckTSUnFixedData(sz_id,c_ngprocess,sz_ErrCode,c_ngModel))
	{
		sprintf(sz_ErrMsg,("This ID=%s have a unfixed data in TS,please check \n此机台在TS中有未修复记录，请送回TS确认"),sz_id);
		return false;
	}

	/* Check retest count data  */
	if (strcmp(sz_process,"FALINK")!=0 && strcmp(sz_process,"PPIDLINK")!=0)  
	{ /*sunlight modify 20120620, FALINK and PPIDLINK don't need check retest count*/ 
		if (!CheckRetestCount(sz_id,sz_process,sz_ErrCode,sz_ErrMsg))
		{
			return false;
		}
	}

	/*  Check retest count data  */
	if (!CheckProcess(0,sz_id, sz_model, sz_process , sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}

	return true;
}

/*****************************************************************************
* Function name: QuerySOCount  
* Summary      : Calculate the count for this SO, used to reprint the lable at FALINK process
* Parameters   : id, so, process
* Return       : next count (for this SO), error code, error message
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/05/30         First version             
*****************************************************************************/
bool QuerySOCount(char * sz_id, char * sz_so, char * sz_process, char * sz_count, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_SO = sz_so;
	CString str_ID = sz_id;
	CString str_Table = "";

	int i_Count = 0;

	str_Table = "PT";
	str_Table += sz_process;

	str_sql = _T("SELECT * FROM "+str_Table+" WHERE id='"+str_ID+"' and so='"+str_SO+"' and OKNG='Y'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("L1");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		i_Count = atoi(str_Temp);
	}
	else
	{
		str_sql = _T("SELECT COUNT (DISTINCT id) FROM "+str_Table+" WHERE so='"+str_SO+"' and OKNG='Y'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("Query information from %s fail, SQL:%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql, str_Table);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
			}
			i_Count = atoi(str_Temp)+1;
		}
	}

    sprintf(sz_count,"%d",i_Count);
    
	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: QueryEmployeeID_Right  
* Summary      : query employeeID level
* Parameters   : input/Employee id, output/level
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/05/30         First version   
*****************************************************************************/
bool QueryEmployeeID_Right(char* sz_employId, char * sz_Level,char* sz_ErrCode, char* sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_EmployeeID = sz_employId;

	str_sql = _T("SELECT * FROM toolcontrol WHERE EmployeeID= '"+str_EmployeeID+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query Employee ID from tool contorl fail\n 数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("ToolLevel");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_Level, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INVALID_EMPLOYEEID);
		sprintf(sz_ErrMsg,("Query Employee ID fai \n 此工号：%s不是合法的工号，请找工程师确认"),str_EmployeeID);
		return false;
	}

	p_recordSet->Close();

	m_mapPICS[_T("employee")]=sz_employId;

	return true;
}

/*****************************************************************************
* Function name: QueryPNinBACH_BySO  
* Summary      : Calculate the count for this SO, used to reprint the lable at FALINK process
* Parameters   : PN, so
* Return       : quantity about of this PN(for this SO), error code, error message
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/06/27        First version  
* 1.1          Sunlight.Qian      2012/01/30        Second version , add return qty 
* 1.2          Alex.Chen          2012/11/08        Modify 
*****************************************************************************/
bool QueryPNinBACH_BySO(char * sz_PN, char * sz_SO, char * sz_qty,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_SO = sz_SO;
	CString str_PN = sz_PN;

	// 读取获取keyparts信息，是从bach读，还是CIM读(description = close为从bach读，description = open为从CIM读)
	str_sql = _T("select * from PTPICSSYS where systype = 'KeypartsCheck'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query PTPICSSYS Fail.\n数据库操作异常，请尝试重新测试或者重启程式.");
		return false;
	}
	if(!p_recordSet->GetadoEOF())
	{
		thevalue = p_recordSet->GetCollect("description");
		if(thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)(_bstr_t)(thevalue);
		}
		p_recordSet->Close();
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYPNINBACH_FAIL);
		sprintf(sz_ErrMsg,"Query PTPICSSYS Fail.\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	// 从BACH读
	if(str_Temp.CompareNoCase("Close") == 0)
	{
		str_sql = _T("SELECT * FROM OPENQUERY (BACHS,'SELECT * FROM FMA WHERE MPROD=''"+str_PN+"'' AND MORD="+sz_SO+"')");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query BACHS Fail.\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}
		if(!p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("MQREQ");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_qty, _T("%s"), str_Temp);
			}
			p_recordSet->Close();
			return true;
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERYPNINBACH_FAIL);
			sprintf(sz_ErrMsg,("Query PN from BACHS Fail\n料号%s不在工单%s中"), str_PN, str_SO);
			return false;
		}
	}

	// 从CIM读
	else if(str_Temp.CompareNoCase("Open") == 0)
	{
		//str_sql = _T("SELECT * FROM OPENQUERY (BACHS,'SELECT * FROM FMA WHERE MPROD=''"+str_PN+"'' AND MORD="+sz_SO+"')");
		str_sql = _T("SELECT * FROM FMA WHERE MPROD = '"+str_PN+"' AND MORD = '"+sz_SO+"' ");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query BachFMA Fail.\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}
		if(!p_recordSet -> GetadoEOF() )
		{
			thevalue=p_recordSet->GetCollect("MQREQ");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_qty, _T("%s"), str_Temp);
			}
			p_recordSet->Close();
			return true;
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERYPNINBACH_FAIL);
			sprintf(sz_ErrMsg,("Query PN from BachFMA Fail.\n料号%s不在工单%s中"), str_PN, str_SO);
			return false;
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERYPNINBACH_FAIL);
		sprintf(sz_ErrMsg, "PTPICSSYS description is not open or close.\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	return true;
}

bool QueryKeyparts_Info(char * sz_PN, char * sz_Model,char * sz_Process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_Model = sz_Model;
	CString str_PN = sz_PN;
	CString str_Process = sz_Process;
	CString str_Info = "";
	int i_count=0;

	str_sql = _T("SELECT * FROM KPPCS WHERE model='"+str_Model+"' and process='"+str_Process+"' and submodel='"+str_PN+"' ORDER BY kpseq");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Employee ID from tool control fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	while(!p_recordSet -> GetadoEOF())
	{
		/* kptype */
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("kptype");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		if ( str_Temp.GetLength() > 0)
		{
			str_Info += str_Temp;
		}
		str_Info += ",";

		/* kp description */
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("description");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		if ( str_Temp.GetLength() > 0)
		{
			str_Info += str_Temp;
		}
		str_Info += ",";

		/* ID rule */
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("kpword");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		if ( str_Temp.GetLength() > 0)
		{
			str_Info += str_Temp;
		}
		str_Info += ",";

		/* kp part number */
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("kppn");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		if ( str_Temp.GetLength() > 0)
		{
			str_Info += str_Temp;
		}
		str_Info += ",";

		/* check kp duplicate if need or not*/
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("chkduplic");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		if ( str_Temp.GetLength() > 0)
		{
			str_Info += str_Temp;
		}
		str_Info += ";";
		i_count++;
        p_recordSet->MoveNext();
	}

	/*if (str_Info == "")
	{
		strcpy(sz_ErrCode, ERROR_QUERYKEYPARTS_FAIL);
		sprintf(sz_ErrMsg,("Query key parts define fail\n当前没有Process=%s,model=%s,PN=%s 对应keyparts定义，请找工程师确认"),str_Process,str_Model,str_PN);
		return false;
	}*/  //sunlight mark, 20110713, no key parts need link still return true
	strcpy(sz_Info,str_Info);
	sprintf(sz_Count, "%d", i_count);

	return true;
}
/*****************************************************************************
* Summary      : Query Error code and save it in '\\DB\\DB_LOG\\ErrorInfo.ini'
* if station is VI,P_VI,AOI use SQL:"SELECT * FROM picsecd where cdgroup ='VI' and model='"+str_Model+"' ORDER BY CODE"
* if station is not in VI,P_VI,AOI use SQL: "SELECT * FROM obangtype ORDER BY ngcode"
*****************************************************************************/
bool QueryOOBErrCode_Info(char * sz_model,char * sz_Process, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_CodeTemp;
	CString str_DetailTemp;
	CString str_Process = sz_Process;
	CString str_Model = sz_model;
	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Get current app directory */
	char sz_currentDir[256] = {0};
	GetModuleFileNameA(NULL, sz_currentDir, MAX_PATH);
	PathRemoveFileSpecA(sz_currentDir);

	CString str_currentPath;
	str_currentPath.Format("%s", sz_currentDir);

	/* Check LOG\\DB_Log exist and create */
	CString str_logFile;
	str_logFile = str_currentPath + "\\DB\\DB_LOG\\ErrorInfo.ini";

	/* Open log file */
	CStdioFile Code_file;


	if (! Code_file.Open(str_logFile, CFile::modeWrite|CFile::modeCreate, NULL)) 
	{
		sprintf(sz_ErrMsg,"Create ErrorInfo.ini file fail\n 创建ErrorInfo.ini文件失败");
		return false;
	}

	/* File format */
	Code_file.SeekToBegin();
	Code_file.WriteString("[ERROR_INFO]\n");

	/* Query code count */
	if (strncmp(str_Process,"VI",2) !=0&&strncmp(str_Process,"P_VI",4) !=0&&strncmp(str_Process,"AOI",3) !=0)
		str_sql = _T("SELECT COUNT(*) FROM obangtype");
	else
		str_sql = _T("SELECT COUNT(*) FROM picsecd where cdgroup ='VI'and model='"+str_Model+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Employee ID from tool control fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		Code_file.Close();
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_CodeTemp.Empty();
		thevalue = p_recordSet->GetCollect("");	
		if (thevalue.vt != VT_NULL)
		{
			str_CodeTemp = (char *)_bstr_t(thevalue);
		}
		sprintf(sz_Info,"ERROR_COUNT=%s\n",str_CodeTemp);
		Code_file.WriteString(sz_Info);
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("No error code in error code table\n没有找到对应的错误代码"));
		Code_file.Close();
		return false;
	}

	Code_file.WriteString("0=PASS\n");

	if (strncmp(str_Process,"VI",2) !=0&&strncmp(str_Process,"P_VI",4) !=0&&strncmp(str_Process,"AOI",3) !=0)
		str_sql = _T("SELECT * FROM obangtype ORDER BY ngcode");
	else
		str_sql = _T("SELECT * FROM picsecd where cdgroup ='VI' and model='"+str_Model+"' ORDER BY CODE");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Employee ID from tool control fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		Code_file.Close();
		return false;
	}

	while(!p_recordSet -> GetadoEOF())
	{
		if (strncmp(str_Process,"VI",2) !=0&&strncmp(str_Process,"P_VI",4) !=0&&strncmp(str_Process,"AOI",3) !=0)
		{
			str_CodeTemp.Empty();
			thevalue = p_recordSet->GetCollect("ngcode");	
			if (thevalue.vt != VT_NULL)
			{
				str_CodeTemp = (char *)_bstr_t(thevalue);
			}

			str_DetailTemp.Empty();
			thevalue = p_recordSet->GetCollect("ngdetail");	
			if (thevalue.vt != VT_NULL)
			{
				str_DetailTemp = (char *)_bstr_t(thevalue);
			}
		}
		else
		{
			str_CodeTemp.Empty();
			thevalue = p_recordSet->GetCollect("code");	
			if (thevalue.vt != VT_NULL)
			{
				str_CodeTemp = (char *)_bstr_t(thevalue);
			}

			str_DetailTemp.Empty();
			thevalue = p_recordSet->GetCollect("chidesc");	
			if (thevalue.vt != VT_NULL)
			{
				str_DetailTemp = (char *)_bstr_t(thevalue);
			}
		}

		sprintf(sz_Info,"%d=%s_%s\n",i_count,str_CodeTemp,str_DetailTemp);
		Code_file.WriteString(sz_Info);
		i_count ++;
		p_recordSet->MoveNext();
	}

	Code_file.Close();
	p_recordSet->Close();
	return true;
}


bool GetErrCodeDetail_QA(char * sz_Errcode, char * sz_NGType, char * sz_NGDetail, char * sz_MajorMin, char * sz_desp)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
    CString str_Temp;
	CString str_code = sz_Errcode;

	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Query  */
	str_sql = _T("SELECT * FROM obangtype where ngcode ='"+str_code+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		DBLOG("Query error code from obangtype fail");
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ngtype");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_NGType, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ngdetail");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_NGDetail, _T("%s"), str_Temp);
		}
		
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("majorminor");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_MajorMin, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("desp");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_desp, _T("%s"), str_Temp);
		}

	}
	
	if (strlen(sz_NGType) == 0 || strlen(sz_NGDetail) == 0 || strlen(sz_MajorMin) == 0 ||strlen(sz_desp) == 0)
	{
		DBLOG("The ngtype, ngretail, majormin, desp is empty");
		return false;
	}

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: GetPicasso  
* Summary      : Query Picasso number by l3/l4(IMEI/PPID), for key parts link
* Parameters   : input/IMEI, model; output/Picasso,partno
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/06/30        First version             
* 1.1          Sunlight.Qian      2011/11/03        Second version  Get last record value
*****************************************************************************/
bool GetPicasso(char * sz_InputID, char * sz_Model,char * sz_Picasso, char * sz_partno,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_Model = sz_Model;
	CString str_InputID = sz_InputID;


	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Query  */
//	str_sql = _T("SELECT * FROM PT_IMEI where model ='"+str_Model+"' and idindex='"+str_InputID+"'");
//	
//	if(!GetQueryResult(str_sql, p_recordSet))
//	{
//		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
//		return false;
//	}
//
//	while(!p_recordSet -> GetadoEOF())
//	{
//		str_Temp.Empty();
//		thevalue = p_recordSet->GetCollect("l1");	
//		if (thevalue.vt != VT_NULL)
//		{
//			str_Temp = (char *)_bstr_t(thevalue);
//			_stprintf(sz_Picasso, _T("%s"), str_Temp);
//		}
//
//		/*Sunlight add 20110801, for key parts link process should get partno to double check*/
//		str_Temp.Empty();
//		thevalue = p_recordSet->GetCollect("partno");	
//		if (thevalue.vt != VT_NULL)
//		{
//			str_Temp = (char *)_bstr_t(thevalue);
//			_stprintf(sz_partno, _T("%s"), str_Temp);
//		}
//		/*Sunlight add end */
//		p_recordSet->MoveNext();
//	}

	str_sql = _T("SELECT l1,partno FROM PTIMEI where model ='"+str_Model+"' and id='"+str_InputID+"' and okng='Y'order by tstdate desc,tsttime desc");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("l1");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_Picasso, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("partno");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_partno, _T("%s"), str_Temp);
		}
	}
	else
	{
		str_sql = _T("SELECT l1,partno FROM PTIMEI where model ='"+str_Model+"' and l4='"+str_InputID+"' and okng='Y'order by tstdate desc,tsttime desc");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}

		if(!p_recordSet -> GetadoEOF())
		{
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("l1");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_Picasso, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("partno");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_partno, _T("%s"), str_Temp);
			}
		}
		else
		{
			str_sql = _T("SELECT l1,partno FROM PTMAC where model ='"+str_Model+"' and id='"+str_InputID+"' and okng='Y'order by tstdate desc,tsttime desc");
			if(!GetQueryResult(str_sql, p_recordSet))
			{
				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
				sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
				return false;
			}

			if(!p_recordSet -> GetadoEOF())
			{
				str_Temp.Empty();
				thevalue = p_recordSet->GetCollect("l1");	
				if (thevalue.vt != VT_NULL)
				{
					str_Temp = (char *)_bstr_t(thevalue);
					_stprintf(sz_Picasso, _T("%s"), str_Temp);
				}

				str_Temp.Empty();
				thevalue = p_recordSet->GetCollect("partno");	
				if (thevalue.vt != VT_NULL)
				{
					str_Temp = (char *)_bstr_t(thevalue);
					_stprintf(sz_partno, _T("%s"), str_Temp);
				}
			}
			else
			{

				strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
				sprintf(sz_ErrMsg,("Can't found Picasso ID by %s in PTIMEI\n%s找不到此ID=%s对应的PICASSO"),str_InputID,str_Model,str_InputID);
				return false;
			}
		}

	}


	if (strlen(sz_Picasso) == 0)
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Can't found Picasso ID by %s in PTIMEI\n%s找不到此ID=%s对应的PICASSO"),str_InputID,str_Model,str_InputID);
		return false;
	}

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: QueryRecordBySO  
* Summary      : Calculate the total, PASS and FAIL count for this SO and workstation, for Unify UI 
* Parameters   : input/so, process,workstation, output/total count, pass count, fail count
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/07/11         First version             
*****************************************************************************/
bool QueryRecordBySO(char * sz_so, char * sz_process, char * sz_wrkstation, char * sz_totalCount, char * sz_passCount,char * sz_failCount,char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_SO = sz_so;
	CString str_Process = sz_process;
	CString str_wrkstation = sz_wrkstation;
	CString str_Table = "";

	int i_TotalCount = 0;
	int i_PassCount = 0;
	int i_FailCount = 0;

	str_Table = "PT";
	str_Table += sz_process;

	/*  Select total count  */ 
	str_sql = _T("SELECT COUNT (DISTINCT id) FROM "+str_Table+" WHERE workstation='"+str_wrkstation+"' and so='"+str_SO+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL:%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		i_TotalCount = atoi(str_Temp);
		_stprintf(sz_totalCount, _T("%s"), str_Temp);

	}
	
	/*  Select PASS count  */
	str_sql = _T("SELECT COUNT (DISTINCT id) FROM "+str_Table+" WHERE workstation='"+str_wrkstation+"' and so='"+str_SO+"' and OKNG='Y'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information from %s fail, SQL:%s "),str_sql, str_Table);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
        str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		i_PassCount = atoi(str_Temp);
		_stprintf(sz_passCount, _T("%s"), str_Temp);	
	}
	
	/* Set fail count  */
	i_FailCount = i_TotalCount - i_PassCount;
    sprintf(sz_failCount,"%d",i_FailCount);
    
	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: QueryFirstProcess  
* Summary      : Query first process to judge need scanner Picasso or not, Just for Unify UI
* Parameters   : input/model name, output/First process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/07/11         First version             
*****************************************************************************/
bool QueryFirstProcess(char * sz_modelname, char * sz_process, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_Model = sz_modelname;

	///*  Select first process by model name  */ 
	str_sql = _T("SELECT * FROM picsrouting WHERE model='"+str_Model+"' and subseq=1 and pcsseq=1");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("process");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		_stprintf(sz_process, _T("%s"), str_Temp);

	}

	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: QuerySegmentProcess  
* Summary      : Query process for assign segment , Just for keyparts link station; if segment="", can query out all process for one model
* Parameters   : input/model name,segment, output/First process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/07/25        First version             
*****************************************************************************/
bool QuerySegmentProcess(char * sz_modelname, char * sz_segment,char * sz_process, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_Model = sz_modelname;
	CString str_Segment = sz_segment;

	ZeroMemory(sz_process,sizeof(sz_process));
	///*  Sunlight modify to support query all process for one model, 20110815  */ 
	if (str_Segment == _T(""))
	{
		str_sql = _T("SELECT * FROM picsrouting WHERE model='"+str_Model+"' ORDER BY subseq, pcsseq");
	}
	else
	{
		str_sql = _T("SELECT * FROM picsrouting WHERE model='"+str_Model+"' and segment='"+str_Segment+"' order by pcsseq");
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("process");	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_process,str_Temp);
		strcat(sz_process,",");

		p_recordSet->MoveNext();

	}
    
	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: Insert PTKPWIP a key parts information
* Summary      : Query process for assign segment , Just for key parts link station
* Parameters   : input/model name,segment, output/First process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/07/25       First version             
*****************************************************************************/
bool InsertKeypartsInfo(char * sz_id, char * sz_process,char * sz_kpid, char * sz_kpname,char * sz_empolyee,char * sz_pn,char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_tstdate;
	CString str_tsttime;
	CString str_id = sz_id;
	CString str_process = sz_process;
	CString str_kpid = sz_kpid;
	CString str_kpname = sz_kpname;
	CString str_empolyee = sz_empolyee;
	CString str_pn = sz_pn;

	CString str_oldkyid = _T("");

	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_tstdate = datetime.date;
		str_tsttime = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		str_tstdate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_tsttime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	///*  Select first process by model name  */ 
	str_sql = _T("SELECT * FROM ptkpwip WHERE id='"+str_id+"' and process='"+str_process+"' and kpname='"+str_kpname+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect(_T("kpid"));	  //process sequence
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		str_oldkyid = str_Temp;

		p_recordSet->MoveNext();

	}

	if (str_oldkyid != _T(""))
	{
		str_sql = _T("exec dbo.usp_InsertPTKPWIP '"+str_id+"','"+str_process+"','"+str_kpid+"','"+str_kpname+"','','','"+str_empolyee+"','"+str_tstdate+"','"+str_tsttime+"','"+str_pn+"','CHANGE','"+str_oldkyid+"'");
	}
	else
	{
        str_sql = _T("exec dbo.usp_InsertPTKPWIP '"+str_id+"','"+str_process+"','"+str_kpid+"','"+str_kpname+"','','','"+str_empolyee+"','"+str_tstdate+"','"+str_tsttime+"','"+str_pn+"','NEW',''");
	}

	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_INSERTNG_FAIL);
		sprintf(sz_ErrMsg,"Insert ptkpwip a key parts record fail\nPTKPWIP插入数据失败");
		return false;
	}
    
	p_recordSet->Close();

	return true;

}

/*****************************************************************************
* Function name: QueryIMEIresult  
* Summary      : Query Picasso number by id(IMEI/Service tag),for IMEI station to check the IMEI is PASS or NOT, if PASS the relative Picasso is what?
* Parameters   : input/IMEI or service tag, model,, output/Picasso
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/04        First version    
* 1.1          Sunlight.Qian      2012/01/17        Second version, 只输出l1，有无PICASSO UI 自行处理
* 1.2          Sunlight.Qian      2012/02/22        Third version,  L30 表示IMEI已写入手机,只要L30=Y,就算测试结果为FAIL，依然视为此IMEI已经跟使用
* 1.3			Luffy.Liu		  2012/09/14	    4th version 查询条件model=改为model like，兼容没有model输入的情况
*****************************************************************************/
bool QueryIMEIresult(char * sz_InputID, char * sz_Model,char * sz_Picasso, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_Model = sz_Model;
	CString str_InputID = sz_InputID;


	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Query  */
//	str_sql = _T("SELECT l1 FROM PTIMEI where model like '"+str_Model+"' and id='"+str_InputID+"' and okng='Y' ORDER BY tstdate DESC, tsttime DESC");
	str_sql = _T("SELECT l1 FROM PTIMEI where id='"+str_InputID+"' and (okng='Y' or l30='Y') ORDER BY tstdate DESC, tsttime DESC");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("l1");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_Picasso, _T("%s"), str_Temp);
		}
	}
//	else
//	{
////		str_sql = _T("SELECT l1 FROM PTIMEI where model like '"+str_Model+"' and id='"+str_InputID+"' and l30='Y' ORDER BY tstdate DESC, tsttime DESC");
//		str_sql = _T("SELECT l1 FROM PTIMEI where id='"+str_InputID+"' and l30='Y' ORDER BY tstdate DESC, tsttime DESC");
//		if(!GetQueryResult(str_sql, p_recordSet))
//		{
//			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
//			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
//			return false;
//		}
//		if(!p_recordSet -> GetadoEOF())
//		{
//			str_Temp.Empty();
//			thevalue = p_recordSet->GetCollect("l1");	
//			if (thevalue.vt != VT_NULL)
//			{
//				str_Temp = (char *)_bstr_t(thevalue);
//				_stprintf(sz_Picasso, _T("%s"), str_Temp);
//			}
//		}
//	}

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: Query Loginwifi info according to Imei
* Summary      : Query process for assign segment
* Parameters   : input/Imei , output/Mac,LoginWifiInfo
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0           Luffy Liu       2011/08/15         First version  
                Luffy.Liu       2011/09/27         second version
*****************************************************************************/
bool QueryWifiLoginInfo(char * sz_Imei, char * sz_Mac, char *sz_LoginWifiInfo, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp = _T("");

	str_sql.Format("SELECT l15 FROM PTIMEI WHERE l3='%s' and  okng = 'Y'", sz_Imei);
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect((long)0);	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)(_bstr_t)(thevalue);
			break;
		}
		_stprintf(sz_Mac, _T("%s"), str_Temp);

		p_recordSet->MoveNext();
	}

	p_recordSet->Close();
	if (str_Temp == _T(""))
	{
		strcpy(sz_ErrCode, ERROR_QUERYMAC_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,Imei: %s\n无该IMEI对应的MAC值"),sz_Imei);
		return false;
	}
	

	CString str_mac = _T("");
	for (int i=0; i < str_Temp.GetLength(); i+=2)
	{
		str_mac += str_Temp.Mid(i,2) + ":";
	}
	str_mac = str_mac.Left(17);
   
	str_sql.Format("select * from WirelessInfo where MAC ='%s' and LoginWifi = 'Success' order by RecordTime desc", str_mac);
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}

	str_Temp = _T("");
	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp = _T("Success");
		
	}
	else
	{
		str_Temp = _T("Fail");
	}
	p_recordSet->Close();
	_stprintf(sz_LoginWifiInfo, _T("%s"), str_Temp);
	
	return true;

}

/*****************************************************************************
* Function name: CheckPicassoStatus  
* Summary      : Query Picasso number by id(IMEI/Service tag),for IMEI station to check the IMEI is PASS or NOT, if PASS the relative Picasso is what?
* Parameters   : input/Picasso, OUT/Timeinfo,status
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/17        First version             
*****************************************************************************/
bool CheckPicassoStatus(char * sz_Picasso, char * sz_TimeInfo, char * sz_Status,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Temp = _T("");
	CString str_Picasso = sz_Picasso;
	CString str_Status = _T("");


	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Query in SMT */
	str_sql = _T("SELECT * FROM SMTPICASSO where picasso ='"+str_Picasso+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no picasso in SMT booking table */
		strcpy(sz_ErrCode, ERROR_PICASSO_NOTINSMT);
		sprintf(sz_ErrMsg,("Can't found Picasso ID: %s in SMT\n找不到此ID=%s在SMT预约的记录"),str_Picasso,str_Picasso);
		return false;
	}
	else
	{
		/*Query in CIM table */
		str_sql = _T("SELECT * FROM picassocheck where picasso ='"+str_Picasso+"'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 "));
			return false;
		}

		if (p_recordSet -> GetadoEOF())
		{
			/* This Picasso is not in Picasso control table */
			return true;
		}
		else
		{
			thevalue = p_recordSet->GetCollect(_T("status"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Status = (char *)_bstr_t(thevalue);
				_stprintf(sz_Status, _T("%s"), str_Status);
			}

			thevalue = p_recordSet->GetCollect(_T("timeinfo"));	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_TimeInfo, _T("%s"), str_Temp);
			}

			if (str_Status == _T("L"))
			{
				strcpy(sz_ErrCode, ERROR_PICASSO_LOCK);
				sprintf(sz_ErrMsg,("The status of Picasso is Lock,Please check with TS or QA\n此PICASSO:%s处于锁定状态，请联系TS或QA"),str_Picasso);
				return false;
			}
			else 
			{
				p_recordSet->Close();
				return true;
			}
		}
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: InsertPicassoControl  
* Summary      :Insert picasso record to picasso control table
* Parameters   : input/picasso, timeinfo ,Status
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/17        First version             
*****************************************************************************/
bool InsertPicassoControl(char * sz_Picasso, char * sz_TimeInfo,char * sz_Status,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Temp = _T("");
	CString str_TimeInfo = sz_TimeInfo;
	CString str_Picasso = sz_Picasso;
	CString str_Status = sz_Status;

	/*Query in CIM table */
	str_sql = _T("SELECT * FROM picassocheck where picasso ='"+str_Picasso+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if (p_recordSet -> GetadoEOF())
	{
		/* This Picasso is not in Picasso control table */
		str_Status = _T("N");
		str_sql = _T("insert into picassocheck (picasso,timeinfo,status) values ('"+str_Picasso+"','"+str_TimeInfo+"','"+str_Status+"')");
	}
	else
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("timeinfo");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}

		if(strcmp(str_Temp,_T("")) == 0)
			str_sql = _T("update picassocheck set timeinfo = '"+str_TimeInfo+"' where picasso ='"+str_Picasso+"'");
		else if (strcmp(sz_TimeInfo,str_Temp) == 0)
		{
			return true;
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("This Picasso duplicate \n此PICASSO:%s已存在记录，重复"),str_Picasso);
			return false;
		}
	}

	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_PICASSO_DUPLICATE);
		sprintf(sz_ErrMsg,("Insert a Picasso record fail, SQL:%s \n插入PICASSO数据失败"),str_sql);
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: CheckPicassoDuplicate_SecondStation
* Summary      :Insert picasso record to picasso control table
* Parameters   : input/picasso, timeinfo 
* Return       : true/false
* Exception    : for Unify UI second station double check PICASSO duplicate
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/17        First version             
*****************************************************************************/
bool CheckPicassoDuplicate_SecondStation(char * sz_Picasso, char * sz_TimeInfo, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Temp = _T("");
	CString str_TimeInfo = sz_TimeInfo;
	CString str_Picasso = sz_Picasso;

	/*Query in CIM table */
	str_sql = _T("SELECT DISTINCT timeinfo FROM picassocheck where picasso ='"+str_Picasso+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if (p_recordSet -> GetadoEOF())
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("This Picasso have no record in picassocheck\n此PICASSO:%s无记录"),str_Picasso);
		return false;
	}
	else
	{
		while(!p_recordSet -> GetadoEOF())
		{
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("timeinfo");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				if (strcmp(str_Temp,sz_TimeInfo) == 0)
				{
                     break;
				}
				else
				{
					strcpy(sz_ErrCode, ERROR_PICASSO_DUPLICATE);
					sprintf(sz_ErrMsg,("This Picasso have more than 1 record in picassocontrol\n此PICASSO在系统中有大于1的记录"),str_Picasso);
					return false;
				}
			}
			p_recordSet->MoveNext();
		}
	}
	return true;
}


/*****************************************************************************
* Function name: CheckPicassoDuplicate
* Summary      :check picasso dupicate for first station
* Parameters   : input/picasso, picasso in FA, timeinfo(return by checkPicassoStatus) , timeinfo in FA, status(return by checkPicassoStatus)
* Return       : true/false
* Exception    : for Unify UI second station double check PICASSO duplicate
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/08/19        First version             
*****************************************************************************/
bool CheckPicassoDuplicate(char * sz_Picasso, char * sz_FAPicasso, char * sz_TimeInfo, char * sz_FATimeInfo, char * sz_status,char * sz_ErrCode, char * sz_ErrMsg)
{
    if (!strcmp(sz_FAPicasso,_T("")))
    {
		if (!strcmp(sz_status,_T("N")))
		{
			strcpy(sz_ErrCode, ERROR_PICASSO_DUPLICATE);
			sprintf(sz_ErrMsg,("There is not Picasso in target, but the status is not R\n此PICASSO状态为%s，但是手机内没有PICASSO"),sz_status);
			return false;
		}
		else
		{
			return true;
		}
    }
	else if (!strcmp(sz_Picasso,sz_FAPicasso) && !strcmp(sz_FATimeInfo,sz_TimeInfo))
	{
		return true;
	}
	else
	{
	   strcpy(sz_ErrCode, ERROR_PICASSO_DUPLICATE);;
	   sprintf(sz_ErrMsg,("The Timeinfo is not same in phone:%s and in CIM:%s\n此PICASSO对应的timeinfo与手机内的不一致"), sz_TimeInfo,sz_FATimeInfo);
	   return false;
	}
}

/*****************************************************************************
* Function name: QuerySO_Info_All
* Summary      : query information about SO
* Parameters   : input/SO, output/model, pn, sotype, toolversion, factory image version, OS version,
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/09/06       First version             
*****************************************************************************/
bool QuerySO_Info_All(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) {
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n输入的工单不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString so=sz_so;
	CString str_Temp = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from SO_INFO where sord = '"+so+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("PN");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_partNo, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Model");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_model, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Rework");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_soType, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Tool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_toolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Img_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_facImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SW_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_userImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("EntTool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_EntranceToolVer, _T("%s"), str_Temp);
			}
		}catch (_com_error e) {
			strcpy(sz_errorCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_errorMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_errorMsg);
			return false;
		}

	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n此工单:%s无效,请确认后重新输入"),sz_so);
		p_recordSet->Close();
		return false;
	}
	
	/* Disconnect DB */
	p_recordSet->Close();

	/*Check the data is valid*/
	if ( strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits \n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}

	if ( strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits \n机种名:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}

	if ( strlen(sz_soType) !=1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO type:%s is not equal to %d digits \n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	m_mapPICS[_T("so")]=so;
	m_mapPICS[_T("sotype")]=sz_soType;
	m_mapPICS[_T("partno")]=sz_partNo;
	m_mapPICS[_T("model")]=sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType);
	DBLOG(str_Temp);
	return true;
}




/*****************************************************************************
* Function name: QueryChargerIDinRunin
* Summary      : query battery  from run_in table
* Parameters   : input/battery ID, charging ID, output/error code, error message
* Return       : true/false
* Exception    : for wolf 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/09/19       First version             
*****************************************************************************/
bool QueryChargerIDinRunin(char * sz_model,char* sz_charger,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_charger) == 0) {
		strcpy(sz_errorCode, ERROR_INPUTCHARGERID_EMPTY);
		sprintf(sz_errorMsg,("Invalid Charger ID:%s \n输入的charger ID为空"),sz_charger);
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_chargerID = sz_charger;
	CString str_Temp = sz_model;
	str_Temp = str_Temp.Left(4);

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from ptrun_in where l4 = '"+str_chargerID+"' and model like '"+str_Temp+"%' and okng='Y' order by tstdate DESC, tsttime DESC");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"SQL QUERRY FAIL\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
        return true;
	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTCHARGERID_NORECORD);
		sprintf(sz_errorMsg,("ChargerID no record in Run_in\n此充电器ID:%s没有Run_In站PASS的记录"),str_chargerID);
		p_recordSet->Close();
		return false;
	}
	
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryBatteryIDinRunin
* Summary      : query battery  from run_in table
* Parameters   : input/battery ID, charging ID, output/error code, error message
* Return       : true/false
* Exception    : for wolf 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/09/19       First version             
*****************************************************************************/
bool QueryBatteryIDinRunin(char * sz_model,char* sz_battery,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_battery) == 0) {
		strcpy(sz_errorCode, ERROR_INPUTBATTERYID_EMPTY);
		sprintf(sz_errorMsg,("Invalid battery ID:%s\n输入的电池ID为空"),sz_battery);
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_batteryID = sz_battery;
	CString str_Temp = sz_model;
	str_Temp = str_Temp.Left(4);

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from ptrun_in where l3 = '"+str_batteryID+"' and model like '"+str_Temp+"%' and okng='Y' order by tstdate DESC, tsttime DESC");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"SQL QUERRY FAIL\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
        return true;
	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTBATTERYID_NORECORD);
		sprintf(sz_errorMsg,("Battery ID no record in Run_in\n此电池ID:%s没有Run_In站PASS的记录"),str_batteryID);
		p_recordSet->Close();
		return false;
	}
	
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: QueryOOBStatus
* Summary      : query the IMEI is need OOB or not, status=1 means need, otherwrise need not
* Parameters   : input/IMEI, output/status,error code, error message
* Return       : true/false
* Exception    : for OOB
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/08       First version             
* 1.1          Sunlight.Qian      2011/11/03       Second version, add ordery by date,time DESC
*****************************************************************************/
bool QueryOOBstatus(char * sz_IMEI,char* sz_status,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	CString str_sql,str_Temp;
	CString str_IMEI = sz_IMEI;
	
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from OOBRCheck where IMEI = '"+str_IMEI+"' order by date DESC,time DESC");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"SQL QUERRY FAIL\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("status");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_status, _T("%s"), str_Temp);
		}
        p_recordSet->Close();
        return true;
	}
	else
	{
	}
	
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: UpdateOOBStatusToFalse
* Summary      : Update OOB status to '0' by IMEI
* Parameters   : input/IMEI, output/error code, error message
* Return       : true/false
* Exception    : for OOB
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/08       First version             
*****************************************************************************/
bool UpdateOOBStatusToFalse(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	CString str_sql,str_Temp;
	CString str_IMEI = sz_IMEI;
	
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("update OOBRCheck set status = 0 where IMEI = '"+str_IMEI+"'and status=1");

    /* Execute DB*/
    if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_errorCode, ERROR_UPDATEOOBSTATUS_FALSE);
		sprintf(sz_errorMsg,("Update IMEI:%s OOB status to 0 fail\n更改OOB状态位到0失败"),str_IMEI);
		return false;
	}
	else
	{
         return true;
	}
}

/*****************************************************************************
* Function name: InsertOOBStatus
* Summary      : Insert a OOB need check record to OOBRCheck 
* Parameters   : input/model, so, pn,IMEI, output/error code, error message
* Return       : true/false
* Exception    : for OOB
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/08      First version             
*****************************************************************************/
bool InsertOOBStatus(char * sz_model, char * sz_so, char * sz_pn, char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	CString str_sql,str_Temp;
	CString str_Model = sz_model;
	CString str_SO = sz_so;
	CString str_PN = sz_pn;
	CString str_IMEI = sz_IMEI;
	CString str_date = _T("");
	CString str_time = _T("");
	
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	GetServerTime(datetime);
	str_date = datetime.date;
	str_time = datetime.time;

	/*SYSTEMTIME time;
	::GetLocalTime(&time);

	str_date.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
	str_time.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);*/

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("insert into OOBRCheck (Model, PN,SO,IMEI,Status,Date,Time) values('"+str_Model+"','"+str_PN+"','"+str_SO+"','"+str_IMEI+"',1,'"+str_date+"','"+str_time+"')");

    /* Execute DB*/
    if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_errorCode, ERROR_INSERTOOBSTATUS_FAIL);
		sprintf(sz_errorMsg,("Insert IMEI:%s OOB status fail\n插入OOB状态记录失败"),str_IMEI);
		return false;
	}
	else
	{
         return true;
	}
}


/*****************************************************************************
* Function name: QueryOOBResult
* Summary      : Query OOB test result
* Parameters   : input/IMEI, output/error code, error message
* Return       : true/false
* Exception    : for OOB
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/08      First version       
* 1.1          Sunlight.Qian      2012/09/18      Second version, return test result 
*****************************************************************************/
bool QueryOOBResult(char * sz_IMEI, char * sz_result,char* sz_errorCode, char* sz_errorMsg)
{
	//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	CString str_sql,str_Temp;
	CString str_IMEI = sz_IMEI;

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from PTOOB where id = '"+str_IMEI+"' order by tstdate DESC,tsttime DESC");

	/* Execute DB*/
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"SQL QUERRY FAIL\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("OKNG");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_result, _T("%s"), str_Temp);
		}
		p_recordSet->Close();
		return true;
	}

	p_recordSet->Close();
	return false;
}




/*****************************************************************************
* Function name: InsertPTOOBRework
* Summary      : Insert a OOB rework record to PTOOB , for warehouse management tool
* Parameters   : input/IMEI, output/error code, error message
* Return       : true/false
* Exception    : for OOB
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/09       First version             
*****************************************************************************/
bool InsertPTOOBRework(char * sz_IMEI,char* sz_errorCode, char* sz_errorMsg)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	std::map<CString, CString>  map_Data; 

	CString str_sql,str_Temp;
	CString str_IMEI = sz_IMEI;
	
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    fieldvalue;
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0;

    /* Get last record value*/
	str_sql = _T("select * from PTOOB where id='"+str_IMEI+"' and OKNG='Y' order by tstdate desc, tsttime desc");
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"Query OOB test record fail\n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog");
		return false;
	}

	if (!p_recordSet->GetadoEOF())
	{
		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_errorCode, ERROR_QUERY_FAIL);
			sprintf(sz_errorMsg,_T("Get PTOOB field fail\n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"));
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_Temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			fieldvalue=p_recordSet->GetCollect(bstr_ColName);  //得到字段对应的值
			if (fieldvalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(fieldvalue);
				map_Data[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
        
	}
	else
	{
		strcpy(sz_errorCode, ERROR_NOOOBPASS_RECORD);
		sprintf(sz_errorMsg,_T("Can't found IMEI:%s OOB PASS record\n没有此IMEI在OOB站PASS的记录"),str_IMEI);
		p_recordSet->Close();
        return true;   //20120103,sunlight modify, no record to update, return true
	}

	/*Insert rework record*/
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		map_Data[_T("tstdate")] = datetime.date;
		map_Data[_T("tsttime")] = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		map_Data[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		map_Data[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}


	map_Data[_T("okng")] = _T("R");
    map_Data[_T("id")] = str_IMEI;



	std::map<CString,CString>::iterator Itr;
	CString str_Value = _T("(");
	CString str_ID = _T("(");
	unsigned int i_count = 0;

	str_sql.Empty();
	for(Itr = map_Data.begin(),i_count=1; Itr != map_Data.end(),i_count <= map_Data.size(); Itr++,i_count++)
	{
		if (Itr->first != "serial"  )
		{
			str_ID += Itr->first;
			str_Value += "'"+Itr->second+"'";
			if (i_count != map_Data.size())
			{
				str_ID += ",";
				str_Value += ",";
			}
		}
	}
	str_ID += ")";
	str_Value += ")";

	str_sql = "Insert into PTOOB" + str_ID;
	str_sql += " values " + str_Value;

	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_errorCode, ERROR_INSERTOOBREWORK_EECORD);
		sprintf(sz_errorMsg,"Insert PTOOB a Rework record fail\n插入PTOOB记录失败");
		return false;
	}

	return true;
}



/*****************************************************************************
* Function name: GetIMEI_Info  
* Summary      : Query SO, PN, MODEL, SW, PICASSO by IMEI
* Parameters   : input/IMEI, output/model,Picasso,partno,SO, SW
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/09        First version             
*****************************************************************************/
bool GetIMEI_Info(char * sz_InputID, char * sz_model,char * sz_Picasso, char * sz_partno,char * sz_SO, char * sz_sw,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_InputID = sz_InputID;


	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

	/* Query  */
	str_sql = _T("SELECT * FROM PTIMEI where id='"+str_InputID+"' and okng='Y' order by tstdate desc, tsttime desc");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet->GetadoEOF())
	{
		str_sql = _T("SELECT * FROM PTMAC where id='"+str_InputID+"' and okng='Y' order by tstdate desc, tsttime desc");

		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}
	}
	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("l1");	//PICASSO
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_Picasso, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("model");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_model, _T("%s"), str_Temp);
		}

	}
	else
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Can't found this IMEI:%s info in PTIMEI\n在PTIMEI找不到此ID=%s对应的信息"),str_InputID,str_InputID);
		return false;
	}

//query SO/partno/sw by IMEI from SSSCTD
	str_sql = _T("SELECT * FROM sssctd where ctdsn1='"+str_InputID+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query SO by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdso");	//so
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_SO, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdsw");	//so
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_sw, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctditem");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_partno, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_SSSCTD_EMPTY);
		sprintf(sz_ErrMsg,("Can't found this IMEI:%s info in SSSCTD\n在SSCTD找不到此ID=%s对应的信息"),str_InputID,str_InputID);
		return false;
	}


	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryIMEI_ByCarton  
* Summary      : Query SO, PN, MODEL, SW, PICASSO by IMEI
* Parameters   : input/Carton ID, output/IMEI list, count ,SO, SW
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/10        First version             
*****************************************************************************/
bool QueryIMEI_ByCarton(char * sz_CartonID, char * sz_count,char * sz_IMEIlist, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_CartonID = sz_CartonID;


	CString str_Info = "";
	int i_count=0;

	/* Query  */
	str_sql = _T("SELECT * FROM sssctd where ctdcid='"+str_CartonID+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query IMEI by Carton ID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	while(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdsn1");	//PICASSO
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
		}
		str_Info += str_Temp;
		str_Info += ",";
		i_count ++;
		p_recordSet->MoveNext();
	}

	strcpy(sz_IMEIlist,str_Info);
	sprintf(sz_count, "%d", i_count);

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: GetProcessResult_new
* Summary      : Check the test result of this process ,一次fail的记录后，最后连续2次记录是PASS才算PASS
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/12        First version             
*****************************************************************************/
bool GetProcessResult_new(char * sz_id, char * sz_model,char * sz_process)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model =sz_model;
	int i_passCount = 0;

	str_table = _T("PT");
	str_table += sz_process;
	
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	}
	else
	{
		str_sql = "select * from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if (!p_recordSet->GetadoEOF())
	{
		while ( (!p_recordSet -> GetadoEOF()) && i_passCount < PASS_COUNT)
		{
			str_result.Empty();
			thevalue = p_recordSet->GetCollect("okng");
			if (thevalue.vt != VT_NULL)
			{
			   str_result = (char *)(_bstr_t)(thevalue);
			}

			if (strncmp(str_result,"Y",1) !=0 )
			{
				return false;
			}

			i_passCount++;

			p_recordSet->MoveNext();
		}
		return true;
	}
    else
	{
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: GetProcessResult_Ngreturn_new
* Summary      : Check all the test result which process is before the current process, 大于TS fix 时间，一次fail的记录后，最后连续2次记录是PASS才算PASS
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/12         First version             
*****************************************************************************/
bool GetProcessResult_NgReturn_new(char * sz_id, char * sz_model, char * sz_process, const CString str_Fixdate, const CString str_Fixtime)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;
	int i_passCount = 0;

	str_table = _T("PT");
	str_table += sz_process;

	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";
	}
	else
	{
	    str_sql = "select * from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";   
	}
	
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if (!p_recordSet->GetadoEOF())
	{
		while ( (!p_recordSet -> GetadoEOF()) && i_passCount < PASS_COUNT)
		{
			str_result.Empty();
			thevalue = p_recordSet->GetCollect("okng");
			if (thevalue.vt != VT_NULL)
			{
				str_result = (char *)(_bstr_t)(thevalue);
			}

			if (strncmp(str_result,"Y",1) !=0 )
			{
				return false;
			}
			i_passCount++;

			p_recordSet->MoveNext();
		}
		return true;
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: QueryRuninID
* Summary      : query battery  from run_in table
* Parameters   : input/ID, model, column name, 
* Return       : true/false
* Exception    : for wolf 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/17       First version             
*****************************************************************************/
bool QueryRuninID(char * sz_model,char* sz_ID,char * sz_column,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_ID) == 0) {
		strcpy(sz_errorCode, ERROR_INPUTID_INVALID);
		sprintf(sz_errorMsg,("Invalid Run_in ID:%s \n输入的Run_in ID为空"),sz_ID);
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_ID = sz_ID;
	CString str_Column = sz_column;
	CString str_Temp = sz_model;
	str_Temp = str_Temp.Left(4);

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from ptrun_in where "+str_Column+" = '"+str_ID+"' and model like '"+str_Temp+"%' and okng='Y' order by tstdate DESC, tsttime DESC");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,"SQL QUERRY FAIL\n数据库操作异常，请尝试重新测试或者重启程式");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
        return true;
	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTCHARGERID_NORECORD);
		sprintf(sz_errorMsg,("%s no record at %s in Run_in\n此ID:%s 在%s栏位没有Run_In PASS的记录"),str_ID,str_Column ,str_ID, str_Column);
		p_recordSet->Close();
		return false;
	}
	
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: QueryOOBSampleRate
* Summary      : Check all the test result which process is before the current process
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/21         First version             
*****************************************************************************/
bool QueryOOBSampleRate(char * sz_model,char* sz_so,char * sz_sampleRate,char* sz_errorCode, char* sz_errorMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_temp = "";
	CString str_SO = sz_so;
	CString str_model = sz_model;


	str_sql = "select SampleRatio From ssssmpRatio where SO ='"+str_SO+"' and flag='Y'";     //根据SO 查询sample ratio
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERY_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s sample Ratio fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("SampleRatio");
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
			_stprintf(sz_sampleRate,_T("%s"),str_temp);
		}

		if (strlen(sz_sampleRate) == 0 || atoi(str_temp) == 0)
		{
			strcpy(sz_errorCode, ERROR_SOSAMPLERATE_EMPTY);
			sprintf(sz_errorMsg,("Get SO:%s sample Ratio fail\n此工单:%s抽检率有设定但设定为空值"),sz_so,sz_so);
			return false;
		}
		else
		{
			return true;  
		}
	}
	else   //根据model查询sample ratio
	{
		str_sql = "select SampleRatio From ssssmpRatio where SO ='"+str_model+"'";   
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_errorCode, ERROR_QUERY_FAIL);
			sprintf(sz_errorMsg,("Get SO:%s sample Ratio fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
			return false;
		}
		if ( !p_recordSet -> adoEOF )
		{
			str_temp.Empty();
			thevalue = p_recordSet->GetCollect("SampleRatio");
			if (thevalue.vt != VT_NULL)
			{
				str_temp = (char *)(_bstr_t)(thevalue);
				_stprintf(sz_sampleRate,_T("%s"),str_temp);
			}

			if (strlen(sz_sampleRate) == 0 || atoi(str_temp) == 0)
			{
				strcpy(sz_errorCode, ERROR_MODELSAMPLERATE_EMPTY);
				sprintf(sz_errorMsg,("Get SO:%s sample Ratio fail\n此工单:%s抽检率有设定但设定为空值"),sz_so,sz_so);
				return false;
			}
			else
			{
				return true;  
			}
		}
		else
		{
			strcpy(sz_errorCode, ERROR_SAMPLERATE_EMPTY);
			sprintf(sz_errorMsg,("Get Model:%s sample Ratio fail\n此机种:%s没有设定抽检率"), sz_model,sz_model);
			return false;
		}
	}
    p_recordSet->Close();
	return true;
}


/* Sunlight add special for luna, 20111031*
/*****************************************************************************
* Function name: GetProcessResult_Luna
* Summary      : Check the test result of this process 
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/31        First version             
*****************************************************************************/
bool GetProcessResult_Luna(char * sz_id, char * sz_model,char * sz_process)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model =sz_model;

	str_table = _T("PT");
	str_table += sz_process;
	
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model like 'GLUN%' and model<> 'GLUNB1A' order by tstdate desc, tsttime desc";   
	}
	else
		str_sql = "select * from "+str_table+" where id='"+str_id+"' and model like 'GLUN%' and model<> 'GLUNB1A'  order by tstdate desc, tsttime desc";   
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_result.Empty();
		thevalue = p_recordSet->GetCollect("okng");
		if (thevalue.vt != VT_NULL)
		{
		   str_result = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_result,"Y",1) ==0 )
		{
			return true;
		}
		else
		{
			return false;  //The result is NG
		}
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: GetProcessResult_Ngreturn_Luna
* Summary      : Check all the test result which process is before the current process
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/10/21         First version             
*****************************************************************************/
bool GetProcessResult_NgReturn_Luna(char * sz_id, char * sz_model, char * sz_process, const CString str_Fixdate, const CString str_Fixtime)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;

	str_table = _T("PT");
	str_table += sz_process;

	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model like 'GLUN%' and model<> 'GLUNB1A' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";
	}
	else
	    str_sql = "select * from "+str_table+" where id='"+str_id+"' and model like 'GLUN%' and model<> 'GLUNB1A' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";   
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		str_result.Empty();
		thevalue = p_recordSet->GetCollect("okng");
		if (thevalue.vt != VT_NULL)
		{
		   str_result = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_result,"Y",1) ==0 )
		{
			return true;
		}
		else
		{
			return false;  //The result is NG
		}
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}

/* Sunlight add special for luna end */



bool CheckRouting_New(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	char c_ngprocess[20] = {0};
	char c_ngErrCode[20] = {0};
	char c_ngErrMsg[512] = {0};
	char c_ngModel[20] = {0};

	if (strlen(sz_id) != 10 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is not matching length\nPicasso的长度不符合要求"),sz_id);
		return false;
	}

	if (strlen(sz_model) < 3 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The model you input:%s is not matching length \n机种名的长度不符合要求"),sz_model);
		return false;
	}

	if (strlen(sz_process) < 2 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The process you input:%s is not matching length \n站别名的长度不符合要求"),sz_process);
		return false;
	}

	/* Check is there TS unfixed data */
	if (!CheckTSUnFixedData(sz_id,c_ngprocess,sz_ErrCode,c_ngModel))
	{
		if(QueryLastErrorcode(sz_id,c_ngprocess,c_ngErrCode,sz_ErrCode,c_ngErrMsg))
		{
			sprintf(sz_ErrCode,"DB1024(%s)",c_ngErrCode);
		}
		sprintf(sz_ErrMsg,("This ID=%s have a unfixed data in TS,please check \n此ID在TS中有未修复记录，请送回TS确认"),sz_id);
		return false;
	}

	/* Check retest count data  */
	if (strcmp(sz_process,"FALINK")!=0 && strcmp(sz_process,"PPIDLINK")!=0)  
	{ /*sunlight modify 20120620, FALINK and PPIDLINK don't need check retest count*/ 
		if (!CheckRetestCount_new(sz_id,sz_process,sz_ErrCode,sz_ErrMsg))
		{
			if(QueryLastErrorcode(sz_id,sz_process,c_ngErrCode,sz_ErrCode,c_ngErrMsg))
			{
				sprintf(sz_ErrCode,"DB1025(%s)",c_ngErrCode);
			}
			return false;
		}
	}

	/*  Check retest count data  */
	if (!CheckProcess(1,sz_id, sz_model, sz_process ,sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: CheckRouting_V2
* Summary      : redefine check routing for allen's new rule(Conversion between CSD and normal model)
* Parameters   : picasso id , model
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0         Nancy.Yang          2013/04/29        First version    
*****************************************************************************/
bool CheckRouting_V2(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	char c_ngprocess[20] = {0};
	char c_ngErrCode[20] = {0};
	char c_ngErrMsg[512] = {0};
	char c_ngModel[20] = {0};
	CString str_model = sz_model;
	CString str_msg;
	bool b_ignore = false;

	if (strlen(sz_id) != 10 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is not matching length\nPicasso的长度不符合要求"),sz_id);
		return false;
	}

	if (strlen(sz_model) < 3 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The model you input:%s is not matching length \n机种名的长度不符合要求"),sz_model);
		return false;
	}

	if (strlen(sz_process) < 2 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The process you input:%s is not matching length \n站别名的长度不符合要求"),sz_process);
		return false;
	}

	/* Check is there TS unfixed data */
	if (!CheckTSUnFixedData(sz_id,c_ngprocess,sz_ErrCode,c_ngModel))
	{
		//CString str_ngModel = c_ngModel;
		//if (str_ngModel != "")//has a 'W' record
		//{
		//	str_msg.Format("=== Got a unfix record, ngModel=%s, ngProcess=%s",c_ngModel,c_ngprocess);
		//	DBLOG(str_msg);

		//	if (str_ngModel != str_model)//CSD and normal model should follow the name rule:  XXXXCS* ==> XXXXB*A
		//	{
		//		if (str_ngModel.Mid(4,2).CompareNoCase(_T("CS")) == 0)//TS model is CSD model
		//		{
		//			CString strTemp;
		//			strTemp.Format(_T("%sB%sA"), str_ngModel.Left(4) ,str_ngModel.Right(1));
		//			if (str_model == strTemp)// current model is normal model
		//			{
		//				DBLOG("=== Current model is normal, and TS model is CSD model,ignore this unfix record");
		//				b_ignore = true;
		//			}
		//		}
		//	}

		//	if (!b_ignore) // need must have fix record 201301220 Magan
		//	{
			if(QueryLastErrorcode(sz_id,c_ngprocess,c_ngErrCode,sz_ErrCode,c_ngErrMsg))
			{
				sprintf(sz_ErrCode,"DB1024(%s)",c_ngErrCode);
			}
			sprintf(sz_ErrMsg,("This ID=%s have a unfixed data in TS,please check \n此ID在TS中有未修复记录，请送回TS确认"),sz_id);
			return false;
		//	}
		//}
		//else
		//	return false;
	}

	/* Check retest count data  */
	if (strcmp(sz_process,"FALINK")!=0 && strcmp(sz_process,"PPIDLINK")!=0)  
	{ /*sunlight modify 20120620, FALINK and PPIDLINK don't need check retest count*/ 
		if (!CheckRetestCount_new(sz_id,sz_process,sz_ErrCode,sz_ErrMsg))
		{
			if(QueryLastErrorcode(sz_id,sz_process,c_ngErrCode,sz_ErrCode,c_ngErrMsg))
			{
				sprintf(sz_ErrCode,"DB1025(%s)",c_ngErrCode);
			}
			return false;
		}
	}

	
	if (!CheckProcess_V2(1,sz_id, sz_model, sz_process ,sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}

	return true;
}

/*****************************************************************************
* Function name: UnlinkKeyparts
* Summary      : when OOB detect fail, the fail one should unlink keyparts so that the keyparts can be linked again
* Parameters   : picasso id , model
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/02        First version    
* 1.1          Sunlight.Qian      2012/08/13        Second version, change inset NG record in stead of update last record to N
*****************************************************************************/
bool UnlinkKeyparts(char * sz_id, char * sz_model,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Value = _T("(");
	CString str_ID = _T("(");
	CString str_id = sz_id;
	CString str_model = sz_model;

	unsigned int i_count = 0;

	std::map<CString,CString>::iterator Itr;
	std::map<CString,CString> map_record;

	/* delete kpwip record */
	str_sql = "delete from PTKPWIP where id='"+str_id+"' and process='PPIDLINK'";   
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_DELETE_KPWIP);
		sprintf(sz_ErrMsg,("Delete kpwip record fail, SQL: %s \n删除kpwip测试数据失败"),str_sql);
		return false;
	}

//	str_sql = "update PTPPIDLINK set okng='N' where id='"+str_id+"' and model='"+str_model+"'";  //20111220, corrent set okng='N'
//	str_sql = "update PTPPIDLINK set okng='N' where id='"+str_id+"' and model='"+str_model+"' and serial in (select MAX(serial ) from PTPPIDLINK where id='"+str_id+"')";  //20120620, sunlight modify, only update last record
//  Sunlight modify 20120813
	if(!GetLastRecord(sz_id,sz_model,"PPIDLINK",map_record, sz_ErrCode, sz_ErrMsg))
		return false;
	else
	{
		if ((map_record["okng"] == "N") ||(map_record["id"]==""))
		{
			DBLOG("Last record is NG, needn't insert NG record");
			return true;
		}
		else
		{
			//Luffy modify get local time to get server time
			MyDatetime datetime;
			if (GetServerTime(datetime))
			{
				map_record[_T("tstdate")] = datetime.date;
				map_record[_T("tsttime")] = datetime.time;
			}
			else
			{
				SYSTEMTIME time;
				::GetLocalTime(&time);

				map_record[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
				map_record[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
			}

			map_record[_T("okng")] = _T("N");
			map_record[_T("errorcode")] = INSERT_UNLINKKEYPARTS_RECORD;
			for(Itr = map_record.begin(),i_count=1; Itr != map_record.end(),i_count <= map_record.size(); Itr++,i_count++)
			{
				if ( Itr->first == "serial") 
				{ 
					continue;
				}
				else
				{
					str_ID += Itr->first;
				}

				str_Value += "'"+Itr->second+"'";

				if (i_count != map_record.size())
				{
					str_ID += ",";
					str_Value += ",";
				}
			}
			str_ID += ")";
			str_Value += ")";

			str_sql = "Insert into PTPPIDLINK" ;
			str_sql += str_ID;
			str_sql += " values " + str_Value;
		}
	}

	
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_UPDATE_PTPPIDLINK);
		sprintf(sz_ErrMsg,("update PTPPIDLINK NG record fail, SQL: %s \n更新PTPPIDLINK 表测试数据失败"),sz_id,str_sql);
		return false;
	}

	DBLOG("Unlink PPID keyparts link success");
	return true;

}


/*****************************************************************************
* Function name: CheckKeyparts_duplicate
* Summary      : when OOB detect fail, the fail one should unlink keyparts so that the keyparts can be linked again
* Parameters   : kpid , kpname
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/02        First version  
* 1.1          Sunlight.Qian      2012/08/13        Second version, add model name check
*****************************************************************************/
bool CheckKeyparts_duplicate(char * sz_picasso, char * sz_kpid/*, char * sz_kpname, char * sz_model*/,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_kpid = sz_kpid;
	//CString str_kpname = sz_kpname;
	CString str_temp = "";
	CString str_id= sz_picasso;
	//CString str_model = sz_model;

	/* delete kpwip record */
	//str_sql = "select * from KPCHECK where kpname='"+str_kpname+"' and model='"+str_model+"'";   
	//if(!GetQueryResult(str_sql, p_recordSet))
	//{
	//	strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//	sprintf(sz_ErrMsg,("Query KPTYPE fail, SQL: %s \n查询需要检查的KPTYE失败"),str_sql);
	//	return false;
	//}

	//if (p_recordSet->adoEOF)
	//{
 //       return true;
	//}
	//else
	{
		str_sql = "select * from PTKPWIP where kpid='"+str_kpid+"'";
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("Query kpid record fail,SQL: %s \n查询KYID记录失败"),str_sql);
			return false;
		}

		if (p_recordSet->adoEOF)
		{
			return true;
		}
		else
		{
			str_temp.Empty();
			thevalue = p_recordSet->GetCollect("id");    //l3:IMEI
			if (thevalue.vt != VT_NULL)
			{
				str_temp = (char *)(_bstr_t)(thevalue);        
			}

			if (strcmp(str_temp,str_id) == 0)
			{
				DBLOG(str_id);
				DBLOG("Same picasso and same kpid, check pass");
			}
			else
			{
				strcpy(sz_ErrCode, ERROR_KPID_DUPLICATE);
				sprintf(sz_ErrMsg,("This KPID:%s had been link before,please check\n此KPID:%s已经被用过"),str_kpid,str_kpid);
				return false;

			}
		}
	}

	DBLOG("Check KPID duplicate success");
	return true;

}

/*****************************************************************************
* Function name: CheckNAL_duplicate
* Summary      : Check NAL 
* Parameters   : IMEI , NAL
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/06          First version   
* 1.1          Sunlight.Qian      2011/12/12          Second version  NAL length define to 15
//* 1.2          Sunlight.Qian      2011/12/13          Third version  IMEI不能被多个NAL  link
*****************************************************************************/
bool CheckNAL_duplicate(char * sz_IMEI, char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_temp = "";
	CString str_IMEI = sz_IMEI;
	CString str_NAL = sz_NAL;

	//if( strlen(sz_IMEI)!=15 || strlen(sz_NAL) !=15)
	if(strlen(sz_NAL) !=15)
	{
		strcpy(sz_ErrCode, ERROR_INPUT_NALIMEI);
		//sprintf(sz_ErrMsg,("The input IMEI/NAL empty\nNAl:%s或者IMEI:%s的长度不对"),sz_NAL,sz_IMEI);
		sprintf(sz_ErrMsg,("The input NAL empty\nNAl:%s的长度不对"),sz_NAL);
		return false;
	}

	/* Query NAL info */
	str_sql = "select * from PTIMEI where l5='"+str_NAL+"'and okng='Y' order by tstdate DESC, tsttime DESC";     //L5:NAL
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query KPTYPE fail, SQL: %s \n查询需要检查的KPTYE失败"),str_sql);
		return false;
	}

	if (p_recordSet->adoEOF)
	{
        return true;
	}
	else
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("l3");    //l3:IMEI
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_IMEI,str_temp,15) != 0)
		{
			strcpy(sz_ErrCode, ERROR_NAL_DUPLICATE);
			sprintf(sz_ErrMsg,("This NAL had been link before,please check\n此NAL:%s已经被IMEI:%s 用过"),str_NAL,str_temp);
			return false;
		}
	}
	//str_sql = "select * from PTIMEI where l5='"+str_NAL+"'and okng='Y' order by tstdate DESC, tsttime DESC";    //L5:NAL
	//if(!GetQueryResult(str_sql, p_recordSet))
	//{
	//	strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//	sprintf(sz_ErrMsg,("Query NAL fail, SQL: %s \n查询NAL失败"),str_sql);
	//	return false;
	//}

	//if (p_recordSet->adoEOF)
	//{
	//	str_sql = "select * from PTIMEI where id='"+str_IMEI+"'and okng='Y' order by tstdate DESC, tsttime DESC";    //L5:NAL
	//	if(!GetQueryResult(str_sql, p_recordSet))
	//	{
	//		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//		sprintf(sz_ErrMsg,("Query IMEI fail, SQL: %s \n查询IMEI失败"),str_sql);
	//		return false;
	//	}
 //       if(p_recordSet->adoEOF)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		str_temp.Empty();
	//		thevalue = p_recordSet->GetCollect("l5");    //l5:IMEI
	//		if (thevalue.vt != VT_NULL)
	//		{
	//			str_temp = (char *)(_bstr_t)(thevalue);
	//		}

	//		if (strncmp(str_NAL,str_temp,15) != 0)
	//		{
	//			strcpy(sz_ErrCode, ERROR_IMEINAL_DUPLICATE);
	//			sprintf(sz_ErrMsg,("This IMEI had been link before,please check\n此IMEI:%s已经被NAL:%s link过"),str_IMEI,str_temp);
	//			return false;
	//		}
	//	}
	//}
	//else
	//{
	//	str_temp.Empty();
	//	thevalue = p_recordSet->GetCollect("id");    //id:IMEI
	//	if (thevalue.vt != VT_NULL)
	//	{
	//		str_temp = (char *)(_bstr_t)(thevalue);
	//	}

	//	if (strncmp(str_IMEI,str_temp,15) != 0)
	//	{
	//		strcpy(sz_ErrCode, ERROR_NAL_DUPLICATE);
	//		sprintf(sz_ErrMsg,("This NAL had been link before,please check\n此NAL:%s已经被IMEI:%s link过"),str_NAL,str_temp);
	//		return false;
	//	}
	//}

	DBLOG("Check NAL duplicate success");
	return true;

}


/*****************************************************************************
* Function name: QueryNAL
* Summary      : Query NAL 
* Parameters   : IMEI , NAL
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/13         First version  
* 1.1          Sunlight.Qian      2012/06/27         Second version NAL from SSSCTD not PTtable
* 1.2          Sunlight.Qian      2012/06/29         If IMEI not link NAL, return true
*****************************************************************************/
bool QueryNAL(char * sz_IMEI, char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_temp = "";
	CString str_IMEI = sz_IMEI;

	/*if( strlen(sz_IMEI)!=15)//maybe not imei
	{
		strcpy(sz_ErrCode, ERROR_INPUT_NALIMEI);
		sprintf(sz_ErrMsg,("The input IMEI/NAL empty\nIMEI:%s的长度不对"),sz_IMEI);
		return false;
	}*/

	/* delete kpwip record */
//	str_sql = "select * from PTIMEI where id='"+str_IMEI+"'and okng='Y' order by tstdate DESC, tsttime DESC";     //L5:NAL
	str_sql = "select ctdnal from SSSCTD where ctdsn1='"+str_IMEI+"'";     
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query PTIMEI fail, SQL: %s \n查询需要PTIMEI失败"),str_sql);
		return false;
	}

	if (p_recordSet->adoEOF)
	{
//		strcpy(sz_ErrCode, ERROR_NAL_UNLINK);
//		sprintf(sz_ErrMsg,("This IMEI:%s haven't link NAL before,please check\n此IMEIL:%s没有对应的NAL"),str_IMEI,str_IMEI);
//		return false;
		DBLOG("Can't found link record");
	}
	else
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdnal");    //PTIMEI l5:NAL
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
			_stprintf(sz_NAL,_T("%s"),str_temp);         
		}
	}

	DBLOG("Query NAL success");
	return true;

}

/*****************************************************************************
* Function name: QueryIMEIbyNAL
* Summary      : Query IMEI by NAL
* Parameters   : IMEI , NAL
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang      2014/11/07         First version  

*****************************************************************************/
bool QueryIMEIbyNAL(char * sz_NAL, char * sz_IMEI, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_temp = "";
	CString str_NAL = sz_NAL;

	str_sql = "select ctdsn1 from SSSCTD where ctdnal='"+str_NAL+"'";     
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query PTIMEI fail, SQL: %s \n查询需要PTIMEI失败"),str_sql);
		return false;
	}

	if (p_recordSet->adoEOF)
	{
		DBLOG("Can't found link record");
	}
	else
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdsn1");    //PTIMEI l5:NAL
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
			_stprintf(sz_IMEI,_T("%s"),str_temp);         
		}
	}

	DBLOG("Query IMEI success");
	return true;

}

/*****************************************************************************
* Function name: QuerySO_Info_All_V2
* Summary      : query information about SO, add return so qty
* Parameters   : input/SO, output/model, pn, sotype, toolversion, factory image version, OS version,SOQty
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/13      First version             
*****************************************************************************/
bool QuerySO_Info_All_V2(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) {
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n输入的工单不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString so=sz_so;
	CString str_Temp = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from SO_INFO where sord = '"+so+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("PN");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_partNo, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Model");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_model, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Rework");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_soType, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SOQty");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_Qty, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Tool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_toolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Img_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_facImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SW_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_userImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("EntTool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_EntranceToolVer, _T("%s"), str_Temp);
			}
		}catch (_com_error e) {
			strcpy(sz_errorCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_errorMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_errorMsg);
			return false;
		}

	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n此工单:%s无效,请确认后重新输入"),sz_so);
		p_recordSet->Close();
		return false;
	}
	
	/* Disconnect DB */
	p_recordSet->Close();

	/*Check the data is valid*/
	if ( strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits \n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}

	if ( strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits \n机种名:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}

	if ( strlen(sz_soType) !=1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO type:%s is not equal to %d digits \n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	m_mapPICS[_T("so")]=so;
	m_mapPICS[_T("sotype")]=sz_soType;
	m_mapPICS[_T("partno")]=sz_partNo;
	m_mapPICS[_T("model")]=sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType);
	DBLOG(str_Temp);
	return true;
}


/*****************************************************************************
* Function name: QuerySOCount_Control
* Summary      : Calculate the count for this SO, used to control the target count for every SO
* Parameters   : Picasso id, so, process
* Return       : next count (for this SO), error code, error message
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/14        First version   
* 1.1          Sunlight.Qian      2012/03/24        Second version, fail 的手机也计数
*****************************************************************************/
/**
* @brief QuerySOCount_Control
*
* 1. Calculate the count for this SO, used to control the target count for every SO
* 2. The cout also used for FCHECK to decide if show check OOB or not
* 3. Tricky: If the device already have the test records, in this function, cout will be return 0, and that means "check OOB" will not happened
*
*     DB Table: PT[Station]
*     SQL: SELECT * FROM PT[Station] WHERE id=sz_id and so=sz_so
*     SQL: SELECT COUNT (DISTINCT id) FROM PT[Station] WHERE so=sz_so
*/
bool QuerySOCount_Control(
		char * sz_id, ///< Picasso or IMEI, input
		char * sz_so, ///< So number, input
		char * sz_process, ///< Station name, input
		char * sz_count, ///< The device couts for this SO, include pass and fail records, output 
		char * sz_ErrCode, ///Error code, output
		char * sz_ErrMsg ///< Error message, output
		)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_SO = sz_so;
	CString str_ID = sz_id;
	CString str_Table = "";

	int i_Count = 0;

	str_Table = "PT";
	str_Table += sz_process;

	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso 
		str_sql = _T("SELECT * FROM "+str_Table+" WHERE l1='"+str_ID+"' and so='"+str_SO+"' ");
	else
	    str_sql = _T("SELECT * FROM "+str_Table+" WHERE id='"+str_ID+"' and so='"+str_SO+"' ");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		i_Count = 0;
	}
	else
	{
		str_sql = _T("SELECT COUNT (DISTINCT id) FROM "+str_Table+" WHERE so='"+str_SO+"' ");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("Query information from %s fail, SQL:%s "),str_sql, str_Table);
			return false;
		}

		if ( !p_recordSet -> GetadoEOF() )
		{

			thevalue=p_recordSet->GetCollect("");	  //process sequence
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
			}
			i_Count = atoi(str_Temp)+1;
			
		}
	}

    sprintf(sz_count,"%d",i_Count);
    
	p_recordSet->Close();
	return true;

}


/*****************************************************************************
* Function name: QueryIfTestAgain  (for UI)
* Summary      : 上一笔测试结果为NG，则count return 1, UI需要提示再测试一次
* Parameters   : id , process
* Return       : sz_count, true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/15        First version             
*****************************************************************************/
/**
* @brief QueryIfTestAgain
*
* 上一笔测试结果为NG，则count return 1, UI需要提示再测试一次
*
*     DB Table: PT[Station]
*     SQL: select * from PT[Station] where id=sz_id and model=sz_model order by tstdate desc, tsttime desc
*/
bool QueryIfTestAgain(
		char * sz_id, ///< Input, Picasso or IMEI
		char * sz_model, ///< Input, Model name
		char * sz_process, ///< Input, Station name
		char * sz_count, ///< Output, 1: need to test again; 0: don't need to test again
		char * sz_ErrCode, /// Output, Error code
		char * sz_ErrMsg /// Output, Error message
		)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;

	str_table = _T("PT");
	str_table += sz_process;

	/* Sunlight add 20111221, TL/TRIGGER;FA/FALINK,VI, RUN_IN;PACKAGE still use old rule*/
	if ( (strcmp(sz_process ,"IMEI")==0) || strcmp(sz_process ,"MAC")==0 || (strcmp(sz_process ,"FCHECK")==0) || (strcmp(sz_process ,"OS_DL")==0)|| (strcmp(sz_process ,"PPIDLINK")==0)|| (strcmp(sz_process ,"FQC")==0)|| (strcmp(sz_process ,"VI")==0)||(strcmp(sz_process ,"P_VI")==0)||(strcmp(sz_process ,"FALINK")==0) || (strcmp(sz_process ,"RUN_IN")==0) || (strcmp(sz_process ,"TRIGGER")==0) || (strcmp(sz_process ,"DL")==0) || (strcmp(sz_process ,"MMI_EM")==0))
	{
		sprintf(sz_count, "%d", 0);
		return true;
	}
	/*Sunlight add temp end */
	
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	}
	else
	{
		str_sql = "select * from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' order by tstdate desc, tsttime desc";   
	}

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query fail, SQL:%s"),str_sql);
		return false;
	}

	if (!p_recordSet->GetadoEOF())
	{
		str_result.Empty();
		thevalue = p_recordSet->GetCollect("okng");
		if (thevalue.vt != VT_NULL)
		{
		   str_result = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_result,"Y",1) ==0 )
		{
			sprintf(sz_count, "%d", 0);
		}
		else
		{
			sprintf(sz_count, "%d", 1);  //last record is fail, need retest again
		}
	}
    else
	{
		sprintf(sz_count, "%d", 0);  //Can't found this ID test result 
	}

	return true;
}


/*****************************************************************************
* Function name: QueryIMEIStatus 
* Summary      : Query IMEI status
* Parameters   : input/IMEI, output/IMEI status
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/23/23       First version  
* 1.1          Sunlight.Qian      2012/01/02       Second version, add output carton_ID
*****************************************************************************/
bool QueryIMEIStatus(char * sz_InputID,char * sz_status,char * sz_cartonID, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_InputID = sz_InputID;


	CString str_Info = "";
	int i_count=1;
	char sz_Info [512]={0};

//query SO by IMEI from SSSCTD
	str_sql = _T("SELECT * FROM sssctd where ctdsn1='"+str_InputID+"'");

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query SO by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(!p_recordSet -> GetadoEOF())
	{
		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdsts");	//so
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_status, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue = p_recordSet->GetCollect("ctdcid");	//caton ID
		if (thevalue.vt != VT_NULL)
		{
			str_Temp = (char *)_bstr_t(thevalue);
			_stprintf(sz_cartonID, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_SSSCTD_EMPTY);
		sprintf(sz_ErrMsg,("Can't found this IMEI:%s info in SSSCTD\n在SSCTD找不到此ID=%s对应的信息"),str_InputID,str_InputID);
		return false;
	}


	p_recordSet->Close();
	return true;
}



/*****************************************************************************
* Function name: UnlinkKeyparts for falink process
* Summary      : the fail one should unlink keyparts so that the keyparts can be linked again
* Parameters   : picasso id , model
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/23        First version  
* 1.1          Sunlight.Qian      2012/08/13        Second version, change inset NG record in stead of update last record to N
* 1.2          Sunlight.Qian      2012/09/05        Third version, add process input
*****************************************************************************/
bool UnlinkKeyparts_FA(char * sz_id, char * sz_model, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_ID = _T("(");
	CString str_Value = _T("(");
	CString str_sql = "";
	CString str_id = sz_id;
	CString str_model = sz_model;
	CString str_process = sz_process;

	std::map<CString, CString> map_record;
	std::map<CString, CString> ::iterator Itr;

	unsigned int i_count=0;

	/* delete kpwip record */
	str_sql = "delete from PTKPWIP where id='"+str_id+"' and process='"+str_process+"'";   
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_DELETE_KPWIP);
		sprintf(sz_ErrMsg,("Delete ptkpwip record fail, SQL:%s, \n删除kpwip FALINK 测试数据失败"),str_sql);
		return false;
	}

//	str_sql = "update PTFALINK set okng='N' where id='"+str_id+"' and model='"+str_model+"'";  //20111220, corrent set okng='N'
//	str_sql = "update PTFALINK set okng='N' where id='"+str_id+"' and model='"+str_model+"' and serial in (select MAX(serial ) from PTFALINK where id='"+str_id+"')";  //20120620, sunlight modify, only update last record
//  Sunlight modify 20120813
	if(!GetLastRecord(sz_id,sz_model,sz_process,map_record, sz_ErrCode, sz_ErrMsg))
		return false;
	else
	{
		if ((map_record["okng"] == "N") ||(map_record["id"]==""))
		{
			DBLOG("Last record is NG, needn't insert NG record");
			return true;
		}
		else
		{
			//Luffy modify get local time to get server time
			MyDatetime datetime;
			if (GetServerTime(datetime))
			{
				map_record[_T("tstdate")] = datetime.date;
				map_record[_T("tsttime")] = datetime.time;
			}
			else
			{
				SYSTEMTIME time;
				::GetLocalTime(&time);

				map_record[_T("tstdate")].Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
				map_record[_T("tsttime")].Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
			}
			map_record[_T("okng")] = _T("N");
			map_record[_T("errorcode")] = INSERT_UNLINKKEYPARTS_RECORD;
			for(Itr = map_record.begin(),i_count=1; Itr != map_record.end(),i_count <= map_record.size(); Itr++,i_count++)
			{
				if ( Itr->first == "serial") 
				{ 
					continue;
				}
				else
				{
					str_ID += Itr->first;
				}

				str_Value += "'"+Itr->second+"'";

				if (i_count != map_record.size())
				{
					str_ID += ",";
					str_Value += ",";
				}
			}
			str_ID += ")";
			str_Value += ")";

			str_sql = "Insert into PT" + str_process;
			str_sql += str_ID;
			str_sql += " values " + str_Value;
		}
	}

	
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_UPDATE_FALINK);
		sprintf(sz_ErrMsg,("update PTFALINK NG record fail, SQL: %s\n更新PTFALINK 测试数据失败"),str_sql);
		return false;
	}

	DBLOG("Unlink FALINK keyparts link success");
	return true;

}


/*****************************************************************************
* Function name: QuerySO_Info_All_V3
* Summary      : query information about SO, add return crtype
* Parameters   : input/SO, output/model, pn, sotype, toolversion, factory image version, OS version,SOQty
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/26      First version             
*****************************************************************************/
bool QuerySO_Info_All_V3(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) {
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n输入的工单不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString so=sz_so;
	CString str_Temp = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from SO_INFO where sord = '"+so+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("PN");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_partNo, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Model");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_model, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Rework");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_soType, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SOQty");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_Qty, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Tool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_toolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Img_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_facImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SW_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_userImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("EntTool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_EntranceToolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("CRType");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_CRtype, _T("%s"), str_Temp);
			}
		}catch (_com_error e) {
			strcpy(sz_errorCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_errorMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_errorMsg);
			return false;
		}

	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n此工单:%s无效,请确认后重新输入"),sz_so);
		p_recordSet->Close();
		return false;
	}
	
	/* Disconnect DB */
	p_recordSet->Close();

	/*Check the data is valid*/
	if ( strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits \n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}

	if ( strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits \n机种名:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}

	if ( strlen(sz_soType) !=1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO type:%s is not equal to %d digits \n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	m_mapPICS[_T("so")]=so;
	m_mapPICS[_T("sotype")]=sz_soType;
	m_mapPICS[_T("partno")]=sz_partNo;
	m_mapPICS[_T("model")]=sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType);
	DBLOG(str_Temp);
	return true;
}



/*****************************************************************************
* Function name: QueryKeypartsByID
* Summary      : query key parts information for a Picasso
* Parameters   : input/Picasso ID, output/record count, information
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/26      First version             
*****************************************************************************/
bool QueryKeypartsByID(char * sz_ID, char * sz_process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_ID) < 10 ) {
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("Invalid Picasso number\n输入的Picasso不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_process = sz_process;
	CString str_ID = sz_ID;
	CString str_Temp = "";
	CString str_TempInfo = "";
	int i_count = 0;

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select kpid, kpname, partno from PTKPWIP where id = '"+str_ID+"' and process='"+str_process+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Get ID:%s keyparts info from PTKPWIP fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_ID);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("kpid");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
			}
			str_TempInfo += str_Temp + ",";

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("kpname");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
			}
			str_TempInfo += str_Temp + ",";

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("partno");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
			}
			str_TempInfo += str_Temp + ";";
            i_count ++;

		}catch (_com_error e) {
			strcpy(sz_ErrCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_ErrMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_ErrMsg);
			return false;
		}

		p_recordSet->MoveNext();
	}

	
    sprintf(sz_Count,"%d",i_count);
	sprintf(sz_Info,"%s",str_TempInfo);
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryKeypartsByID_V2
* Summary      : query key parts information for a ID(maybe picasso or other)
* Parameters   : input/Picasso ID or other ID, output/record count, information
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei      2014/02/13      First version             
*****************************************************************************/
bool QueryKeypartsByID_V2(char * sz_ID, char * sz_process,char * sz_Count, char * sz_Info, char * sz_ErrCode, char * sz_ErrMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	///* Check input */
	//if (strlen(sz_ID) < 10 ) {
	//	strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
	//	sprintf(sz_ErrMsg,("Invalid Picasso number\n输入的Picasso不符合长度定义"));
	//	return false;
	//}
	///* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_process = sz_process;
	CString str_ID = sz_ID;
	CString str_Temp = "";
	CString str_TempInfo = "";
	int i_count = 0;

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select kpid, kpname, partno from PTKPWIP where id = '"+str_ID+"' and process='"+str_process+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Get ID:%s keyparts info from PTKPWIP fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_ID);
		return false;
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("kpid");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
			}
			str_TempInfo += str_Temp + ",";

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("kpname");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
			}
			str_TempInfo += str_Temp + ",";

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("partno");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
			}
			str_TempInfo += str_Temp + ";";
            i_count ++;

		}catch (_com_error e) {
			strcpy(sz_ErrCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_ErrMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_ErrMsg);
			return false;
		}

		p_recordSet->MoveNext();
	}

	
    sprintf(sz_Count,"%d",i_count);
	sprintf(sz_Info,"%s",str_TempInfo);
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: InsertTSData_forTS
* Summary      : Insert a TS record for TS Re-DL tool
* Parameters   : input/Picasso ID, output/record count, information
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/01/04     First version             
*****************************************************************************/
bool InsertTSData_forTS(char * sz_id, char * sz_model,char * sz_rtnprocess, char * sz_ngprocess, char * sz_ngwrkstation, char * sz_ErrCode, char * sz_ErrMsg)
{

	CString str_sql = _T("");
	CString str_id = sz_id;
	CString str_rtnprocess = sz_rtnprocess;
	CString str_model = sz_model;
	CString str_ngwrkstation = sz_ngwrkstation;
	CString str_ngprocess = sz_ngprocess;
	CString str_date;
	CString str_time;

	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_date = datetime.date;
		str_time = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);

		str_date.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_time.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	str_sql = "Insert into TS (id, model, ngprocess, ngwrkstation, rtnprocess,ngdate, ngtime, nguser,fixdate,fixtime,fixuser,status) values ('"+str_id+"','"+str_model+"','"+str_ngprocess+"','"+str_ngwrkstation+"','"+str_rtnprocess+"','"+str_date+"','"+str_time+"','1234567','"+str_date+"','"+str_time+"','9999999','F')";
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_INSERTTSREWORK_RECORD);
		sprintf(sz_ErrMsg,("Insert TS a record for TS DL fail,%s\n插入TS记录失败"),str_sql);
		return false;
	}

	return true;

}




/*****************************************************************************
* Function name: QueryIMEI_ByPicasso
* Summary      : query IMEI/Service for a Picasso
* Parameters   : input/Picasso ID, output/IMEI or Service Tag information
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/01/11        First version             
*****************************************************************************/
bool QueryIMEI_ByPicasso(char * sz_picassoid ,char * sz_IMEI,char * sz_ErrCode, char * sz_ErrMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_picassoid) < 10 ) {
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("Invalid Picasso number,picasso:%s\n输入的Picasso不符合长度定义"),sz_picassoid);
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString str_ID = sz_picassoid;
	CString str_Temp = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
//	str_sql = _T("select id from PTIMEI where l1 = '"+str_ID+"' and OKNG='Y' order by tstdate DESC, tsttime DESC");
	str_sql = _T("select id from PTIMEI where l1 = '"+str_ID+"' and (OKNG='Y'or l30='Y') order by tstdate DESC, tsttime DESC");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Get ID:%s IMEI station info from PTIMEI fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_picassoid);
		return false;
	}

    if( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("id");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_IMEI,"%s",str_Temp);
		}
	}
	//else
	//{
	//	str_sql = _T("select id from PTIMEI where l1 = '"+str_ID+"' and l30='Y' order by tstdate DESC, tsttime DESC");

	//	if(!GetQueryResult(str_sql, p_recordSet))
	//	{
	//		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
	//		sprintf(sz_ErrMsg,("Get ID:%s IMEI station info from PTIMEI fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_picassoid);
	//		return false;
	//	}

	//	if( !p_recordSet -> GetadoEOF() )
	//	{
	//		str_Temp.Empty();
	//		thevalue=p_recordSet->GetCollect("id");	
	//		if (thevalue.vt != VT_NULL)
	//		{
	//			str_Temp=(char *)(_bstr_t)(thevalue);
	//			sprintf(sz_IMEI,"%s",str_Temp);
	//		}
	//	}
	//}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryholdInfo
* Summary      : query IMEI/Picasso hold information
* Parameters   : input/Picasso or IMEI, output/status, hold process,hold reason
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/01/12         First version         
* 1.1          Sunlight.Qian      2012/03/19         Second version, 如果有多笔记录将process拼接
*****************************************************************************/
/**
* @brief QueryholdInfo
*
* ####1. Query IMEI/Picasso hold information####
* ####2. If there is no hold record, the sz_status = N #### 
* ####3. If there are more than one hold record, the sz_process will be returned multi-process####
*
*     DB Table: sssholdinfo
*     SQL: select status, process, holdreason from sssholdinfo where mnid =sz_id and model=sz_model and status=H order by holddate desc, holdtime desc
*/
bool QueryholdInfo(
		char * sz_id, ///< Picasso or IMEI, input
		char * sz_model, ///< Model name, input
		char * sz_status, ///< Hold status, output
		char * sz_process, ///< Station name[s], output
		char * sz_holdreas, ///< Hold reason, output
		char * sz_ErrCode, ///< Error code, output
		char * sz_ErrMsg ///< Error message, output
		)
{
	//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	/* Check input */
	if (strlen(sz_id) < 10 ) {
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("Invalid ID number\n输入的ID不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */
	CString str_sql;
	CString str_ID = sz_id;
	CString str_model = sz_model;
	CString str_Temp = "";
	CString str_holdpro = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	str_sql = _T("select status, process, holdreason from sssholdinfo where mnid = '"+str_ID+"' and model='"+str_model+"' and status='H' order by holddate desc, holdtime desc");

	/* Execute DB*/
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query data fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_id);
		return false;
	}

	if(p_recordSet -> GetadoEOF())
	{
		strcpy(sz_status,"N");
	}
	else
	{
		while( !p_recordSet -> GetadoEOF() )
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("status");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				sprintf(sz_status,"%s",str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("process");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
//				sprintf(sz_process,"%s",str_Temp);
			}

			str_holdpro += str_Temp;
			str_holdpro += ",";

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("holdreason");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				sprintf(sz_holdreas,"%s",str_Temp);
			}
			p_recordSet->MoveNext();
		}
	}

	strcpy(sz_process,str_holdpro);
	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryholdInfo_V2
* Summary      : query IMEI/Picasso hold information
* Parameters   : input/Picasso or IMEI, current process, output/status, hold reason
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/03/19         First version         
*****************************************************************************/
/**
* @brief QueryholdInfo_V2
*
* ####1. Query the hold information of the IMEI/Picasso in the sz_process####
* ####2. The sz_holdreas included status, holdNo, holdreason, holdUser####
* ####3. If there is no hold record, the sz_status = N #### 
*
*     DB Table: sssholdinfo
*     SQL: select status, process, holdreason from sssholdinfo where mnid =sz_id and model=sz_model and status=H and process like sz_process order by holddate desc, holdtime desc
*/
bool QueryholdInfo_V2(
		char * sz_id, ///< Picasso or IMEI, input
		char * sz_model, ///< Model name, input
		char * sz_process, ///< Station name, input, The defference between V1 and V2 
		char * sz_status, ///< Hold status, output
		char * sz_holdreas, ///< Hold reason, output, The defference between V1 and V2
		char * sz_ErrCode, ///< Error code, output
		char * sz_ErrMsg ///< Error message, output
		)
{
	//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	/* Check input */
	if (strlen(sz_id) < 10 ) {
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("Invalid ID number\n输入的ID不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */
	CString str_sql;
	CString str_ID = sz_id;
	CString str_model = sz_model;
	CString str_Temp = "";
	CString str_process = sz_process;

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	str_sql = _T("select status, process, HoldNo,holdreason,HoldUser from sssholdinfo where mnid = '"+str_ID+"' and model='"+str_model+"' and status='H' and process like '%"+str_process+"%'order by holddate desc, holdtime desc");

	/* Execute DB*/
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query data fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_id);
		return false;
	}

	if(p_recordSet -> GetadoEOF())
	{
		strcpy(sz_status,"N");
	}
	else
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("status");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_status,"%s",str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("holdNo");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_holdreas,"[HoldNo:]%s\n",str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("holdreason");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			strcat(sz_holdreas,"[HoldReason:]");
			strcat(sz_holdreas,str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("holdUser");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			strcat(sz_holdreas,"\n[HoldUser:]");
			strcat(sz_holdreas,str_Temp);
		}
	}

	p_recordSet->Close();
	return true;
}



/*****************************************************************************
* Function name: CheckPicassoSMT  
* Summary      : Query Picasso in SMT or not
* Parameters   : input/Picasso, 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/01/19        First version             
*****************************************************************************/
bool CheckPicassoSMT(char * sz_Picasso,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = _T("");
	CString str_Picasso = sz_Picasso;


	/* Query in SMT */
	str_sql = _T("SELECT * FROM SMTPICASSO where picasso ='"+str_Picasso+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no picasso in SMT booking table */
		strcpy(sz_ErrCode, ERROR_PICASSO_NOTINSMT);
		sprintf(sz_ErrMsg,("Can't found Picasso ID: %s in SMT\n找不到此ID=%s在SMT预约的记录"),str_Picasso,str_Picasso);
		return false;
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: CheckProcessTestDate
* Summary      : check 某个segment内process的测试时间是否是先后顺序
* Parameters   : input/Picasso,model, segment,process 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/01       First version             
*****************************************************************************/
bool CheckProcessTestDate(char * sz_Picasso,char * sz_model, char * sz_segment,char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	char c_tempProcess[512] = {0};
	char c_time[100] = {0};

	CString str_Picasso = sz_Picasso;
	CString str_process = sz_process;
	CString str_allProcess = _T("");
	CString str_tempProcess = _T("");
	CString str_tablename = _T("");
	CString str_Temp = _T("");
	CString str_date = _T("");
	CString str_time = _T("");
	CString str_sql = _T("");

	std::map<int, CString>  m_mapTime;             //Process数据
	std::map< int ,CString>::iterator Itr;

	int i_dot1=0,i_dot2=0;

	/* Query process */
	if (!QuerySegmentProcess(sz_model, sz_segment, c_tempProcess, sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}
    
	str_allProcess = c_tempProcess;
	if (str_allProcess.GetLength() == 0)
	{
		strcpy(sz_ErrCode, ERROR_SEGMENTPROCESS_EMPTY);
		sprintf(sz_ErrMsg,("Process is empty\n%s process集合为空"),sz_segment);
		return false;
	}

	/* Get tstdate,tsttime for every process*/
	for (int i=0;i_dot2!=-1;i++)
	{  
		i_dot2 = str_allProcess.Find(",",i_dot1);
	    str_tempProcess = str_allProcess.Mid(i_dot1,i_dot2-i_dot1);
		i_dot1 = i_dot2 + 1;
//		str_allProcess = str_allProcess.Mid(i_dot2+1);

		if (strcmp(str_tempProcess,"OOB")== 0 || strcmp(str_tempProcess,"FQC") == 0 )
		{
			continue;
		}

		if (strcmp(str_tempProcess,str_process)== 0 )
		{
			break;
		}
    
		str_tablename =  "PT" + str_tempProcess;

		if(strcmp(str_tempProcess,"IMEI")==0)
			str_sql = _T("SELECT tstdate, tsttime,okng FROM "+str_tablename+" where l1 ='"+str_Picasso+"' order by tstdate desc, tsttime desc");
		else
		   str_sql = _T("SELECT tstdate, tsttime,okng FROM "+str_tablename+" where id ='"+str_Picasso+"' order by tstdate desc, tsttime desc");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}

		if(p_recordSet -> GetadoEOF())  
		{
			strcpy(sz_ErrCode, ERROR_PICASSO_NOTINSMT);
			sprintf(sz_ErrMsg,("Can't found test record in %s\n找不到此ID在%s的测试记录"), str_tablename,str_tablename);
			return false;
		}
		else
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("okng");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				if (strncmp(str_Temp,"Y",1) !=0 )
				{
					strcpy(sz_ErrCode, ERROR_PICASSO_NOTINSMT);
					sprintf(sz_ErrMsg,("Last test result in %s is %s\n在%s最后一次测试记录是%s"), str_tablename,str_Temp,str_tablename,str_Temp);
					return false;
				}
			}
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("tstdate");	
			if (thevalue.vt != VT_NULL)
			{
				str_date=(char *)(_bstr_t)(thevalue);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("tsttime");	
			if (thevalue.vt != VT_NULL)
			{
				str_time=(char *)(_bstr_t)(thevalue);
				int i_length = 6-str_time.GetLength();
				if (i_length > 0)
				{
					for ( int i=0; i<i_length; i++)
					{
						str_time = "0" + str_time;
					}
				}
			}
		}		
		strcpy(c_time,str_date);
		strcat(c_time,str_time);
		m_mapTime[i] = c_time;

	}

	/* check test time sequence */
	CString str_temp="";
	CString str_first="";
	CString str_second="";
	int i_vcount = 1;
	long l_first = 0;
	long l_second = 0;
	for(Itr = m_mapTime.begin(),i_vcount=1; Itr != m_mapTime.end(),i_vcount <= (int)m_mapTime.size(); Itr++,i_vcount++)
	{
		str_temp =  Itr->second;
		str_first = str_temp.Left(8);
		str_second = str_temp.Right(6);

		if((atol(str_first) < l_first) || ((atol(str_first) == l_first) && (atol(str_second) < l_second)) )
		{
			strcpy(sz_ErrCode, ERROR_RROUTING_TIMEERROR);
			sprintf(sz_ErrMsg,("The process:%d test time not sequence\n站别的测试时间前后不符合要求"),Itr->first);
			m_mapTime.clear();
			return false;
		}
		l_first = atol(str_first);
		l_second = atol(str_second);
	}

	m_mapTime.clear();

	return true;
}



/*****************************************************************************
* Function name: CheckSO
* Summary      : Check the SO is same with first process,must be used after checkrouting
* Parameters   : input/Picasso 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/21        First version   
* 1.1          Sunlight.Qian      2012/02/28        Second version, if normal SO and id not in CR, can be test
* 1.2          Nancy.Yang         2013/04/22        add 'D' SO for DOA/RMA
*****************************************************************************/
bool CheckSO(char * sz_so,char * sz_soCRtype,char * sz_picasso,char * sz_model,char * sz_process,char * sz_segment, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_table = _T("");
	CString str_segment = sz_segment;
	CString str_model = sz_model;
	CString str_picasso = sz_picasso;
	char c_CRflag[10]={0};
	char c_DOAFlag[10]={0};
	char c_process[20] = {0};

	/* query id CR info */
	if (!QueryCRHoldInfo(sz_picasso,c_CRflag, sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}
	str_Temp.Format("=== SOType:%s,picasso flag in CRSOIMEIINFO: %s",sz_soCRtype,c_CRflag);
	DBLOG(str_Temp);

	/* query id DOA info */
	if (!QueryDOAInfo(sz_picasso,c_DOAFlag, sz_ErrCode, sz_ErrMsg))
	{
		return false;
	}

	str_Temp.Format("=== SOType:%s,picasso flag in CRSOIMEIINFO: %s",sz_soCRtype,c_CRflag);
	DBLOG(str_Temp);


	if(strcmp(sz_soCRtype,"")==0)   //normal SO
	{
		DBLOG("Normal SO");
		if ( strcmp(c_CRflag,"H") != 0 && strcmp(c_DOAFlag,"D") != 0)    //sunlight add 20120228 ; Nancy and 'D' check 20130419
		{
			return true;
		}
		else
		{
			if (strcmp(c_CRflag,"H") == 0)
			{
				strcpy(sz_ErrCode, ERROR_CRSTATUS);
				sprintf(sz_ErrMsg,"This CR target can't be test with normal SO\nCR的手机不能用正常的工单生产");
			}
			if (strcmp(c_DOAFlag,"D") == 0)
			{
				strcpy(sz_ErrCode, ERROR_DOASTATUS);
				sprintf(sz_ErrMsg,"This DOA target can't be test with normal SO\nDOA的手机不能用正常的工单生产");
			}
			return false;
		}
	}
	else if(strcmp(sz_soCRtype,"D")==0)//DOA/RMA add by Nancy 20130419
	{
		DBLOG("D type SO");
		if (strcmp(c_CRflag,"H") == 0)
		{
			strcpy(sz_ErrCode, ERROR_DOASTATUS);
			sprintf(sz_ErrMsg,"This CR target can't be test with DOA SO\nCR的手机不能用DOA的工单生产");
			return false;
		}
		
		return true;

	}
	else//CR/BO2
	{
		DBLOG("CR type SO");
		if (strcmp(c_DOAFlag,"D") == 0)
		{
			strcpy(sz_ErrCode, ERROR_DOASTATUS);
			sprintf(sz_ErrMsg,"This DOA target can't be test with CR SO\nDOA的手机不能用CR的工单生产");
			return false;
		}

		/*query first process SO and check */
		if(!QueryFirstProcessOfSegment(sz_model,sz_segment,c_process,sz_ErrCode,sz_ErrMsg))
		{
			return false;
		}
		else
		{
			if (strcmp(c_process,sz_process) == 0 )
			{
				return true;
			}
			else
			{
				str_Temp = c_process;
				str_table = _T("PT") + str_Temp;
				if (str_Temp == _T("IMEI"))
				{
					str_sql = _T("select so, okng from "+str_table+" where model='"+str_model+"' and l1='"+str_picasso+"' order by tstdate desc, tsttime desc");
				}
				else 
					str_sql = _T("select so, okng from "+str_table+" where model='"+str_model+"' and id='"+str_picasso+"' order by tstdate desc, tsttime desc");
				
				if(!GetQueryResult(str_sql, p_recordSet))
				{
					strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
					sprintf(sz_ErrMsg,("Query CRSOIMEI hold fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
					p_recordSet->Close();
					return false;
				}

				if(p_recordSet -> GetadoEOF())  
				{
					/* no picasso in SMT booking table */
					strcpy(sz_ErrCode, ERROR_FIRSTPROCESS_NOTTEST);
					sprintf(sz_ErrMsg,("Can't found Picasso:%s in %s\n找不到此ID=%s在%测试的记录"),str_picasso,str_table,str_picasso,str_table);
					p_recordSet->Close();
					return false;
				}
				else
				{
					str_Temp.Empty();
					thevalue=p_recordSet->GetCollect("okng");	
					if (thevalue.vt != VT_NULL)
					{
						str_Temp=(char *)(_bstr_t)(thevalue);
						if (strcmp(str_Temp,"Y") !=0)
						{
							strcpy(sz_ErrCode, ERROR_FIRSTPROCESS_NOTPASS);
							sprintf(sz_ErrMsg,("Last record not PASS\n找不到此ID=%s在%s最后PASS的记录"),str_picasso,str_table);
							p_recordSet->Close();
							return false;
						}
					}

					str_Temp.Empty();
					thevalue=p_recordSet->GetCollect("so");	
					if (thevalue.vt != VT_NULL)
					{
						str_Temp=(char *)(_bstr_t)(thevalue);
						if (strcmp(str_Temp,sz_so) !=0)
						{
							strcpy(sz_ErrCode, ERROR_SONOTSAME);
							sprintf(sz_ErrMsg,("The SO is not same with first process\n现在使用的工单:%s与第一站用的工单:%s不一致"),sz_so,str_Temp);
							p_recordSet->Close();
							return false;
						}
					}
				}

				p_recordSet->Close();
			}
		}
	}

	return true;
}


/*****************************************************************************
* Function name: QueryCRHoldInfo
* Summary      : query hold information
* Parameters   : input/Picasso,IMEI; output/status. 'H'=hold, 'R'=release, empty means no hold record 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/21        First version   
* 1.1          Sunlight.Qian      2012/08/22        Second version, 不只看最后一笔，只要有H就认为有hold（因为同一个picasso可能有2笔记录，一笔是以IMEI 为mnid）
*****************************************************************************/
/**
* @brief QueryCRHoldInfo
*
* ####Query the device have ever had hold record####
*
*     DB Table: CRSOIMEIINFO
*     SQL: SELECT * FROM CRSOIMEIINFO where picasso=sz_picasso or mnid=sz_picasso order by insertdate, inserttime
*/
bool QueryCRHoldInfo(
		char * sz_picasso, ///< Input, Picasso or IMEI
		char * sz_status, ///< Output, H=hold, R=release, empty means no hold record
		char * sz_ErrCode, ///< Output, Error code
		char * sz_ErrMsg ///<Output, Error message
		)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_id = sz_picasso;
 
	/* Query in SMT */
	str_sql = _T("SELECT * FROM CRSOIMEIINFO where (picasso ='"+str_id+"' or mnid='"+str_id+"') order by insertdate , inserttime");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query CRSOIMEI hold fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	while(!p_recordSet -> GetadoEOF())  
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("flag");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_status,"%s",str_Temp);
			if(strncmp(sz_status,"H",1)==0 || strncmp(sz_status, "D", 1) == 0)
				break;
		}

		p_recordSet->MoveNext();
	}

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: QueryCRHoldInfo_byMNID
* Summary      : query hold information
* Parameters   : input/Picasso/IMEI,; output/status. 'H'=hold, 'R'=release,'D" = D Hold empty means no hold record 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei		 2013/10/23        First version   
* *****************************************************************************/
/**
* @brief QueryCRHoldInfo_byMNID
*
* ####1. Query the device have ever had hold record only by mnid####
* ####2. The API QueryCRHoldInfo was used by UI, and this QueryCRHoldInfo_byMNID was used by Warehouse tool####
* ####3. Warehouse tool can't not use QueryCRHoldInfo to do test?
*
*     DB Table: CRSOIMEIINFO
*     SQL: SELECT * FROM CRSOIMEIINFO where mnid=sz_mnid order by insertdate, inserttime
*/
bool QueryCRHoldInfo_byMNID(
		char * sz_mnid,
		char * sz_status, 
		char * sz_ErrCode, 
		char * sz_ErrMsg
		)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_id = sz_mnid;
 
	/* Query in SMT */
	str_sql = _T("SELECT * FROM CRSOIMEIINFO where mnid='"+str_id+"' order by insertdate , inserttime");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query CRSOIMEI hold fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	while(!p_recordSet -> GetadoEOF())  
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("flag");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_status,"%s",str_Temp);
			if(strncmp(sz_status,"H",1)==0 || strncmp(sz_status, "D", 1) == 0)
				break;
		}

		p_recordSet->MoveNext();
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: InsertCRHoldInfo
* Summary      : Check hold information, under the SO is CR type
* Parameters   : input/Picasso 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/21        First version  
* 1.1          Sunlight.Qian      2012/08/22        Second version, add parno in reserve1 column；IMEI always insert a hold record
* 1.2          Nancy.Yang         2013/04/22        add 'D' SO don't need insert/Update CR info
*****************************************************************************/
/**
* @brief InsertCRHoldInfo
*
* ####1. Only pass record need to do InsertCRHoldInfo####
* ####2. CR type empty(Normal SO) or CR type D don't need to do InsertCRHoldInfo ####
* ####3. Do not need to do Insert hold record again if already have 'H' status hold record####
* ####4. Do not insert hold record, if the process is not the first process in the segment or not IMEI station####
* ####5. CR type SO, Pass record, never have hold record and the first station in the segment or IMEI station would be insert hold record here ####
*
*     DB Table: CRSOIMEIINFO
*     Related Function:  QueryCRHoldInfo, QueryFirstProcessOfSegment
*/
bool InsertCRHoldInfo(char * sz_picasso,char * sz_IMEI,char * sz_so,char * sz_soCRtype, char * sz_model,char * sz_process, char * sz_segment,char * sz_employID,char * sz_partno,char * sz_ErrCode, char * sz_ErrMsg)
{
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_table = _T("");
	CString str_so = sz_so;
	CString str_segment = sz_segment;
	CString str_model = sz_model;
	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_IMEI;
	CString str_process = sz_process;
	CString str_employID = sz_employID;
	CString str_partno = sz_partno;
	
	CString str_tstdate = _T("");
	CString str_tsttime = _T("");

	char c_CRflag[10]={0};
	char c_process[20] = {0};

	bool b_db = false;

	/* Get current time */
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_tstdate = datetime.date;
		str_tsttime = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		str_tstdate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_tsttime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	/* Normal SO don't need insert/update CR info */      //Nancy add 'D' SO don't need insert/Update CR info
	if (strcmp(sz_soCRtype,"")==0 || strcmp(sz_soCRtype,"D")==0)
	{
		return true;
	}
 
	/* Query CRhold info*/

	if (strcmp(str_process,"IMEI") == 0)
		b_db = QueryCRHoldInfo(sz_IMEI,c_CRflag, sz_ErrCode, sz_ErrMsg);
	else
		b_db = QueryCRHoldInfo(sz_picasso,c_CRflag, sz_ErrCode, sz_ErrMsg);

	if (!b_db)
	{
		return false;
	}

	/* have a hold record , don't need inset again */
	if(strcmp(c_CRflag,"H") == 0)
	{
		return true;
	}
	else
	{
		if(!QueryFirstProcessOfSegment(sz_model,sz_segment,c_process,sz_ErrCode,sz_ErrMsg))
		{
			return false;
		}
		else
		{
			if (strcmp(c_process,sz_process) != 0 && strcmp(sz_process, "IMEI") !=0)
			{
				return true;  //if not first process or IMEI station ,don't need insert or update CR control record
			}
			else
			{
				if (strcmp(c_CRflag,"R") == 0)
				{
					if (strcmp(str_process,"IMEI") == 0)
					{
						str_sql = _T("update CRSOIMEIINFO set flag='H',process='"+str_process +"', segment='"+str_segment+"',insertdate='"+str_tstdate+"',inserttime='"+str_tsttime+"',insertuser='"+str_employID+"',so='"+str_so+"',reserve1='"+str_partno+"' where mnid='"+str_IMEI+"'");
					}
					else 
					{
					   str_sql = _T("update CRSOIMEIINFO set flag='H',process='"+str_process +"', segment='"+str_segment+"' ,insertdate='"+str_tstdate+"',inserttime='"+str_tsttime+"',insertuser='"+str_employID+"',so='"+str_so+"',reserve1='"+str_partno+"' where (mnid ='"+str_picasso+"' or picasso='"+str_picasso+"')");
					}
				}
				else
				{
					if (strcmp(str_process, "IMEI") == 0)
					{
						str_sql = _T("Insert into CRSOIMEIINFO (mnid,model,segment,process,so,insertdate,inserttime,insertuser,lmdate,lmtime,lmuser,flag,picasso,reserve1) values ('"+str_IMEI+"','"+str_model+"','"+str_segment+"','"+str_process+"','"+str_so+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','H','"+str_picasso+"','"+str_partno+"')");
					}
					else
					{
					   str_sql = _T("Insert into CRSOIMEIINFO (mnid,model,segment,process,so,insertdate,inserttime,insertuser,lmdate,lmtime,lmuser,flag,picasso,reserve1) values ('"+str_picasso+"','"+str_model+"','"+str_segment+"','"+str_process+"','"+str_so+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','H','"+str_picasso+"','"+str_partno+"')");
					}
				}

				if(!ExecuteSQL(str_sql))
				{
					strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
					sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
					return false;
				}
			}
		}
	}
	return true;
}


/*****************************************************************************
* Function name: InsertWareHouseOutHoldInfo
* Summary      : Check hold information, under the SO is CR type
* Parameters   : input/Picasso 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang       2012/03/26           Support WareHouse D status control Flag, Modified Base on InsertCRHoldInfo
*****************************************************************************/
bool InsertWareHouseOutHoldInfo(char * sz_picasso,char * sz_IMEI, char * sz_model,char * sz_partno, char * sz_ErrCode, char * sz_ErrMsg)
{
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_table = _T("");
	CString str_so = _T("");
	CString str_segment = _T("");
	CString str_model = sz_model;
	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_IMEI;
	CString str_process = _T("");
	CString str_employID = _T("");
	CString str_partno = sz_partno;
	
	CString str_tstdate = _T("");
	CString str_tsttime = _T("");

	char c_CRflag_P[10]={0};
	char c_CRflag_I[10]={0};
	char c_process[20] = {0};

	bool b_db = false;

	/* Get current time */
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_tstdate = datetime.date;
		str_tsttime = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		str_tstdate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_tsttime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	/*Query Hold info by IMEI*/
	b_db = QueryCRHoldInfo_byMNID(sz_IMEI, c_CRflag_I, sz_ErrCode, sz_ErrMsg);
	if (!b_db)
	{
		return false;
	}
	if((strcmp(c_CRflag_I,"H") != 0) && (strcmp(c_CRflag_I, "D") != 0))
	{// There is no hold record and insert
		str_sql = _T("Insert into CRSOIMEIINFO (mnid,model,segment,process,so,insertdate,inserttime,insertuser,lmdate,lmtime,lmuser,flag,picasso,reserve1) values ('"+str_IMEI+"','"+str_model+"','"+str_segment+"','"+str_process+"','"+str_so+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','D','"+str_picasso+"','"+str_partno+"')");
		if(!ExecuteSQL(str_sql))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}
	}

	b_db = QueryCRHoldInfo_byMNID(sz_picasso, c_CRflag_P, sz_ErrCode, sz_ErrMsg);
	if (!b_db)
	{
		return false;
	}

	if((strcmp(c_CRflag_P,"H") != 0) && (strcmp(c_CRflag_P, "D") != 0))
	{// There is no hold record and insert
		str_sql = _T("Insert into CRSOIMEIINFO (mnid,model,segment,process,so,insertdate,inserttime,insertuser,lmdate,lmtime,lmuser,flag,picasso,reserve1) values ('"+str_process+"','"+str_model+"','"+str_segment+"','"+str_process+"','"+str_so+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','"+str_tstdate+"','"+str_tsttime+"','"+str_employID+"','D','"+str_picasso+"','"+str_partno+"')");
		if(!ExecuteSQL(str_sql))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,"Query Picasso by IMEI/PPID fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
			return false;
		}
	}

	return true;
}



/*****************************************************************************
* Function name: QueryFirstProcessOfSegment
* Summary      : query first process of the define segment
* Parameters   : input/model,segment; output/first process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/22        First version             
*****************************************************************************/
bool QueryFirstProcessOfSegment(char * sz_model,char * sz_segment, char * sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_model = sz_model;
	CString str_segment = sz_segment;
 
	/* Query in SMT */
	str_sql = _T("SELECT process FROM picsrouting where model ='"+str_model+"' and segment='"+str_segment+"' and pcsseq=1");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query routing fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no picasso in SMT booking table */
		strcpy(sz_ErrCode, ERROR_QUERYPSEQ_FAIL);
		sprintf(sz_ErrMsg,("Can't found routing for this model:%s\n找不到%s在Picsrouting中的顺序"),str_model,str_model);
		return false;
	}
	else
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("process");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_process,"%s",str_Temp);
		}
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryFirstProcessOfSegment
* Summary      : query first process of the define segment
* Parameters   : input/model,segment; output/first process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/02/23        First version             
*****************************************************************************/
bool QueryLastErrorcode(char * sz_id,char * sz_process,char * sz_NGcode,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_table = _T("");
	CString str_process = sz_process;
	CString str_id = sz_id;
 
    str_table = _T("PT") + str_process;

	if (str_process == _T("IMEI"))
	{
		str_sql = _T("SELECT * FROM "+str_table+" where l1='"+str_id+"' and OKNG='N' order by tstdate desc,tsttime desc");
	}
	else
	{
	   str_sql = _T("SELECT * FROM "+str_table+" where id='"+str_id+"' and OKNG='N' order by tstdate desc,tsttime desc");
	}
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query routing fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no picasso in SMT booking table */
		strcpy(sz_ErrCode, ERROR_QUERYLASTERROR);
		sprintf(sz_ErrMsg,("Can't found NG record\n找不到NG记录"),str_id,str_id);
		return false;
	}
	else
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("errorcode");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_NGcode,"%s",str_Temp);
		}
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: CheckNAL_Validity
* Summary      : For IMEI station, check input NAL is valid
* Parameters   : input/partnumber,NAL; output/errorcode, errormsg
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/03/22        First version             
*****************************************************************************/
bool CheckNAL_Validity(char * sz_pn ,char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_pn = sz_pn;
	CString str_NAL = sz_NAL;
 
	/* check input NAL invalid or not */
	str_sql = _T("select * from sssnalinfo where nal='"+str_NAL+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query NAL fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no picasso in SMT booking table */
		strcpy(sz_ErrCode, ERROR_NAL_INVALID);
		sprintf(sz_ErrMsg,("The NAL:%s is invalid\n输入的NAL:%s无效"),str_NAL,str_NAL);
		return false;
	}

	/* check input NAL is belong to this PN or not */
	str_sql = _T("select checknal From sssmdm where mdmitem ='"+str_pn+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query checknal fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		strcpy(sz_ErrCode, ERROR_PNNAL_EMPTY);
		sprintf(sz_ErrMsg,("Can't found NAL rule for this PN\n找不到此料号:%s对应的NAL规则"),str_pn);
		return false;
	}
	else
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("checknal");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_Temp,str_NAL,str_Temp.GetLength())!=0 || str_Temp.GetLength()==0)
		{
			strcpy(sz_ErrCode, ERROR_FIRSTNAL_NOTMATCH);
			sprintf(sz_ErrMsg,("The NAL not match the rule\n输入的NAL:%s不符合规则:%s"),str_NAL,str_Temp);
			return false;
		}
	}

	p_recordSet->Close();
	return true;
}


/*****************************************************************************
* Function name: QueryEmployeeID_Right  
* Summary      : query employeeID level
* Parameters   : input/Employee id, output/level,status (Y means can used, N means can't be used)
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/04/09         First version 
* 1.1          Sunlight.Qian      2012/04/11        Second version , add return workstation name
*****************************************************************************/
bool QueryEmployeeID_Right_V2(char* sz_employId, char * sz_Level,char * sz_status,char * sz_wrkstation, char* sz_ErrCode, char* sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_EmployeeID = sz_employId;

	str_sql = _T("SELECT * FROM toolcontrol WHERE EmployeeID= '"+str_EmployeeID+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Employee ID from tool control fail\n 数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("ToolLevel");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_Level, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("Remark4");	  //status
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_status, _T("%s"), str_Temp);
		}

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("Remark3");	  //workstation name
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_wrkstation, _T("%s"), str_Temp);
		}
	}
	else
	{
		strcpy(sz_ErrCode, ERROR_INVALID_EMPLOYEEID);
		sprintf(sz_ErrMsg,("Query Employee ID fai \n 此工号：%s不是合法的工号，请找工程师确认"),str_EmployeeID);
		return false;
	}

	p_recordSet->Close();
	m_mapPICS[_T("employee")]=sz_employId;
	return true;
}


/*****************************************************************************
* Function name: UpdateEmployeeID_Status
* Summary      : Update employee ID status, N->Y, Y->N
* Parameters   : input/Employee id, status  (Y means can used, N means can't be used)
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/04/09         First version   
* 1.1          Sunlight.Qian      2012/04/11        Second version , add update workstation name
*****************************************************************************/
bool UpdateEmployeeID_Status(char* sz_employId, char * sz_status,char * sz_wrkstation, char* sz_ErrCode, char* sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_status = sz_status;
	CString str_EmployeeID = sz_employId;
	CString str_wrkstation = sz_wrkstation;

	if (str_status != _T("Y") && str_status != _T("N"))
	{
		strcpy(sz_ErrCode, ERROR_INVALID_EMPLOYEEIDSTATUS);
		sprintf(sz_ErrMsg,("Input employee id status is invalid\n输入的工号状态异常%s"),str_status);
		return false;
	}

	if (str_status == _T("Y"))
        str_wrkstation = _T("");

	str_sql = _T("update toolcontrol set Remark4='"+str_status+"', Remark3='"+str_wrkstation+"' where EmployeeID= '"+str_EmployeeID+"'");
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_UPDATE_EMPLOYEEIDSTATUS);
		sprintf(sz_ErrMsg,("Update Employee ID status fail,%s\n更新工号状态失败"),str_sql);
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: QuerySO_Info_All_V4
* Summary      : query information about SO, add return crtype
* Parameters   : input/SO, output/model, pn, sotype, toolversion, factory image version, OS version, SOQty, CR type, segment, sostatus
* Return       : true/false
* Exception    : for PVCS get test version from CIM
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/04/09      First version             
*****************************************************************************/
bool QuerySO_Info_All_V4(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char * sz_segment,char* sz_errorCode, char* sz_errorMsg)
{
//	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);
	
	/* Check input */
	if (strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) {
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n输入的工单不符合长度定义"));
		return false;
	}
	/* Check sz_so length is less than DB define length */

	CString str_sql;
	CString so=sz_so;
	CString str_Temp = "";

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	str_sql = _T("select * from SO_INFO where sord = '"+so+"'");

    /* Execute DB*/
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO fail\n数据库操作异常，请尝试重新测试或者重启程式"),sz_so);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
		try
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("PN");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				_stprintf(sz_partNo, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Model");
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_model, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Rework");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_soType, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SOQty");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_Qty, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Tool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_toolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("Img_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_facImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("SW_Version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_userImageVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("EntTool_version");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_EntranceToolVer, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("CRType");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_CRtype, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("segment");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				_stprintf(sz_segment, _T("%s"), str_Temp);
			}

			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("sostatus");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)_bstr_t(thevalue);
				if (strcmp(str_Temp,"SZ")==0)
				{
					strcpy(sz_errorCode, ERROR_CLOSESO);
					sprintf(sz_errorMsg,("This SO:%s is close SO\n此工单已经被关闭，不能用于生产"),sz_so);
					return false;
				}
			}
		}catch (_com_error e) {
			strcpy(sz_errorCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_errorMsg,("Get so_info filed value fail:%s"),LPCTSTR(e.Description()));
			DBLOG(sz_errorMsg);
			return false;
		}

	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n此工单:%s无效,请确认后重新输入"),sz_so);
		p_recordSet->Close();
		return false;
	}
	
	/* Disconnect DB */
	p_recordSet->Close();

	/*Check the data is valid*/
	if ( strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits \n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}

	if ( strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits \n机种:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}

	if ( strlen(sz_soType) !=1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO type:%s is not equal to %d digits \n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	m_mapPICS[_T("so")]=so;
	m_mapPICS[_T("sotype")]=sz_soType;
	m_mapPICS[_T("partno")]=sz_partNo;
	m_mapPICS[_T("model")]=sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType);
	DBLOG(str_Temp);
	return true;
}


/* Sunlight add special for CSD, 20120504*
/*****************************************************************************
* Function name: GetProcessResult_CSD
* Summary      : Check the test result of this process 
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/05/04        First version             
*****************************************************************************/
bool GetProcessResult_CSD(char * sz_id, char * sz_model,char * sz_process)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model =sz_model;
	CString str_modellike = str_model.Left(4);
	str_modellike += "%";
	
	int i_passCount = 0;

	str_table = _T("PT");
	str_table += sz_process;
	
	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model like '"+str_modellike+"' order by tstdate desc, tsttime desc";   
	}
	else
		str_sql = "select * from "+str_table+" where id='"+str_id+"' and model like '"+str_modellike+"' order by tstdate desc, tsttime desc";   
	
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		while ( (!p_recordSet -> GetadoEOF()) && i_passCount < PASS_COUNT)
		{
			str_result.Empty();
			thevalue = p_recordSet->GetCollect("okng");
			if (thevalue.vt != VT_NULL)
			{
				str_result = (char *)(_bstr_t)(thevalue);
			}

			if (strncmp(str_result,"Y",1) !=0 )
			{
				return false;
			}

			i_passCount++;

			p_recordSet->MoveNext();
		}
		return true;
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}

/*****************************************************************************
* Function name: GetProcessResult_Ngreturn_CSD
* Summary      : Check all the test result which process is before the current process
* Parameters   : id , process
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/05/04         First version             
*****************************************************************************/
bool GetProcessResult_NgReturn_CSD(char * sz_id, char * sz_model, char * sz_process, const CString str_Fixdate, const CString str_Fixtime)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;
	CString str_modellike = str_model.Left(4);
	str_modellike += "%";

	int i_passCount = 0;

	str_table = _T("PT");
	str_table += sz_process;

	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select * from "+str_table+" where l1='"+str_id+"' and model like '"+str_modellike+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";
	}
	else
	    str_sql = "select * from "+str_table+" where id='"+str_id+"' and model like '"+str_modellike+"' and (( tstdate = "+str_Fixdate+" and tsttime > "+str_Fixtime+") or tstdate > "+str_Fixdate+") order by tstdate desc, tsttime desc";   

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		return false;
	}

	if ( !p_recordSet -> adoEOF )
	{
		while ( (!p_recordSet -> GetadoEOF()) && i_passCount < PASS_COUNT)
		{
			str_result.Empty();
			thevalue = p_recordSet->GetCollect("okng");
			if (thevalue.vt != VT_NULL)
			{
				str_result = (char *)(_bstr_t)(thevalue);
			}

			if (strncmp(str_result,"Y",1) !=0 )
			{
				return false;
			}
			i_passCount++;

			p_recordSet->MoveNext();
		}
		return true;
	}
	else
	{
        return false;   //Can't found this ID test result 
	}

}



/*****************************************************************************
* Function name: UnlinkIMEIPicasso
* Summary      : for IMEI TS mode, where earse IMEI, unlink the IMEI and Picasso Link, so this picasso can write another IMEI 
* Parameters   : picasso id , IMEI
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/05/22       First version             
*****************************************************************************/
bool UnlinkIMEIPicasso(char * sz_imei, char * sz_picasso,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_imei;

	str_sql = "update PTIMEI set l30='N',okng='N' where id='"+str_IMEI+"' and l1='"+str_picasso+"'";   
	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_UNLINK_IMEI_PICASSO);
		sprintf(sz_ErrMsg,("Unlink IMEI and picasso fail, SQL: %s \n解除IMEI和PICASSO匹配关系失败"),str_sql);
		return false;
	}

	DBLOG("Unlink IMEI and Picasso success");
	return true;

}


/*****************************************************************************
* Function name: Query_PCBA_NO
* Summary      : for FA print process, query PCBA number
* Parameters   : input/picasso id , output/PCBA no 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/06/19       First version             
* 1.1          Sunlight.qian      2012/09/21       Second version, add process, for CSD_Print
*****************************************************************************/
bool Query_PCBA_NO(char * sz_picasso, char * sz_process,char * sz_PCBAno,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_picasso = sz_picasso;
	CString str_Temp = "";
	CString str_table = "PT";
	str_table+= sz_process;

	str_sql = "select l5 from "+str_table+" where id='"+str_picasso+"' and okng='Y' order by tstdate desc, tsttime desc";   
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_PCBA_NO);
		sprintf(sz_ErrMsg,("Get PCBA NO from PTFAPRINT by PICASSO:%s fail\n在PTFAPRINT表中查找%s对应的PCBA NO 失败"),str_picasso,str_picasso);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{
        str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("l5");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			_stprintf(sz_PCBAno, _T("%s"), str_Temp);
		}
	}

	DBLOG("Query PCBA no by picasso success");
	return true;

}

/*****************************************************************************
* Function name: Query_PCBA_SerialNO
* Summary      : for FA print process, query PCBA serial no for current PN and current YWW
* Parameters   : input/AcerMB , YWW, employee, output/Serial
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/06/18       First version             
*****************************************************************************/
bool Query_PCBA_SerialNO(char * sz_picasso,char * sz_AcerMB, char * sz_YWW, char * sz_Employee,char * sz_Serial, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_picasso = sz_picasso;
	CString str_MB = sz_AcerMB;
	CString str_YWW = sz_YWW;
	CString str_employee = sz_Employee;
	int i_serial= 0;
	CString str_lmdate;
	CString str_lmtime;
	CString str_Temp;

	while (true)
	{
		//Luffy modify get local time to get server time
		MyDatetime datetime;
		if (GetServerTime(datetime))
		{
			str_lmdate = datetime.date;
			str_lmtime = datetime.time;
		}
		else
		{
			SYSTEMTIME time;
			::GetLocalTime(&time);

			str_lmdate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
			str_lmtime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
		}

		str_sql = "Select * from PTPICSSYS where systype ='"+str_MB+"' and term ='"+str_YWW+"' order by lmdate desc, lmtime desc";   
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_PCBA_SERIAL);
			sprintf(sz_ErrMsg,("Query PCBA serial fail\n查找%s,%s对应的PCBA NO 失败"),str_MB,str_YWW);
			return false;
		}

		// Step1. Intial a new record if no serial record
		if ( p_recordSet -> GetadoEOF() )
		{
			str_sql = "insert into PTPICSSYS (systype,term,description,lmuser, lmdate, lmtime) values ('"+str_MB+"' ,'"+str_YWW+"','"+str_Temp+"','"+str_employee+"','"+str_lmdate+"','"+str_lmtime+"')";
			if(!ExecuteSQL(str_sql))
			{
				strcpy(sz_ErrCode, ERROR_UPDATE_PCBA_SERIAL);
				sprintf(sz_ErrMsg,("Excute %s 失败"),str_sql);
				return false;
			}
			continue;
		}

		// Step2. Check column "reserve1" value if it is equal to picasso
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("reserve1");
		if (thevalue.vt != VT_NULL)
			str_Temp = (char *)(_bstr_t)(thevalue);
		if(str_Temp != str_picasso)
		{
			if (str_Temp.GetLength() != 0)
			{
				CString str_ctimeSrv;
				CString str_ctimeRrd;
				CString str_rddate;
				CString str_rdtime;
				thevalue=p_recordSet->GetCollect("lmdate");
				if (thevalue.vt != VT_NULL)
					str_rddate = (char *)(_bstr_t)(thevalue);
				thevalue=p_recordSet->GetCollect("lmtime");
				if (thevalue.vt != VT_NULL)
					str_rdtime = (char *)(_bstr_t)(thevalue);

				str_ctimeSrv.Format(_T("%08s%06s"),str_lmdate,str_lmtime);
				str_ctimeRrd.Format(_T("%08s%06s"),str_rddate,str_rdtime);

				CTime timeSrv(atoi(str_ctimeSrv.Mid(0,4)), atoi(str_ctimeSrv.Mid(4,2)),atoi(str_ctimeSrv.Mid(6,2)),atoi(str_ctimeSrv.Mid(8,2)),atoi(str_ctimeSrv.Mid(10,2)),atoi(str_ctimeSrv.Mid(12,2)));	
				CTime timeRrd(atoi(str_ctimeRrd.Mid(0,4)), atoi(str_ctimeRrd.Mid(4,2)),atoi(str_ctimeRrd.Mid(6,2)),atoi(str_ctimeRrd.Mid(8,2)),atoi(str_ctimeRrd.Mid(10,2)),atoi(str_ctimeRrd.Mid(12,2)));				
				CTimeSpan ts = timeSrv - timeRrd;
				if (ts.GetTotalSeconds() <= 30)
				{
					Sleep(3000);
					continue;
				}
			}
			str_sql = "update PTPICSSYS set reserve1='"+str_picasso+"',lmdate='"+str_lmdate+"',lmtime='"+str_lmtime+"',lmuser='"+str_employee+"' where systype ='"+str_MB+"' and term ='"+str_YWW+"'";   
			if(!ExecuteSQL(str_sql))
			{
				strcpy(sz_ErrCode, ERROR_UPDATE_PCBA_SERIAL);
				sprintf(sz_ErrMsg,("Excute %s 失败"),str_sql);
				return false;
			}
			continue;
		}

		// Step3. Genereate serial and + 1
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("description");
		if (thevalue.vt != VT_NULL)
			str_Temp = (char *)(_bstr_t)(thevalue);
		else
			str_Temp = "0";
		i_serial = atoi(str_Temp);
		i_serial ++;
		sprintf(sz_Serial,"%05X",i_serial);
		str_Temp.Empty();
		str_Temp.Format("%d",i_serial);

        str_sql = "update PTPICSSYS set description='"+str_Temp+"',reserve1='"+"',lmdate='"+str_lmdate+"',lmtime='"+str_lmtime+"',lmuser='"+str_employee+"' where systype ='"+str_MB+"' and term ='"+str_YWW+"'";   
		if(!ExecuteSQL(str_sql))
		{
			strcpy(sz_ErrCode, ERROR_UPDATE_PCBA_SERIAL);
			sprintf(sz_ErrMsg,("Update PCBA serial fail\n更新%s,%s对应的PCBA序列号失败"),str_MB,str_YWW);
			return false;
		}
		DBLOG("Update PCBA serial success");
		break;
	}
	return true;
}


/*****************************************************************************
* Function name: CheckNAL_duplicate
* Summary      : Check NAL 
* Parameters   : IMEI , NAL
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2011/12/06          First version   
*****************************************************************************/
bool CheckNAL_duplicate_V2(char * sz_process,char * sz_IMEI, char * sz_NAL,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql = "";
	CString str_temp = "";
	CString str_IMEI = sz_IMEI;
	CString str_NAL = sz_NAL;
	CString str_table = "PT";
	str_table += sz_process;

	//if( strlen(sz_IMEI)!=15 || strlen(sz_NAL) !=15)
	if(strlen(sz_NAL) !=15)
	{
		strcpy(sz_ErrCode, ERROR_INPUT_NALIMEI);
		//sprintf(sz_ErrMsg,("The input IMEI/NAL empty\nNAl:%s或者IMEI:%s的长度不对"),sz_NAL,sz_IMEI);
		sprintf(sz_ErrMsg,("The input NAL empty\nNAl:%s的长度不对"),sz_NAL);
		return false;
	}

	/* Query NAL info */
	str_sql = "select * from "+str_table+" where l5='"+str_NAL+"'and okng='Y' order by tstdate DESC, tsttime DESC";     //L5:NAL
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query KPTYPE fail, SQL: %s \n查询需要检查的KPTYE失败"),str_sql);
		return false;
	}

	if (p_recordSet->adoEOF)
	{
        return true;
	}
	else
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("l3");    //l3:IMEI
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_IMEI,str_temp,15) != 0)
		{
			strcpy(sz_ErrCode, ERROR_NAL_DUPLICATE);
			sprintf(sz_ErrMsg,("This NAL had been link before,please check\n此NAL:%s已经被IMEI:%s 用过"),str_NAL,str_temp);
			return false;
		}
	}

	DBLOG("Check NAL duplicate success");
	return true;

}


bool ReturnErrNumber(char* sz_model,char* sz_process,char * sz_ErrCode, char * sz_ErrMsg)
{   

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
    
	CString str_Temp = _T("");
	CString str_sql = _T("");
	CString str_model = sz_model;
	CString str_subseq = _T("");
	CString str_pcsseq = _T("");
	CString str_process = _T("");
	int subseq = 0;
	int pcsseq = 0;
	G_ErrNumber = G_ErrNumber - 1100;
	subseq = G_ErrNumber/10;
	pcsseq = G_ErrNumber%10;
    
    str_subseq.Format("%d",subseq);
	str_pcsseq.Format("%d",pcsseq);

    str_sql = _T("select * from picsrouting where model='")+str_model+_T("' and subseq='")+str_subseq+_T("' and pcsseq='")+str_pcsseq+_T("'");
    
    
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query Picsrouting fail, SQL: %s \n查询fail站process失败"),str_sql);
		return false;
	}
    

	if ( !p_recordSet -> GetadoEOF() )
	{   

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("process");	
		if (thevalue.vt != VT_NULL)
		{
			//str_Temp = "FoxTest";
			//AfxMessageBox(str_Temp);
			str_Temp=(char *)(_bstr_t)(thevalue);
			//str_Temp.Format("%s", (char *)(_bstr_t)(thevalue));
			_stprintf(sz_process, _T("%s"), str_Temp);
		}
	}
    
    return true;
}


/*****************************************************************************
* Function name: GetTestInfo
* Summary      : Get test detail infomation by picasso
* Parameters   : input/picasso, model, process, output/column info
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Sunlight.Qian      2012/07/23          First version   
*****************************************************************************/
bool GetTestInfo(char* sz_picasso, char * sz_model,char* sz_process,char * sz_count,char * sz_info,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	FieldsPtr     pFields;
	_variant_t    thevalue, filedvalue;	
	BSTR          bstr_ColName;
	HRESULT       hr;
	CString       str_colName;
	long          ColCount = 0,i=0; 

	CString str_sql = "";
	CString str_temp = "";
	CString str_info = "";

	CString str_model = sz_model;
	CString str_id = sz_picasso;

	CString str_table = "PT";
	str_table += sz_process;


	if (strcmp(sz_process,"IMEI")==0)
	{
	    str_sql = "select * from "+str_table+" where l1='"+str_id+"'and model='"+str_model+"' order by tstdate DESC, tsttime DESC"; 
	}
	else if (strcmp(sz_process,"NAL_SCAN")==0)
	{
		str_sql = "select * from "+str_table+" where l3='"+str_id+"'and model='"+str_model+"' order by tstdate DESC, tsttime DESC"; 
	}
	else
        str_sql = "select * from "+str_table+" where id='"+str_id+"'and model='"+str_model+"' order by tstdate DESC, tsttime DESC"; 
	
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query fail, SQL: %s \n查询失败"),str_sql);
		return false;
	}

	if (p_recordSet->adoEOF)
	{
		strcpy(sz_ErrCode, ERROR_QUERY_TESTINFO);
		sprintf(sz_ErrMsg,("Can't found PASS record for this id \n没有此ID：%s在%s的PASS记录"),str_id,str_table);
        return false;
	}
	else
	{
		str_temp.Empty();
		thevalue = p_recordSet->GetCollect("OKNG");    
		if (thevalue.vt != VT_NULL)
		{
			str_temp = (char *)(_bstr_t)(thevalue);
		}

		if (strncmp(str_temp,"Y",1) !=0 )
		{
			strcpy(sz_ErrCode, ERROR_QUERY_TESTINFO);
			sprintf(sz_ErrMsg,("Can't found PASS record for this id \n没有此ID：%s在%s的PASS记录"),str_id,str_table);
			return false;
		}

		hr = p_recordSet->get_Fields(&pFields);
		if(SUCCEEDED(hr)) 
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Query fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			return false;
		}

		for(i=0;i<ColCount;i++)			
		{		
			str_temp.Empty();
			pFields->Item[i]->get_Name(&bstr_ColName);	//得到记录集中的字段名

			str_colName = bstr_ColName;

			thevalue=p_recordSet->GetCollect(bstr_ColName);
			if (thevalue.vt != VT_NULL)
			{
				str_temp=(char *)_bstr_t(thevalue);
			}

			str_info += str_colName;
			str_info += ",";
			str_info += str_temp;
			str_info += ";";

		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针

	}

	sprintf(sz_count,"%d",ColCount);
	sprintf(sz_info,"%s",str_info);

    p_recordSet->Close();
	return true;

}



/*****************************************************************************
* Function name: GetLastRecord
* Summary      : GetLastRecord  
* Parameters   : id,model ,process,...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Luffy.Liu        2012/07/30          First version   
* 1.1          Sunlight.Qian    2012/08/13          Second version, output char* change to map
*****************************************************************************/
bool GetLastRecord(char* sz_id,char* sz_model,char* sz_process,std::map<CString, CString> & map_Record, char * sz_ErrCode, char * sz_ErrMsg)
{
	if (strlen(sz_id) != 10 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is not matching length\nPicasso的长度不符合要求"),sz_id);
		return false;
	}

	if (strlen(sz_model) < 3 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The model you input:%s is not matching length \n机种名的长度不符合要求"),sz_model);
		return false;
	}

	if (strlen(sz_process) < 2 )
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The process you input:%s is not matching length \n站别名的长度不符合要求"),sz_process);
		return false;
	}

	CString str_sql = "";
	CString str_table = "";
	CString str_result = "";
	CString str_id = sz_id;
	CString str_model = sz_model;
	CString str_process = sz_process;
	CString str_Temp = _T("");
	CString str_TempInfo = _T("");

	str_table = _T("PT");
	str_table += sz_process;

	if (strcmp(sz_process ,"IMEI")==0 || strcmp(sz_process ,"MAC")==0) //IMEI 站ID=IMEI， L1=Picasso
	{
		str_sql = "select top(1)* from "+str_table+" where l1='"+str_id+"' and model='"+str_model+"' and process='" + str_process + "' order by tstdate desc, tsttime desc";
	}
	else
	{
		str_sql = "select top(1)* from "+str_table+" where id='"+str_id+"' and model='"+str_model+"' and process='" + str_process + "' order by tstdate desc, tsttime desc";   
	}

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	FieldsPtr     pFields;
	BSTR          bstr_ColName;
	long   ColCount = 0,i=0;
	CString       str_colName;
	//std::map<CString,CString>::iterator Itr;
	HRESULT       hr;

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
		{
			pFields->get_Count(&ColCount);			//得到记录集的字段集合中的字段的总个数	
			//sprintf(sz_count,"%d", ColCount);		//传出count值
		}
		else
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,_T("Get PN reserve field fail, SQL:%s \n数据库操作异常，请尝试重新测试或者联系工程师查看DBlog"),str_sql);
			p_recordSet->Close();
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
				map_Record[str_colName] = str_Temp;
			}
		}		

		if(SUCCEEDED(hr))
			pFields->Release();    //释放指针
	}

	//str_TempInfo.Empty();
	//for(Itr = map_value.begin(); Itr != map_value.end(); Itr++)
	//{
	//	str_TempInfo += Itr->first;
	//	str_TempInfo += ",";
	//	str_TempInfo += Itr->second;
	//	str_TempInfo += ";";
	//}
	//sprintf(sz_info,"%s",str_TempInfo);
	//sprintf(sz_okng,"%s",map_Record[_T("okng")]);

	/* Disconnect DB */
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: Get server time
* Summary      : get server time  
* Parameters   : in/out datetime
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Luffy.Liu        2012/09/19           First version   
*****************************************************************************/
bool GetServerTime( MyDatetime &datetime )
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString strSql = _T("select CONVERT(varchar(100), GETDATE(), 120) as tstdate");

	if(!GetQueryResult(strSql, p_recordSet))
	{
		DBLOG(_T("Fail to get server time"));
		return false;
	}


	CString str_Temp = _T("");
	if ( !p_recordSet -> GetadoEOF() )
	{   

		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("tstdate");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			str_Temp.Remove('-');
			str_Temp.Remove(':');
			datetime.date = str_Temp.Left(str_Temp.Find(_T(" ")));
			datetime.time = str_Temp.Mid(str_Temp.Find(_T(" "))+1);
		}
		else
		{
			DBLOG(_T("Server time is invalide"));
			return false;
		}
	}

	return true;
}


/*****************************************************************************
* Function name: QuerySO_Info_All_V5
* Summary      : 在QuerySO_Info_All_V4版本上，新增返回Fix_EntTool_ver, Fix_Img_ver
* Parameters   : Input:so
                 Output: model, partNo, soType, toolVer, facImageVer, userImageVer, EntranceToolVer, SOQty, CR type, segment,Fix_EntTool_ver, Fix_Img_ver
* Return       : true / false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0           Alex.Chen          2012/11/27          First version             
*****************************************************************************/
bool QuerySO_Info_All_V5(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char* sz_toolVer, char* sz_facImageVer, char* sz_userImageVer, char* sz_EntranceToolVer, char* sz_Qty, char* sz_CRtype,char* sz_segment, char* sz_Fix_EntTool_ver, char* sz_Fix_Img_ver, char* sz_errorCode, char* sz_errorMsg)
{
	// 检查输入参数
	if(strlen(sz_so) == 0 || strlen(sz_so) < SO_LENGTH) 
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n输入的工单不符合长度定义"));
		return false;
	}
	
	// 变量声明
	CString str_sql;
	CString str_so;
	CString str_Temp;
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	// 赋值
	str_so.Format("%s", sz_so);
	str_sql = _T("select * from SO_INFO where sord = '"+str_so+"'");

	// 执行查询
    if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_errorCode, ERROR_QUERYSO_FAIL);
		sprintf(sz_errorMsg,("Get SO:%s from SO_INFO Fail\n查询SO_INFO失败"), sz_so);
		return false;
	}

	// 查询结果
	if(!p_recordSet->GetadoEOF())
	{
		try
		{
			// PartNumber
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("PN");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)(_bstr_t)(thevalue);
				_stprintf(sz_partNo, _T("%s"), str_Temp);
			}

			// ModelName
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Model");
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_model, _T("%s"), str_Temp);
			}

			// SO Type
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Rework");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_soType, _T("%s"), str_Temp);
			}

			// SOQty
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("SOQty");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_Qty, _T("%s"), str_Temp);
			}

			// Tool Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Tool_version");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_toolVer, _T("%s"), str_Temp);
			}

			// Image Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Img_Version");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_facImageVer, _T("%s"), str_Temp);
			}

			// SW Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("SW_Version");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_userImageVer, _T("%s"), str_Temp);
			}

			// EntranceTool Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("EntTool_version");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_EntranceToolVer, _T("%s"), str_Temp);
			}

			// CR Type
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("CRType");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_CRtype, _T("%s"), str_Temp);
			}

			// Segment
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("segment");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_segment, _T("%s"), str_Temp);
			}

			// Fix EntranceTool Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Fix_EntTool_ver");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_Fix_EntTool_ver, _T("%s"), str_Temp);
			}

			// Fix Image Version
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("Fix_Img_ver");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				_stprintf(sz_Fix_Img_ver, _T("%s"), str_Temp);
			}

			// SO Status
			str_Temp.Empty();
			thevalue = p_recordSet->GetCollect("sostatus");	
			if(thevalue.vt != VT_NULL)
			{
				str_Temp = (char *)_bstr_t(thevalue);
				if(strcmp(str_Temp,"SZ")==0)
				{
					strcpy(sz_errorCode, ERROR_CLOSESO);
					sprintf(sz_errorMsg, ("This SO:%s is close SO\n此工单已经被关闭，不能用于生产"), sz_so);
					return false;
				}
			}
		}
		catch(_com_error e) 
		{
			strcpy(sz_errorCode, ERROR_FILEDVALUE_EMPTY);
			sprintf(sz_errorMsg,("Get so_info filed value fail:%s"), LPCTSTR(e.Description()));
			DBLOG(sz_errorMsg);
			return false;
		}
	}
	else
	{
		strcpy(sz_errorCode, ERROR_INPUTSO_INVALID);
		sprintf(sz_errorMsg,("Invalid SO number\n此工单:%s无效,请确认后重新输入"), sz_so);
		p_recordSet->Close();
		return false;
	}
	
	// 断开数据库连接
	p_recordSet->Close();

	// 检查数据
	if(strlen(sz_partNo) != PN_LENGTH)
	{
		strcpy(sz_errorCode, ERROR_PNLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of PN:%s is not equal to %d digits \n料号：%s 长度不符合要求"),sz_partNo,PN_LENGTH,sz_partNo);
		return false;
	}
	if(strlen(sz_model) > MODEL_LENGTH || strlen(sz_model) == 0)
	{
		strcpy(sz_errorCode, ERROR_MODELLENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of Model:%s is more than %d digits \n机种:%s长度不符合要求"),sz_model,MODEL_LENGTH,sz_model);
		return false;
	}
	if(strlen(sz_soType) != 1)
	{
		strcpy(sz_errorCode, ERROR_SOTYPELENGTH_FAIL);
		sprintf(sz_errorMsg,("The length of SO type:%s is not equal to %d digits \n工单类型: %s 长度不符合要求"),sz_soType,SOTYPE_LENGTH,sz_soType);
		return false;
	}

	// 保存PICS
	m_mapPICS[_T("so")]     = str_so;
	m_mapPICS[_T("sotype")] = sz_soType;
	m_mapPICS[_T("partno")] = sz_partNo;
	m_mapPICS[_T("model")]  = sz_model;

	str_Temp.Format("SQL:%s, Result:%s,%s,%s,%s", str_sql,sz_model,sz_partNo,sz_soType,sz_Qty);
	DBLOG(str_Temp);

	return true;
}

/*****************************************************************************
* Function name: Insert_WareHouseIn
* Summary      : Insert_WareHouseIn  
* Parameters   :  sz_picasso,sz_imei,sz_so...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang       2013/03/26          First version   
*****************************************************************************/
bool Insert_WareHouseIn (char* sz_picasso,char* sz_imei,char* sz_so, char* sz_partNo,char* sz_sw,char * sz_Qty ,char* sz_CartonID,char* sz_ErrCode, char* sz_ErrMsg)
{
	CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	if (strlen(sz_picasso) != 10 )  //Check Picasso Length
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_picasso);
		DBLOG(sz_ErrMsg);
		return false;
	}

	CString str_sql = _T("");
	CString str_table = _T("");

	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_imei;
	CString str_so = sz_so;
	CString str_partno = sz_partNo;
	CString str_sw = sz_sw;
	CString str_Qty = sz_Qty;
	CString str_CartonID = sz_CartonID;
	CString str_InsertDate = _T("");
	CString str_InsertTime = _T("");

	bool b_db = false;

	/* Get current time */
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_InsertDate = datetime.date;
		str_InsertTime = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		str_InsertDate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_InsertTime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	str_sql = _T("Insert into IMEICheckInfo (SO,PartNo,SW,Qty,Carton,IMEI,Picasso,InsertDate,InsertTime,Status) values ('" + 
		str_so+"','"+str_partno+"','"+str_sw+"','"+str_Qty+"','"+str_CartonID+"','"+str_IMEI+"','"+str_picasso+"','"+str_InsertDate+"','"+str_InsertTime+"','L')");

	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Insert into IMEICheckInfo\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	return true;
}

/*****************************************************************************
* Function name: Query_WareHouseStatus
* Summary      : Query_WareHouseStatus  
* Parameters   :  sz_picasso,sz_imei,sz_Status...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang       2013/03/26          First version   
*****************************************************************************/
bool Query_WareHouseStatus(char* sz_picasso,char* sz_imei,char* sz_Status, char* sz_ErrCode, char* sz_ErrMsg)
{
	//CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	if (strlen(sz_picasso) != 10 )  //Check Picasso Length
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_picasso);
		DBLOG(sz_ErrMsg);
		return false;
	}

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");

	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_imei;
 
	/* Query in IMEICheckInfo */
//	str_sql = _T("SELECT * FROM IMEICheckInfo where (IMEI ='"+str_IMEI+"' and Picasso='"+str_picasso+"') order by InsertDate , InsertTime");
	str_sql = _T("SELECT * FROM IMEICheckInfo where (IMEI ='"+str_IMEI+"' or Picasso='"+str_picasso+"') order by InsertDate , InsertTime");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query IMEICheckInfo fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	while(!p_recordSet -> GetadoEOF())  
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("Status");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_Status,"%s",str_Temp);
			if(strcmp(sz_Status, _T("L")) == 0)
				break;
		}
		p_recordSet->MoveNext();
	}
	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: Update_WareHouseOut
* Summary      : Update_WareHouseOut  
* Parameters   :  sz_picasso,sz_imei,sz_ReworkReason...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang       2013/03/26          First version   
*****************************************************************************/
bool Update_WareHouseOut  (char* sz_picasso,char* sz_imei,char* sz_ReworkReason, char* sz_DOARMANo, char* sz_ErrCode, char* sz_ErrMsg)
{
	if (strlen(sz_picasso) != 10 )  //Check Picasso Length
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_picasso);
		DBLOG(sz_ErrMsg);
		return false;
	}

	CString str_sql = _T("");
	CString str_table = _T("");

	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_imei;
	CString str_ReworkReason = sz_ReworkReason;
	CString str_DOARMANo = sz_DOARMANo;
	CString str_InsertDate = _T("");
	CString str_InsertTime = _T("");

	bool b_db = false;

	/* Get current time */
	//Luffy modify get local time to get server time
	MyDatetime datetime;
	if (GetServerTime(datetime))
	{
		str_InsertDate = datetime.date;
		str_InsertTime = datetime.time;
	}
	else
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		str_InsertDate.Format(_T("%04d%02d%02d"),time.wYear,time.wMonth,time.wDay);
		str_InsertTime.Format(_T("%02d%02d%02d"),time.wHour,time.wMinute,time.wSecond);
	}

	str_sql = _T("update IMEICheckInfo set Status = 'U', reworkReason = '" + str_ReworkReason +
		"',DOARMANo='"+str_DOARMANo + 
		"',ReworkDate='" + str_InsertDate + 
		"',ReworkTime='" + str_InsertTime +
		"' where IMEI ='" + str_IMEI +
		//"' and Status='L'"+
		"' or Picasso='" + str_picasso + "'");

	if(!ExecuteSQL(str_sql))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"update IMEICheckInfo fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: QueryApkTestItem  
* Summary      : Query APK test item
* Parameters   : input/Partno, output/test item and flag which is indicate if this item should be test or not
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Nancy.Yang      2013/03/07        First version             
*****************************************************************************/
bool QueryApkTestItem(char * sz_PN, char * sz_Item,char * sz_SDStatus, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_PN = sz_PN;
	
	ZeroMemory(sz_Item,sizeof(sz_Item));

	
	str_sql = _T("SELECT * FROM PICSTESTITEM WHERE Partno='"+str_PN+"'");
	

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}
    
    if(!p_recordSet -> GetadoEOF())
	{

		thevalue=p_recordSet->GetCollect("SDStatus");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_SDStatus,str_Temp);
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("Desc");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_Item,str_Temp);
		strcat(sz_Item,":");

		thevalue=p_recordSet->GetCollect("Flag");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_Item,str_Temp);
		strcat(sz_Item,",");


		p_recordSet->MoveNext();

	}
    
	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: QueryDOAInfo
* Summary      : query if DOA ir RMA
* Parameters   : input/Picasso,IMEI; output/status. 'D'=DOA/RMA 
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Nancy.Yang      2013/04/22        First version   
*****************************************************************************/
bool QueryDOAInfo(char * sz_picasso,char * sz_status, char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_id = sz_picasso;
 
	/* Query in SMT */
	str_sql = _T("SELECT * FROM CRSOIMEIINFO where picasso ='"+str_id+"'order by insertdate , inserttime");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query CRSOIMEI DOA fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	if ( !p_recordSet -> GetadoEOF() )
	{   

		thevalue=p_recordSet->GetCollect("flag");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_status,"%s",str_Temp);
			/*if(strncmp(sz_status,"D",1)==0)
				break;*/
		}
	}

	p_recordSet->Close();
	return true;
}

bool QueryApkTestItem_V2(char * sz_PN, char * sz_Item,char * sz_SDStatus,char* sz_Switch, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_PN = sz_PN;
	
	ZeroMemory(sz_Item,sizeof(sz_Item));

	
	str_sql = _T("SELECT * FROM PICSTESTITEM WHERE Partno='"+str_PN+"'");
	

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}
    
    if(!p_recordSet -> GetadoEOF())
	{

		thevalue=p_recordSet->GetCollect("SDStatus");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_SDStatus,str_Temp);
	}

	   if(!p_recordSet -> GetadoEOF())
	{

		thevalue=p_recordSet->GetCollect("switch");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_Switch,str_Temp);
	}

	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("Desc");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_Item,str_Temp);
		strcat(sz_Item,":");

		thevalue=p_recordSet->GetCollect("Flag");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_Item,str_Temp);
		strcat(sz_Item,",");


		p_recordSet->MoveNext();

	}
    
	p_recordSet->Close();
	return true;

}
/*****************************************************************************
* Function name: Query_WareHouseStatus_V2
* Summary      : Query_WareHouseStatus_V2 
* Parameters   :  sz_picasso,sz_imei,sz_Status...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei       2013/11/15          get L record insert time/date   
*****************************************************************************/
bool Query_WareHouseStatus_V2(char* sz_picasso,char* sz_imei,char* sz_Status,char* szInsertData, char* szInsertTime, char* sz_ErrCode, char* sz_ErrMsg)
{
	//CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	if (strlen(sz_picasso) != 10 )  //Check Picasso Length
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The id you input:%s is null \nPicasso的长度不符合要求"),sz_picasso);
		DBLOG(sz_ErrMsg);
		return false;
	}

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");

	CString str_picasso = sz_picasso;
	CString str_IMEI = sz_imei;
 
	/* Query in IMEICheckInfo */
//	str_sql = _T("SELECT * FROM IMEICheckInfo where (IMEI ='"+str_IMEI+"' and Picasso='"+str_picasso+"') order by InsertDate , InsertTime");
	str_sql = _T("SELECT * FROM IMEICheckInfo where (IMEI ='"+str_IMEI+"' or Picasso='"+str_picasso+"') order by InsertDate , InsertTime");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query IMEICheckInfo fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	while(!p_recordSet -> GetadoEOF())  
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("InsertDate");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(szInsertData,"%s",str_Temp);
		}
		thevalue=p_recordSet->GetCollect("InsertTime");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(szInsertTime,"%s",str_Temp);
		}
		thevalue=p_recordSet->GetCollect("Status");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(sz_Status,"%s",str_Temp);
			if(strcmp(sz_Status, _T("L")) == 0)
				break;
		}
		p_recordSet->MoveNext();
	}
	p_recordSet->Close();
	return true;
}
/*****************************************************************************
* Function name: QueryConsumeType
* Summary      : QueryConsumeType
* Parameters   :  sz_model,sz_process, sz_consume, ..
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei       2014/05/26          get consume type by model/process   
*****************************************************************************/
bool QueryConsumeType(char * sz_model, char * sz_process,char * sz_consume, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_Model = sz_model;
	CString str_Process = sz_process;
	
	ZeroMemory(sz_consume,sizeof(sz_consume));

	
	str_sql = _T("SELECT * FROM consumePCS WHERE model='"+str_Model+"'and process ='"+str_Process+"'");
	

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}
    
	while ( !p_recordSet -> GetadoEOF() )
	{
		thevalue=p_recordSet->GetCollect("consumetype");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_consume,str_Temp);
		strcat(sz_consume,":");

		thevalue=p_recordSet->GetCollect("maxtimes");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcat(sz_consume,str_Temp);
		strcat(sz_consume,",");


		p_recordSet->MoveNext();

	}
    
	p_recordSet->Close();
	return true;

}

/*****************************************************************************
* Function name: QueryConsumeInfo
* Summary      : QueryConsumeInfo
* Parameters   :  sz_sn,sz_type, sz_times, ..
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei       2014/05/26          get consume info by comsume type and sn  
*****************************************************************************/
bool QueryConsumeInfo(char * sz_sn, char * sz_type,char * sz_times, char * sz_ErrCode,char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
    CString str_sn = sz_sn;
	CString str_type = sz_type;
	
	ZeroMemory(sz_times,sizeof(sz_times));

	
	str_sql = _T("SELECT * FROM consumeInfo WHERE sn='"+str_sn+"'and sntype ='"+str_type+"'");
	

	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query information fail,SQL: %s\n数据库操作异常，请尝试重新测试或者重启程式"),str_sql);
		return false;
	}
    
    if(!p_recordSet -> GetadoEOF())
	{
		thevalue=p_recordSet->GetCollect("usedtimes");
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
		}
		strcpy(sz_times,str_Temp);
	}
    
	p_recordSet->Close();
	return true;

}
/*****************************************************************************
* Function name: Query_IDType
* Summary      : Query_IDType
* Parameters   : szpartno, szIDtype...
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Magan.Mei       2014/05/04          Query id type for IMEI/FCHECK/OOB/Warehouse   
*****************************************************************************/
bool Query_IDType(char* szpartno, char* szIDtype, char* szLength, char* sz_ErrCode, char* sz_ErrMsg)
{
	//CSingleLock singleLock(&m_obj_loadADBFileCritSection, TRUE);

	if (strlen(szpartno) == 0 )  //Check Picasso Length
	{
		strcpy(sz_ErrCode, ERROR_INPUTID_INVALID);
		sprintf(sz_ErrMsg,("The partno you input:%s is null \nPart No 长度不符合要求"),szpartno);
		DBLOG(sz_ErrMsg);
		return false;
	}

	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");

	CString str_partno = szpartno;

	//select idtype From SSSMDM where mdmitem ='9J.299PR.001'
 
	str_sql = _T("select * From SSSMDM where mdmitem = '"+str_partno+"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
		return false;
	}

	while(!p_recordSet -> GetadoEOF())  
	{
		str_Temp.Empty();
		thevalue=p_recordSet->GetCollect("idtype");	
		if (thevalue.vt != VT_NULL)
		{
			str_Temp=(char *)(_bstr_t)(thevalue);
			sprintf(szIDtype,"%s",str_Temp);
			break;
		}
		p_recordSet->MoveNext();
	}
	p_recordSet->Close();

	if(str_Temp != _T(""))
	{
		str_sql = _T("select * From CheckIDType where idtype = '"+str_Temp+"'");
		if(!GetQueryResult(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
			return false;
		}

		while(!p_recordSet -> GetadoEOF())  
		{
			str_Temp.Empty();
			thevalue=p_recordSet->GetCollect("IDlength");	
			if (thevalue.vt != VT_NULL)
			{
				str_Temp=(char *)(_bstr_t)(thevalue);
				sprintf(szLength,"%s",str_Temp);
				break;
			}
			p_recordSet->MoveNext();
		}
		p_recordSet->Close();
	}
	return true;
}

/*****************************************************************************
* Function name: InsertConsumeUsedTimes
* Summary      : InsertConsumeUsedTimes
* Parameters   : input: sz_sn,sz_type/ output: sz_usedtimes, sz_ErrCode, sz_ErrMsg
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Karen.Wang       2014/07/22          insert consume usedtimes by consume type and sn  
*****************************************************************************/
/**
* @brief InsertConsumeUsedTimes
*
* ####Increase the number of accessory used times, and return the number (usedtimes)####
*
*     DB Table: ConsumeInfo
*     SQL: SELECT * FROM consumeInfo WHERE sn=sz_sn and sntyp =sz_type
*
*/
bool InsertConsumeUsedTimes(
		char * sz_sn, ///< Accessory SN ID, input
		char * sz_type, ///< Accessory Type Name, input
		char * sz_usedtimes, ///< used times after increased, output
		char * sz_ErrCode, ///< error code, output
		char * sz_ErrMsg ///< error message, output
		)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;

	CString str_sql;
	CString str_Temp;
	CString str_sn = sz_sn;
	CString str_type = sz_type;
	CString str_tstdate = _T("");
	CString str_tsttime = _T("");

	SYSTEMTIME ltime;
	::GetLocalTime(&ltime);
	str_tstdate.Format(_T("%04d%02d%02d"),ltime.wYear,ltime.wMonth,ltime.wDay);
	str_tsttime.Format(_T("%02d%02d%02d"),ltime.wHour,ltime.wMinute,ltime.wSecond);

	str_sql = _T("SELECT * FROM consumeInfo WHERE sn='"+str_sn+"' and sntype ='"+str_type+"'");

	try {
		if(!GetQueryResult_RW(str_sql, p_recordSet))
		{
			strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
			sprintf(sz_ErrMsg,("Query idtype from sssmdm fail,%s\n数据库操作异常，请尝试重新测试或者重启程式 "),str_sql);
			return false;
		}

		if(!p_recordSet -> GetadoEOF()) /*update record, usedtimes +=1*/
		{
			_bstr_t strOldusedtimes = p_recordSet->Fields->GetItem("usedtimes")->Value;
			str_Temp.Format("%d", (atoi((LPSTR)strOldusedtimes)+1));
			sprintf(sz_usedtimes,"%s",str_Temp);

			p_recordSet->Fields->GetItem("usedtimes")->Value = (_bstr_t)str_Temp;
			p_recordSet->Fields->GetItem("lastdate")->Value = (_bstr_t)str_tstdate;
			p_recordSet->Fields->GetItem("lasttime")->Value = (_bstr_t)str_tsttime;
			p_recordSet->Update();

			DBLOG("[InsertConsumeUsedTimes] update record");
		}
		else /*insert new record*/
		{
			sprintf(sz_usedtimes,"%s","1");
			p_recordSet->AddNew();
			p_recordSet->Fields->GetItem("sn")->Value = (_bstr_t)str_sn;
			p_recordSet->Fields->GetItem("sntype")->Value =(_bstr_t) str_type;
			p_recordSet->Fields->GetItem("usedtimes")->Value = (_bstr_t) ("1");
			p_recordSet->Fields->GetItem("firstdate")->Value = (_bstr_t)str_tstdate;
			p_recordSet->Fields->GetItem("firsttime")->Value =(_bstr_t) str_tsttime;
			p_recordSet->Fields->GetItem("lastdate")->Value = (_bstr_t) str_tstdate;
			p_recordSet->Fields->GetItem("lasttime")->Value = (_bstr_t) str_tsttime;
			p_recordSet->Update();
			DBLOG("[InsertConsumeUsedTimes] insert record ");
		}

	}catch(_com_error &e) {
		strcpy(sz_ErrCode, ERROR_INSERT_CONSUMEINFO_FAIL);
		sprintf(sz_ErrMsg,("Get value fail:%s"), LPCTSTR(e.Description()));
		DBLOG(sz_ErrMsg);
		return false;
	}

	p_recordSet->Close();
	return true;
}

/*****************************************************************************
* Function name: CheckBadge_Validity
* Summary      : For KeypartsLink Tool, check input Badge is valid
* Parameters   : input/partnumber,Badge; output/errorcode, errormsg
* Return       : true/false
* Exception    : 
* Version       Author             Date                Abstract                 
* 1.0          Lennon.Zhang      2014/10/20        First version             
*****************************************************************************/
bool CheckBadge_Validity(char * sz_Badge,char * sz_pn ,char * sz_ErrCode, char * sz_ErrMsg)
{
	_RecordsetPtr p_recordSet;
	_variant_t thevalue;
	CString str_sql = _T("");
    CString str_Temp = _T("");
	CString str_pn = sz_pn;
	CString str_Badge = sz_Badge;
 
	/* check input Badge invalid or not */
	str_sql = _T("select * from sssbadgenoinfo where BadgeNO='"+ str_Badge +"'");
	if(!GetQueryResult(str_sql, p_recordSet))
	{
		strcpy(sz_ErrCode, ERROR_QUERY_FAIL);
		sprintf(sz_ErrMsg,"Query Badge fail\n数据库操作异常，请尝试重新测试或者重启程式 ");
		return false;
	}

	if(p_recordSet -> GetadoEOF())  
	{
		/* no Badge in sssbadgenoinfo table */
		strcpy(sz_ErrCode, ERROR_BADGE_INVALID);
		sprintf(sz_ErrMsg,("The Badge:%s is invalid\n输入的Badge:%s无效"),str_Badge,str_Badge);
		return false;
	}

	/* check input Badge is belong to this PN or not */
	thevalue=p_recordSet->GetCollect("partno");	
	if (thevalue.vt != VT_NULL)
	{
		str_Temp=(char *)_bstr_t(thevalue);
	}
	if (str_Temp.GetLength() != 0 && str_Temp != str_pn)
	{
		/* Compare PN in sssbadgenoinfo table */
		strcpy(sz_ErrCode, ERROR_BADGE_PN_NOMATCH);
		sprintf(sz_ErrMsg,("The PN of Badge:%s is not match!\n当前PN:%s,系统中PN:%s"),str_Badge,str_pn,str_Temp);
		p_recordSet->Close();
		return false;
	}
	p_recordSet->Close();
	return true;
}