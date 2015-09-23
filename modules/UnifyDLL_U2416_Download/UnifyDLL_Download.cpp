
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
	m_i_imageCount = 0;
	m_vector_image.clear();
	m_map_picassoList.clear();
	m_map_deviceStruct.clear();
	m_map_deviceStructMultiDLFlag.clear();
	m_map_COMPicasso.clear();
	memset(m_sz_DLMode, 0, sizeof(m_sz_DLMode));

	/* Init google log */
	//obj_gLog.InitializeGLog(LOG_PATH);
	obj_gLog.InitializeGLog("");

	if(::CoInitialize(NULL) != S_OK)
	{
		::CoUninitialize();
		Sleep(200);
		::CoInitialize(NULL);
	}
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

	/* Check image file exist */
	if (IsImageFileExist(i_slot) != true)
	{
		m_map_picassoList.erase(i_slot);
		SetErrorMessage(_T("Check image file exist Fail."), i_slot);
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
		if ((str_DLMode == DOWNLOAD_MODE_REDL) || ( str_DLMode == DOWNLOAD_MODE_REDL_OSDL ))
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

	/* Set COMPicasso */
	p_obj_DL->SetCOMPicasso(m_map_COMPicasso);

	/* Set SlotPicasso */
	p_obj_DL->SetPicassoList(m_map_picassoList);

	/* Set DLMode */
	p_obj_DL->SetDLMode(m_sz_DLMode);

	/* Set SW version */
	p_obj_DL->SetSWVerion(m_str_SWVersion);

	/* Set HW version */
	p_obj_DL->SetHWVerion(m_str_HWVersion);

	/* Set Tool version */
	p_obj_DL->SetToolVerion(m_str_ToolVersion);

	/* Set Factory version */
	p_obj_DL->SetFactoryVerion(m_str_fatoryVersion);

	/* Set Customer SW version */
	p_obj_DL->SetCustomerSwVerion(m_str_CustomerSwVersion);

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

	return true;
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
	bool b_res = false;
	CBaseProject* p_obj_DL = NULL;
	/* Get COMPort */
	int i_comPort = m_map_deviceStruct[i_slot].i_COMPort;
	/* Get Multi-download flag */
	CString str_multiDLFlag = m_map_deviceStructMultiDLFlag[i_slot].str_multiDLFlag;
	p_obj_DL = new CU2416(i_comPort, str_multiDLFlag);

	p_obj_DL->SetDLLIniFileName(m_str_iniFileName);
	b_res = p_obj_DL->PostRun(i_slot);

	if ( !b_res){
		/*can't reboot save err log*/
	}

	char szModulePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	PathRemoveFileSpec(szModulePath);
	char Path_UpLog_Bat[100];
	sprintf(Path_UpLog_Bat, _T("%s\\Qisda\\UpLog.bat"), szModulePath);

	char str_modelName[32] = {0};
	strcpy(str_modelName, CT2CA(m_str_modelName.GetBuffer()));

	if (::_taccess( Path_UpLog_Bat, 0 ) == 0){
		if (p_obj_DL->LogUploadByBAT(str_modelName) )
			SetErrorMessage(_T("LogUpload by bat Success."), 0);
		else
			SetErrorMessage(_T("LogUpload Fail."), 0);
	}
	else
	{ 
		if ( p_obj_DL->LogUpload(str_modelName))
			SetErrorMessage(_T("LogUpload Success."), 0);
		else
			SetErrorMessage(_T("LogUpload Fail."), 0);
	}


	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
}

