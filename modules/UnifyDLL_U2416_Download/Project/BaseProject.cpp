
#include "stdafx.h"
#include "BaseProject.h"


CBaseProject::CBaseProject()
{
	DefineNotify(UI_MESSAGE);
	DefineNotify(UI_PROGRESS);
	DefineNotify(UI_RESULT);
	DefineNotify(UI_PICSDATA);
	DefineNotify(UI_SHOWDIALOG);

	/* Init */
	m_i_slot = 0;
	memset(m_sz_errorCode, 0, sizeof(m_sz_errorCode));

	memset(m_sz_iniFile, 0, sizeof(m_sz_iniFile));

	/* Get current path*/
	GetCurrentPath();
}

CBaseProject::~CBaseProject()
{

}

bool CBaseProject::Register(INotify_ANSI* pNotify, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pNotify, strEvent);
}

bool CBaseProject::Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}

bool CBaseProject::Unregister(INotify_ANSI* pNotify, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}

bool CBaseProject::Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent)
{
	return CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}

void CBaseProject::DefineNotify(const std::string& strNewEvent)
{
	CNotifyBase_ANSI::DefineNotify(strNewEvent);
}

void CBaseProject::Fire(const std::string& strEvent, long nParam)
{
	CNotifyBase_ANSI::Fire(strEvent, nParam);
}

void CBaseProject::Event(const std::string& strEvent,long nParam)
{
}

bool CBaseProject::CheckBreak ()
{
	return false;
}

bool CBaseProject::WaitInput (const TSTRING& strName, TCHAR* szBuffer, int nBufferSize)
{
	if (strName == "COMPort")
	{
		sprintf (szBuffer, "%d", GetCOMport());
	}

	return true;
}

bool CBaseProject::Print (const TSTRING& strName, const TSTRING& strOutput)
{
	return true;
}


/*****************************************************************************
* Function name: SetSlotNumber      
* Summary      : Get slot
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetSlotNumber(int i_slot)
{
	m_i_slot = i_slot;
}


/*****************************************************************************
* Function name: SetImageVector      
* Summary      : Get image file name and type 
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetImageVector (std::vector<st_Image> vector_image)
{
	m_vImage = vector_image;
}


/*****************************************************************************
* Function name: SetPicassoList      
* Summary      : Get picasso
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetPicassoList(std::map<int, CString> map_picassoList)
{
	m_map_picassoList = map_picassoList;
}

//void CBaseProject::SetCOMPicasso(std::map<int, CString> map_COMPicasso)
//{
//	m_map_COMPicasso = map_COMPicasso;
//}

void CBaseProject::SetDLMode(const char* sz_DLMode)
{
	m_str_DLMode.Format("%s", sz_DLMode);
}

void CBaseProject::SetSWVerion(CString str_SWVeriosn)
{
	m_str_SWVersion = str_SWVeriosn;
}

//void CBaseProject::SetHWVerion(CString str_HWVeriosn)
//{
//	m_str_HWVersion = str_HWVeriosn;
//}

void CBaseProject::SetToolVerion(CString str_ToolVeriosn)
{
	m_str_ToolVersion = str_ToolVeriosn;
}

void CBaseProject::SetFactoryVerion(CString str_fatoryVersion)
{
	m_str_fatoryVersion = str_fatoryVersion;
}

//void CBaseProject::SetCustomerSwVerion(CString str_CustomerSwVersion)
//{
//	m_str_CustomerSwVersion = str_CustomerSwVersion;
//}

/*****************************************************************************
* Function name: SetDLLIniFileName      
* Summary      : Get dll ini file name
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetDLLIniFileName(const char* sz_iniFileName)
{
	sprintf_s(m_sz_iniFile, "%s", sz_iniFileName);
}

/*****************************************************************************
* Function name: GetError      
* Summary      : Get error code
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::GetError(char* sz_errorCode, int i_size)
{
	memset(sz_errorCode, 0, i_size);
	if (i_size < BUFSIZE)
	{
		strncpy(sz_errorCode, m_sz_errorCode, i_size-1);
	}
	else
	{
		strcpy(sz_errorCode, m_sz_errorCode);
	}
	
	return;
}


/*****************************************************************************
* Function name: SendMessageToUI      
* Summary      : 
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SendMessageToUI (const char* sz_message)
{
	/* Check Input */
	if (sz_message == NULL)
	{
		return;
	}

	st_UIMessage st_message;
	st_message.i_slot = this->m_i_slot;
	sprintf_s(st_message.sz_message, "%s", sz_message);

	Fire(UI_MESSAGE, (long)&st_message);
}


