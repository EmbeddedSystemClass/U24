
#include "stdafx.h"
#include "UnifyDLL_Download.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 


/*****************************************************************************
* Function name: NewInterface      
* Summary      : new CDLInstance. --- Export Function
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
IDL_API bool NewInterface(IFacTestToolInterface** pp_IDL)
{
	bool b_res = false;
	if (!(*pp_IDL))
	{
		*pp_IDL = new CDLInstance;
		b_res = true;
	}

	return b_res;
}


/*****************************************************************************
* Function name: DeleteInterface      
* Summary      : delete CDLInstance. --- Export Function
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
IDL_API bool DeleteInterface(IFacTestToolInterface* p_IDL)
{
	bool b_res = false;
	if (p_IDL)
	{
		delete (CDLInstance*)p_IDL;
		b_res = true;
	}

	return b_res;
}

CDLInstance::CDLInstance()
{
	/* Define UI Event */
	DefineNotify(UI_PROGRESS);
	DefineNotify(UI_MESSAGE);
	DefineNotify(UI_RESULT);
	DefineNotify(UI_PICSDATA);
	DefineNotify(UI_SHOWDIALOG);
	DefineNotify(UI_RELAYBOARD);

	/* Init member */
	m_b_readIniFile = false;
	m_b_SupportQDownload = false;
	i_checkflow = 0;
	i_InsertData = 0;
	i_checkSWVersion = 0;
	m_i_imageCount = 0;
	m_vector_image.clear();
	m_map_picassoList.clear();
	m_map_deviceStruct.clear();
	m_map_deviceStructMultiDLFlag.clear();
	m_map_COMPicasso.clear();
	memset(m_sz_DLMode, 0, sizeof(m_sz_DLMode));

	/* Init google log */
	//obj_gLog.InitializeGLog(LOG_PATH);
	//obj_gLog.InitializeGLog("");

	if(::CoInitialize(NULL) != S_OK)
	{
		::CoUninitialize();
		Sleep(200);
		::CoInitialize(NULL);
	}

	st_Test_Flow[0] = _T("BBTEST") ;
	st_Test_Flow[1] = _T("BTWLAN") ;
	st_Test_Flow[2] = _T("MMI") ;
	st_Test_Flow[3] = _T("WLS") ;
	st_Test_Flow[4] = _T("RUN_IN") ;
	st_Test_Flow[5] = _T("OS_DL") ;
	st_Test_Flow[6] = _T("WRITE") ;
}

CDLInstance::~CDLInstance()
{
	/* Free google log */
	obj_gLog.ShutdownGLog();

	::CoUninitialize();
}

bool CDLInstance::Register(INotify_Char* pNotify, const char* strEvent)
{
	return CNotifyBase_Char::Register(pNotify, strEvent);
}

bool CDLInstance::Register(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return CNotifyBase_Char::Register(pEventFunc, strEvent);
}
bool CDLInstance::Unregister(INotify_Char* pNotify, const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pNotify, strEvent);
}
bool CDLInstance::Unregister(EventFunc_Char* pEventFunc, const char* strEvent)
{
	return CNotifyBase_Char::Unregister(pEventFunc, strEvent);
}
void CDLInstance::DefineNotify(const char* strNewEvent)
{
	CNotifyBase_Char::DefineNotify(strNewEvent);
}
void CDLInstance::Fire(const char* strEvent, long nParam)
{
	CNotifyBase_Char::Fire(strEvent, nParam);
}


/*****************************************************************************
* Function name: Event      
* Summary      : CBaseProject Fire
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDLInstance::Event(const std::string& strEvent, long nParam)
{
	if (strEvent == UI_MESSAGE)
	{
		Fire(UI_MESSAGE, nParam);

		/* Write Log */
		st_UIMessage* p_st_message = (st_UIMessage*)nParam;
		CString str_message;
		str_message.Format(_T("[%d] %s"), p_st_message->i_slot, p_st_message->sz_message);
		WriteLog(str_message);
	}
	else if (strEvent == UI_PROGRESS)
	{
		Fire(UI_PROGRESS, nParam);
	}
	else if (strEvent == UI_PICSDATA)
	{
		Fire(UI_PICSDATA, nParam);
	}
	else if (strEvent == UI_SHOWDIALOG)
	{
		Fire(UI_SHOWDIALOG, nParam);
	}
}


/*****************************************************************************
* Function name: PreRun      
* Summary      : PreRun
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::PreRun(int i_slot)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		m_map_picassoList.erase(i_slot);
		SetErrorMessage(_T("Slot is not in the range."), i_slot);
		return false;
	}

	/* Check read ini result */
	if (!m_b_readIniFile)
	{
		m_map_picassoList.erase(i_slot);
		SetErrorMessage(_T("Read ini file Fail."), i_slot);
		return false;
	}

	///* Check IsConnected */
	//if (IsConnected(i_slot) != true)
	//{
	//	m_map_picassoList.erase(i_slot);
	//	SetErrorMessage(_T("Check handset IsConnected Fail."), i_slot);
	//	return false;
	//}

	/* Check COM port exist */
		/* ReDL download mode */
		CString str_DLMode;
		str_DLMode.Format(_T("%s"), m_sz_DLMode);
		if ((str_DLMode == DOWNLOAD_MODE_REDL) 
			|| ( str_DLMode == DOWNLOAD_MODE_REDL_OSDL )
			|| ( str_DLMode == DOWNLOAD_MODE_OSDL1 )
			)
		{
			return true;
		}

		if (IsCOMPortExist(i_slot) == true)
		{
			return true;
		}
		else
		{
			m_map_picassoList.erase(i_slot);
			SetErrorCode(DLERR_NO_HANDSET, i_slot);
			SetErrorMessage(_T("Check COMPort exist Fail."), i_slot);
			return false;

			/*
			if (ForceToEnterDLMode(i_slot) != true)
			{
				m_map_picassoList.erase(i_slot);
				SetErrorCode(DLERR_NO_HANDSET, i_slot);
				SetErrorMessage(_T("Not Find ADB port too."), i_slot);
				return false;	
			}
			else
			{
				// Check COM port exist
				if (IsCOMPortExist(i_slot) != true)
				{
					m_map_picassoList.erase(i_slot);
					SetErrorCode(DLERR_NO_HANDSET, i_slot);
					SetErrorMessage(_T("Check COMPort exist Fail."), i_slot);
					return false;
				}
			}*/
		}
		/* Check COM port exist */
		if (IsCOMPortExist(i_slot) != true)
		{
			m_map_picassoList.erase(i_slot);
			SetErrorCode(CommErr_No_Phone_Connected, i_slot);
			SetErrorMessage(_T("Check COMPort exist Fail."), i_slot);
			return false;
		}


 //runCheckFlow( int i_type);// i_type 1 = pcbaid,  2 = scalarId
 //runCheckFlowAllStation( 2 );// i_type 1 = pcbaid,  2 = scalarId
 //runReadScalarID( char *szvalue, int iSize );

	//lion
	//DL_PASSPORT_IMGS.clear();
	//DL_ALL_CHECK1_IMGS.clear();
	//FORMAT_IMGS.clear();
	//ERASE_ALL_IMGS.clear();;

	return true;
}


/*****************************************************************************
* Function name: Run      
* Summary      : PreRun -> Run
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::Run(int i_slot)
{	
	//GetSWVersion();//liontest

	if ( i_checkSWVersion != 0 ){//liontes
		if ( i_checkSWVersion == 2){
			char *sz_value = new char[ID_SIZE_BUFFER]  ;
			runReadScalarID( sz_value, ID_SIZE);
		}
		if (!GetPartNo(i_checkSWVersion)) return false;
		if (!GetModelByPartNo(i_checkSWVersion)) return false;
		if (!changeModel()) return false;
		if (!GetSWVersion()) return false;
		if (str_SWVersion.Find(m_szSWver.c_str()) == -1 ){
			CString cs;
			cs.Format(_T("Check SoftWare Version Fail，PVCS Version = %s  Current DL Version = %s"), m_szSWver.c_str(),  str_SWVersion);
			::MessageBox(NULL, cs.GetBuffer(0), _T("Warnning!!"), MB_TASKMODAL|MB_TOPMOST);
			cs.ReleaseBuffer();
			ErrMsg = cs;
			SetErrorMessage(ErrMsg.c_str() , 0);
			return false;
		}
		ErrMsg = "Check SoftWare Version OK";
		SetErrorMessage(ErrMsg.c_str() , 0);
	}

	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}

	/* Get COMPort */
	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;

	/* Initialize project instance */
	CBaseProject* p_obj_DL = NULL;
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);
	if (p_obj_DL == NULL)
	{
		return false;
	}

	/* Register Event */
	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);
	p_obj_DL->Register(this, UI_PICSDATA);
	p_obj_DL->Register(this, UI_SHOWDIALOG);

	/* Set slot */
	p_obj_DL->SetSlotNumber(i_slot);

	/* Set image path name */
	p_obj_DL->SetImageVector(m_vector_image);

	/* Set DLL Ini file name */
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);

	/* Set SlotPicasso */
	p_obj_DL->SetPicassoList(m_map_picassoList);

	/* Set SW version */
	p_obj_DL->SetSWVerion(m_str_SWVersion);

	/* Set Tool version */
	p_obj_DL->SetToolVerion(m_str_ToolVersion);

	/* Set Factory version */
	p_obj_DL->SetFactoryVerion(m_str_fatoryVersion);
	//liontest
	if (i_InsertData != 0)
	{
	 	runInsertData( i_InsertData );// 1 picasso, 2 dell id;
	}


	bool passFail = false;
	if (i_checkflow == 0)
	{
		//do nothing 
	}
	else if  (i_checkflow == 1)
	{
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		//passFail = runReadScalarID( sz_value, ID_SIZE);

		passFail = runCheckFlow( 1 );// i_type 1 = pcbaid,  2 = scalarId
		delete [] sz_value;
		if ( !passFail) return false; 
	}
	else if  (i_checkflow == 2)
	{
		char *sz_value = new char[ID_SIZE_BUFFER]  ;
		passFail = runReadScalarID( sz_value, ID_SIZE);

		passFail = runCheckFlow( 2 );// i_type 1 = pcbaid,  2 = scalarId
		delete [] sz_value;
		if ( !passFail) return false; 
	}
	/* Run */
	bool b_res = false;
	b_res = p_obj_DL->Run();
	if (!b_res)
	{
		char sz_errorCode[4096] = {0};
		p_obj_DL->GetError(sz_errorCode, sizeof(sz_errorCode));
		SetErrorCode(sz_errorCode, i_slot);
	}

	/* Unregister Event */
	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);
	p_obj_DL->Unregister(this, UI_PICSDATA);
	p_obj_DL->Unregister(this, UI_SHOWDIALOG);

	delete p_obj_DL;
	p_obj_DL = NULL;

	/* Clear Map */
	m_map_picassoList.erase(i_slot);

	if (!b_res)
	{
		return false;
	}