bool CDLInstance::Begin(int i_slot)
{
	/* Read INI file */
	m_b_readIniFile = ReadDLLIniFile();
	if (m_b_readIniFile != true)
	{
		SetErrorMessage(_T("Read ini file Fail."), 0);
		return false;
	}

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
		if ((m_str_DLMode != DOWNLOAD_MODE_PREDL) && (m_str_DLMode != DOWNLOAD_MODE_REDL) && (m_str_DLMode != DOWNLOAD_MODE_REDL_OSDL))
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
	b_res = p_obj_DL->ReadFASector(i_sectorNum, sz_sectorData, i_sectorSize);

	p_obj_DL->Unregister (this, UI_MESSAGE);
	p_obj_DL->Unregister(this, UI_PROGRESS);
	p_obj_DL->Unregister(this, UI_RESULT);

	delete p_obj_DL;
	p_obj_DL = NULL;

	return b_res;
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

	GetSupportQDownload();
	if(m_b_SupportQDownload)
	{
		/* Get COM Port */
		if (GetCOMPort() != true)
		{
			return false;
		}
	}
	else
	{
		if (ReadSetupIniFile() != true)
		{
			return false;
		}
	}

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

	/* Get image file name */
	if (GetImageCount() != true)
	{
		return false;
	}



	/* Get tool version */
	if (GetToolVersion() != true)
	{
		return false;
	}

	if (Get_PID_VID() != true)
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

	if (m_str_DLMode.Compare(_T("PreDL")) == 0 ){
		csStation.Format(_T("Trigger"));
	}
	else if (m_str_DLMode.Compare(_T("ReDL")) == 0 )
	{
		csStation.Format(_T("OS_DL"));
	}


	str_iniFile.Format(_T("%s\\QISDA\\%s_%s_TestItem.xml"), sz_currentPath, m_str_projectName, csStation );
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
bool CDLInstance::GetSupportQDownload(void)
{
	if (m_str_iniFileName == "")
	{
		return false;
	}

	CIniAccess obj_dllIni("Setting", m_str_iniFileName);
	int i_SupportQDownload = obj_dllIni.GetValue("SupportQDownload", -1);

	m_b_SupportQDownload = (i_SupportQDownload == 1) ? true : false;
	return true;
}
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
bool CDLInstance::GetImageCount(void)
{
	/* Check member */
	if (m_str_DLMode == "")
	{
		return false;
	}
	if (m_str_iniFileName == "")
	{
		return false;
	}
	if (m_str_imageFilePath == "")
	{
		return false;
	}

	m_i_imageCount = 0;

	char sz_key[ARRAY_SIZE]    = {0};
	char sz_type[ARRAY_SIZE]   = {0};
	char sz_filename[MAX_PATH] = {0};

	/* Get image count */
	CIniAccess obj_countIni(m_sz_DLMode, m_str_iniFileName);
	int i_count = 0;
	i_count = obj_countIni.GetValue(_T("Count"));
	if (i_count == 0)
	{
		return false;
	}

	/* Get image name */
	for (int i = 0; i < i_count; i++)
	{
		/* Init */
		memset(sz_key, 0, sizeof(sz_key));
		memset(sz_type, 0, sizeof(sz_type));
		memset(sz_filename, 0, sizeof(sz_filename));

		/* Get image type */
		CIniAccess obj_typeIni(m_sz_DLMode, m_str_iniFileName);
		sprintf_s(sz_key, "%02d", i+1);
		obj_typeIni.GetValue(sz_key, sz_type, sizeof(sz_type));
	
		/* Need download */
		sprintf_s(sz_key, "Download_%02d", i+1);
		if (obj_typeIni.GetValue(sz_key) == 0)
		{
			continue;
		}

		/* Get File Name */
		CIniAccess obj_imageIni (_T("ImageFile"), m_str_iniFileName);
		sprintf_s(sz_key, "%02d", i+1);
		obj_imageIni.GetValue(sz_key, sz_filename);
	
		/* Save image file */
		st_Image st_image;
		st_image.str_imageType = sz_type;
		st_image.str_imageName = sz_filename;
		st_image.str_imagePath = m_str_imageFilePath;
		m_vector_image.push_back(st_image);

		if (m_str_imageFilePath == "")
		{
			return false;
		}
	}

	m_i_imageCount = m_vector_image.size();

	return true;
}


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
bool CDLInstance::IsImageFileExist(const int i_slot)
{
	/* Check image num */
	if (m_vector_image.size() == 0)
	{
		SetErrorCode(DLErr_Image_Is_not_Exist, i_slot);
		SetErrorMessage(_T("All image files is missing."), i_slot);
		return false;
	}

	/* Check image exist */
	std::vector<st_Image>::iterator iter;
	for (iter = m_vector_image.begin(); iter != m_vector_image.end(); iter++)
	{
		CString str_imageFileName;
		str_imageFileName.Format(_T("%s\\%s"), m_str_imageFilePath, (*iter).str_imageName.c_str());
		if (_taccess(str_imageFileName, 0) != 0) 
		{
			SetErrorCode(DLErr_Image_Is_not_Exist, i_slot);
			CString str_errorMessage;
			str_errorMessage.Format(_T("Image file %s is missing."), (*iter).str_imageName.c_str());
			SetErrorMessage(str_errorMessage, i_slot);
			return false;
		}
	}

	return true;
}


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
	str_fileName += _T(".log");
	SetInfoLogFileName(str_fileName);
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

/* Alex.Chen Add for C7 */
bool CDLInstance::GetImage(void)
{
	/* Check member */
	if (m_str_DLMode == "")
	{
		return false;
	}
	if (m_str_iniFileName == "")
	{
		return false;
	}
	if (m_str_imageFilePath == "")
	{
		return false;
	}

	m_i_imageCount = 0;

	char sz_key[ARRAY_SIZE]    = {0};
	char sz_type[ARRAY_SIZE]   = {0};
	char sz_filename[MAX_PATH] = {0};

	/* Get image count */
	CIniAccess obj_countIni(m_sz_DLMode, m_str_iniFileName);
	int i_count = 0;
	i_count = obj_countIni.GetValue(_T("Count"));
	if (i_count == 0)
	{
		return false;
	}

	/* Get image name */
	for (int i = 0; i < i_count; i++)
	{
		/* Init */
		memset(sz_key, 0, sizeof(sz_key));
		memset(sz_type, 0, sizeof(sz_type));
		memset(sz_filename, 0, sizeof(sz_filename));

		/* Get image type */
		CIniAccess obj_typeIni(m_sz_DLMode, m_str_iniFileName);
		sprintf_s(sz_key, "%02d", i+1);
		obj_typeIni.GetValue(sz_key, sz_type, sizeof(sz_type));

		/* Need download */
		sprintf_s(sz_key, "Download_%02d", i+1);
		if (obj_typeIni.GetValue(sz_key) == 0)
		{
			continue;
		}

		/* Get File Name */
		CIniAccess obj_imageIni (_T("ImageFile"), m_str_iniFileName);
		sprintf_s(sz_key, "%02d", i+1);
		obj_imageIni.GetValue(sz_key, sz_filename);

		CString str_imageType;
		str_imageType.Format("%s", sz_type);
		CString str_imageName;
		str_imageName.Format("%s", sz_filename);
		if (str_imageType == _T("flex"))
		{
			CStringArray stra_fileName;
			if (GetFileNameFromDir(m_str_imageFilePath, stra_fileName) != true)
			{
				return false;
			}
			if (stra_fileName.GetSize() < 1)
			{
				return false;
			}

			CStringArray stra_spiltName;
			if ((SpiltString(str_imageName, "*", stra_spiltName) != true))
			{
				return false;
			}
			if (stra_spiltName.GetSize() < 3)
			{
				return false;
			}
			
			CString str_findOne = stra_spiltName.GetAt(0);
			CString str_findTwo = stra_spiltName.GetAt(1);
			CString str_findThree = _T(".bchecc.img");

			CString str_temp;
			bool b_findRes = false;
			for (int i = 0; i < stra_fileName.GetSize(); i++)
			{
				str_temp = stra_fileName.GetAt(i);
				if (str_temp.Find(str_findOne) != -1)
				{
					if (str_temp.Find(str_findTwo) != -1)
					{
						if (str_temp.Find(str_findThree) == -1)
						{
							b_findRes = true;
							break;
						}
					}	
				}
			}
			if (b_findRes == false)
			{
				return false;
			}

			/* Save image file */
			st_Image st_image;
			st_image.str_imageType = sz_type;
			st_image.str_imageName = str_temp;
			st_image.str_imagePath = m_str_imageFilePath;
			m_vector_image.push_back(st_image);	
		}
		else
		{
			/* Save image file */
			st_Image st_image;
			st_image.str_imageType = sz_type;
			st_image.str_imageName = sz_filename;
			st_image.str_imagePath = m_str_imageFilePath;
			m_vector_image.push_back(st_image);
		}

		if (m_str_imageFilePath == "")
		{
			return false;
		}
	}

	m_i_imageCount = m_vector_image.size();

	return true;
}
bool CDLInstance::GetFileNameFromDir(CString str_directoryName, CStringArray& stra_fileName)
{
	/* Check Input */
	if(str_directoryName == _T(""))
	{
		return false;
	}

	stra_fileName.RemoveAll();

	CFileFind obj_tempFind;               
	char sz_tempFileFind[256];
	memset(sz_tempFileFind, 0,sizeof(sz_tempFileFind));

	sprintf_s(sz_tempFileFind , "%s\\*.img", str_directoryName);

	BOOL b_isFinded = (BOOL)obj_tempFind.FindFile(sz_tempFileFind);
	while(b_isFinded) 
	{
		b_isFinded = ( BOOL )obj_tempFind.FindNextFile();
		if(!obj_tempFind.IsDots())
		{
			if(!obj_tempFind.IsDirectory())
			{
				/* It is not directory */
				CString str_tempFileName;
				str_tempFileName = obj_tempFind.GetFileName().GetBuffer(256);
				stra_fileName.Add(str_tempFileName);
			}
		}
	}

	obj_tempFind.Close();

	return true;
}
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

//
//CDLCommand::CDLCommand(const TSTRING& strDevice, const std::string& strPlatform)
//:CInterfaceBase(strDevice, strPlatform)
//,m_nProgress(0)
//,CLog(strDevice)
//,CLastError(strDevice)
//{
//	DefineNotify(EVENT_DL_PROGRESS);
//    DefineNotify(EVENT_DL_PARTITION_DIF);
//}
//
//CDLCommand::CDLCommand(int nCOMPort, const std::string& strPlatform)
//:CInterfaceBase(COMDeviceName(nCOMPort), strPlatform)
//,m_nProgress(0)
//,CLog(COMDeviceName(nCOMPort))
//,CLastError(COMDeviceName(nCOMPort))
//{
//	DefineNotify(EVENT_DL_PROGRESS);
//    DefineNotify(EVENT_DL_PARTITION_DIF);
//}
//
//CDLCommand::~CDLCommand(void)
//{}


//void CDLCommand::AddMsg(const char* szMsg, int nStep, int nProgress)
//{
//    if(!(m_nProgress == nProgress && m_strMsg == szMsg))
//	{
//		m_nProgress = nProgress;
//		m_strMsg = szMsg;
//		DL_PROGRESS Msg = {(char*)szMsg, (DownloadStep)nStep, nProgress};
//		Fire(EVENT_DL_PROGRESS, (long)&Msg);
//		TRACE(szMsg);
//	}
//}