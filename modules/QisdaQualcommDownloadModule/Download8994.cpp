
#include "StdAfx.h"
#include "Download8994.h"
#include "../../lib/Qualcomm/QDART4823/inc/QLib.h"
//#include "../UnifyDLL_U2416_Download/SparseLib/sparse.h"
#include <SetupAPI.h>
#pragma comment(lib, "setupapi")
#include <Cfgmgr32.h>
#include <string>
#include <sstream>

CCriticalSection CDownload8994::m_obj_loadcheckSumCritSection;
CDownload8994* g_pFirehoseDownload = NULL;

template <class T> 
std::string ConvertToString(T);


CDownload8994::CDownload8994(int i_COMPort, CString str_multiDLFlag)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	m_i_reboot = 0;
	m_i_RebootMode = 0;
	m_i_checkSum = 0;
	m_i_SupportQDownload = 0;
	m_map_COMPicasso.clear();
	m_map_imageNameCheckSum.clear();
	m_str_multiDLFlag = str_multiDLFlag;
	m_i_COMPort = i_COMPort;
}

CDownload8994::~CDownload8994(void)
{
	//if (m_p_adbDevice) 
	//{
	//	delete m_p_adbDevice;
	//	m_p_adbDevice = NULL;
	//}

	if (m_hQMSL_MSVC10R != NULL)
	{
		FreeLibrary (m_hQMSL_MSVC10R);
		m_hQMSL_MSVC10R = NULL;
	}

	//if (m_hSparselibModule != NULL)
	//{
	//	FreeLibrary (m_hSparselibModule);
	//	m_hSparselibModule = NULL;
	//}
	
}


/*****************************************************************************
* Function name: MultiStartDownload      
* Summary      : Execute download (Hex DL + fastboot DL)
* Parameters   : b_speedUp = false; nDLPROTOCOL = QTT_FASTDOWNLOAD;
* Return       : true : success.
*                false: failure.
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19            First version             
*****************************************************************************/
bool CDownload8994::MultiDownload(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL)  
{

	bool b_result = false;
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];

	if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	{ 
		/* Hex Download */
		b_result = Download(b_speedUp, b_reOpenAfterReset, nDLPROTOCOL);
	
		//b_result = FastbootEXE_Download();

		///* Fastboot Download */
		//if (b_result)
		//{
		//	b_result = FastbootDownload(); 
		//}
	}
 	else if ((m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL))
	{
		bool b_wait_fastboot = false;
		int nLimitTime = 0 ; 
		//if (  FindADBdevice(m_i_COMPort) ){
			CString cs_Qphone;
			cs_Qphone.Format(_T("QPHONE%d"), m_i_COMPort);
			if (bGetADB(m_i_COMPort)){

				AddMsg("Get Adb Success.", None, 10);
				//SendMessageToUI("Get Adb Success.");
				
				//Sleep(1000);//for adb mode to fastboot mode
				//if (bGetFastboot(_T("devices"), output,ErrorCode, m_i_COMPort)){
				while ( !b_wait_fastboot){
					if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"),output,ErrorCode, cs_Qphone) ){
						b_wait_fastboot = true;
						AddMsg("Get Fastboot Success.", None, 10);
					}
					Sleep(1000);
					nLimitTime ++;
					if ( nLimitTime > 60 ) break;
				}
				if ( b_wait_fastboot) b_result = FastbootEXE_Download(m_i_COMPort);
				else AddMsg("Fail can't find fastboot", DownloadStep::None, 100);
			}

			
		//b_result = FastbootEXE_OS_DL();
		//}
		//else return false;
		/* Fastboot Download */
	//	b_result = FastbootDownload();
	}
	else
	{
		AddMsg("DL mode is not PreDL or ReDL.", DownloadStep::None, 100);
		return false;
	}

	delete[] output;
	delete[] ErrorCode;
	return b_result;
}


bool CDownload8994::bGetADB(int nPhone){ 
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];
	int nRetryTime = 60;
	bool bGetAdb = false; 
	CString csPhone;
	CString csCmd;

	//csCmd = _T("devices ");
	csPhone.Format("QPHONE%d", nPhone);

	for(int i =0; i <nRetryTime; i++){
		//if ( bGetAdb = bAdbCMD(csCmd, output, ErrorCode, nPhone))
		if (bCallAdbFastbootCMD(_T("adb.exe"), _T("devices "),output,ErrorCode, csPhone))
		{
			csCmd.Format(_T("reboot bootloader -s QPHONE%d"), nPhone);
			bCallAdbFastbootCMD(_T("adb.exe"), csCmd,output,ErrorCode, _T("NULL"));
			bGetAdb = true;
			Sleep(1000);
			//bAdbCMD(csCmd, output, ErrorCode, 0);
			
			break;
		}
	}

	
	//CDownload8994::FastbootEXE_OS_DL(){
	return bGetAdb;
}

//bool CDownload8994::FastbootEXE_OS_DL(){
//
//
//	map<int,CString>::iterator iter;       
//	CString cs_MapValue = NULL;
//	CString csImgPath;// = NULL;
//	CString csImgType;// = NULL;
//	CString csExePath;
//	bool bDLFlag = false;
//
//	/*get fastboot.exe path */
//	char sz_currentPath[MAX_PATH] = { 0 };
//	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
//	::PathRemoveFileSpec(sz_currentPath);
//
//	CString str_FastbootPath;
//	str_FastbootPath.Format(_T("%s\\fastboot.exe"), sz_currentPath);
//
//	str_FastbootPath = _T("C:\\Image\\GF80B1A\\u2416_certification\\fastboot.exe");
//	//m_i_COMPort += 1;
//	/*** DL_PASSPORT_IMGS ****/
//	for (int i = 0 ; i < mapDL_PASSPORT_IMGS_TYPE.size() ; i++){
//		iter = mapDL_PASSPORT_IMGS_TYPE.find(i); 
//		if(iter != mapDL_PASSPORT_IMGS_TYPE.end()){
//			csImgType = iter->second;
//		}
//
//		iter = mapDL_PASSPORT_IMGS.find(i); 
//		if(iter != mapDL_PASSPORT_IMGS.end()){
//			csImgPath = m_str_imageFilePath + "\\" + iter->second;
//		}
//
//		//csExePath.Format(_T("\"%s\" flash %s \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
//		//csExePath.Format(_T("\"%s\" flash %s \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
//		csExePath.Format(_T("flash %s \"%s\""), csImgType, csImgPath);
//		
//
//		bool isOk = false;
//	DWORD nTimeout = (csExePath.Find(_T("system")) != -1) ? 600 * 1000 : 50 * 1000; // system > 15 + size/2 sec, other 10 sec
//	CString param;
//	param += csExePath;
//
//	TRACE(_T("fastboot command: %s\n"), csExePath);
//	TRACE(_T("fastboot timeout: %d\n"), nTimeout);
//		
//
//	if (Exec(str_FastbootPath, param, nTimeout)) {
//		if (param.Find(_T("FAIL")) != -1) { // handle FAIL
//			isOk = false;
//			if (str_FastbootPath.Find(_T("status read failed")) != -1) {
//			//	SendMsg(Command, "FAIL"); // ERROR_CONNECTION_INVALID 1229
//			//	SendMsg(_T("ERROR: USB status read failed!\nPlease check USB hub, USB cable, and try again.\n"), "FAIL");
//			}
//			else if (str_FastbootPath.Find(_T("getvar")) != -1) {
//				// skip getvar fail
//			}
//			else {
//				//SendMsg(param, "FAIL");
//			}
//		}
//		else { // handle OKAY
//			isOk = true;
//			//if (Command.Find(_T("reboot")) != -1) {
//			//	// hide reboot command
//			//}
//			//else if (Command.Find(_T("flash")) != -1) {
//			//	// hide flash command
//			//}
//			//else if (Command.Find(_T("getvar")) != -1) {
//			//	// hide getvar command
//			//}
//			//else {
//			//	SendMsg(param, "INFO");
//			//}
//		}
//		//Command = param;
//	}
//	else {
//		isOk = false;
//	//	SendMsg(_T("ERROR: Execute fastboot fail!\n"), "FAIL");
////#ifdef _DEBUG
//	//	SendMsg(_T("#DEBUG: cmd -") + Command + _T("\n"), "FAIL");
////#endif
//	}
//	return true;
//
//		
//		bool bDLFlag = false;
//		bDLFlag = bFastbootDL(csExePath);
//		if ( bDLFlag ){
//			AddMsg("Passport OK", None, 10);
//		}else{
//			AddMsg("Passport Fail", None, 10);
//			return false;
//		}
//	}
//
//
//	/*** DL_ALL_CHECK1_IMG ****/
//	
//	for (int i = 0 ; i < mapDL_ALL_CHECK1_IMGS_TYPE.size() ; i++){
//		iter = mapDL_ALL_CHECK1_IMGS_TYPE.find(i); 
//		if(iter != mapDL_ALL_CHECK1_IMGS_TYPE.end()){
//			csImgType = iter->second;
//		}
//
//		iter = mapDL_ALL_CHECK1_IMGS.find(i); 
//		if(iter != mapDL_ALL_CHECK1_IMGS.end()){
//			csImgPath = m_str_imageFilePath + "\\" + iter->second;
//		}
//		//csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
//		//csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
//		//bDLFlag = bFastbootDL(csExePath);
//
//		CString csMsg;
//		if ( bDLFlag ){
//			csMsg.Format("%s ok",  iter->second);
//			AddMsg(csMsg, None, 10);
//		}
//		else
//		{
//			csMsg.Format("%s fail",  iter->second);
//			AddMsg(csMsg, None, 10);
//			return false;
//		}
//	}
//
//	/*** mapFORMAT_IMGS ****/
//	Sleep(3000);
//	for (int i = 0 ; i < mapFORMAT_IMGS.size() ; i++){
//		iter = mapFORMAT_IMGS.find(i); 
//		if(iter != mapFORMAT_IMGS.end()){
//			csImgType = iter->second;
//		}
//
//		//csExePath.Format(_T("\"%s\" format \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, m_i_COMPort);
//	//	bDLFlag = bFastbootDL(csExePath);
//
//		CString csMsg;
//		if ( bDLFlag ){
//			csMsg.Format("%s ok",  iter->second);
//			AddMsg(csMsg, None, 10);
//		}
//		else
//		{
//			csMsg.Format("%s fail",  iter->second);
//			AddMsg(csMsg, None, 10);
//			return false;
//		}
//	}
//
//	/**** mapERASE_ALL_IMGS ********/
//	for (int i = 0 ; i < mapERASE_ALL_IMGS.size() ; i++){
//		iter = mapERASE_ALL_IMGS.find(i); 
//		if(iter != mapERASE_ALL_IMGS.end()){
//			csImgType = iter->second;
//		}
//
//		//csExePath.Format(_T("\"%s\" erase \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, m_i_COMPort);
//		//bDLFlag = bFastbootDL(csExePath);
//
//		CString csMsg;
//		if ( bDLFlag ){
//			csMsg.Format("%s ok",  iter->second);
//			AddMsg(csMsg, None, 10);
//		}
//		else
//		{
//			csMsg.Format("%s fail",  iter->second);
//			AddMsg(csMsg, None, 10);
//			return false;
//		}
//	}
//
//	return true;
//
//
//	for (int i = 0 ; i < mapDL_ALL_CHECK1_IMGS.size() ; i++){
//		iter = mapDL_ALL_CHECK1_IMGS.find(i); 
//		if(iter != mapDL_ALL_CHECK1_IMGS.end()){
//			csImgPath = m_str_imageFilePath + "\\" + iter->second;
//
//		}
//	}
////	csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
////	bFastbootDL(csExePath);
//
//	//mapDL_PASSPORT_IMGS;
//	//std::map < int, CString> mapDL_PASSPORT_IMGS;
//	//std::map < int, CString> mapDL_ALL_CHECK1_IMGS;
//	//std::map < int, CString> mapFORMAT_IMGS;
//	//std::map < int, CString> mapERASE_ALL_IMGS;
//
//	return true;
//}