//	bool b_res = false;
//	bool passFail = false;

	if (i_InsertData != 0)
	{
		b_res = runInsertData( i_InsertData );// 1 picasso, 2 dell id;
	}

	return b_res;

	//return true;
}

bool CDLInstance::changeModel(){
	CString cs_DBModelNamel = "";
	cs_DBModelNamel = m_ModelName.c_str();
	cs_DBModelNamel.Trim();

	if (cs_DBModelNamel.Compare( _T("U2417HWi") ) == 0 )
	{
		m_str_modelName = _T("GBROB1A");
		ErrMsg = (_T("set  m_str_modelName  = "));
	//	ErrMsg  = ErrMsg + m_str_modelName;
	//	st_XMLSetting = m_str_CMD;
	}
	else 	if (cs_DBModelNamel.Compare( _T("S2317HWi") ) == 0 )
	{
		m_str_modelName = _T("GBROB2A");
		ErrMsg = (_T("set  m_str_modelName  = GBROB2A"));
	}
	else 	if (cs_DBModelNamel.Compare( _T("WR517B") ) == 0 )
	{
		m_str_modelName = _T("GMARB1A");
		ErrMsg = (_T("set  m_str_modelName  = GMARB1A"));
	}
	else
	{
		ErrMsg = (_T("cant find cs_DBModelNamel  = "));
		ErrMsg  = ErrMsg + m_ModelName;
		AfxMessageBox( ErrMsg.c_str() );
		return false;
	}

	SetErrorMessage(ErrMsg.c_str() , 0);
	return true;
}

bool CDLInstance::GetModelByPartNo(int n_type)
{
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef void (_stdcall *lpGetMonitorInfoByPartNo)(const unsigned char* PartNo,    unsigned short PartNoLen,
                                                  unsigned char* WBCFileName, unsigned short WBCFileNameLen,
                                                  unsigned char* ModelName,   unsigned short ModelNameLen,
                                                  unsigned char* DDCFileName, unsigned short DDCFileNameLen,
                                                  unsigned char* Info,        unsigned short InfoLen,
                                                  unsigned char* SWInfo,      unsigned short SWInfoLen,
                                                  unsigned char* Port,        unsigned short PortLen
                                                  );
         lpGetMonitorInfoByPartNo iGetMonitorInfoByPartNo = (lpGetMonitorInfoByPartNo)::GetProcAddress(hDll,"GetMonitorInfoByPartNo");

		//unsigned char sz_ID[ID_SIZE_BUFFER] ="";
	//	unsigned char szStation[ID_SIZE_BUFFER] ="";

         if(NULL != iGetMonitorInfoByPartNo)
         {    
			 unsigned char szPartNo[]="9j.2vm72.dlu";
              unsigned char szWbcFileName[30]= {0};
              unsigned char szModelName[30] = {0};
              unsigned char szDdcFileName[30] = {0};
              unsigned char szInfo[30] = {0};
              unsigned char szSwInfo[30] = {0};
              unsigned char szPort[30] = {0};

			 sprintf_s((char*)szPartNo, 13,"%s", m_szPartNo.c_str() );

			 iGetMonitorInfoByPartNo(szPartNo,13,szWbcFileName,30,szModelName,30,szDdcFileName,30,szInfo,30,szSwInfo,30,szPort,30);

			m_ModelName =  (char*) szModelName;

			if ( m_ModelName.empty() || m_ModelName.length() <1 ){
				ErrMsg = "iGetMonitorInfoByPartNo  fail, m_ModelName = " + m_ModelName;
				goto Exit_FreeLibrary;
			}
			else
			{
				ErrMsg = "iGetMonitorInfoByPartNo  ok, m_ModelName = " + m_ModelName;
				bReturn = true;
			}
		}
		else
		{
			ErrMsg = ("GetModelByPartNo Load iGetMonitorInfoByPartNo Fail");
			AfxMessageBox( ErrMsg.c_str() );
			goto Exit_FreeLibrary;
		 }
	}
	else
	{
			ErrMsg = ("GetModelByPartNo Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			goto Exit_FreeLibrary;
	}

Exit_FreeLibrary:
	if ( n_type == 2 )
	{
		ErrMsg = ErrMsg + " szScalarId = " +  std_ScalarId ;
	}
	else if ( n_type == 1)
	{
		ErrMsg = ErrMsg + " std_Picasso = " +  std_Picasso ;
	}
	SetErrorMessage(ErrMsg.c_str() , 0);
//	TraceLog(MSG_INFO,  ErrMsg);
    FreeLibrary(hDll);

	return bReturn;
}

bool CDLInstance::GetPartNo(int n_type)
{
	bool bReturn = false;
	unsigned char szPartNo[27] = {0};
	CString str_dllF32SERVER2 = F32SERVERDB;
	unsigned char sz_ID[ID_SIZE_BUFFER] ="";
	int nIDLength;
	if (n_type==2)
	{
		if(std_ScalarId.empty() || std_ScalarId.length() != ID_SIZE){
		  //  m_szPartNo =  (char*) szPartNo;
			ErrMsg = "ScalarId fail id  = " + std_ScalarId;
			AfxMessageBox(ErrMsg.c_str());
			goto Exit_FreeLibrary;
		}
		//scalar id
		sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_ScalarId.c_str() );
		nIDLength = 11;
	}
	else if ( n_type ==1){
		if(std_Picasso.empty() || std_Picasso.length() != 10){
		  //  m_szPartNo =  (char*) szPartNo;
			ErrMsg = "Picasso fail id  = " + std_Picasso;
			AfxMessageBox(ErrMsg.c_str());
			goto Exit_FreeLibrary;
		}
		//scalar id
		sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_Picasso.c_str() );
		nIDLength = 10;
	}

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef unsigned short (_stdcall *lpGetPartNoById)(const unsigned char* Id,unsigned short IdLen,unsigned char* PartNo,unsigned short PartNoLen);
		lpGetPartNoById iGetPartNoById = (lpGetPartNoById)::GetProcAddress(hDll,"GetPartNoById");
		if ( NULL != iGetPartNoById )
		{	
			if( 0 != iGetPartNoById( sz_ID , nIDLength, szPartNo, 13))
			{	
			    m_szPartNo =  (char*) szPartNo;
				ErrMsg = "iGetPartNoById  ok, m_szPartNo = " + m_szPartNo;
				bReturn = true;
			}
			else
			{	
				ErrMsg = "GetPartNo.  iGetPartNoById Fail ";
				AfxMessageBox(ErrMsg.c_str());
				goto Exit_FreeLibrary;
			}
		}
		else
		{
				ErrMsg = "GetPartNo. Load str_dllF32SERVER2 Fail ";
				AfxMessageBox(ErrMsg.c_str());
				goto Exit_FreeLibrary;
		}
	}
	else
	{
			ErrMsg = "GetPartNo. iGetPartNoById NULL ";
			AfxMessageBox(ErrMsg.c_str());
			goto Exit_FreeLibrary;
	}

Exit_FreeLibrary:
	if ( n_type == 2 )
	{
		ErrMsg = ErrMsg + " std_ScalarId = " +  std_ScalarId ;
	}
	else if ( n_type == 1)
	{
		ErrMsg = ErrMsg + " std_Picasso = " +  std_Picasso ;
	}
	SetErrorMessage(ErrMsg.c_str() , 0);
    FreeLibrary(hDll);

	return bReturn;
}

bool CDLInstance::GetSWVersion(void)
{    
     bool bReturn = false;

     char szModel[ID_SIZE_BUFFER] = "";
     //char szModel[8] = "GNAMB1A";
     char szSWVersion[30] = "";
     char szErrCode[128] = "";
     char szErrMsg[512] = "";
     char szPartNoInfo[128] = "";
	 char m_sz9JPartNo[31] ="";//9J.2VM72.DLU
	 char m_szImagerVer[31] = "";


     CString str_dllVMS_DBACCESS = VMS_DBACCESS;
     HMODULE hDll = ::LoadLibrary(str_dllVMS_DBACCESS);
     if(hDll == NULL)
     {    
		 goto Exit_FreeLibrary;
     }

     typedef bool (*fpVMS_ConnectDB)(void);
     typedef bool (*fpVMS_CloseDB)(void);
     fpVMS_CloseDB iVMS_CloseDB = (fpVMS_CloseDB)::GetProcAddress (hDll,"VMS_CloseDB");
     if(iVMS_CloseDB == NULL)
     {    //没有VMS_ConnectDB
		ErrMsg = "iVMS_CloseDB = NULL" ;
		AfxMessageBox(ErrMsg.c_str());
         goto Exit_FreeLibrary;
     }
     //char PartNo9J[31] = "9J.2EE12.33E";

     fpVMS_ConnectDB iVMS_ConnectDB = (fpVMS_ConnectDB)::GetProcAddress(hDll,"VMS_ConnectDB");
     if (iVMS_ConnectDB != NULL)
     {  
		 if(iVMS_ConnectDB())
         {    
			 typedef bool (*fpVMS_QuerySQNinfo_S3)(char *szModel, char *szPartNo, char *szSWVersion, char *st_PartnoInfo, char *szErrCode, char *szErrMsg);
              fpVMS_QuerySQNinfo_S3 iVMS_QuerySQNinfo_S3 = (fpVMS_QuerySQNinfo_S3)::GetProcAddress(hDll,"VMS_QuerySQNinfo_S3");
              if (iVMS_QuerySQNinfo_S3 != NULL )
              {  

				 if ( m_str_modelName.GetLength() < 1 ) {
						ErrMsg = "fail  m_str_modelName  = 0 ";
						ErrMsg = ErrMsg + m_str_modelName.GetBuffer(0);
						AfxMessageBox(ErrMsg.c_str());
						m_str_modelName.ReleaseBuffer();
						goto Exit_FreeLibrary;
				 }

				 if(m_szPartNo.empty() || m_szPartNo.length() < 1){
						ErrMsg = "fail  m_szPartNo  =  " + m_szPartNo;
						//ErrMsg = ErrMsg + m_szPartNo;
						AfxMessageBox(ErrMsg.c_str());
						goto Exit_FreeLibrary;
				 }

				 	//AfxMessageBox(m_szPartNo.c_str());
				  sprintf_s(szModel, ID_SIZE_BUFFER, "%s", m_str_modelName);
				  sprintf_s(m_sz9JPartNo, ID_SIZE_BUFFER, "%s", m_szPartNo.c_str());

				  bool bRef = iVMS_QuerySQNinfo_S3(szModel, m_sz9JPartNo, m_szImagerVer, szPartNoInfo, szErrCode, szErrMsg);
                   if(bRef)
                   {  
					   m_szSWver = m_szImagerVer;
						ErrMsg = "iVMS_QuerySQNinfo_S3  ok, m_szSWver = " + m_szSWver;
						//SetErrorMessage(ErrMsg.c_str() , 0);
                       bReturn = true;
                   }
                   else
                   {    //查询失败
						ErrMsg = "iVMS_QuerySQNinfo_S3 fail " ;//+ std_ScalarId;
						AfxMessageBox(ErrMsg.c_str());
				//		sprintf_s( szvalue , ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());
                       goto Exit_CloseDB;
                   }
			  }
              else
              {    //没有VMS_QuerySQNinfo
					ErrMsg = "iVMS_QuerySQNinfo_S3 NULL fail " ;
					AfxMessageBox(ErrMsg.c_str());
                   goto Exit_CloseDB;
			  }
		 }
         else
         {    //链接VMS数据失败
			  ErrMsg = "iVMS_ConnectDB  fail " ;
			  AfxMessageBox(ErrMsg.c_str());
              goto Exit_FreeLibrary;
         }
     }
     else
     {    //没有VMS_ConnectDB
		  ErrMsg = "iVMS_ConnectDB  NULL " ;
		  AfxMessageBox(ErrMsg.c_str());
         goto Exit_FreeLibrary;
     }

Exit_CloseDB:
     iVMS_CloseDB();
Exit_FreeLibrary:
	 SetErrorMessage(ErrMsg.c_str() , 0);
     FreeLibrary(hDll);

     return bReturn;
}