/*****************************************************************************
* Function name: SetProgress      
* Summary      : 
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetProgress (const int i_mainProgress, const int i_subProgress)
{
	st_UIProgress st_progress = {m_i_slot, i_mainProgress, i_subProgress};

	Fire (UI_PROGRESS, (long)&st_progress);
}


/*****************************************************************************
* Function name: SetPICSData      
* Summary      : 
* Parameters   : 
* Return       : bool.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetPICSData(const char* sz_key, const char* sz_value)
{
	st_PICSData st_picsData;
	st_picsData.i_slot = m_i_slot;
	sprintf_s(st_picsData.sz_key, "%s", sz_key);
	sprintf_s(st_picsData.sz_value, "%s", sz_value);

	Fire (UI_PICSDATA, (long)&st_picsData);
}


/*****************************************************************************
* Function name: SetError      
* Summary      : 
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
void CBaseProject::SetError(const char* sz_errorCode)
{
	strcpy(m_sz_errorCode, sz_errorCode);
}


/*****************************************************************************
* Function name: SearchImageFileByType      
* Summary      : 
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/06          First version             
*****************************************************************************/
std::string CBaseProject::SearchImageFileByType(const char* sz_type)
{
	std::string str_fileName;
	vector<st_Image>::iterator vIter;

	for (vIter = m_vImage.begin(); vIter != m_vImage.end(); vIter++)
	{
		CString str_image = (*vIter).str_imageType.c_str();
		if (str_image.CompareNoCase (sz_type) == 0)
		{
			str_fileName = (*vIter).str_imagePath + "\\" + (*vIter).str_imageName;
			break;
		}
	}

	return str_fileName;
}


/*****************************************************************************
* Function name: SetAllPICSData      
* Summary      : Download PICS XML form
				<PICS>
				<ColType>V2</ColType> 
				<L1>PICASSO</L1>
				<L2>MOBILEID</L2>
				<L3>TOOL_VERSION</L3>
				<L4>FA_HW_VERSION</L4>
				<L5>FA_SW_VERSION</L5>
				<L6>SLOT</L6>
				</PICS>
* Parameters   : 
* Return       : void.
*               
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen       2011/07/20          First version             
*****************************************************************************/
void CBaseProject::SetAllPICSData(void)
{
	/* Set id --- For UI deal with CIM */
	//SetPICSData("id", m_map_picassoList[m_i_slot]);

	/* Set Picasso */
	SetPICSData("PICASSO", m_map_picassoList[m_i_slot]);

	/* Set MobileID */

	/* Set SW version */
	SetPICSData("SW_Version", m_str_SWVersion);

	/* Set HW version */
	SetPICSData("HW_Version", m_str_HWVersion);

	/* Set Tool version */
	SetPICSData("TOOL_VERSION", m_str_ToolVersion);

	/* Set Factory version */
	//SetPICSData("FACTORY_VERSION", m_str_fatoryVersion);
	SetPICSData("FACTORY_VERSION", m_str_fatoryVersionDevice);

	/* Set Factory version */
	SetPICSData("CUSTOMER_VERSION", m_str_CustomerSwVersion);

	///* Set SW Version */
	//memset(sz_temp, 0, sizeof(sz_temp));
	//if (!GetSWVersion(sz_temp, sizeof(sz_temp)))
	//{
	//	SetPICSData("FA_SW_VERSION", "");
	//}
	//else
	//{
	//	SetPICSData("FA_SW_VERSION", sz_temp);
	//}

	///* Set HW Version */
	//memset(sz_temp, 0, sizeof(sz_temp));
	//if (!GetHWVersion(sz_temp, sizeof(sz_temp)))
	//{
	//	SetPICSData("FA_HW_VERSION", "");
	//}
	//else
	//{
	//	SetPICSData("FA_HW_VERSION", sz_temp);
	//}

	///* Set Tool Version */
	//char sz_temp[128];
	//memset(sz_temp, 0, sizeof(sz_temp));
	//if (!GetToolVersion(sz_temp, sizeof(sz_temp)))
	//{
	//	SetPICSData("TOOL_VERSION", "");
	//}
	//else
	//{
	//	SetPICSData("TOOL_VERSION", sz_temp);
	//}

	/* Set Slot */
	CString str_slot;
	str_slot.Format(_T("%d"), m_i_slot);
	SetPICSData("SLOT", str_slot);

	return;
}


bool CBaseProject::GetHWPlatform(char* sz_outBuffer, DWORD dw_size)
{
	/* Check Input */
	if (sz_outBuffer == NULL)
	{
		return false;
	}

	/* Read ini */
	CIniAccess obj_dllIni("Setting", m_sz_iniFile);
	if (!obj_dllIni.GetValue ("HWPlatform", sz_outBuffer, dw_size)) 
	{
		return false;
	}

	return true;
}