bool CDownload8994::FastbootEXE_Download(int nPort){
	map<int,CString>::iterator iter;       
	CString cs_MapValue = NULL;
	CString csImgPath;// = NULL;
	CString csImgType;// = NULL;
	CString csExePath;
	bool bDLFlag = false;

	/*get fastboot.exe path */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	CString str_FastbootPath;
	str_FastbootPath.Format(_T("%s\\fastboot.exe"), sz_currentPath);

	/*** DL_PASSPORT_IMGS ****/
	for (int i = 0 ; i < mapDL_PASSPORT_IMGS_TYPE.size() ; i++){
		iter = mapDL_PASSPORT_IMGS_TYPE.find(i); 
		if(iter != mapDL_PASSPORT_IMGS_TYPE.end()){
			csImgType = iter->second;
		}

		iter = mapDL_PASSPORT_IMGS.find(i); 
		if(iter != mapDL_PASSPORT_IMGS.end()){
			csImgPath = m_str_imageFilePath + "\\" + iter->second;
		}
		//csExePath.Format(_T("\"%s\" flash %s \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
		csExePath.Format(_T("\"%s\" flash %s \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, nPort);

		
		bDLFlag = bFastbootDL(csExePath);
		if ( bDLFlag ){
			AddMsg("Passport OK", None, 10);
		}else{
			AddMsg("Passport Fail", None, 10);
			return false;
		}
	}


	/*** DL_ALL_CHECK1_IMG ****/
	
	for (int i = 0 ; i < mapDL_ALL_CHECK1_IMGS_TYPE.size() ; i++){
		iter = mapDL_ALL_CHECK1_IMGS_TYPE.find(i); 
		if(iter != mapDL_ALL_CHECK1_IMGS_TYPE.end()){
			csImgType = iter->second;
		}

		iter = mapDL_ALL_CHECK1_IMGS.find(i); 
		if(iter != mapDL_ALL_CHECK1_IMGS.end()){
			csImgPath = m_str_imageFilePath + "\\" + iter->second;
		}
		csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
		//csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
		bDLFlag = bFastbootDL(csExePath);

		CString csMsg;
		if ( bDLFlag ){
			csMsg.Format("flash %s ok",  iter->second);
			AddMsg(csMsg, None, 10);
		}
		else
		{
			csMsg.Format("flash %s fail",  iter->second);
			AddMsg(csMsg, None, 10);
			return false;
		}
	}

	/*** mapFORMAT_IMGS ****/
	Sleep(3000);
	for (int i = 0 ; i < mapFORMAT_IMGS.size() ; i++){
		iter = mapFORMAT_IMGS.find(i); 
		if(iter != mapFORMAT_IMGS.end()){
			csImgType = iter->second;
		}

		csExePath.Format(_T("\"%s\" format \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, m_i_COMPort);
		bDLFlag = bFastbootDL(csExePath);

		CString csMsg;
		if ( bDLFlag ){
			csMsg.Format("format %s ok",  iter->second);
			AddMsg(csMsg, None, 10);
		}
		else
		{
			csMsg.Format("format %s fail",  iter->second);
			AddMsg(csMsg, None, 10);
			return false;
		}
	}

	/**** mapERASE_ALL_IMGS ********/
	for (int i = 0 ; i < mapERASE_ALL_IMGS.size() ; i++){
		iter = mapERASE_ALL_IMGS.find(i); 
		if(iter != mapERASE_ALL_IMGS.end()){
			csImgType = iter->second;
		}

		csExePath.Format(_T("\"%s\" erase \"%s\" -s QPHONE%d"),str_FastbootPath, csImgType, m_i_COMPort);
		bDLFlag = bFastbootDL(csExePath);

		CString csMsg;
		if ( bDLFlag ){
			csMsg.Format("erase %s ok",  iter->second);
			AddMsg(csMsg, None, 10);
		}
		else
		{
			csMsg.Format("erase %s fail",  iter->second);
			AddMsg(csMsg, None, 10);
			return false;
		}
	}

	return true;


	for (int i = 0 ; i < mapDL_ALL_CHECK1_IMGS.size() ; i++){
		iter = mapDL_ALL_CHECK1_IMGS.find(i); 
		if(iter != mapDL_ALL_CHECK1_IMGS.end()){
			csImgPath = m_str_imageFilePath + "\\" + iter->second;

		}
	}
	csExePath.Format(_T("%s flash %s %s -s QPHONE%d"),str_FastbootPath, csImgType, csImgPath, m_i_COMPort);
	bFastbootDL(csExePath);

	//mapDL_PASSPORT_IMGS;
	//std::map < int, CString> mapDL_PASSPORT_IMGS;
	//std::map < int, CString> mapDL_ALL_CHECK1_IMGS;
	//std::map < int, CString> mapFORMAT_IMGS;
	//std::map < int, CString> mapERASE_ALL_IMGS;

	return true;
}

bool CDownload8994::setIMGPath(CString imagePath){
	m_str_imageFilePath = imagePath;
	return true;
}
bool CDownload8994::ReadIMG( CString imagePath )
{
	CStdioFile readFile;
	CString strLine, restoken;
	int curPos=0;	
	int nIndex = 0 ;

	CString SettingFilePath;
	SettingFilePath.Format("%s\\common.sh", imagePath);
	printf("%s", imagePath);

	if (_taccess(SettingFilePath, 0) != 0) 
	{
		CString csMsg;
		csMsg.Format("%s access fail", SettingFilePath);
		AddMsg(csMsg, None, 10);
		return false;
	}


	readFile.Open(SettingFilePath , CFile::modeRead);
	readFile.SeekToBegin(); 


	CString csReplaceString;
	int nCount = 0;
	bool bERASE_ALL_IMGS_DONE = false;
	while(readFile.ReadString(strLine))
	{
		if (strLine.Find(_T("DL_PASSPORT_IMGS")) != -1 ){
			nCount = 0;
			while(readFile.ReadString(strLine))
			{
				if (strLine.Find(_T(")")) != -1 ){
					break; 
				}
				strLine.Replace("\"", NULL); //pair<int,int>(i, 1*10)
				int curPos = 0;
				CAtlString resToken;

				resToken= strLine.Tokenize(_T(" "),curPos);
				mapDL_PASSPORT_IMGS_TYPE.insert( pair<int,CString >(nCount, resToken));
				
				resToken= strLine.Tokenize(_T(" "),curPos);
				mapDL_PASSPORT_IMGS.insert( pair<int,CString >(nCount, resToken));

				nCount++;
			}
		}

		if (strLine.Find(_T("DL_ALL_CHECK1_IMGS")) != -1 ){
			nCount = 0;
			while(readFile.ReadString(strLine))
			{
				if (strLine.Find(_T(")")) != -1 ){
					break; 
				}
				strLine.Replace("\"", NULL); //pair<int,int>(i, 1*10)

				int curPos = 0;
				CAtlString resToken;

				resToken= strLine.Tokenize(_T(" "),curPos);
				mapDL_ALL_CHECK1_IMGS_TYPE.insert( pair<int,CString >(nCount, resToken));

				resToken= strLine.Tokenize(_T(" "),curPos);
				mapDL_ALL_CHECK1_IMGS.insert( pair<int,CString >(nCount, resToken));

				nCount++;
			}
		}

		if (strLine.Find(_T("FORMAT_IMGS"))  != -1 ){
			nCount = 0;
			while(readFile.ReadString(strLine))
			{
				if (strLine.Find(_T(")")) != -1 ){
					break; 
				}
				strLine.Replace("\"", NULL); //pair<int,int>(i, 1*10)
				mapFORMAT_IMGS.insert( pair<int,CString >(nCount, strLine));
				nCount++;
			}
		}
		if (strLine.Find(_T("ERASE_ALL_IMGS"))  != -1 ){
			nCount = 0;
			while(readFile.ReadString(strLine))
			{
				if (strLine.Find(_T(")")) != -1 ){
					bERASE_ALL_IMGS_DONE = true;
					break; 
				}
				strLine.Replace("\"", NULL); //pair<int,int>(i, 1*10)
				mapERASE_ALL_IMGS.insert( pair<int,CString >(nCount, strLine));
				nCount++;
			}
		}

		if ( bERASE_ALL_IMGS_DONE ) break; // done exit while;
	}
	return true;
}

/*****************************************************************************
* Function name: Download     
* Summary      : DMSSDL + StreamingDL
* Parameters   : 
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/
bool CDownload8994::Download(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL) 
{
	AddMsg("Hex Download", None, 100);
	
	m_b_reOpenAfterReset = b_reOpenAfterReset;

//	CString csImgPath;
	//csImgPath.Format("%s %d", _T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150721_Dell_Debug\\fastboot.exe flash system C:\\Image\\GF80B1A\\un2416_60531\\system.img -s QPHONE"), devStruct.nRealPort);	

	/* DMSS DL + Streaming DL */
	//if (SAHARADL() && StreamingDL(b_speedUp, nDLPROTOCOL))
	//{
	//} 
	m_hQMSL_MSVC10R = LoadLibrary("QMSL_MSVC10R.dll");
	if(m_hQMSL_MSVC10R == NULL)
	{
		AddMsg(false, "Load QMSL_MSVC10R Fail", FireHoseProtocol);
		return false;
	}

	//std::map<std::string, std::string> map_strstrCOMDeviceName;
	//std::string m_strKeyword1;
	//m_strKeyword1.assign("QUALCOMM");

	//GetCOMPortDevByWDK( map_strstrCOMDeviceName );

	//USBDEVICE_STRUCT devStruct = {0};

	memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

	if ( !FindQualcomDevice(GetQualcommport()) ) {
		AddMsg(false, "FindQualcomDevice Fail", FireHoseProtocol);
		return false;
	}

	if (FlashProgrammer() && FireHose())
	{
		FreeLibrary(m_hQMSL_MSVC10R);

		AddMsg("Sahara , FireHose PASS , reboot ", None, 20);
		//return true;
	} 
	else
	{
		//if(m_b_resetMode) 
		//{
		//	/* Init PotocolType to NONE mode */
		//	CInterfaceBase::ResetMode();
		//}
		AddMsg("Hex Download Fail.", None, 100);
		return false;
	}


	//CString csImgPath;
	//csImgPath.Format("%s%d", _T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150721_Dell_Debug\\fastboot.exe flash passport C:\\Image\\GF80B1A\\un2416_60531\\passport_FactoryDLTool -s QPHONE"), devStruct.nRealPort);
	//bFastbootDL(csImgPath);

	//csImgPath.Format("%s%d", _T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150721_Dell_Debug\\fastboot.exe flash partition C:\\Image\\GF80B1A\\un2416_60531\\gpt_both0.bin -s QPHONE"), devStruct.nRealPort);
	//bFastbootDL(csImgPath);

	return true;


	AddMsg("Hex Download Success.", None, 100);
	return true;
}

//bool Exec(CString& path, CString& param, DWORD msTimeout = INFINITE, bool hasResponse = true);
//bool CDownload8994::Exec(CString& path, CString& param, DWORD msTimeout, bool hasResponse)
//{
//
//	if (!IsPathExist(path)) {
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	DWORD nPipeSize = 1024;
//	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
//		::AfxMessageBox(_T("ERROR: CreatePipe fail!\r\n"));
//		return false;
//	}
//
//	TCHAR szNowPath[MAX_PATH] = {0};
//	GetCurrentDirectory(MAX_PATH, szNowPath);
//	TRACE(_T("current path: %s\n"), szNowPath);
//	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
//	TRACE(_T("working path: %s\n"), WorkDir);
//	SetCurrentDirectory(WorkDir);
//
//	bool isOk = false;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//	CString Command = path;
//	if (!param.IsEmpty()) {
//		Command = Command + _T(" ") + param;
//	}
//	TRACE(_T("exec: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
//		DWORD TimeoutFlag = WaitForSingleObject(processInfo.hProcess, msTimeout);
//		isOk = true;
//
//		if (hasResponse) {
//			DWORD bytesRead;
//			DWORD dwBytesWritten;
//			char message[512];
//			memset(message, 0, sizeof(message));
//			if (TimeoutFlag == WAIT_TIMEOUT)
//			{
//				WriteFile(hWrite, "FAIL", 4, &dwBytesWritten, NULL);
//				AfxMessageBox("timeout");
//			}
//			::ReadFile(hRead, message, 511, &bytesRead, NULL);
//			if (strlen(message) > 0) {
//				param = CA2T(message);
//			}
//			else {
//				param.Empty();
//			}
//		}
//	}
//	else {
//		isOk = false;
//		CString error_msg(_T("ERROR: Execute fail!\r\ncmd:"));
//		error_msg += Command;
//		::AfxMessageBox(error_msg);
//	}
//	Command.ReleaseBuffer();
//	CloseHandle(hRead);
//	CloseHandle(hWrite);
//	SetCurrentDirectory(szNowPath);
//
//	return isOk;
//}
//


template <class T> 

std::string ConvertToString(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

GUID GUIDQualcommDiagUsbClass1	= {0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};

bool CDownload8994::FindQualcomDevice(int nComport)
{
	GUID *pGUID = &GUIDQualcommDiagUsbClass1;

	//USBDEVICE_STRUCT devStruct = {0};
	BYTE chTemp [MAX_PATH];
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;
	
	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs(pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i = 0;
	DWORD dataType;
	std::string sComport = ConvertToString(nComport);

	while (1)
	{
		// Get device interface data
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces(hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		//memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

		// Get device path
		pDeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) &&
			(0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // We only care about usb device
		{
			strcpy_s(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			// Get device friendly name
		
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
			{
				strcpy_s(devStruct.chFriendlyName, (char*)chTemp);
				std::string stmp = std::string	( reinterpret_cast< char const* >(chTemp));

				if (stmp.find(sComport) != std::string::npos){
					//	m_strPROLIFICcomPort = stmp.substr(3, (stmp.size() - 3));

					unsigned char out_data[1024] = {0};
					if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_ADDRESS, &dataType, (unsigned char*)out_data, MAX_PATH, &dwRequired))

					sz_id = new char;
					_snprintf(sz_id, sizeof(out_data), "%d", out_data[0]);

					devStruct.nRealPort  = atoi( sz_id);
					//qualcommComPort = atoi(sz_id);
					//printf("%d", qualcommComPort);

					// Get the device ID of the USB root.
					CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
					strcpy_s(devStruct.chParentName, (char*)chTemp);

					// Get device instance name
					if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
						strcpy_s(devStruct.chInstanceName, (char*)chTemp);

					if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
						strcpy_s(devStruct.chDeviceDescr, (char*)chTemp);

					bFound = true;
					break;
				}
			}
		}
		++i;
		if( i > 20 ) {
			AfxMessageBox("can't find qualcomm port");
			break; // force break;
		}
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}

GUID GUIDAndroidUsbClassID1		= {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};

bool CDownload8994::FindADBdevice(int nADBPort)
{
	GUID *pGUID = &GUIDAndroidUsbClassID1;

//	USBDEVICE_STRUCT devStruct = {0};
	DWORD dwRequired;

	BYTE bTmpStringData[MAX_PATH];
	HDEVINFO hDeviceInfo;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDeviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)bTmpStringData;

	SP_DEVINFO_DATA devInfo = {0};
	devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

	hDeviceInfo = SetupDiGetClassDevs(pGUID, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if (INVALID_HANDLE_VALUE == hDeviceInfo)
		return false;

	bool bFound = false;

	// enum devices
	int i = 0;
	while (1)
	{
		// Get device interface data
		deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (! SetupDiEnumDeviceInterfaces(hDeviceInfo, 0, pGUID, i, &deviceInfoData))
			break;

		//memset(&devStruct, 0, sizeof(USBDEVICE_STRUCT));

		// Get device path
		pDeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (SetupDiGetDeviceInterfaceDetail(hDeviceInfo,&deviceInfoData,pDeviceDetailData,256 ,NULL, &devInfo) &&
			(pDeviceDetailData->cbSize > 0) 
			&&(0 == _strnicmp("\\\\?\\usb", pDeviceDetailData->DevicePath, 7))) // We only care about usb device
		{
			strcpy_s(devStruct.chHandleName, pDeviceDetailData->DevicePath);

			// Get the device instance of parent. This points to USB Composite Device
			DEVINST devInstParent;
			CM_Get_Parent(&devInstParent, devInfo.DevInst, 0);

			DWORD dataType;
			unsigned char out_data[1024] = {0};
			if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_ADDRESS, &dataType, (unsigned char*)out_data, MAX_PATH, &dwRequired))

			sz_id = new char;
			_snprintf(sz_id, sizeof(out_data), "%d", out_data[0]);
			
			/*return any port*/
			if ( nADBPort == -1 ){
				return atoi( sz_id);
			}
			else
			{/*return specific port */
				if( nADBPort = atoi( sz_id)) return true;;
			}
			return false;

			//if (devStruct.nRealPort ==  atoi( sz_id)){
			//	printf("got it");
			//	bFound = true;
			//	break;
			//}

			//devStruct.nRealPort  = atoi( sz_id);
			////qualcommComPort = atoi(sz_id);
			////printf("%d", qualcommComPort);

			//// Get the device ID of the USB root.
			//CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			//strcpy_s(devStruct.chParentName, (char*)chTemp);


			//// Get the device ID of the USB root.
			//CM_Get_Device_ID(devInstParent, (char*)chTemp, MAX_PATH, 0);
			//strcpy_s(devStruct.chParentName, (char*)chTemp);

			//// Get device instance name
			//if (SetupDiGetDeviceInstanceId(hDeviceInfo, &devInfo, (PSTR)chTemp, MAX_PATH, &dwRequired))
			//	strcpy_s(devStruct.chInstanceName, (char*)chTemp);

			//// Get device friendly name
			//DWORD dataType;
			//if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_FRIENDLYNAME, &dataType, chTemp, MAX_PATH, &dwRequired))
			//	strcpy_s(devStruct.chFriendlyName, (char*)chTemp);

			//if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_DEVICEDESC, &dataType, chTemp, MAX_PATH, &dwRequired))
			//	strcpy_s(devStruct.chDeviceDescr, (char*)chTemp);

			/*---------*/
			//DWORD dataType;
			//unsigned char out_data[1024] = {0};
			//if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfo, SPDRP_ADDRESS, &dataType, (unsigned char*)out_data, MAX_PATH, &dwRequired))

		
			//sz_id = new char;
			//_snprintf(sz_id, sizeof(out_data), "%d", out_data[0]);

			//
			//qualcommComPort = atoi( sz_id);
			//if ( qualcommComPort == devStruct.nRealPort ){
			//	printf("get it");
			//}
			//printf("%d", qualcommComPort);

			/*---------*/

			//inUSBdevice.push_back(devStruct);
		
		}
		++i;
		if ( i > 100) break; // time out
	}

	// Destroy handle
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return -1;
}
/*****************************************************************************
* Function name: DMSSDL     
* Summary      : DMSSDL
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/01/19          First version             
*****************************************************************************/

/*****************************************************************************
* Function name: QMSLCallBack_8994
* Summary      : CallBack function for SAHARADL
* Parameters   : 
*                
* Return       : void
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Erin.Chang     2013/09/18          First version             
*****************************************************************************/
void QMSLCallBack_8994(HANDLE hQMSLContext,
      char	*sMsg,
      unsigned short iMsgSize)
{
	BYTE readBuffer[256] = {0};
	memcpy(readBuffer, sMsg, iMsgSize);

	FILE *pFile = NULL;
	pFile = fopen("D:\\Log\\UI_RD\\QMSLCallBack_8994.log", "wb");
	fseek(pFile, 0, SEEK_END);
	fwrite(readBuffer, 1, sizeof(readBuffer)-1, pFile);
	fclose(pFile);
}


bool CDownload8994::SAHARADL(void)
{
	pQLIB_QPHONEMS_ConnectServer_Sahara ConnectServer_Sahara = (pQLIB_QPHONEMS_ConnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_Sahara");
	pQLIB_QPHONEMS_Sahara_FlashProgrammer Sahara_FlashProgrammer = (pQLIB_QPHONEMS_Sahara_FlashProgrammer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_Sahara_FlashProgrammer");
	pQLIB_DisconnectServer DisconnectServer = (pQLIB_DisconnectServer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_Sahara");

	unsigned int iPort = m_i_COMPort;
	HANDLE m_hPhone = NULL;
	unsigned long saharaVer = 0;
	unsigned char cret;

	unsigned long* szMsmid = 0;
	unsigned char szBGetMsmId= '0';
	int n_mode = 0;
	unsigned long n_timeout = 100;

		unsigned long version = 0;
		unsigned long msmid = 0;
		unsigned long timeout = 0;
		unsigned char bGetMsmId = 0;
		int mode = 0;

	//QLIB_QPHONEMS_SaharaConfigureCallback SWDL_QPHONEMS_CB = QMSLCallBack_8994;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_8994;

	if(ConnectServer_Sahara != NULL)
	{
		//m_hPhone = ConnectServer_Sahara(iPort, &saharaVer, szMsmid, szBGetMsmId,n_mode, n_timeout, SWDL_QPHONEMS_CB);
		m_hPhone = ConnectServer_Sahara(iPort, &version, &msmid, bGetMsmId, mode, timeout, SWDL_QPHONEMS_CB);
		
		AddMsg(true, "ConnectServer_Sahara", SaharaProtocol);
	}
	else
	{
		return false;
	}

	if(saharaVer == 0){
		AddMsg(false, "ConnectServer_Sahara_fail", SaharaProtocol);
		//return false;
	}

	if(Sahara_FlashProgrammer != NULL)
	{
		char cHexPath[256] = {0};
		strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());
		cret = Sahara_FlashProgrammer(m_hPhone, cHexPath);

		if(cret) //1
		{
			//SetUIMessage("Flash Programmer Success");
		}
		else         //0
		{
			//SetUIMessage("Flash Programmer Fail");
			return false;
		}

		AddMsg(true, "Sahara_FlashProgrammer", SaharaProtocol);
	}
	else
	{
		return false;
	}
    Sleep(1000);

	if (m_hPhone != NULL && DisconnectServer != NULL)
	{
		DisconnectServer(m_hPhone);
		m_hPhone = NULL;
	}

	return true;
}


void QMSLCallBack_FireHose(HANDLE hQMSLContext,
				  char	*szMsg,
				  unsigned short iMsgSize)
{
	if (NULL ==g_pFirehoseDownload)
		return;

	g_pFirehoseDownload->RecordQMSLCallBack(szMsg);
}


void CDownload8994::RecordQMSLCallBack( char	*szMsg)
{
	if (NULL ==g_pFirehoseDownload)
		return;
//	TraceLog(MakeSTDstring("[QMSLCallBack]%s", szMsg));

	AddMsg("[RecordQMSLCallBack]", DownloadStep::None, 10);

	g_pFirehoseDownload->RecordQMSLCallBack(szMsg);

		//Update Progress
		m_iFileIndex++;

		if (m_iFirehoseDLMode == FIREHOSE_DL_MODE_FASTBOOT_ONLY){
			AddMsg("Get ADB Fail.", DownloadStep::None, ((int)((m_iFileIndex * 80) / m_iFileCount) + 10, 100));
		}
		else{

			AddMsg("Get ADB Fail.", DownloadStep::None,((int)((m_iFileIndex * 60) / m_iFileCount) + 10, 100));
		}


		
	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
}

void swdlCallBack_8994(
   HANDLE hContextID,
   char * sMsg,
   unsigned short iMsgSize)
{

   printf("\n%s", sMsg);

	FILE *pFile = NULL;
	pFile = fopen("C:\\FireHoseQMSLCallBack_8916.log", "wb");
	fseek(pFile, 0, SEEK_END);
	fwrite(sMsg, 1, sizeof(sMsg)-1, pFile);
	fclose(pFile);
}


//void QMSLCallBack_FireHose(HANDLE hQMSLContext,
//				  char	*szMsg,
//				  unsigned short iMsgSize)
//{
//	if (NULL ==g_pFirehoseDownload)
//		return;
//
//	g_pFirehoseDownload->RecordQMSLCallBack(szMsg);
//}

bool CDownload8994::FlashProgrammer()
{
	unsigned int iPort = m_i_COMPort;
	HANDLE hPhone = NULL;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_FireHose;

	//Connect Server
	pQLIB_QPHONEMS_ConnectServer_Sahara ConnectServer_Sahara = (pQLIB_QPHONEMS_ConnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_Sahara");
	if(ConnectServer_Sahara != NULL)
	{
		unsigned long version = 0;
		unsigned long msmid = 0;
		unsigned long timeout = 0;
		unsigned char bGetMsmId = 0;
		int mode = 0;
		hPhone = ConnectServer_Sahara(iPort, &version, &msmid, bGetMsmId, mode, timeout, SWDL_QPHONEMS_CB);

		if (hPhone == 0)
		{
			//SetUIMessage("Connect Sahara Server Fail");
			return false;
		}
		else
		{
			//SetUIMessage("Connect Sahara Server Success");
		}
	}
	else
	{
		return false;
	}

	//Configure Callback
	pQLIB_QPHONEMS_SaharaConfigureCallback Sahara_ConfigureCallback = (pQLIB_QPHONEMS_SaharaConfigureCallback)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_SaharaConfigureCallback");
	Sahara_ConfigureCallback(hPhone, SWDL_QPHONEMS_CB);

	//Flash Programmer
	pQLIB_QPHONEMS_Sahara_FlashProgrammer Sahara_FlashProgrammer = (pQLIB_QPHONEMS_Sahara_FlashProgrammer)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_Sahara_FlashProgrammer");
	if(Sahara_FlashProgrammer != NULL)
	{
		char szHexPath[MAX_PATH] = {0};
		unsigned char szResult = 0;

		char cHexPath[256] = {0};
		//strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());

		sprintf_s(szHexPath, MAX_PATH, "%s", m_str_hex.c_str());
		//sprintf_s(szHexPath, MAX_PATH, "%s", m_strProgrammerFilePath.c_str());

		szResult = Sahara_FlashProgrammer(hPhone, szHexPath);
		if(szResult) //1
		{
			//SetUIMessage("Flash Programmer Success");
		}
		else         //0
		{
		//	SetUIMessage("Flash Programmer Fail");
			return false;
		}
	}
	else
	{
		return false;
	}

	//Disconnect Server
	pQLIB_DisconnectServer_Sahara DisconnectServer_Sahara = (pQLIB_DisconnectServer_Sahara)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_Sahara");
	if (hPhone != NULL && DisconnectServer_Sahara != NULL)
	{
		DisconnectServer_Sahara(hPhone);
		//SetUIMessage("Disconnect Sahara Server Success");
	}
	hPhone = NULL;


	return true;
}

void swdlCallBack(
   HANDLE hContextID,
   char * sMsg,
   unsigned short iMsgSize)
{

   printf("\n%s", sMsg);

	FILE *pFile = NULL;
	pFile = fopen("C:\\FireHoseQMSLCallBack_8916.log", "wb");
	fseek(pFile, 0, SEEK_END);
	fwrite(sMsg, 1, sizeof(sMsg)-1, pFile);
	fclose(pFile);
}

bool CDownload8994::FireHose(void)
{
	unsigned int iPort = m_i_COMPort;
	unsigned char szResult = 0;

	HANDLE hPhone = NULL;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_FireHose;

	std::string m_strRawProgramFile;
	std::string m_strRawProgramFilePath;
	std::string m_strPatchFile;
	std::string m_strPatchFilePath;


	Sleep(3000);//wait for 3 second form qualcomm demo code


	//pQLIB_QPHONEMS_QLIB_SetLogFlags SetLogFlags = (pQLIB_QPHONEMS_QLIB_SetLogFlags)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_SetLogFlags");
	//pQLIB_QPHONEMS_QLIB_StartLogging StartLogging  = (pQLIB_QPHONEMS_QLIB_StartLogging)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_StartLogging");
	/*loading Lib*/
	pQLIB_QPHONEMS_ConnectServer_FireHose ConnectServer_FireHose = (pQLIB_QPHONEMS_ConnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_FireHose");
	pQLIB_DisconnectServer_FireHose DisconnectServer_FireHose = (pQLIB_DisconnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_FireHose");
	pQLIB_QPHONEMS_FireHosePower FireHose_Reboot = (pQLIB_QPHONEMS_FireHosePower)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHosePower");
//	pQLIB_QPHONEMS_FireHosePower_WithDelay FireHosePower_WithDelay = (pQLIB_QPHONEMS_FireHosePower_WithDelay)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHosePower_WithDelay");

	//Configure Callback
	//pQLIB_QPHONEMS_FireHoseConfigureCallback FireHose_ConfigureCallback = (pQLIB_QPHONEMS_FireHoseConfigureCallback)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseConfigureCallback");
//	FireHose_ConfigureCallback(hPhone, SWDL_QPHONEMS_CB);

	//FireHose_ConfigureCallback(g_hResourceContext, swdlCallBack);
  //  QLIB_QPHONEMS_FireHoseConfigureCallback(g_hResourceContext, swdlCallBack);

 //   SetLogFlags( g_hResourceContext, LOG_ALL );
 //   StartLogging( g_hResourceContext,  "C:\\QlibDemo_Sahara_Firehose.txt");


	//Connect Server

	hPhone = ConnectServer_FireHose(iPort, swdlCallBack);
	if (hPhone == 0)
	{
		//SetUIMessage("Connect Firehose Server Fail");
		return false;
	}
	else
	{
		//SetUIMessage("Connect Firehose Server Success");
	}



	//Ping device
	pQLIB_QPHONEMS_FireHoseNOP FireHose_NOP = (pQLIB_QPHONEMS_FireHoseNOP)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseNOP");
	szResult = FireHose_NOP(hPhone);
	if(szResult) //1
	{
		//SetUIMessage("Ping Firehose device Success");
	}
	else         //0
	{
		//SetUIMessage("Ping Firehose device Fail");
		return false;
	}

	//Configure
	char *MaxPayloadSizeToTargetInBytes = "49152";
	char *memory_name = "eMMC";
	char *target_name = "8x26";
	char *AckPacketNumber = "100";
	unsigned char bUseAckRawDataAttr = false;
	unsigned char bUseSkipWriteAttr = false;
	unsigned char bUseAlwaysValidateAttr = false;
	unsigned char bUseVerboseAttr = false;
	unsigned char bUseAckRawDataEveryNumPackets = false;

	pQLIB_QPHONEMS_FireHoseConfigure FireHose_Configure = (pQLIB_QPHONEMS_FireHoseConfigure)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseConfigure");
	szResult = FireHose_Configure( hPhone,
		MaxPayloadSizeToTargetInBytes,
		memory_name,
		target_name,
		AckPacketNumber,
		bUseAckRawDataAttr,
		bUseSkipWriteAttr,
		bUseAlwaysValidateAttr,
		bUseVerboseAttr,
		bUseAckRawDataEveryNumPackets
		);

	if(szResult) //1
	{
		//SetUIMessage("Configure Firehose Success");
	}
	else         //0
	{
		//SetUIMessage("Configure Firehose Fail");
		return false;
	}

	//Download image
	char szRawProgramFile[MAX_PATH] = {0};
	char szPatchFile[MAX_PATH] = {0};
	float imageSizeInMB = 0.0;
	float throughput = 0.0;

		//char cHexPath[256] = {0};
		//strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());
		//cret = Sahara_FlashProgrammer(m_hPhone, cHexPath);

	//CString csStrRawProgramFilePath, csStrPatchFilePath;
	CString csImgPath =  m_str_hex.c_str();
	csImgPath = csImgPath.Left (csImgPath.ReverseFind(_T('\\'))); 
	
	//csStrRawProgramFilePath = csImgPath + _T("\\rawprogram0.xml");
	//m_strPatchFilePath = csImgPath + _T("\\patch0_fastbootimage.xml.xml");
	m_strRawProgramFilePath = csImgPath + _T("\\rawprogram0_fastbootimage.xml");
	m_strPatchFilePath = csImgPath + _T("\\patch0_fastbootimage.xml");
	sprintf_s(szRawProgramFile, MAX_PATH, "%s", m_strRawProgramFilePath.c_str());
	sprintf_s(szPatchFile, MAX_PATH, "%s", m_strPatchFilePath.c_str());

	//	GetCurrentDirectory(MAX_PATH, szNowPath);
	//CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
	//SetCurrentDirectory(WorkDir);

	//SetUIProgress(10, 100);
	//SetUIMessage("Download image...");

	//orig: pass
	pQLIB_QPHONEMS_UploadEmmcImage_FireHose UploadEmmcImage_FireHose = (pQLIB_QPHONEMS_UploadEmmcImage_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_UploadEmmcImage_FireHose");
	szResult = UploadEmmcImage_FireHose(hPhone, szRawProgramFile, szPatchFile, &imageSizeInMB, &throughput);
	if(szResult) //1
	{
	//	SetUIMessage("Download image Success");
	}
	else         //0
	{
		//SetUIMessage("Download image Fail");
		return false;
	}

	//Reboot device

	szResult = FireHose_Reboot(hPhone, _T("reset"));
	if(szResult) //1
	{
		//SetUIMessage("Reboot device Success");
	}
	else         //0
	{
		//SetUIMessage("Reboot device Fail");
		return false;
	}



    printf("\n----------------QLIB_DisconnectServer_FireHose---------------------------\n");
    DisconnectServer_FireHose(hPhone);




	return true;
}

bool CDownload8994::IsPathExist(const CString& path)
{
	return (::GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}



/*****************************************************************************
* Function name: DDRTest      
* Summary      : DDR Check.
* Parameters   : void
*                
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/28          First version             
*****************************************************************************/


bool CDownload8994::GetCOMPortDevByWDK(std::map<std::string, std::string>& map_strstrCOMDevice)
{
	map_strstrCOMDevice.clear();

	BOOL bContinue = FALSE;

	// 1. Retrieves the GUID(s) associated with the specified class name "Ports"
	DWORD uiGuidSize = 0;
	GUID* ptGuidBuf = NULL;

	::SetupDiClassGuidsFromNameA("Ports", NULL, 0, &uiGuidSize);

	if (uiGuidSize > 0)
	{
		ptGuidBuf = new GUID[uiGuidSize];
		ASSERT(ptGuidBuf != NULL);

		if (ptGuidBuf != NULL)
		{
			bContinue = ::SetupDiClassGuidsFromNameA("Ports", ptGuidBuf, uiGuidSize, &uiGuidSize);
		}
	}

	// 2. Returns a device information set that contains all devices of a specified class
	HDEVINFO hDevInfoSet = INVALID_HANDLE_VALUE;

	if (bContinue)
	{
		hDevInfoSet = ::SetupDiGetClassDevsA(ptGuidBuf, NULL, NULL, DIGCF_PRESENT);
		bContinue = (hDevInfoSet != INVALID_HANDLE_VALUE);
	}

	// 3. Start enumeration
	DWORD uiIndex = 0;

	while (bContinue)
	{
		// Returns one context structure for a device information element of a device information set.
		SP_DEVINFO_DATA tDevInfo;
		tDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		if ((bContinue = ::SetupDiEnumDeviceInfo(hDevInfoSet, uiIndex, &tDevInfo)))
		{
			// Opens a registry storage key for device-specific configuration information
			HKEY hKey = ::SetupDiOpenDevRegKey(hDevInfoSet, &tDevInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

			if (hKey != NULL)
			{
				// Read the name of the port.
				char szDev[256];
				DWORD uiType = 0, uiDevSize = 256;
				memset(szDev, 0, 256);

				if ((::RegQueryValueExA(hKey, "PortName", NULL, &uiType, (LPBYTE)szDev, &uiDevSize) == ERROR_SUCCESS) && (uiType == REG_SZ))
				{
					std::string strTmp = szDev;

					memset(szDev, 0, 256);
					uiDevSize = 256;
					if (SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &tDevInfo, SPDRP_DEVICEDESC, &uiType, (PBYTE)szDev, uiDevSize, &uiDevSize) && (uiType == REG_SZ))
						map_strstrCOMDevice[strTmp] = std::string(szDev);
					else
						map_strstrCOMDevice[strTmp] = std::string("");
				}

				// Close the key.
				::RegCloseKey(hKey);
			}
		}

		++uiIndex;
	}

	// clean-up.
	if (ptGuidBuf != NULL)
		delete [] ptGuidBuf;

	if (hDevInfoSet != INVALID_HANDLE_VALUE)
		::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return true;
}

bool CDownload8994::DetectDiagPort()
{

	std::map<std::string, std::string> map_strstrCOMDeviceName;
	m_strKeyword.assign("QUALCOMM");

	if (GetCOMPortDevByWDK(map_strstrCOMDeviceName))
	{
		std::map<std::string, std::string>::iterator itr;

		if (map_strstrCOMDeviceName.size() < 1)
		{
			//CmdLog("No COM device detected");
			return false;
		}
		else
		{
			bool bFound = false;
			for (itr = map_strstrCOMDeviceName.begin(); itr != map_strstrCOMDeviceName.end(); ++itr)
			{
				std::string strDev = itr->first;
				std::string strDevName = itr->second;
				std::transform(strDevName.begin(), strDevName.end(), strDevName.begin(), ::toupper);

				if (strDevName.find(m_strKeyword) !=  std::string::npos)
				{
					bFound = true;
					size_t iPosition;
					if ((iPosition = strDev.find("COM")) != std::string::npos)
						m_strPROLIFICcomPort = m_strDeviceName = strDev.substr(3, (strDev.size() - 3));
					else
						m_strPROLIFICcomPort = m_strDeviceName = strDev;
				}
			}

			if (! bFound)
			{

				AddMsg(_T("Fail to find QUALCOMM Comport"), DownloadStep::None, 100);
				return false;
			}

		}
	}
	else
	{
		//CmdLog("Fail to detect COM device with WDK");
		return false;
	}


	return true;
}




/*****************************************************************************
* Function name: GetErrorCode     
* Summary      : Return error code.
* Parameters   : 
*                
* Return       : 
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19          First version             
*****************************************************************************/
CString CDownload8994::GetErrorCode(void) 
{
	return m_str_errorCode;
}

/*****************************************************************************
* Function name: SetImageFilePath      
* Summary      : Set image file path to member variable.
* Parameters   : i_imageType : image type
*                str_filePath: image file path
* Return       : bool
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0           Alex.Chen     2011/01/19            First version             
*****************************************************************************/
bool CDownload8994::SetImageFilePath(const int i_imageType, const std::string& str_filePath)
{
	bool b_res = false;

	/* Check Input */
	if(str_filePath.c_str() == _T("")) 
	{
		return false;
	}

	switch(i_imageType)
	{
	case IMAGE_TYPE_8916::Q8K_HEX:
		m_str_hex = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_MSIMAGE:
		m_str_msimage = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PARTITIONBIN:
		m_str_partitionBin = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SBL1:
		m_str_sbl1 = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_HYP:
		m_str_hyp = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_HYPBAK:
		m_str_hypbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RPM:
		m_str_rpm = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_TZ:
		m_str_tz = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_APPBOOT:
		m_str_appboot = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SBL1BAK:
		m_str_sbl1bak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RPMBAK:
		m_str_rpmbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_TZBAK:
		m_str_tzbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_ABOOTBAK:
		m_str_abootbak = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_BOOT:
		m_str_boot = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_RECOVERY:
		m_str_recovery = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SYSTEM:
		m_str_system = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_CACHE:
		m_str_cache = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PERSIST:
		m_str_persist = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_QGLOGFILTER:
		m_str_qlogfilter = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_QGLOG:
		m_str_qglog = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_NONHLOS:
		m_str_nonhlos = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_OEM:
		m_str_oem = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_SPLASH:
		m_str_splash = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_USERDATA:
		m_str_userdata = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_DRM:
		m_str_drm = str_filePath;
		break;

	case IMAGE_TYPE_8916::Q8K_PASSPORT:
		m_str_passport = str_filePath;
		break;
	

	default:
		return false;
	}

	/* Check image file if exist */
	if(::_taccess(str_filePath.c_str(), 0) == 0) 
	{
		b_res = true;
	} 
	else 
	{
		b_res = false;
	}

	return b_res;
}




/*****************************************************************************
* Function name: Reboot      
* Summary      : Reboot mobile.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen     2011/09/19             First version             
*****************************************************************************/
bool CDownload8994::Reboot(void)
{
	AddMsg("Fastboot reboot.", DownloadStep::None, 100);

	///* Get ADB */
	//if (!GetADB()) 
	//{

	//	//StreamingDLeMMCLog(8194);

	//	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	///* Write command "reboot" */
	//if (m_p_adbDevice->Write("reboot", 6, 60000)) 
	//{
	//	/* Read response command "reboot" */
	//	if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY) 
	//	{
	//	} 
	//	else 
	//	{
	//		AddMsg("Read reboot command response Fail.", DownloadStep::None, 100);
	//		return false;
	//	}
	//}
	//else 
	//{
	//	AddMsg("Write reboot command Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	AddMsg("Fastboot reboot Success.", DownloadStep::None, 100);
	return true;
}




/*****************************************************************************
* Function name: ReadFA      
* Summary      : Read FA.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8994::ReadFA(char* sz_FAData)
{
	AddMsg("Read FA.", DownloadStep::None, 100);

	/* Get ADB */
	//if (!GetADB()) 
	//{
	//	//StreamingDLeMMCLog(8194);

	//	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	//int i_length = FALENGTH;
	//bool b_result = false;
	//char sz_buffer[512] = {0};

	///* dump protocol --- "dump:%s"  example:"dump:otpfa" */
	//char sz_command[64] = {0};
	//sprintf(sz_command, "dump:%s", "otpfa");

	//if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	//{
	//	RESPONSE enum_readRosponse = UNKNOW;
	//	enum_readRosponse = m_p_adbDevice->ReadResponse(60000);
	//	if (enum_readRosponse == RESPONSE::OKAY)
	//	{
	//		/* upload command --- "upload:%08x"  example:"upload:0007d000" */
	//		char sz_command[64] = {0};
	//		sprintf(sz_command, "upload:%08x", i_length);
	//		if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	//		{
	//			b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA);
	//			if (b_result) 
	//			{
	//				unsigned long l_readLen = 0;
	//				if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 60000) && (l_readLen == FALENGTH)) 
	//				{
	//					memcpy(sz_FAData, sz_buffer, FALENGTH);
	//				}
	//				else
	//				{
	//					AddMsg("Read upload data Fail.", DownloadStep::None, 100);
	//					return false;
	//				}

	//				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
	//				if (!b_result)
	//				{
	//					AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
	//					return false;
	//				}
	//			}
	//			else
	//			{
	//				AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
	//				return false;
	//			}
	//		} 
	//		else 
	//		{
	//			AddMsg("Write upload command Fail.", DownloadStep::None, 100);
	//			return false;
	//		}
	//	}
	//	/* FA is empty */
	//	else if (enum_readRosponse == RESPONSE::FAILFAEMPTY)
	//	{
	//		char sz_buffer[512] = {0};
	//		memcpy(sz_FAData, sz_buffer, FALENGTH);
	//	} 
	//	else 
	//	{
	//		AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
	//		return false;
	//	}
	//}
	//else
	//{
	//	AddMsg("Write dump command Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	AddMsg("Read FA Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: WriteFA      
* Summary      : Write FA.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/09/27          First version             
*****************************************************************************/
bool CDownload8994::WriteFA(char* sz_FAData)
{
	AddMsg("Write FA.", DownloadStep::None, 100);

	/* Get ADB */
	//if (!GetADB()) 
	//{
	//	//StreamingDLeMMCLog(8194);

	//	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
	//	return false;
	//}
	
	int i_length = FALENGTH;
	bool b_result = false;

	///* download protocol --- "download:%08x"  example:"download:00162c00" */
	//char sz_command[64] = {0};
	//sprintf(sz_command, "download:%08x", i_length);
	//if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000))
	//{
	//	if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::DATA)
	//	{
	//		char* c_p_data = (char*) sz_FAData;
	//		int i_count = 0;
	//		bool b_writeSuccess = true;

	//		// Max write byte:4096
	//		while (i_length > 0 && b_writeSuccess) 
	//		{
	//			int xfer = (i_length > 4096) ? 4096 : i_length;

	//			// ADB Write
	//			b_writeSuccess = m_p_adbDevice->Write(c_p_data, xfer);
	//			i_count   += xfer;
	//			i_length -= xfer;
	//			c_p_data  += xfer;
	//		};

	//		if (!b_writeSuccess)
	//		{
	//			AddMsg("Write download data Fail.", DownloadStep::None, 100);
	//			return false;
	//		}

	//		if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
	//		{
	//			char sz_command[64] = {0};
	//			sprintf(sz_command, "flash:%s", "otpfa");

	//			if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 120000))
	//			{
	//				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
	//				if (!b_result) 
	//				{
	//					AddMsg("Read flash response Fail.", DownloadStep::None, 100);
	//					return false;
	//				}
	//			} 
	//			else 
	//			{
	//				AddMsg("Write flash command Fail.", DownloadStep::None, 100);
	//				return false;
	//			}
	//		}
	//		else 
	//		{
	//			AddMsg("Read download data response Fail.", DownloadStep::None, 100);
	//			return false;
	//		}
	//	} 
	//	else
	//	{
	//		AddMsg("Read download response Fail.", DownloadStep::None, 100);
	//		return false;
	//	}
	//}
	//else 
	//{
	//	AddMsg("Write download command Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	AddMsg("Write FA Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: ReadFASector      
* Summary      : Read FA --- New FA structure.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/11/23          First version             
*****************************************************************************/
bool CDownload8994::ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	//bool b_speedUp = false;
	//bool b_reOpenAfterReset = false;
	//bool b_res = true;
	//DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;
	//	
	//if (!GetADB()) 
	//{
	//	//StreamingDLeMMCLog(8194);

	//	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
	//	return false;
	//}
	AddMsg("Read FA.", DownloadStep::None, 100);

	//bool b_result = false;
	//int  i_length = i_sectorSize;
	//char sz_buffer[512] = {0};

	///* dump protocol --- "dump:%s"  example:"dump:otpfa0x0+512" */
	//char sz_sectorSize[32] = {0};
	//sprintf(sz_sectorSize, "%d+%d", i_sectorNum * 512, i_sectorSize);
	//char sz_command[1024] = {0};
	//sprintf(sz_command, "dump:%s%s", "Qfa@", sz_sectorSize);

	//if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	//{
	//	RESPONSE enum_readRosponse = UNKNOW;
	//	enum_readRosponse = m_p_adbDevice->ReadResponse(60000);
	//	if (enum_readRosponse == RESPONSE::OKAY)
	//	{
	//		/* upload command --- "upload:%08x"  example:"upload:0007d000" */
	//		char sz_command[64] = {0};
	//		sprintf(sz_command, "upload:%08x", i_length);
	//		if (m_p_adbDevice->Write(sz_command, strlen(sz_command), 60000)) 
	//		{
	//			b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
	//			if (b_result) 
	//			{
	//				unsigned long l_readLen = 0;
	//				if (m_p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen, 60000) && (l_readLen == FALENGTH)) 
	//				{
	//					memcpy(sz_sectorData, sz_buffer, i_sectorSize);
	//				}
	//				else
	//				{
	//					AddMsg("Read upload data Fail.", DownloadStep::None, 100);
	//					return false;
	//				}

	//				b_result = (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY);
	//				if (!b_result)
	//				{
	//					AddMsg("Read upload data response Fail.", DownloadStep::None, 100);
	//					return false;
	//				}
	//			}
	//			else
	//			{
	//				AddMsg("Read upload command response Fail.", DownloadStep::None, 100);
	//				return false;
	//			}
	//		} 
	//		else 
	//		{
	//			AddMsg("Write upload command Fail.", DownloadStep::None, 100);
	//			return false;
	//		}
	//	}
	//	/* FA is empty */
	//	else if (enum_readRosponse == RESPONSE::FAILFAEMPTY)
	//	{
	//		char sz_buffer[512] = {0};
	//		memcpy(sz_sectorData, sz_buffer, i_sectorSize);
	//	} 
	//	else 
	//	{
	//		AddMsg("Read dump command response Fail.", DownloadStep::None, 100);
	//		return false;
	//	}
	//}
	//else
	//{
	//	AddMsg("Write dump command Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	AddMsg("Read FA Success.", DownloadStep::None, 100);
	return true;
}

/*****************************************************************************
* Function name: WriteFASector      
* Summary      : Write FA --- New FA structure.
* Parameters   : 
* Return       : bool.
*                
* Exception    : 
* Version       Author          Date                Abstract                 
* 1.0          Alex.Chen        2011/11/23          First version             
*****************************************************************************/
bool CDownload8994::WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize)
{
	AddMsg("Write FA.", DownloadStep::None, 100);

	///* Get ADB */
	//if (!GetADB()) 
	//{
	//	////StreamingDLeMMCLog(8194);

	//	AddMsg("Get ADB Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	//int i_length = i_sectorSize;
	bool b_result = false;

	///* fastboot oem write Qfa offset length value */
	//char sz_command[1024] = {0};
	//int offset = i_sectorNum * 512;

	//if(i_sectorNum == 0){
	//	sprintf(sz_command, "oem write Qfa %d %d %s", offset, 255, sz_sectorData);
	//	b_result = runWriteFASector(sz_command);
	//	if (b_result){
	//		sprintf(sz_command, "oem write Qfa %d %d %s", offset+256, 512-1, sz_sectorData+256);
	//		b_result = runWriteFASector(sz_command);
	//	}
	//}
	//else{
	//	sprintf(sz_command, "oem write Qfa %d %d %s", offset, offset+i_sectorSize-1, sz_sectorData);
	//	b_result = runWriteFASector(sz_command);
	//}
	
	return b_result;
}


bool CDownload8994::runWriteFASector(char *sz_sectorData){
	//if (m_p_adbDevice->Write(sz_sectorData, strlen(sz_sectorData), 60000))
	//{
	//	if (m_p_adbDevice->ReadResponse(60000) == RESPONSE::OKAY)
	//	{
	//	} 
	//	else 
	//	{
	//		AddMsg("Read download response Fail.", DownloadStep::None, 100);
	//		return false;
	//	}
	//}
	//else 
	//{
	//	AddMsg("Write download command Fail.", DownloadStep::None, 100);
	//	return false;
	//}

	AddMsg("Write FA Success.", DownloadStep::None, 100);
	return true;
}


bool CDownload8994::SpiltString(CString str_sourceString, CString str_delimiter, CStringArray& stra_strArg)
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



static void saveLog_Value(CString message)
{
	CString pthLog;
	pthLog = _T("D:\\Log\\UI_RD");
	
	if (::GetFileAttributes(pthLog) != INVALID_FILE_ATTRIBUTES)
		CreateDirectory(pthLog, NULL);

	pthLog = _T("D:\\Log\\UI_RD\\system.CSV");

	CStdioFile fLog;
	UINT nOpenFlags =
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite;

	if (fLog.Open(pthLog, nOpenFlags))
	{
		fLog.SeekToEnd();
		fLog.WriteString(message);
		fLog.Close();
	}
}



//
//
//bool CDownload8994::bAdbCMD(CString Command, char* output, char* ErrorCode)
//{
//	bool isOk = false;
//	DWORD nPipeSize = 1024 * 1024; //1M pipeline
//
//	CString pthToolDir;
//	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
//	pthToolDir.ReleaseBuffer();
//	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
//	CString path_adb = pthToolDir + _T("\\adb.exe");
//	if (_taccess(path_adb, 0) == -1)
//	{
//		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize))
//	{
//		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
//		return false;
//	}
//
//	//HANDLE hProcess = NULL;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//
//	Command = _T("\"") + path_adb + _T("\" ") + Command;
//	TRACE(_T("Cmd: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
//	{
//		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 10 * 1000); // timeout in 10 seconds
//
//		CloseHandle(hWrite);
//		hWrite = NULL;
//		//if timeout then exit the process
//		if (TimeOutSignal == WAIT_TIMEOUT)
//		{
//			isOk = false;
//			TerminateProcess(processInfo.hProcess, 0);
//			strcpy(ErrorCode, "ERROR: Adb timeout");
//		}
//		else
//		{
//
//			isOk = true;
//			DWORD bytesRead;
//			char* message = new char[nPipeSize];
//			memset(message, 0, sizeof(message));
//			::ReadFile(hRead, message, nPipeSize, &bytesRead, NULL);
//			message[bytesRead] = '\0';
//
//			strcpy(output, message);
//			strcpy(ErrorCode, "Adb command ok");
//			delete [] message;
//		}
//	}
//	else
//	{
//		isOk = false;
//		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
//	}
//	Command.ReleaseBuffer();
//	CloseHandle(hRead);
//	if (hWrite)CloseHandle(hWrite);
//	//CloseHandle(hProcess);
//
//	CloseHandle(processInfo.hProcess);
//	CloseHandle(processInfo.hThread);
//	//hProcess = NULL;
//
//	return isOk;
//}

//bool bCallAdbFastbootCMD(CString Command, char* output, char* ErrorCode, char* sz_FindData);
bool CDownload8994::bCallAdbFastbootCMD(CString csAdbFastboot, CString Command, char* output, char* ErrorCode, CString cs_FindData)
{
	bool isOk = false;
	CString pthToolDir;

	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	CString path_adb_fastboot = pthToolDir + _T("\\") + csAdbFastboot;
	
	if (_taccess(path_adb_fastboot, 0) == -1)
	{
		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
		return false;
	}

	HANDLE hRead, hWrite;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, BUFFER_SIZE))
	{
		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
		return false;
	}

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= SW_HIDE;
	startupInfo.hStdError = hWrite;
	startupInfo.hStdOutput = hWrite;

	Command = _T("\"") + path_adb_fastboot + _T("\" ") + Command;
	TRACE(_T("Cmd: %s\n"), Command);

	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 100 * 1000); // timeout in 10 seconds

		CloseHandle(hWrite);
		hWrite = NULL;
		//if timeout then exit the process
		if (TimeOutSignal == WAIT_TIMEOUT)
		{
			isOk = false;
			TerminateProcess(processInfo.hProcess, 0);
			strcpy(ErrorCode, "ERROR: fastboot timeout");
		}
		else
		{
			if ( (cs_FindData.Find("NULL") == -1) ) {//need to check return payload
			   DWORD dwRead;
			   CHAR chBuf[4096]; 
			   bool bSuccess = FALSE;

			   for (int i = 0; i < 60; i++) //get QPHONE
			   { 
				  bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				  chBuf[dwRead] = '\0';
				  CString csBuf = chBuf;
				  if(csBuf.Find(cs_FindData) != -1) {
					  isOk = true; //get it
					  break;
				  }
				  if( ! bSuccess || dwRead == 0 ) break; 
			   }
			    strcpy(ErrorCode, _T("Adb command ok"));
			}
			else /*do not neet to find anything*/
			{
				//strcpy(output, message);
				
			//	delete [] message;
			}
		}
	}
	else
	{
		isOk = false;
		strcpy(ErrorCode, "ERROR: Execute fastboot.exe fail!");
	}

	Command.ReleaseBuffer();
	if(hRead){
		CloseHandle(hRead);
		hRead = NULL;
	}
	if (hWrite){
		CloseHandle(hWrite);
		hWrite = NULL;
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	processInfo.hProcess = NULL;
	processInfo.hThread = NULL;


	return isOk;
}


//bool CDownload8994::bGetFastboot(CString Command, char* output, char* ErrorCode, int nPhoneIndex)
//{
//	bool isOk = false;
//	CString csQphone;
//	//DWORD nPipeSize = 4096; //1M pipeline
//	//DWORD nPipeSize = 2048 * 1024; //1M pipeline
//
//	CString pthToolDir;
//	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
//	pthToolDir.ReleaseBuffer();
//	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
//	CString path_adb = pthToolDir + _T("\\fastboot.exe ");
//	
//	if (_taccess(path_adb, 0) == -1)
//	{
//		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	if (!CreatePipe(&hRead, &hWrite, &sa, BUFFER_SIZE))
//	{
//		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
//		return false;
//	}
//
//	//HANDLE hProcess = NULL;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//
//	//Command = _T("\"") + path_adb + _T("\" ") + Command;
//	Command = path_adb +  Command;
//	TRACE(_T("Cmd: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
//	{
//		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 100 * 1000); // timeout in 10 seconds
//
//		CloseHandle(hWrite);
//		hWrite = NULL;
//		//if timeout then exit the process
//		if (TimeOutSignal == WAIT_TIMEOUT)
//		{
//			isOk = false;
//			TerminateProcess(processInfo.hProcess, 0);
//			strcpy(ErrorCode, "ERROR: fastboot timeout");
//		}
//		else
//		{
//			if ( nPhoneIndex != 0){
//
//			   DWORD dwRead, dwWritten; 
//			   CHAR chBuf[4096]; 
//			   bool bSuccess = FALSE;
//			   HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//			   for (int i = 0; i < 60; i++) //get QPHONE
//			   { 
//				  bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
//				  CString csBuf = chBuf;
//				  csQphone.Format(_T("QPHONE%d"), nPhoneIndex);
//				  if(csBuf.Find(csQphone) != -1) {
//					  isOk = true; //get it
//					  break;
//				  }
//				  if( ! bSuccess || dwRead == 0 ) break; 
//			   }
//			    strcpy(ErrorCode, _T("Adb command ok"));
//			}
//			else /*do not neet to find anything*/
//			{
//				//strcpy(output, message);
//				
//			//	delete [] message;
//			}
//		}
//	}
//	else
//	{
//		isOk = false;
//		strcpy(ErrorCode, "ERROR: Execute fastboot.exe fail!");
//	}
//	Command.ReleaseBuffer();
//	CloseHandle(hRead);
//	if (hWrite)CloseHandle(hWrite);
//	//CloseHandle(hProcess);
//
//	CloseHandle(processInfo.hProcess);
//	CloseHandle(processInfo.hThread);
//	//hProcess = NULL;
//
//	return isOk;
//}


//bool CDownload8994::bAdbCMD(CString Command, char* output, char* ErrorCode, int nPhoneIndex)
//{
//	bool isOk = false;
//	CString csQphone;
//	CString csBuf;
//	////1M pipeline
//
//	CString pthToolDir;
//	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
//	pthToolDir.ReleaseBuffer();
//	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
//	CString path_adb = pthToolDir + _T("\\adb.exe ");
//	
//	if (_taccess(path_adb, 0) == -1)
//	{
//		strcpy(ErrorCode, "ERROR: No adb.exe exist!");
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	if (!CreatePipe(&hRead, &hWrite, &sa, BUFFER_SIZE))
//	{
//		strcpy(ErrorCode, "ERROR: CreatePipe fail!");
//		return false;
//	}
//
//	//HANDLE hProcess = NULL;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//
//	//Command = _T("\"") + path_adb + _T("\" ") + Command;
//	Command = path_adb +  Command;
//	TRACE(_T("Cmd: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo))
//	{
//		DWORD TimeOutSignal = WaitForSingleObject(processInfo.hProcess, 100 * 1000); // timeout in 10 seconds
//
//		//CloseHandle(hWrite);
//		//hWrite = NULL;
//	//	//if timeout then exit the process
//		if (TimeOutSignal == WAIT_TIMEOUT)
//		{
//			isOk = false;
//			//TerminateProcess(processInfo.hProcess, 0);
//			strcpy(ErrorCode, "ERROR: Adb timeout");
//		}
//		else
//		{
//			if ( nPhoneIndex != 0){
//
//			   DWORD dwRead;// dwWritten; 
//			   CHAR chBuf[BUFFER_SIZE] = {0}; 
//			   bool bSuccess = FALSE;
//			 //  HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//			   for (int i = 0; i < 60; i++) //get QPHONE, maxmum read time = 60
//			   { 
//				  bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
//				  csBuf = chBuf;
//				  csQphone.Format(_T("QPHONE%d"), nPhoneIndex);
//				  if(csBuf.Find(csQphone) != -1) {
//					  isOk = true; //get it
//					  break;
//				  }
//				  if( ! bSuccess || dwRead == 0 ) break; 
//			   }
//			   strcpy(ErrorCode, _T("Adb command ok"));
//			}
//			else /*do not neet to find anything*/
//			{
//				//strcpy(output, message);
//				
//			//	delete [] message;
//			}
//		}
//	}
//	else
//	{
//		isOk = false;
//		strcpy(ErrorCode, "ERROR: Execute adb.exe fail!");
//	}
//	Command.ReleaseBuffer();
//	if(hRead){
//		CloseHandle(hRead);
//		hRead = NULL;
//	}
//	if (hWrite){
//		CloseHandle(hWrite);
//		hWrite = NULL;
//	}
//	//CloseHandle(hProcess);
//
//	CloseHandle(processInfo.hProcess);
//	CloseHandle(processInfo.hThread);
//	processInfo.hProcess = NULL;
//	processInfo.hProcess = NULL;
//
//	return isOk;
//}

bool CDownload8994::bFastbootDL(CString folderPath){

	CString csExeCMD;
	//csExeCMD.Format(_T("CMD.EXE /C %s") ,folderPath);
	//csExeCMD.Format(_T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150729_debug\\fastboot.exe %s\"") ,folderPath);
	csExeCMD.Format(_T("%s") ,folderPath);
	/* Run */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < csExeCMD.GetLength(); i ++) 
	{
		sz_commandLine[i] = csExeCMD[i];   
	}
	PROCESS_INFORMATION pi_bFastbootDL = {0};;
	STARTUPINFO si = {0};
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 

	BOOL b_createRes = FALSE;	

	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, true, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi_bFastbootDL);

	if(b_createRes == FALSE) 
	{
		AddMsg("b_createRes Fail", None, 10);
		return false;
	}

	DWORD dw_waitResult = 0;
	DWORD dw_timeout;
	dw_timeout = 180 * 1000;

	
	dw_waitResult = ::WaitForSingleObject(pi_bFastbootDL.hProcess, dw_timeout);

	CloseHandle(pi_bFastbootDL.hThread);
	pi_bFastbootDL.hThread = NULL;	
	::CloseHandle(pi_bFastbootDL.hProcess);
	pi_bFastbootDL.hProcess = NULL;

	switch(dw_waitResult)
	{
	case WAIT_FAILED:
		::CloseHandle(pi_bFastbootDL.hProcess);
		pi_bFastbootDL.hProcess = NULL;
		AddMsg("WAIT_FAILED Fail", None, 10);
		return false;

	case WAIT_TIMEOUT:
		::CloseHandle(pi_bFastbootDL.hProcess);
		pi_bFastbootDL.hProcess = NULL;
		AddMsg("WAIT_TIMEOUT Fail", None, 10);
		return false;

	case WAIT_OBJECT_0:
		break;
	}
	::CloseHandle(pi_bFastbootDL.hProcess);
	pi_bFastbootDL.hProcess = NULL;

	return true;
}



//bool CDownload8994::bFastbootDL_New2(char* szReturn, const char* szCmd1, const char* szCmd2)
//{
//	bool ret = true;
//	HANDLE hWrite, hRead;
//
//	PROCESS_INFORMATION processInfo;
//	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
//
//	SECURITY_ATTRIBUTES saAttr;
//	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//	saAttr.bInheritHandle = TRUE;
//	saAttr.lpSecurityDescriptor = NULL;
//
//	try
//	{
//		if (!CreatePipe(&hRead, &hWrite, &saAttr, 0))
//		{
//			throw _T("CreatePipe");
//		}
//
//		STARTUPINFO si = {0};
//		si.cb = sizeof(STARTUPINFO);
//		si.hStdError = hWrite;
//		si.hStdOutput = hWrite;
//		si.hStdInput = hRead;
//		si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
//		si.wShowWindow = SW_HIDE;
//
//		TCHAR szWorkingDir[MAX_PATH], szExePath[MAX_PATH];
//		TCHAR szCmdLine[1024];
//
//		// Set WorkPath and ExePath
//		GetModuleFileNameA(NULL, szExePath, MAX_PATH);
//		PathRemoveFileSpecA(szExePath);
//		_tcscpy(szWorkingDir, szExePath);
//		//PathAppend(szExePath, "adb.exe");
//	PathAppend(szExePath, "fastboot.exe");
//
//		//bool bIsAdbDevices = false;
//		//if ((strstr(szCmd1, "devices") != NULL) || (strstr(szCmd1, "devices") != NULL))
//		//	bIsAdbDevices = true;
//
//		//if (m_bIsMultiMode && (bIsAdbDevices == false))
//		//	_stprintf(szCmdLine, _T("\"%s\" -s QPHONE%d %s %s"), szExePath, (m_PhoneIndex + 1), szCmd1, szCmd2);
//		//else
//		//	_stprintf(szCmdLine, _T("\"%s\" %s %s"), szExePath, szCmd1, szCmd2);
//
//		_stprintf(szCmdLine, _T("\"%s\" %s"), szExePath, szCmd1);
//
//		if (!CreateProcess(
//		        NULL,
//		        szCmdLine,
//		        NULL,
//		        NULL,
//		        TRUE,
//		        CREATE_DEFAULT_ERROR_MODE,
//		        NULL,
//		        szWorkingDir,
//		        &si,
//		        &processInfo
//		    ))
//		{
//			throw _T("CreateProcess");
//		}
//		DWORD dw_waitResult = WaitForSingleObject(processInfo.hProcess, 10 * 1000);
//		switch(dw_waitResult)
//		{
//		case WAIT_FAILED:
//			::CloseHandle(processInfo.hProcess);
//			processInfo.hProcess = NULL;
//			AddMsg("WAIT_FAILED Fail", None, 10);
//			return false;
//
//		case WAIT_TIMEOUT:
//			::CloseHandle(processInfo.hProcess);
//			processInfo.hProcess = NULL;
//			AddMsg("WAIT_TIMEOUT Fail", None, 10);
//			return false;
//
//		case WAIT_OBJECT_0:
//			break;
//		}
//
//		// if needs return value
//		if (szReturn != NULL)
//		{
//			DWORD dwAvail, numread;
//			BOOL bRet = PeekNamedPipe(hRead, NULL, 0, NULL, &dwAvail, 0);
//			if (bRet && dwAvail > 0)
//			{
//				ReadFile(hRead, szReturn, dwAvail, &numread, 0);
//			}
//		}
//	}
//	catch (const TCHAR* szError)
//	{
//		LPVOID lpMsgBuf;
//		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		              NULL, GetLastError(),
//		              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		              (LPTSTR)&lpMsgBuf,
//		              0, NULL);
//
//		TCHAR szMessage[1024];
//		_stprintf(szMessage, "%s: %s", szError, lpMsgBuf);
//
//		LocalFree(lpMsgBuf);
//		ret = false;
//	}
//
//	CloseHandle(hRead);
//	CloseHandle(hWrite);
//	CloseHandle(processInfo.hProcess);
//	processInfo.hProcess = NULL;
//	CloseHandle(processInfo.hThread);
//	processInfo.hThread = NULL;
//
//	return ret;
//}

//bool CDownload8994::bFastbootDL_5(CString& path, CString& param, DWORD msTimeout, bool hasResponse)
//{
//	if (!IsPathExist(path)) {
//		return false;
//	}
//
//	HANDLE hRead, hWrite;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = NULL;
//	sa.bInheritHandle = TRUE;
//	DWORD nPipeSize = 1024;
//	if (!CreatePipe(&hRead, &hWrite, &sa, nPipeSize)) {
//		::AfxMessageBox(_T("ERROR: CreatePipe fail!\r\n"));
//		return false;
//	}
//
//	TCHAR szNowPath[MAX_PATH] = {0};
//	GetCurrentDirectory(MAX_PATH, szNowPath);
//	TRACE(_T("current path: %s\n"), szNowPath);
//	CString WorkDir = path.Left(path.ReverseFind(_T('\\')));
//	TRACE(_T("working path: %s\n"), WorkDir);
//	SetCurrentDirectory(WorkDir);
//
//	bool isOk = false;
//	PROCESS_INFORMATION processInfo;
//	STARTUPINFO startupInfo;
//	::ZeroMemory(&startupInfo, sizeof(startupInfo));
//	startupInfo.cb = sizeof(startupInfo);
//	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//	startupInfo.wShowWindow	= SW_HIDE;
//	startupInfo.hStdError = hWrite;
//	startupInfo.hStdOutput = hWrite;
//	CString Command = path;
//	if (!param.IsEmpty()) {
//		Command = Command + _T(" ") + param;
//	}
//	TRACE(_T("exec: %s\n"), Command);
//	if (::CreateProcess(NULL, Command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
//		DWORD TimeoutFlag = WaitForSingleObject(processInfo.hProcess, msTimeout);
//		isOk = true;
//
//		if (hasResponse) {
//			DWORD bytesRead;
//			DWORD dwBytesWritten;
//			char message[512];
//			memset(message, 0, sizeof(message));
//			if (TimeoutFlag == WAIT_TIMEOUT)
//			{
//				WriteFile(hWrite, "FAIL", 4, &dwBytesWritten, NULL);
//			}
//			::ReadFile(hRead, message, 511, &bytesRead, NULL);
//			if (strlen(message) > 0) {
//				param = CA2T(message);
//			}
//			else {
//				param.Empty();
//			}
//		}
//	}
//	else {
//		isOk = false;
//		CString error_msg(_T("ERROR: Execute fail!\r\ncmd:"));
//		error_msg += Command;
//		::AfxMessageBox(error_msg);
//	}
//	Command.ReleaseBuffer();
//	CloseHandle(hRead);
//	CloseHandle(hWrite);
//	SetCurrentDirectory(szNowPath);
//
//	return isOk;
//}
//
//bool CDownload8994::bFastbootDL_3(const char* szExeName, const char* szCmd, const char* szSerialNumber, char *szReturn, long dwReturnLen, int iTimeout)
//	{
//		bool ret = true;
//		HANDLE hWrite, hRead;
//		PROCESS_INFORMATION processInfo;
//
//		memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));
//
//		SECURITY_ATTRIBUTES saAttr;
//		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//		saAttr.bInheritHandle = TRUE;
//		saAttr.lpSecurityDescriptor = NULL;
//
//		try
//		{
//			if (! CreatePipe(&hRead, &hWrite, &saAttr, 0))
//				throw _T("CreatePipe");
//
//			STARTUPINFO si = {0};
//			si.cb = sizeof(STARTUPINFO);
//			si.hStdError = hWrite;
//			si.hStdOutput = hWrite;
//			si.hStdInput = hRead;
//			si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
//			si.wShowWindow = SW_HIDE;
//
//
//			// Set WorkPath and ExePath
//			char szModuleFilePath[256];
//			std::string strExeCmdPath;
//			memset(szModuleFilePath, 0, 256);
//			GetModuleFileNameA(NULL, szModuleFilePath, 256);
//			PathRemoveFileSpecA(szModuleFilePath);
//			SetCurrentDirectory(szModuleFilePath);
//			strExeCmdPath = szModuleFilePath;
//
//
//
//			// Set cmd
//			char szCmdLine[1024];
//			std::string strSerialNumber;
//
//			memset(szCmdLine, 0, 1024);
//			strSerialNumber = szSerialNumber;
//
//			if (strstr(szCmd, "devices") != NULL)
//			{
//				// Enumerate devices, do not need serial number
//				sprintf_s(szCmdLine, 1024, "\"%s\\%s\" %s", strExeCmdPath.c_str(), szExeName, szCmd);
//			}
//			else
//			{
//				//if (! strSerialNumber.empty())
//				//	sprintf_s(szCmdLine, 1024, "\"%s\\%s\" -s %s %s", strExeCmdPath.c_str(), szExeName, strSerialNumber.c_str(), szCmd);
//				//else
//					sprintf_s(szCmdLine, 1024, "\"%s\\%s\" %s", strExeCmdPath.c_str(), szExeName, szCmd);
//			}
//
//
//			if (! CreateProcess(
//				NULL,
//				szCmdLine,
//				NULL,
//				NULL,
//				TRUE,
//				CREATE_DEFAULT_ERROR_MODE,
//				NULL,
//				strExeCmdPath.c_str(),
//				&si,
//				&processInfo
//				))
//			{
//				throw _T("CreateProcess");
//			}
//
//			WaitForSingleObject(processInfo.hProcess, iTimeout);
//
//			// if needs return value
//			if (szReturn != NULL)
//			{
//				DWORD dwAvail, numread;
//
//				BOOL bRet = PeekNamedPipe(hRead, NULL, 0, NULL, &dwAvail, 0);
//				if (bRet && ((int)dwAvail > 0))
//				{
//					char *pBuffer = new char [dwAvail+1];
//					memset(pBuffer, 0, dwAvail+1);
//
//					::ReadFile(hRead, pBuffer, dwAvail, &numread, 0);
//
//					memset(szReturn, 0, dwReturnLen);
//
//					if ((int)dwReturnLen >= (int)dwAvail)
//						memcpy(szReturn, pBuffer, dwAvail);
//					else
//						memcpy(szReturn, pBuffer, dwReturnLen);
//
//					delete [] pBuffer;
//				}
//			}
//		}
//		catch(const TCHAR* szError)
//		{
//			LPVOID lpMsgBuf;
//
//			FormatMessage(
//				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//				NULL, GetLastError(),
//				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//				(LPTSTR)&lpMsgBuf,
//				0,
//				NULL
//				);
//
//			TCHAR szMessage[1024] = {0};
//			sprintf_s(szMessage, 1024, "%s: %s", szError, lpMsgBuf);
//
//			LocalFree(lpMsgBuf);
//			ret = false;
//		}
//
//		CloseHandle(hRead);
//		CloseHandle(hWrite);
//		CloseHandle(processInfo.hThread);
//		processInfo.hThread = NULL;
//		CloseHandle(processInfo.hProcess);
//		processInfo.hProcess = NULL;
//
//		return ret;
//	}
//
//bool CDownload8994::bFastbootDL_4(CString folderPath){
//
//	CString csExeCMD;
//	//csExeCMD.Format(_T("CMD.EXE /C %s") ,folderPath);
//	//csExeCMD.Format(_T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150729_debug\\fastboot.exe %s\"") ,folderPath);
//	csExeCMD.Format(_T("%s") ,folderPath);
//	/* Run */
//	TCHAR sz_commandLine[1024];
//	memset(sz_commandLine, 0, sizeof(sz_commandLine));
//	for (int i = 0; i < csExeCMD.GetLength(); i ++) 
//	{
//		sz_commandLine[i] = csExeCMD[i];   
//	}
//	PROCESS_INFORMATION pi_bFastbootDL = {0};;
//	STARTUPINFO si = {0};
//	memset(&si, 0, sizeof(si));
//	si.cb          = sizeof(si);
//	si.wShowWindow = SW_HIDE;
//	si.dwFlags     = STARTF_USESHOWWINDOW;
//
//	BOOL b_createRes = FALSE;	
//
//
//	//b_createRes = CreateProcess(_T("fastboot.exe"), _T("devices"), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
//
//	//b_createRes = CreateProcess(_T("C:\\Windows\\System32\\cmd.exe"), sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
//
//	//b_createRes = CreateProcess(_T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150729_debug\\fastboot.exe"), sz_commandLine, NULL, NULL, false, NULL, NULL, NULL, &si, &pi_bFastbootDL);
//
//	//b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, true, NULL, NULL, NULL, &si, &pi);
//	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, false, NULL, NULL, NULL, &si, &pi_bFastbootDL);
////	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
//	if(b_createRes == FALSE) 
//	{
//		//SendMessageToUI("b_createRes Fail");
//		AddMsg("b_createRes Fail", None, 10);
//		return false;
//	}
//
//	//CloseHandle( pi.hProcess );
// //   CloseHandle( pi.hThread );
//	//return true;
//
//	DWORD dw_waitResult = 0;
//	DWORD dw_timeout;
//	dw_timeout = 30000;
//	dw_waitResult = ::WaitForSingleObject(pi_bFastbootDL.hProcess, dw_timeout);
//	//dw_waitResult = ::WaitForSingleObject(pi.hProcess, INFINITE);
//	switch(dw_waitResult)
//	{
//	case WAIT_FAILED:
//		::CloseHandle(pi_bFastbootDL.hProcess);
//		pi_bFastbootDL.hProcess = NULL;
//		AddMsg("WAIT_FAILED Fail", None, 10);
//		return false;
//
//	case WAIT_TIMEOUT:
//		::CloseHandle(pi_bFastbootDL.hProcess);
//		pi_bFastbootDL.hProcess = NULL;
//		AddMsg("WAIT_TIMEOUT Fail", None, 10);
//		return false;
//
//	case WAIT_OBJECT_0:
//		break;
//	}
//	::CloseHandle(pi_bFastbootDL.hProcess);
//	pi_bFastbootDL.hProcess = NULL;
//
//	return true;
//}


//bool CDownload8994::bAdbCMD(CString csCMD){
//
//	CString csExeCMD;
//	bool bDLFlag = false;
//
//	/*get fastboot.exe path */
//	char sz_currentPath[MAX_PATH] = { 0 };
//	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
//	::PathRemoveFileSpec(sz_currentPath);
//
//	CString str_FastbootPath;
//	str_FastbootPath.Format(_T("%s\\adb.exe"), sz_currentPath);
//
//
//	//csExeCMD.Format(_T("CMD.EXE /C %s") ,folderPath);
//	//csExeCMD.Format(_T("D:\\03.Factory\\00.DELL\\GF80B1A_Trigger_V1.004_20150729_debug\\fastboot.exe %s\"") ,folderPath);
//	csExeCMD.Format(_T("%s %s") ,str_FastbootPath, csCMD);
//	/* Run */
//	TCHAR sz_commandLine[1024];
//	memset(sz_commandLine, 0, sizeof(sz_commandLine));
//	for (int i = 0; i < csExeCMD.GetLength(); i ++) 
//	{
//		sz_commandLine[i] = csExeCMD[i];   
//	}
//	PROCESS_INFORMATION pi_bFastbootDL = {0};;
//	STARTUPINFO si = {0};
//	memset(&si, 0, sizeof(si));
//	si.cb          = sizeof(si);
//	si.wShowWindow = SW_HIDE;
//	si.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
//
//	BOOL b_createRes = FALSE;	
//
//	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, true, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi_bFastbootDL);
//
//	if(b_createRes == FALSE) 
//	{
//		AddMsg("b_createRes Fail", None, 10);
//		return false;
//	}
//
//	DWORD dw_waitResult = 0;
//	DWORD dw_timeout;
//	dw_timeout = 180 * 1000;
//
//	
//	dw_waitResult = ::WaitForSingleObject(pi_bFastbootDL.hProcess, dw_timeout);
//
//	CloseHandle(pi_bFastbootDL.hThread);
//	pi_bFastbootDL.hThread = NULL;	
//	::CloseHandle(pi_bFastbootDL.hProcess);
//	pi_bFastbootDL.hProcess = NULL;
//
//	switch(dw_waitResult)
//	{
//	case WAIT_FAILED:
//		::CloseHandle(pi_bFastbootDL.hProcess);
//		pi_bFastbootDL.hProcess = NULL;
//		AddMsg("WAIT_FAILED Fail", None, 10);
//		return false;
//
//	case WAIT_TIMEOUT:
//		::CloseHandle(pi_bFastbootDL.hProcess);
//		pi_bFastbootDL.hProcess = NULL;
//		AddMsg("WAIT_TIMEOUT Fail", None, 10);
//		return false;
//
//	case WAIT_OBJECT_0:
//		break;
//	}
//	::CloseHandle(pi_bFastbootDL.hProcess);
//	pi_bFastbootDL.hProcess = NULL;
//
//	return true;
//}