/*****************************************************************************
* Function name: PostRun      
* Summary      : Run -> PostRun
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::PostRun(int i_slot)
{
	return true;

}

bool CDLInstance::Begin(int i_slot)
{
	/* Read INI file */
	/* Set log path and file name */


	m_b_readIniFile = ReadDLLIniFile();
	if (m_b_readIniFile != true)
	{
		SetErrorMessage(_T("Read ini file Fail."), 0);
		return false;
	}

	SetLogFilePath();
	SetLogFileName();

	/* Check MD5 */
	if (m_str_toolMode == "RD")
	{
		return true;
	}
	if (CheckMD5() != true)
	{
		SetErrorMessage(_T("Check MD5 Fail."), 0);
		return false;
	}

	return true;
}

bool CDLInstance::runReadScalarID( char *szvalue, int iSize )
{
	bool b_res = true;
	std::string ErrMsg;

	memset(szvalue,'\0',iSize);
	if(Id.ReadId())
	{	
		//Id.GetId();
		std_ScalarId = Id.GetId();
	//	g_strScalarID = std_ScalarId;
		if(std_ScalarId.empty() || std_ScalarId.length() != ID_SIZE)
		{		
			ErrMsg = "Fail to read ID, ID = "  + std_ScalarId;
			AfxMessageBox( ErrMsg.c_str());
			//SetErrorMessage(ErrMsg.c_str() , 0);
			SetErrorMessage(ErrMsg.c_str() , 0);
			b_res = false;
		}
		else
		{	
			ErrMsg = "Pass to read ID, ID = " + std_ScalarId;
			SetErrorMessage(ErrMsg.c_str() , 0);
			sprintf_s( szvalue , ID_SIZE_BUFFER, "%s", std_ScalarId.c_str());
		}
	}
	else
	{
		ErrMsg = "Read ID fail.";
		AfxMessageBox( ErrMsg.c_str());
		SetErrorMessage(ErrMsg.c_str() , 0);
		b_res = false;
	}
	return b_res;
}

bool CDLInstance::runCheckFlowAllStation( int i_type ){
	int i ;
	int i_limitStation = 0; 
//	g_str_station = "BBTEST";
//	runInsertData(i_type);
	bool b_Res = false;
	for (i = 0 ; i < i_Station_Count ; i ++){
		if (st_Test_Flow[i].find( str_prestation)  != std::string::npos ){
			b_Res = true; 
			i_limitStation = i;
			break;
		}
	}

	if ( b_Res ){
			CString csInsertCmd;
			csInsertCmd.Format(_T("need to check , szStation = %s "), str_prestation );
			ErrMsg =  csInsertCmd;
			SetErrorMessage(ErrMsg.c_str() , 0);		
			//	ErrMsg = "can't find station , runCheckFlowAllStation";
				//AfxMessageBox(ErrMsg.c_str());
			//	SetErrorMessage(ErrMsg.c_str() , 0);		
		for (i = 0 ; i <= i_limitStation ; i ++){
			checkStation = st_Test_Flow[i];
			if  (!runCheckFlow(i_type)){
				b_Res = false;
				return false;
			}else{
				//CString csInsertCmd;
				//csInsertCmd.Format(_T(" check OK, Station = %s " ), checkStation);
				//ErrMsg =  csInsertCmd;
				//SetErrorMessage(ErrMsg.c_str() , 0);
			}
		}
	}else{
			ErrMsg = "can't find station , runCheckFlowAllStation";
			AfxMessageBox(ErrMsg.c_str());
			SetErrorMessage(ErrMsg.c_str() , 0);
			return false;		
	}

	return true;
}
bool CDLInstance::runCheckFlow( int i_type)
{	
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef bool (_stdcall *lpGetYrstationInfoByIdAndStation)( 
																unsigned short WeekCount,const unsigned char* szId,          
			                                                    unsigned short IdLen,const unsigned char* Station,       
																unsigned short StationLen);
		lpGetYrstationInfoByIdAndStation iGetYrstationInfoByIdAndStation = (lpGetYrstationInfoByIdAndStation)::GetProcAddress(hDll,"GetYrstationInfoByIdAndStation");

		unsigned char sz_ID[ID_SIZE_BUFFER] ="";
		unsigned char szStation[ID_SIZE_BUFFER] ="";
		
		if( iGetYrstationInfoByIdAndStation != NULL)
		{	

			sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", checkStation.c_str());//station name (before this station)
			//sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", g_str_station.c_str());

			if ( i_type == 1 ){ //arm board id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_Picasso.c_str() );
				ErrMsg = " check flow1 = " ;
				ErrMsg =  ErrMsg + std_Picasso.c_str() ;
				SetErrorMessage(ErrMsg.c_str() , 0);
			}
			else if( i_type == 2 )
			{
			//scalar id
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_ScalarId.c_str() );
				ErrMsg = " check flow2 = ";
				ErrMsg = ErrMsg 	+ std_ScalarId.c_str() ;
			//	AfxMessageBox(ErrMsg.c_str());
				SetErrorMessage(ErrMsg.c_str() , 0);
			}
			else
			{
				ErrMsg = "can't find id type ";
				AfxMessageBox(ErrMsg.c_str());
				SetErrorMessage(ErrMsg.c_str() , 0);
				return false;
			}

			////scalar id
			//sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_ScalarId.c_str() );
			//ErrMsg = " check flow2 = ";
			//ErrMsg = ErrMsg 	+ std_ScalarId.c_str() ;
			////AfxMessageBox(ErrMsg.c_str());
			//SetErrorMessage(ErrMsg.c_str() , 0);
			
			std::string sta =(char*) sz_ID;
			int i_week = GetCurrentWeek();

			CString csiGetYrstationInfoByIdAndStation;
			csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
			ErrMsg = "csiGetYrstationInfoByIdAndStation cmd = ";
			ErrMsg = ErrMsg + csiGetYrstationInfoByIdAndStation.GetBuffer(0);
			csiGetYrstationInfoByIdAndStation.ReleaseBuffer();
			SetErrorMessage(ErrMsg.c_str() , 0);

			if(  iGetYrstationInfoByIdAndStation( i_week, sz_ID, ID_SIZE_BUFFER, szStation, ID_SIZE_BUFFER) ) {
				csiGetYrstationInfoByIdAndStation.Format(_T("i_week = %d, sz_ID = %s, szStation = %s "), i_week,  sz_ID, szStation);
				ErrMsg = "csiGetYrstationInfoByIdAndStation pass";	
				SetErrorMessage(ErrMsg.c_str() , 0);
				bReturn =  true; //exist id 
			}
			else
			{

				ErrMsg = " iGetYrstationInfoByIdAndStation false = " + sta ;
				SetErrorMessage(ErrMsg.c_str() , 0);

				CString csMsg;
				csMsg.Format("%s 站无测试纪录，请回去重测, %s no test record, ", checkStation.c_str(), checkStation.c_str());
				ErrMsg = csMsg;// CW2A(csMsg.GetBuffer(0));
				SetErrorMessage(ErrMsg.c_str() , 0);

				AfxMessageBox(csMsg);
				//showMsg(csMsg);
				//AfxMessageBox(ErrMsg.c_str());
			//	SetErrorMessage(ErrMsg.c_str() , 0);
				bReturn =  false;//id didn't exist
				return false;
			}

	//		unsigned char Id[15] = "F1008B28887";
	//		unsigned char szStation[20] = "MMI_EM";
	//		int i_week = GetCurrentWeek();
	//		std::string sta = (char*)Id;

	//		if(  iGetYrstationInfoByIdAndStation( i_week, Id, 15, szStation, 10) ) {
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation ok = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());						
	//		}
	//		else
	//		{
	//			
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation fail = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());				
	//		}

	//		unsigned char Id1[15] = "F1008B28888";
	//		sta = (char*)Id1;
	////		 Id[15] = "F1008B28888";
	//		if(  iGetYrstationInfoByIdAndStation( i_week, Id1, 15, szStation, 10) ) {
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation ok = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());						
	//		}
	//		else
	//		{
	//			
	//			ErrMsg = "Load iGetYrstationInfoByIdAndStation fail = " + sta ;
	//			//ErrMsg = ErrMsg + Id;
	//			AfxMessageBox(ErrMsg.c_str());				
	//		}

	//brunGetExistHDCPKEY("F1008B28887" );
	//brunGetExistHDCPKEY("F1008B28888" );

		}
		else
		{
			ErrMsg = ("Load GetYrstationInfoByIdAndStation Fail");
			AfxMessageBox( ErrMsg.c_str() );
			SetErrorMessage(ErrMsg.c_str() , 0);
			return false;
		}
	}
	else
	{
			ErrMsg = ("Load str_dllF32SERVER2 Fail");
			AfxMessageBox( ErrMsg.c_str() );
			SetErrorMessage(ErrMsg.c_str() , 0);
			return false;
	}
	return bReturn;
}
bool CDLInstance::End(int i_slot)
{
	return true;
}