bool CBaseProject::GetSWVersion(char* sz_outBuffer, DWORD dw_size)
{
	/* Check Input */
	if (sz_outBuffer == NULL) 
	{
		return false;
	}

	/* Read ini */
	CIniAccess obj_dllIni("Setting", m_sz_iniFile);
	if (!obj_dllIni.GetValue ("SWVersion", sz_outBuffer, dw_size))
	{
		return false;
	}

	/* Check Length */
	int i_length = 0;
	i_length = strlen(sz_outBuffer);
	if (i_length != SWVERSION_LENGTH)
	{
		return false;
	}

	return true;
}

bool CBaseProject::CheckSWVersion()
{
	////-- Check SW Version --//
	//char SWver[1024];
	//if (strcmp(m_VersionType.c_str(), "Factory") == 0)
	//{
	//	GetFactoryVersion(SWver);
	//}
	//else if (strcmp(m_VersionType.c_str(), "Ship") == 0)
	//{
	//	GetSWVersion(SWver);
	//}
	//else
	//{
	//	m_strMessage = "Get XML Version Type fail";
	//	m_strResult = "FAIL";
	//	m_strErrorCode = CommErr_Get_Software_Version_Fail;
	//	TraceLog(MSG_ERROR, m_strMessage);
	//	FactoryLog();
	//	return false;
	//}

	//StrVtr vToken;
	//CStr::ParseString(SWver, _T(","), vToken);
	//bool b_match = false;
	//for (size_t i = 0; i < vToken.size(); i++)
	//{
	//	if (strstr(output, vToken[i].c_str()) != NULL)
	//	{
	//		b_match = true;
	//		break;
	//	}
	//}

	//char msg[1024];
	//if (!b_match)
	//{
	//	strcpy(msg, "SWVer are diff. Server=");
	//	strcat(msg, SWver);
	//	m_strMessage = msg;
	//	m_strResult = "FAIL";
	//	m_strMeasured = output;
	//	m_strErrorCode = CommErr_Check_Software_Version_Fail;
	//	TraceLog(MSG_ERROR, m_strMessage);
	//	FactoryLog();
	//	return false;
	//}

	//strcpy(msg, "Server=");
	//strcat(msg, SWver);
	//m_strMessage = msg;
	//m_strResult = "PASS";
	//m_strErrorCode = "-";
	//m_strMeasured = output;
	//TraceLog(MSG_INFO, m_strMessage);
	//FactoryLog();
	return true;
}

bool CBaseProject::GetHWVersion(char* sz_outBuffer, DWORD dw_size)
{
	/* Check Input */
	if (sz_outBuffer == NULL)
	{
		return false;
	}

	/* Read ini */
	CIniAccess obj_dllIni("Setting", m_sz_iniFile);
	if (!obj_dllIni.GetValue ("HWVersion", sz_outBuffer, dw_size)) 
	{
		return false;
	}

	/* Check Length */
	int i_length = 0;
	i_length = strlen(sz_outBuffer);
	if (i_length != HWVERSION_LENGTH)
	{
		return false;
	}

	return true;
}

bool CBaseProject::GetToolVersion(char* sz_outBuffer, DWORD dw_size)
{
	/* Check Input */
	if (sz_outBuffer == NULL) 
	{
		return false;
	}

	/* Read ini */
	CIniAccess obj_dllIni("Setting", m_sz_iniFile);
	if (!obj_dllIni.GetValue ("ToolVersion", sz_outBuffer, dw_size)) 
	{
		return false;
	}

	return true;
}


//===========================Only For Leopard and Wolf ===========================================//
char* CBaseProject::GetImagePath (char* sz_imagePath, DWORD dw_size)
{
	/* Check */
	if (m_vImage.empty())
	{
		return NULL;
	}

	memset(sz_imagePath, 0, dw_size);
	strncpy(sz_imagePath, m_vImage[0].str_imagePath.c_str(), dw_size);
	
	return sz_imagePath;
}

void CBaseProject::GetCurrentPath(void)
{
	/* Get current DLL path */
	TCHAR sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);

	size_t i = _tcslen(sz_currentPath);
	while (sz_currentPath[--i] != '\\');
	sz_currentPath[++i] = 0;

	//::PathRemoveFileSpec(sz_currentPath);

	strcpy(m_sz_currentPath, sz_currentPath);

	return;
}

int CBaseProject::GetCOMport(void)
{
	/* Get current DLL path */
	TCHAR sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	CString str_iniFile;
	/* Check dada file exist */
	str_iniFile.Format(_T("%s\\QISDA\\%sDLL.ini"), sz_currentPath, "Wolf");
	if (_taccess(str_iniFile, 0) != 0) 
	{
		return false;
	}

	/* Get COMPort */
	CIniAccess obj_dllIni(_T("COMPort"), str_iniFile);
	CString str_comkey;
	int i_comPort = 0;
	CString str_comPort;
	str_comkey.Format(_T("Port_%02d"), m_i_slot + 1);
	i_comPort = obj_dllIni.GetValue(str_comkey);

	return i_comPort;
}