/*****************************************************************************
* Function name: Stop      
* Summary      : Stop
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::Stop(int i_slot)
{
	return true;
}


/*****************************************************************************
* Function name: SetParameterValue      
* Summary      : UI -> DLL
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::SetParameterValue(char* sz_keyword, char* sz_value)
{
	/* Check Input */
	if ((sz_keyword == NULL) || (sz_value == NULL))
	{
		return false;
	}

	CString str_temp = _T("");
	if (strcmp(sz_keyword, "MODEL_NAME") == 0)
	{
		str_temp.Format(_T("%s"), sz_value);
		m_str_projectName = str_temp;
	}

	CString str_key;
	str_key.Format(_T("%s"), sz_keyword);

	/* Image path */
	if (str_key == PARAMETER_ONE)
	{
		m_str_imageFilePath.Format(_T("%s"), sz_value);

		std::vector<st_Image>::iterator iter;
		for (iter = m_vector_image.begin(); iter != m_vector_image.end(); iter++)
		{
			(*iter).str_imagePath = m_str_imageFilePath;
		}
	}

	/* Picasso list --- Format: slot,picasso*/
	else if (str_key == PARAMETER_THREE) 
	{
		CString str_temp;
		str_temp.Format(_T("%s"), sz_value);
		int i_first = -1;
		i_first = str_temp.FindOneOf(_T(","));
		if (i_first != -1)
		{
			try 
			{
				/* Get slot */
				int i_slot = -1;
				i_slot = ::atoi(str_temp.Left(i_first));
				if ((i_slot < 0) || (i_slot >= MAX_SLOT)) 
				{
					return false;
				}

				/* Get picasso */
				CString str_picasso;
				str_picasso = str_temp.Right(str_temp.GetLength() - i_first - 1);
				m_map_picassoList[i_slot] = str_picasso;
				str_picasso.Trim();
				std_Picasso = str_picasso.GetBuffer(0);
				str_picasso.ReleaseBuffer();
				m_map_COMPicasso[m_map_deviceStruct[i_slot].i_COMPort] = str_picasso;
			}
			catch(CMemoryException* e)
			{
				e->Delete();
				return false;
			}
		}
		else
		{
			return false;
		}

		/* Set DL log filename */
		SetLogFilePath();
		SetLogFileName();
	}

	/* SW Version */
	else if (str_key == PARAMETER_FOUR)
	{
		m_str_SWVersion.Format(_T("%s"), sz_value);
	}

	/* HW Version */
	else if (str_key == PARAMETER_FIVE)
	{
		m_str_HWVersion.Format(_T("%s"), sz_value);
	}

	/* Tool Version */
	else if (str_key == PARAMETER_SEVEN)
	{
		m_str_ToolVersion.Format(_T("%s"), sz_value);
	}

	/* Factory Version */
	else if (str_key == PARAMETER_EIGHT)
	{
		m_str_fatoryVersion.Format(_T("%s"), sz_value);
	}

	/* LOG path */
	else if (str_key == UI_LOG_PATH)
	{
		m_str_UI_LogPath.Format(_T("%s"), sz_value);
		obj_gLog.SetGLogPath(m_str_UI_LogPath);
	}

	/* Tool Mode */
	else if (str_key == TOOL_MODE)
	{
		m_str_toolMode.Format(_T("%s"), sz_value);
	}

	/* Download mode */
	else if (str_key == DLMODE)
	{
		strcpy(m_sz_DLMode, sz_value);
		m_str_DLMode.Format("%s", m_sz_DLMode);
		if ((m_str_DLMode != DOWNLOAD_MODE_PREDL) 
			&& (m_str_DLMode != DOWNLOAD_MODE_REDL) 
			&& (m_str_DLMode != DOWNLOAD_MODE_REDL_OSDL)
			&& (m_str_DLMode != DOWNLOAD_MODE_OSDL1)
			)
		{
			return false;
		}
	}

	/* Model Name */
	else if (str_key == MODEL_NAME)
	{
		m_str_modelName.Format(_T("%s"), sz_value);
	}

	else if (str_key == CUSTOMER_SW_VER)
	{
		m_str_CustomerSwVersion.Format(_T("%s"), sz_value);
	}

	else
	{
		return true;
	}

	return true;
}


/*****************************************************************************
* Function name: GetParameterValue      
* Summary      : DLL -> UI
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetParameterValue(char* sz_keyword, char* sz_value, int i_size)
{
	/* Check Input */
	if ((sz_keyword == NULL) || (sz_value == NULL) || (i_size <= 0))
	{
		return false;
	}

	CString str_key;
	str_key.Format(_T("%s"), sz_keyword);

	/* Get COM port list */
	if (str_key == PARAMETER_TWO)
	{
		sprintf_s(sz_value, i_size, _T("%s"), m_str_comPortList);
	}
	else if (str_key == PARAMETER_SEVEN)
	{
		sprintf_s(sz_value, i_size, _T("%s"), m_str_ToolVersion);
	}
	else if (str_key == PARAMETER_NINE)
	{
		//sprintf_s(sz_value, i_size, _T("1D45&0138"));
		//sprintf_s(sz_value, i_size, _T("0918&0116"));
		sprintf_s(sz_value, i_size, m_sz_PID_VID);
		
		//sprintf_s(sz_value, i_size, _T("0918&0116,1D45&459B"));
	}
	else
	{
		return false;
	}

	return true;
}


/*****************************************************************************
* Function name: SetFAData      
* Summary      : UI -> DLL FA data(512 bytes)
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::SetFAData(int i_slot, char* sz_value)
{	
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}

	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;
	CBaseProject* p_obj_DL = NULL;

	/* Project */
	//if (strcmp(m_str_projectName, PROJECT_NAME) == 0)
	//{		
	//	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);
	//}
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);

	if (p_obj_DL == NULL)
	{
		return false;
	}

	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);

	/* Set slot */
	p_obj_DL->SetSlotNumber(i_slot);

	/* Set DLL Ini file name */
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);

	bool b_res = false;
	b_res = p_obj_DL->WriteFA(sz_value);

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}


/*****************************************************************************
* Function name: GetFAData      
* Summary      : DLL -> UI FA data(512 bytes)
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetFAData(int i_slot, char* sz_value, int i_size)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}

	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;
	CBaseProject* p_obj_DL = NULL;

	/* Project */
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);

	if (p_obj_DL == NULL)
	{
		return false;
	}

	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);

	/* Set slot */
	p_obj_DL->SetSlotNumber(i_slot);

	/* Set DLL Ini file name */
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);

	char sz_data[512] = {0};
	bool b_res = false;
	b_res = p_obj_DL->ReadFA(sz_data);
	memcpy(sz_value, sz_data, i_size);

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}

/*****************************************************************************
* Function name: GetFASector      
* Summary      : Read FA --- New FA structure
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/11/23          First version             
*****************************************************************************/
bool CDLInstance::GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}
	if ((i_sectorNum < 0) || (i_sectorSize > 512))
	{
		return false;
	}

	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;
	CBaseProject* p_obj_DL = NULL;

	/* Project */
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);
	if (p_obj_DL == NULL)
	{
		return false;
	}

	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);

	/* Set slot */
	p_obj_DL->SetSlotNumber(i_slot);

	/* Set DLL Ini file name */
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);
	/* Set image path name */
	p_obj_DL->SetImageVector(m_vector_image);

	bool b_res = false;
	int i_tmp = 1;
	b_res = p_obj_DL->ReadFASector(i_sectorNum, sz_sectorData, i_sectorSize,  i_tmp);

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}


bool CDLInstance::SetTag(int i_slot, char *sz_sectorData, int i_sectorSize)
{
	return true;
}
bool CDLInstance::SetSn(int i_slot, char *sz_sectorData, int i_sectorSize)
{
	return true;
}
/*****************************************************************************
* Function name: SetFASector      
* Summary      : Write FA --- New FA structure
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/11/23          First version             
*****************************************************************************/
bool CDLInstance::SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}
	if ((i_sectorNum < 0) || (i_sectorSize > 512))
	{
		return false;
	}

	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;
	CBaseProject* p_obj_DL = NULL;

	/* Project */
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);
	if (p_obj_DL == NULL)
	{
		return false;
	}

	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);

	/* Set slot */
	p_obj_DL->SetSlotNumber(i_slot);

	/* Set DLL Ini file name */
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);

	bool b_res = false;
	b_res = p_obj_DL->WriteFASector(i_sectorNum, sz_sectorData, i_sectorSize);

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}


/*****************************************************************************
* Function name: GetItemCount      
* Summary      : Image file number.
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
int CDLInstance::GetItemCount(void)
{
	return m_i_imageCount;
}


/*****************************************************************************
* Function name: GetItemByIndex      
* Summary      : Get image file name by the slot.
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetItemByIndex(int i_index, char* sz_name, int i_size)
{
	/* Check Input */
	if (i_index >= m_i_imageCount)
	{
		return false;
	}

	/* Get file name */
	strncpy(sz_name, m_vector_image[i_index].str_imageName.c_str(), i_size);

	return true;
}


/*****************************************************************************
* Function name: ResopnseToJig      
* Summary      : UI response to Jig.  --- Jig control request
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::ResopnseToJig(bool b_wait)
{
	return true;
}


/*****************************************************************************
* Function name: ResopnseToShowDlg      
* Summary      : UI response to DLL.
* Parameters   : 
*
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::ResopnseToShowDlg(char* sz_showReslut)
{
	return true;
}


/*****************************************************************************
* Function name: ReadDLLIniFile      
* Summary      : Read DLL ini file
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::ReadDLLIniFile(void)
{
	/* Get DLL ini file */
	if (GetDLLIniFile() != true)
	{
		return false;
	}

	//GetSupportQDownload();
	//if(m_b_SupportQDownload)
	//{
		/* Get COM Port */
		if (GetCOMPort() != true)
		{
			return false;
		}
	//}
	//else
	//{
	//	if (ReadSetupIniFile() != true)
	//	{
	//		return false;
	//	}
	//}

	/* Get Multi-download flag */
	if (GetMultiDLFlag() != true)
	{
		return false;
	}

	/* Get current DL mode */
	if (GetDLMode() != true)
	{
		return false;
	}

	/*read image */
	//if (ReadIMG() != true)
	//{
	//	return false;
	//}

	
	/* Get SetImagePath */
	if (SetImagePath() != true)
	{
		return false;
	}

	///* Get image file name */
	//if (GetImageCount() != true)
	//{
	//	return false;
	//}

	/* Get tool version */
	if (GetToolVersion() != true)
	{
		return false;
	}

	if (Get_PID_VID() != true)
	{
		return false;
	}

	if (GetCheckFlowInsertData	() != true)
	{
		return false;
	}

	return true;
}

/* Support for factory tool */
bool CDLInstance::ReadSetupIniFile(void)
{
	/* Setup.ini FilePath */
	if (m_str_modelName == "")
	{
		return false;
	}
	CString str_iniPath;
	str_iniPath.Format("C:\\PE_Config\\%s\\Setting\\DL_Setup.ini", m_str_modelName);
	if (_taccess(str_iniPath, 0) != 0) 
	{
		return false;
	}

	CIniAccess obj_dllMd5Ini(_T("MD5checkTime"), str_iniPath);
	CGlobal::nCheckMd5TimeLimit = obj_dllMd5Ini.GetValue("checkTime");
	//printf("%d", CGlobal::nCheckMd5TimeLimit);

	/* Get COM port */
	DeviceStruct st_device ;
	st_device.i_COMPort = 0;
	st_device.str_errorCode = _T("");
	CIniAccess obj_dllIni(_T("COMPort"), str_iniPath);
	CString str_comkey;
	int i_comPort = 0;
	CString str_comPort;
	for (int i = 0; i < MAX_SLOT; i++)
	{
		str_comkey.Format(_T("Port_%02d"), i+1);
		i_comPort = obj_dllIni.GetValue(str_comkey);
		str_comPort.Format(_T("%d"), i_comPort);
		if (i == MAX_SLOT - 1) 
		{
			m_str_comPortList += str_comPort;
		}
		else
		{
			m_str_comPortList += str_comPort + _T(",");
		}

		/* Save to Map */
		st_device.str_errorCode = _T("");
		if (i_comPort == 0)
		{
			st_device.str_errorCode = CommErr_No_Phone_Connected;
		}
		st_device.i_COMPort = i_comPort;
		m_map_deviceStruct[i] = st_device;
	}

	return true;
}


/*****************************************************************************
* Function name: GetDLLIniFile      
* Summary      : Get DLL ini file
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetDLLIniFile(void)
{
	/* Get current DLL path */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	/* Check data file exist */
	CString str_iniFile;
	CString str_message;

	if (m_str_DLMode.Compare(_T("PREDL")) == 0 ){ 
		csStation.Format(_T("PREDL"));
	}
	else if (m_str_DLMode.Compare(_T("ReDL")) == 0 )
	{
		csStation.Format(_T("OS_DL"));
	}


	str_iniFile.Format(_T("%s\\QISDA\\%s_%s_TestItem_RD.xml"), sz_currentPath, m_str_projectName, csStation );
	if (_taccess(str_iniFile, 0) != 0) 
	{
		
		str_message.Format(_T("read xml %s fail"), str_iniFile);
		WriteLog(str_message);		
	}
	else
	{
		/* Write Log */
		str_message.Format(_T("read xml %s successful"), str_iniFile);
		WriteLog(str_message);		
	}

	m_str_iniFileName = str_iniFile; 
	return true;
}

/*****************************************************************************
* Function name: GetSupportQDownload      
* Summary      : Get SupportQDownload flag from ini file
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Erin.Chang      2013/06/26          First version             
*****************************************************************************/
//bool CDLInstance::GetSupportQDownload(void)
//{
//	if (m_str_iniFileName == "")
//	{
//		return false;
//	}
//
//	CIniAccess obj_dllIni("Setting", m_str_iniFileName);
//	int i_SupportQDownload = obj_dllIni.GetValue("SupportQDownload", -1);
//
//	m_b_SupportQDownload = (i_SupportQDownload == 1) ? true : false;
//	return true;
//}
/*****************************************************************************
* Function name: GetCOMPort      
* Summary      : Get COM port from ini file
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetCOMPort(void)
{
	/* Init */
	DeviceStruct st_device ;
	st_device.i_COMPort = 0;
	st_device.str_errorCode = _T("");

	/* Get COMPort list */
	CIniAccess obj_dllIni(_T("COMPort"), m_str_iniFileName);
	CString str_comkey;
	int i_comPort = 0;
	CString str_comPort;
	for (int i = 0; i < MAX_SLOT; i++)
	{
		str_comkey.Format(_T("Port_%02d"), i+1);
		i_comPort = obj_dllIni.GetValue(str_comkey);
		str_comPort.Format(_T("%d"), i_comPort);
		if (i == MAX_SLOT - 1) 
		{
			m_str_comPortList += str_comPort;
		}
		else
		{
			m_str_comPortList += str_comPort + _T(",");
		}

		/* Save to Map */
		st_device.str_errorCode = _T("");
		if (i_comPort == 0)
		{
			st_device.str_errorCode = CommErr_No_Phone_Connected;
		}
		st_device.i_COMPort = i_comPort;

		m_map_deviceStruct[i] = st_device;
	}

	return true;
}

bool CDLInstance::GetMultiDLFlag(void)
{
	/* Init */
	DeviceStruct st_device ;
	st_device.str_multiDLFlag = _T("");
	st_device.str_errorCode = _T("");

	/* Get flag list */
	CIniAccess obj_dllIni(_T("MultiDLFlag"), m_str_iniFileName);
	CString str_comkey;
	int i_flag = 0;
	CString str_flag = _T("");
	for (int i = 0; i < MAX_SLOT; i++)
	{
		str_comkey.Format(_T("Port_%02d"), i+1);
		i_flag = obj_dllIni.GetValue(str_comkey);
		str_flag.Format(_T("%0d"), i_flag);

		if (i == MAX_SLOT - 1) 
		{
			m_str_multiDLFlagList += str_flag;
		}
		else
		{
			m_str_multiDLFlagList += str_flag + _T(",");
		}

		/* Save to Map */
		st_device.str_errorCode = _T("");
		if (str_flag == _T(""))
		{
			st_device.str_errorCode = CommErr_No_Phone_Connected;
		}
		st_device.str_multiDLFlag = str_flag;

		m_map_deviceStructMultiDLFlag[i] = st_device;
	}

	return true;
}


/*****************************************************************************
* Function name: GetDLMode      
* Summary      : Get download mode from ini file.
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::GetDLMode(void)
{
	/* Get Download mode */
	char sz_mode[ARRAY_SIZE] = { 0 };
	CIniAccess obj_dllIni(_T("Current_Mode"), m_str_iniFileName);
	obj_dllIni.GetValue(_T("Mode"), sz_mode, sizeof(sz_mode));
	
	strcpy_s(m_sz_DLMode, sz_mode);

	if (strlen(m_sz_DLMode) == 0)
	{
		return false;
	}

	return true;
}


bool CDLInstance::Get_PID_VID(void)
{
	/* Get Download mode */
	char sz_PID_VID[ARRAY_SIZE] = { 0 };
	CIniAccess obj_dllIni(_T("PID_VID"), m_str_iniFileName);
	obj_dllIni.GetValue(_T("PID_VID"), sz_PID_VID, sizeof(sz_PID_VID));
	
	strcpy_s(m_sz_PID_VID, sz_PID_VID);

	if (strlen(m_sz_PID_VID) == 0)
	{
		return false;
	}
	/* Write Log */
	CString str_message = sz_PID_VID;
	//str_message.Format(_T("[%d] %s"), p_st_message.i_slot, p_st_message.sz_message);
	WriteLog(str_message);
	return true;
}

bool CDLInstance::GetToolVersion(void)
{
	/* Get ToolVersion */
	char sz_toolVersion[ARRAY_SIZE] = { 0 };
	CIniAccess obj_dllIni(_T("Setting"), m_str_iniFileName);
	//obj_dllIni.GetValue(_T("ToolVersion"), sz_toolVersion, sizeof(sz_toolVersion));
	if ( !obj_dllIni.GetToolVersion(_T("Tool_Version"), sz_toolVersion, sizeof(sz_toolVersion))) 
	{
		SetErrorMessage(_T("Get Tool_Version Fail"), 0);
		return false;
	}
	else
	{
		SetErrorMessage(_T("Get Tool_Version PASS"), 0);
	}

	m_str_ToolVersion.Format("%s", sz_toolVersion);

	return true;
}

bool CDLInstance::GetCheckFlowInsertData(void)
{
	if (m_str_iniFileName == "")
	{
		return false;
	}

/*	bool                         m_b_checkFlow;     
	bool                         m_b_insertData;    */  
	CIniAccess obj_dllIni("Setting", m_str_iniFileName);
	// 0 do nothing , 1 check by picasso, 2 check by dell id
	i_InsertData = obj_dllIni.GetValue("insertdata", 0); 

	i_checkflow = obj_dllIni.GetValue("checkflow", 0);
	//m_b_checkFlow = (i_checkflow == 1) ? true : false;

	i_checkSWVersion = obj_dllIni.GetValue("checkSWVersion", 0);
	//m_b_checkSWVersion = (i_checkSWVersion == 1) ? true : false;

//	int m_b_checkSWVersion;
	char sz_prestation[128] = {0};
	obj_dllIni.GetValue(_T("prestation"), sz_prestation, sizeof(sz_prestation));
	str_prestation.Format(_T("%s"), sz_prestation);

	if (str_prestation == _T(""))
	{
		SetErrorMessage("Get str_prestation Fail from QISDA.", 0);
		return false;
	}
	checkStation = str_prestation;

	char sz_SWVersion[128] = {0};
	obj_dllIni.GetValue(_T("SWVersion"), sz_SWVersion, sizeof(sz_SWVersion));
	str_SWVersion.Format(_T("%s"), sz_SWVersion);

	if (str_prestation == _T(""))
	{
		SetErrorMessage("Get str_prestation Fail from QISDA.", 0);
		return false;
	}

	

	//m_str_DLMode = str_DLMode;

	return true;


}



bool CDLInstance::SetImagePath(void)
{
		/* Save image file */
		st_Image st_image;
		st_image.str_imageType = "";
		st_image.str_imageName = "";
		st_image.str_imagePath = m_str_imageFilePath;
		m_vector_image.push_back(st_image);
		return true;
}
/*****************************************************************************
* Function name: GetImageCount      
* Summary      : Get image count and image file name.
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
//bool CDLInstance::GetImageCount(void)
//{
//		/* Save image file */
//		st_Image st_image;
//		st_image.str_imageType = "";
//		st_image.str_imageName = "";
//		st_image.str_imagePath = m_str_imageFilePath;
//		m_vector_image.push_back(st_image);
//		return true;
//	/* Check member */
//	if (m_str_DLMode == "")
//	{
//		return false;
//	}
//	if (m_str_iniFileName == "")
//	{
//		return false;
//	}
//	if (m_str_imageFilePath == "")
//	{
//		return false;
//	}
//
//	m_i_imageCount = 0;
//
//	char sz_key[ARRAY_SIZE]    = {0};
//	char sz_type[ARRAY_SIZE]   = {0};
//	char sz_filename[MAX_PATH] = {0};
//
//	/* Get image count */
//	CIniAccess obj_countIni(m_sz_DLMode, m_str_iniFileName);
//	int i_count = 0;
//	i_count = obj_countIni.GetValue(_T("Count"));
//	if (i_count == 0)
//	{
//		return false;
//	}
//
//	/* Get image name */
//	for (int i = 0; i < i_count; i++)
//	{
//		/* Init */
//		memset(sz_key, 0, sizeof(sz_key));
//		memset(sz_type, 0, sizeof(sz_type));
//		memset(sz_filename, 0, sizeof(sz_filename));
//
//		/* Get image type */
//		CIniAccess obj_typeIni(m_sz_DLMode, m_str_iniFileName);
//		sprintf_s(sz_key, "%02d", i+1);
//		obj_typeIni.GetValue(sz_key, sz_type, sizeof(sz_type));
//	
//		/* Need download */
//		sprintf_s(sz_key, "Download_%02d", i+1);
//		if (obj_typeIni.GetValue(sz_key) == 0)
//		{
//			continue;
//		}
//
//		/* Get File Name */
//		CIniAccess obj_imageIni (_T("ImageFile"), m_str_iniFileName);
//		sprintf_s(sz_key, "%02d", i+1);
//		obj_imageIni.GetValue(sz_key, sz_filename);
//	
//		/* Save image file */
//		st_Image st_image;
//		st_image.str_imageType = sz_type;
//		st_image.str_imageName = sz_filename;
//		st_image.str_imagePath = m_str_imageFilePath;
//		m_vector_image.push_back(st_image);
//
//		if (m_str_imageFilePath == "")
//		{
//			//return false;
//		}
//	}
//
//	m_i_imageCount = m_vector_image.size();
//
//	return true;
//}


/*****************************************************************************
* Function name: SetErrorCode      
* Summary      : SetErrorCode to UI.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDLInstance::SetErrorCode(const char* sz_errorCode, const int i_slot)
{	
	/* Check input */
	if ((sz_errorCode == NULL) || (i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return;
	}
	
	/* UI_RESULT Event to UI */
	st_UIResult p_st_result;
	p_st_result.i_slot = i_slot;
	strcpy_s(p_st_result.sz_errCode, sz_errorCode);
	Fire(UI_RESULT, (long)&p_st_result);

	return;
}


/*****************************************************************************
* Function name: SetErrorMessage      
* Summary      : SetErrorMessage to UI.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDLInstance::SetErrorMessage(const char* sz_errorMessage, const int i_slot)
{
	/* Check input */
	if ((sz_errorMessage == NULL) || (i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return;
	}

	/* UI_MESSAGE Event to UI */
	st_UIMessage p_st_message;
	p_st_message.i_slot = i_slot;
	sprintf_s(p_st_message.sz_message, _T("%s"), sz_errorMessage);
	Fire(UI_MESSAGE, (long)&p_st_message);

	/* Write Log */
	CString str_message;
	str_message.Format(_T("[%d] %s"), p_st_message.i_slot, p_st_message.sz_message);
	WriteLog(str_message);

	return;
}


/*****************************************************************************
* Function name: IsConnected      
* Summary      : Check COMPort if exist or not.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::IsConnected(const int i_slot)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		SetErrorCode(CommErr_No_Phone_Connected, i_slot);
		SetErrorMessage(_T("IsConnected --- Slot is not in the range."), i_slot);
		return false;
	}

	/* Check COM port valid */
	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	if (0 == i_comPort)
	{
		SetErrorCode(CommErr_No_Phone_Connected, i_slot);
		SetErrorMessage(_T("IsConnected --- COM port is 0."), i_slot);
		return false;
	}
	
	/* Check COM Port */
	char sz_comPort[MAX_PATH] = {0};
	if (i_comPort < 10)
	{
		sprintf_s(sz_comPort, "COM%d", i_comPort);            // For COM port is < 10
	}
	else if (i_comPort >= 10)
	{
		sprintf_s(sz_comPort, "\\\\.\\COM%d", i_comPort);      // For COM port is >= 10
	}

	/* Timeout 10s */
	int i_retryCount = 10;
	for (int i = 0; i <= i_retryCount; i++)
	{
		HANDLE h_handle = ::CreateFile(sz_comPort, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if (INVALID_HANDLE_VALUE == h_handle)
		{
			if (i == i_retryCount - 1)
			{
				SetErrorCode(CommErr_No_Phone_Connected, i_slot);
				CString str_message;
				str_message.Format(_T("IsConnected --- Open COM Fail,ErrorCode=%d"), GetLastError());
				SetErrorMessage(str_message, i_slot);
				return false;
			}
			else
			{
				Sleep(1000);
				continue;
			}
		} 
		else
		{
			::CloseHandle(h_handle);
			break;
		}
	}

	return true;
}


/*****************************************************************************
* Function name: IsCOMPortExist      
* Summary      : Check COMPort if exist or not.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
bool CDLInstance::IsCOMPortExist(const int i_slot)
{
	/* Check input */
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		//SetErrorCode(DLERR_NO_HANDSET, i_slot);
		SetErrorMessage(_T("IsCOMPortExist --- Slot is not in the range."), i_slot);
		return false;
	}

	/* Check COM port valid */
	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	if (0 == i_comPort)
	{
		//SetErrorCode(DLERR_NO_HANDSET, i_slot);
		SetErrorMessage(_T("IsCOMPortExist --- COM port is 0."), i_slot);
		return false;
	}

	/* Check COM Port exist */
	CString str_COMPort;
	str_COMPort.Format(_T("COM%d"), i_comPort);

	HKEY hKey = NULL;
	long ret = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_QUERY_VALUE, &hKey));
	if(ret != ERROR_SUCCESS)
	{
		//SetErrorCode(DLERR_NO_HANDSET, i_slot);
		SetErrorMessage(_T("IsCOMPortExist --- RegOpenKeyEx Fail."), i_slot);
		return false;
	}

	DWORD retCode = ERROR_SUCCESS;
	DWORD dwIndex = 0;
	char szValueName[128] = {0};
	unsigned char szValueData[128] = {0};
	ULONG ulValueNameLength = sizeof(szValueName);
	ULONG ulValueDataLength = sizeof(szValueData);
	bool b_checkRes = false;

	Sleep(3000);
	for (int i = 0; i < 15; i++)
	{
		retCode = ERROR_SUCCESS;
		while ((retCode == ERROR_SUCCESS))
		{
			memset(szValueName, 0, sizeof(szValueName));
			memset(szValueData, 0, sizeof(szValueData));
			ulValueNameLength = sizeof(szValueName);
			ulValueDataLength = sizeof(szValueData);

			retCode = RegEnumValue(hKey, dwIndex++, szValueName, &ulValueNameLength, NULL, NULL, szValueData, &ulValueDataLength);
			if((retCode == ERROR_SUCCESS))
			{
				CString str_portName;
				str_portName.Format(_T("%s"), szValueData);
				//SetErrorMessage(str_portName, i_slot);
				if (str_portName == str_COMPort)
				{
					b_checkRes = true;
					break;
				}
				else
				{
					Sleep(1000);
					b_checkRes = false;
				}
			}
		}

		if (b_checkRes != true)
		{
			Sleep(1000);
			continue;
		}
		else
		{
			break;
		}
	}
	RegCloseKey(hKey);

	if (b_checkRes == false)
	{
		//SetErrorCode(DLERR_NO_HANDSET, i_slot);
		SetErrorMessage(_T("IsCOMPortExist --- Check COM port exist Fail by Register."), i_slot);
		//return false;
	}


	return true;
}


/*****************************************************************************
* Function name: IsImageFileExist      
* Summary      : Check image file path if exist or not.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
//bool CDLInstance::IsImageFileExist(const int i_slot)
//{
//	/* Check image num */
//	if (m_vector_image.size() == 0)
//	{
//		SetErrorCode(DLErr_Image_Is_not_Exist, i_slot);
//		SetErrorMessage(_T("All image files is missing."), i_slot);
//		return false;
//	}
//
//	/* Check image exist */
//	std::vector<st_Image>::iterator iter;
//	for (iter = m_vector_image.begin(); iter != m_vector_image.end(); iter++)
//	{
//		CString str_imageFileName;
//		str_imageFileName.Format(_T("%s\\%s"), m_str_imageFilePath, (*iter).str_imageName.c_str());
//		if (_taccess(str_imageFileName, 0) != 0) 
//		{
//			SetErrorCode(DLErr_Image_Is_not_Exist, i_slot);
//			CString str_errorMessage;
//			str_errorMessage.Format(_T("Image file %s is missing."), (*iter).str_imageName.c_str());
//			SetErrorMessage(str_errorMessage, i_slot);
//			return false;
//		}
//	}
//
//	return true;
//}


/*****************************************************************************
* Function name: SetLogFileName      
* Summary      : Set log file name.
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CDLInstance::SetLogFilePath()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	CString currDate;
	currDate.Format(_T("%04d%02d%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	CString logPath;
	logPath.Format(_T("D:\\LOG\\DLL\\%s\\%s"), csStation, currDate);
	obj_gLog.InitializeGLog(logPath);
}

void CDLInstance::SetLogFileName(void)
{
	CString str_fileName = _T("");
	for (unsigned int i = 0; i < m_map_picassoList.size(); i++)
	{
		if (m_map_picassoList[i] != _T(""))
		{
			str_fileName += m_map_picassoList[i] + _T("&");
		}
	}

	try 
	{
		if (str_fileName != _T(""))
		{
			str_fileName = str_fileName.Left(str_fileName.GetLength() - 1);
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}

	if (str_fileName == _T(""))
	{
		/* Set default log file name */
		str_fileName = _T("DL");
	}
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
    CString currTime;
	currTime.Format(_T("%04d%02d%02d_%02d%02d_%02d"),
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	CString filename;
	filename.Format(_T("%s_%s.log"), str_fileName, currTime);

	//str_fileName += _T(".log");
	SetInfoLogFileName(filename);
	m_str_logFileName = str_fileName;

	return;
}

void CDLInstance::SetInfoLogFileName(CString str_fileName)
{
	obj_gLog.SetGLogFileName_Info(str_fileName);

	return;
}

void CDLInstance::WriteLog(CString str_log, const int i_severity)
{
	if (m_str_logFileName == _T(""))
	{	
		SYSTEMTIME st_systemTime;
		CString str_currentTime;
		GetLocalTime(&st_systemTime);
		str_currentTime.Format(_T("%4d%02d%02d%02d%02d"), st_systemTime.wYear, st_systemTime.wMonth, st_systemTime.wDay, st_systemTime.wHour, st_systemTime.wMinute);
		CString str_logFileName;
		str_logFileName.Format("DL_%s.log", str_currentTime);
		SetInfoLogFileName(str_logFileName);
		m_str_logFileName = str_logFileName;
	}

	obj_gLog.Log(i_severity, str_log);

	return;
}

/* 
//Only For Orange project
bool CDLInstance::ForceToEnterDLMode(const int i_slot)
{
	// Check input
	if ((i_slot < 0) || (i_slot >= MAX_SLOT))
	{
		return false;
	}

	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	COrange* p_obj_DL = NULL;
	p_obj_DL = new COrange(i_comPort);
	if (p_obj_DL == NULL)
	{
		return false;
	}

	p_obj_DL->Register (this, UI_MESSAGE);
	p_obj_DL->Register(this, UI_PROGRESS);
	p_obj_DL->Register(this, UI_RESULT);

	p_obj_DL->SetSlotNumber(i_slot);
	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);

	bool b_res = false;
	b_res = p_obj_DL->ForceToEnterDLMode();

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}*/
bool CDLInstance::CheckMD5(void)
{
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);
	
	CString str_MD5LocalPath;
	str_MD5LocalPath.Format("%s\\MD5\\%s_%s_TestItem_MD5.xml", sz_currentPath, m_str_projectName, csStation);
	if (_taccess(str_MD5LocalPath, 0) != 0) 
	{
		SetErrorMessage(_T("\\MD5\\s_TestItem_MD5.xml is not exist 0."), 1);

		str_MD5LocalPath.Format("%s\\MD5\\DL_MD5.xml", sz_currentPath);

		if (_taccess(str_MD5LocalPath, 0) != 0) 
		{
			SetErrorMessage(_T("\\MD5\\DL_MD5.xml is not exist 1."), 1);
			return false;
		}
	}


	/* Replace xml path */
	if (ConvertXML(str_MD5LocalPath, m_str_iniFileName) != true)
	{
		SetErrorMessage(_T("ConvertXML Fail."), 1);
		return false;
	}

	CString str_logName;
	str_logName = "C:\\MD5.log";
	if (MD5Verify(m_str_iniFileName, str_MD5LocalPath, str_logName) != true)
	{
		SetErrorMessage(_T("MD5 verify Fail."), 1);
		return false;
	}

	if (CheckVerifyResult(str_logName) != true)
	{
		SetErrorMessage(_T("Check MD5 verify result Fail."), 1);
		return false;
	}

	return true;
}

bool CDLInstance::ConvertXML(CString str_XMLFilePathName, CString str_newName)
{
	/* Check input */
	if(str_XMLFilePathName == _T("") || str_newName == _T(""))
	{
		return false;
	}
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument26));
	if(FAILED(hr)) { 
		return FALSE;
	}

	if(_taccess(str_XMLFilePathName, 0) != 0)
	{
		return false;
	}
	pDoc->load((LPCTSTR)str_XMLFilePathName);
	if(pDoc == NULL)
	{
		return false;
	}

	MSXML2::IXMLDOMNodePtr pNode;
	pNode = pDoc->selectSingleNode("//FCIV");
	if(pNode == NULL) {
		return false;
	}
	MSXML2::IXMLDOMNodePtr pItem;
	pItem = pNode->GetfirstChild();
	while(NULL != pItem)
	{
		MSXML2::IXMLDOMNodePtr pNode;
		pNode = pItem->selectSingleNode("name");
		if(pNode == NULL)
		{
			return false;
		}
		CString str_nameValue;
		str_nameValue = (char *)pNode->text;
		str_nameValue.Replace(str_nameValue, str_newName);
		pNode->Puttext((_bstr_t)(const char *)str_nameValue);

		pItem = pItem->GetnextSibling();
	}

	pDoc->save((LPCTSTR)str_XMLFilePathName);

	return true;
}

bool CDLInstance::MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile)
{
	/* Check Input */
	if(str_dirPath == _T("") || str_XMLFilePathName == _T("")  || str_logFile == _T("")) 
	{
		return false;
	}

	/* Check fciv.exe exist */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);
	CString str_fcivPathName;
	str_fcivPathName.Format(_T("%s\\MD5\\fciv.exe"), sz_currentPath);
	if (_taccess(str_fcivPathName, 0) != 0) 
	{
		return false;
	}

	/* Command */
	CString str_command;
	CString str_commandOne;
	CString str_commandTwo;
	CString str_commandThree;
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePathName;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_command = str_commandOne + str_commandTwo + str_commandThree;

	/* Run */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_command.GetLength(); i ++) 
	{
		sz_commandLine[i] = str_command[i];
	}
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) 
	{
		return false;
	}
	DWORD dw_waitResult = 0;
	DWORD dw_timeout;
	dw_timeout = 10000;
	dw_waitResult = ::WaitForSingleObject(pi.hProcess, dw_timeout);
	switch(dw_waitResult)
	{
	case WAIT_FAILED:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_TIMEOUT:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_OBJECT_0:
		break;
	}
	::CloseHandle(pi.hProcess);
	pi.hProcess = NULL;

	return true;
}

bool CDLInstance::CheckVerifyResult(CString str_logFile)
{
	/* Check Input */
	if(str_logFile == _T("")) 
	{
		return false;
	}

	/* Check file exist */
	int i_existentResult = 0;
	int i_retry = 10;
	while(1)
	{
		i_existentResult = _taccess(str_logFile, 0);
		if(i_existentResult != 0)
		{
			if(i_retry <= 0)
			{
				break;
			} 
			else 
			{
				Sleep(1000);
				i_retry--;
				continue;
			}
		} 
		else 
		{
			break;
		}
	}
	if(i_existentResult != 0) 
	{
		return false;
	}

	/* Check file Size */
	DWORD dw_fileSize = 0;
	i_retry = 10;
	while(1) 
	{
		HANDLE h_handle = NULL;
		h_handle = CreateFile(str_logFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(h_handle != INVALID_HANDLE_VALUE)
		{
			dw_fileSize = GetFileSize(h_handle, NULL);
			CloseHandle(h_handle);
			if(dw_fileSize == 0)
			{
				if(i_retry <= 0) 
				{
					break;
				} 
				else 
				{
					Sleep(1000);
					i_retry--;
					continue;
				}
			} 
			else 
			{
				Sleep(1000);
				break;
			}
		} 
		else {
			break;
		}
	}
	if(dw_fileSize == 0)
	{
		return false;
	}

	/* Read */
	int i_succeedFlag = 0;
	CStdioFile obj_stdioFile;
	if (!obj_stdioFile.Open(str_logFile, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL))
	{
		return false;
	} 
	try 
	{
		CString str_string;
		while(obj_stdioFile.ReadString(str_string)) 
		{
			if(str_string.Find(_T("All files verified successfully")) != -1) 
			{
				i_succeedFlag = 1;
				break;
			}
			str_string = _T("");
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
	}

	obj_stdioFile.Close();

	if (i_succeedFlag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

///* Alex.Chen Add for C7 */
//bool CDLInstance::GetImage(void)
//{
//	/* Check member */
//	if (m_str_DLMode == "")
//	{
//		return false;
//	}
//	if (m_str_iniFileName == "")
//	{
//		return false;
//	}
//	if (m_str_imageFilePath == "")
//	{
//		return false;
//	}
//
//	m_i_imageCount = 0;
//
//	char sz_key[ARRAY_SIZE]    = {0};
//	char sz_type[ARRAY_SIZE]   = {0};
//	char sz_filename[MAX_PATH] = {0};
//
//	/* Get image count */
//	CIniAccess obj_countIni(m_sz_DLMode, m_str_iniFileName);
//	int i_count = 0;
//	i_count = obj_countIni.GetValue(_T("Count"));
//	if (i_count == 0)
//	{
//		return false;
//	}
//
//	/* Get image name */
//	for (int i = 0; i < i_count; i++)
//	{
//		/* Init */
//		memset(sz_key, 0, sizeof(sz_key));
//		memset(sz_type, 0, sizeof(sz_type));
//		memset(sz_filename, 0, sizeof(sz_filename));
//
//		/* Get image type */
//		CIniAccess obj_typeIni(m_sz_DLMode, m_str_iniFileName);
//		sprintf_s(sz_key, "%02d", i+1);
//		obj_typeIni.GetValue(sz_key, sz_type, sizeof(sz_type));
//
//		/* Need download */
//		sprintf_s(sz_key, "Download_%02d", i+1);
//		if (obj_typeIni.GetValue(sz_key) == 0)
//		{
//			continue;
//		}
//
//		/* Get File Name */
//		CIniAccess obj_imageIni (_T("ImageFile"), m_str_iniFileName);
//		sprintf_s(sz_key, "%02d", i+1);
//		obj_imageIni.GetValue(sz_key, sz_filename);
//
//		CString str_imageType;
//		str_imageType.Format("%s", sz_type);
//		CString str_imageName;
//		str_imageName.Format("%s", sz_filename);
//		if (str_imageType == _T("flex"))
//		{
//			CStringArray stra_fileName;
//			if (GetFileNameFromDir(m_str_imageFilePath, stra_fileName) != true)
//			{
//				return false;
//			}
//			if (stra_fileName.GetSize() < 1)
//			{
//				return false;
//			}
//
//			CStringArray stra_spiltName;
//			if ((SpiltString(str_imageName, "*", stra_spiltName) != true))
//			{
//				return false;
//			}
//			if (stra_spiltName.GetSize() < 3)
//			{
//				return false;
//			}
//			
//			CString str_findOne = stra_spiltName.GetAt(0);
//			CString str_findTwo = stra_spiltName.GetAt(1);
//			CString str_findThree = _T(".bchecc.img");
//
//			CString str_temp;
//			bool b_findRes = false;
//			for (int i = 0; i < stra_fileName.GetSize(); i++)
//			{
//				str_temp = stra_fileName.GetAt(i);
//				if (str_temp.Find(str_findOne) != -1)
//				{
//					if (str_temp.Find(str_findTwo) != -1)
//					{
//						if (str_temp.Find(str_findThree) == -1)
//						{
//							b_findRes = true;
//							break;
//						}
//					}	
//				}
//			}
//			if (b_findRes == false)
//			{
//				return false;
//			}
//
//			/* Save image file */
//			st_Image st_image;
//			st_image.str_imageType = sz_type;
//			st_image.str_imageName = str_temp;
//			st_image.str_imagePath = m_str_imageFilePath;
//			m_vector_image.push_back(st_image);	
//		}
//		else
//		{
//			/* Save image file */
//			st_Image st_image;
//			st_image.str_imageType = sz_type;
//			st_image.str_imageName = sz_filename;
//			st_image.str_imagePath = m_str_imageFilePath;
//			m_vector_image.push_back(st_image);
//		}
//
//		if (m_str_imageFilePath == "")
//		{
//			return false;
//		}
//	}
//
//	m_i_imageCount = m_vector_image.size();
//
//	return true;
//}
//bool CDLInstance::GetFileNameFromDir(CString str_directoryName, CStringArray& stra_fileName)
//{
//	/* Check Input */
//	if(str_directoryName == _T(""))
//	{
//		return false;
//	}
//
//	stra_fileName.RemoveAll();
//
//	CFileFind obj_tempFind;               
//	char sz_tempFileFind[256];
//	memset(sz_tempFileFind, 0,sizeof(sz_tempFileFind));
//
//	sprintf_s(sz_tempFileFind , "%s\\*.img", str_directoryName);
//
//	BOOL b_isFinded = (BOOL)obj_tempFind.FindFile(sz_tempFileFind);
//	while(b_isFinded) 
//	{
//		b_isFinded = ( BOOL )obj_tempFind.FindNextFile();
//		if(!obj_tempFind.IsDots())
//		{
//			if(!obj_tempFind.IsDirectory())
//			{
//				/* It is not directory */
//				CString str_tempFileName;
//				str_tempFileName = obj_tempFind.GetFileName().GetBuffer(256);
//				stra_fileName.Add(str_tempFileName);
//			}
//		}
//	}
//
//	obj_tempFind.Close();
//
//	return true;
//}
bool CDLInstance::SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg)
{
	/* Check Input */
	if (str_sourceString == _T(""))
	{
		return false;
	}

	stra_strArg.RemoveAll();

	CString str_temp;
	int i_position = 0;
	int i_strLength = 0;

	/* Delimiter is space case */
	if (str_delimiter == _T(" "))
	{
		str_sourceString.TrimLeft();
		str_sourceString.TrimRight();
	}

	/* Find the first delimiter */
	i_position = str_sourceString.FindOneOf(str_delimiter);
	if (i_position == -1)
	{
		/* The source string did not have delimiter */
		stra_strArg.Add(str_sourceString);		
		return true;
	}

	while(str_sourceString != _T("")) 
	{
		/* Get the first of position of delimiter in source string */
		i_position = str_sourceString.FindOneOf(str_delimiter);
		i_strLength = str_sourceString.GetLength();
		if (i_position == -1 )
		{
			/* The source string did not have delimiter */
			stra_strArg.Add(str_sourceString);
			break ;
		}
		try
		{
			/* Get the first string */
			str_temp = str_sourceString.Mid(0, i_position);
		} 
		catch(CMemoryException e)
		{
			e.Delete();
			return false;
		}

		/* Delete right space of string */
		if (str_delimiter == _T(" "))
		{
			str_temp.TrimRight();
		}

		stra_strArg.Add(str_temp);

		try
		{
			/* Get the next string */
			str_sourceString = str_sourceString.Mid(i_position + 1);
		} 
		catch(CMemoryException e) 
		{
			e.Delete();
			return false;
		}

		if (str_delimiter == _T(" ")) 
		{
			/* delete left space of string */
			str_sourceString.TrimLeft();
		}

		/* The end string */
		if (i_position == i_strLength - 1)
		{
			stra_strArg.Add(str_sourceString);
		}
	}	

	return true;
}

bool CDLInstance ::runInsertData(int n_type)
{	
	bool bReturn = false;
	CString str_dllF32SERVER2 = F32SERVERDB;

	HMODULE hDll ;
	hDll = ::LoadLibrary(str_dllF32SERVER2);

	if( hDll != NULL )
	{	
		typedef bool (_stdcall *lpInsertYrstation)(const unsigned char* Model,     unsigned short ModelLen,
				   		        				   const unsigned char* Id,        unsigned short IdLen,
												   const unsigned char* Shift,     unsigned short ShiftLen,
												   const unsigned char* Station,   unsigned short StationLen,
												   const unsigned char* Trid,      unsigned short TridLen,
												   const unsigned char* Operator,  unsigned short OperatorLen,
												   unsigned short CycleTime,
												   const unsigned char* CheckInfo, unsigned short CheckInfoLen);
		lpInsertYrstation iInsertYrstation = (lpInsertYrstation)::GetProcAddress(hDll,"InsertYrstation");
		if( iInsertYrstation != NULL)
		{	
			unsigned char sz_ID[ID_SIZE_BUFFER] ="";
			unsigned char szStation[ID_SIZE_BUFFER] ="";
			unsigned char szLine[5]="B21";

			sprintf_s((char*)szStation, ID_SIZE_BUFFER, "%s", csStation);

			unsigned char szModel[ID_SIZE_BUFFER]="";
			sprintf_s((char*)szModel, ID_SIZE_BUFFER, "%s", m_str_modelName);
			unsigned char szTrid[5]="";
			unsigned char szOperator[5]="";
			unsigned char szCheckInfo[5]="";

			//char szTemp[ID_SIZE_BUFFER];
			//sprintf_s(szTemp,ID_SIZE_BUFFER, "F1008B28888");
			//sprintf_s(szTemp,10,"%s",m_pIPhone->m_szSN.c_str());

			unsigned char Station[ID_SIZE_BUFFER]  = {0};
			if ( n_type == 1 ){ //arm board id
				if ( std_Picasso.empty()){
					ErrMsg = "  picasso is empty = " ;
					ErrMsg =  ErrMsg + std_ScalarId.c_str() ;
					AfxMessageBox(ErrMsg.c_str());
					SetErrorMessage(ErrMsg.c_str() , 0);
					return false;
				}
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_Picasso.c_str() );

				ErrMsg = "  iInsertYrstation 1 = " ;
				ErrMsg =  ErrMsg + std_Picasso ;
				SetErrorMessage(ErrMsg.c_str() , 0);
			}
			else 	if ( n_type == 2 ){
				if ( std_ScalarId.empty()){
					ErrMsg = "  scalar board is empty = " ;
					ErrMsg =  ErrMsg + std_ScalarId.c_str() ;
					//SetErrorMessage(ErrMsg.c_str() , 0);
					AfxMessageBox(ErrMsg.c_str());
					SetErrorMessage(ErrMsg.c_str() , 0);
					return false;
				}
				sprintf_s((char*)sz_ID, ID_SIZE_BUFFER,"%s", std_ScalarId.c_str() );
				ErrMsg = " iInsertYrstation 2 = ";
				ErrMsg = ErrMsg 	+ std_ScalarId.c_str() ;
				SetErrorMessage(ErrMsg.c_str() , 0);
			}
			else
			{
				ErrMsg = "can't find id type ";
				//ErrMsg = " iInsertYrstation 2 = ";
				ErrMsg = ErrMsg 	+ std_ScalarId.c_str() ;
				SetErrorMessage(ErrMsg.c_str() , 0);
				AfxMessageBox(ErrMsg.c_str());
				return false;
			}

			CString csInsertCmd;
			csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
			ErrMsg = "iInsertYrstation cmd = " + csInsertCmd;
			SetErrorMessage(ErrMsg.c_str() , 0);

			//memcpy(sz_ID,szTemp,ID_SIZE_BUFFER);
			bReturn = iInsertYrstation(szModel,   (unsigned short)strlen((char*)szModel),
									   sz_ID,	(unsigned short)strlen((char*)sz_ID),
									   szLine,    (unsigned short)strlen((char*)szLine), 
									   szStation, (unsigned short)strlen((char*)szStation),
									   szTrid,     5,
									   szOperator,   5,
									   0,
									   szCheckInfo,  5);

			if(bReturn) 
			{
			//	CString csInsertCmd;
			//	csInsertCmd.Format(_T("szModel = %s, sz_ID = %s, szOperator = %s , szStation = %s "), szModel,  sz_ID, szOperator, szStation);
				ErrMsg = "iInsertYrstation ok";
				SetErrorMessage(ErrMsg.c_str() , 0);
				//SetErrorMessage(ErrMsg.c_str() , 0);
			}
			else
			{
				ErrMsg = "iInsertYrstation fail ";
				AfxMessageBox(ErrMsg.c_str());			
				SetErrorMessage(ErrMsg.c_str() , 0);

			}
			goto Exit_FreeLibrary;
		}
		else
		{	
			ErrMsg = "Load InsertYrstation fail ";
			AfxMessageBox(ErrMsg.c_str());				
			SetErrorMessage(ErrMsg.c_str() , 0);
			goto Exit_FreeLibrary;
		}
	}
	else
	{	
		ErrMsg = "Load str_dllF32SERVER2 fail ";
		AfxMessageBox(ErrMsg.c_str());				
		SetErrorMessage(ErrMsg.c_str() , 0);
		//SetErrorMessage(ErrMsg.c_str() , 0);
		goto Exit;
	}
Exit_FreeLibrary:
	FreeLibrary(hDll);
Exit:
	return bReturn;
}



int CDLInstance ::GetWeek	(	struct tm* date	)
{
	if (NULL == date)
	{
		return 0; // or -1 or throw exception
	}
	if (::mktime(date) < 0) // Make sure _USE_32BIT_TIME_T is NOT defined.
	{
		return 0; // or -1 or throw exception
	}
	// The basic calculation:
	// {Day of Year (1 to 366) + 10 - Day of Week (Mon = 1 to Sun = 7)} / 7
	int monToSun = (date->tm_wday == 0) ? 7 : date->tm_wday; // Adjust zero indexed week day
	int week = ((date->tm_yday + 11 - monToSun) / 7); // Add 11 because yday is 0 to 365.

	// Now deal with special cases:
	// A) If calculated week is zero, then it is part of the last week of the previous year.
	if (week == 0)
	{
		// We need to find out if there are 53 weeks in previous year.
		// Unfortunately to do so we have to call mktime again to get the information we require.
		// Here we can use a slight cheat - reuse this function!
		// (This won't end up in a loop, because there's no way week will be zero again with these values).
		tm lastDay = { 0 };
		lastDay.tm_mday = 31;
		lastDay.tm_mon = 11;
		lastDay.tm_year = date->tm_year - 1;
		// We set time to sometime during the day (midday seems to make sense)
		// so that we don't get problems with daylight saving time.
		lastDay.tm_hour = 12;
		week = GetWeek(&lastDay);
	}
	// B) If calculated week is 53, then we need to determine if there really are 53 weeks in current year
	//    or if this is actually week one of the next year.
	else if (week == 53)
	{
		// We need to find out if there really are 53 weeks in this year,
		// There must be 53 weeks in the year if:
		// a) it ends on Thurs (year also starts on Thurs, or Wed on leap year).
		// b) it ends on Friday and starts on Thurs (a leap year).
		// In order not to call mktime again, we can work this out from what we already know!
		int lastDay = date->tm_wday + 31 - date->tm_mday;
		if (lastDay == 5) // Last day of the year is Friday
		{
			// How many days in the year?
			int daysInYear = date->tm_yday + 32 - date->tm_mday; // add 32 because yday is 0 to 365
			if (daysInYear < 366)
			{
				// If 365 days in year, then the year started on Friday
				// so there are only 52 weeks, and this is week one of next year.
				week = 1;
			}
		}
		else if (lastDay != 4) // Last day is NOT Thursday
		{
			// This must be the first week of next year
			week = 1;
		}
		// Otherwise we really have 53 weeks!
	}
	return week;
}

int CDLInstance ::GetCurrentWeek()
{
	tm today;
	time_t now;
	time(&now);
	errno_t error = ::localtime_s(&today, &now);
	//int a = GetWeek(&today);
	return GetWeek(&today);
}