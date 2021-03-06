
#include "StdAfx.h"
#include "Download8994.h"
#include <SetupAPI.h>
#pragma comment(lib, "setupapi")
#include <Cfgmgr32.h>
#include <string>
#include <sstream>

CCriticalSection CDownload8994::m_obj_loadcheckSumCritSection;
CDownload8994* g_pFirehoseDownload = NULL;

template <class T> 
std::string ConvertToString(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

CDownload8994::CDownload8994(int i_COMPort, CString str_multiDLFlag)
{
	m_str_errorCode = "";
	m_str_DLMode = "";
	/*m_i_reboot = 0;
	m_i_RebootMode = 0;
	m_i_checkSum = 0;
	m_i_SupportQDownload = 0;*/
	m_map_COMPicasso.clear();
	//m_map_imageNameCheckSum.clear();
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
bool CDownload8994::MultiDownload(bool b_speedUp, bool b_reOpenAfterReset, DownloadProtocol nDLPROTOCOL, int i_idtype)  
{

	bool b_result = false;
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];

	if (m_str_DLMode == DOWNLOAD_MODE_PREDL)
	{ 
		/* Hex Download */
		b_result = Download(b_speedUp, b_reOpenAfterReset, nDLPROTOCOL);
	
		if ( b_result ){
			Sleep(3000);
			bool b_wait_fastboot = false;
			int nLimitTime = 0 ; 
			m_str_multiDLFlag = _T("0") ;//slingle dl only

			CString cs_Qphone; 
			cs_Qphone = _T("QPHONE");

			while ( !b_wait_fastboot){
				if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"),output,ErrorCode, cs_Qphone) ){
					b_wait_fastboot = true;
					AddMsg("Get Fastboot Success.", None, 10);
				}
				Sleep(1000);
				nLimitTime ++;
				if ( nLimitTime > 60 ) break;
			}

			if ( b_wait_fastboot) b_result = FastbootEXE_Download_SINGLE();
			else AddMsg("Fail can't find fastboot", DownloadStep::None, 100);

			b_result = postCmd();
		}
	}
 	else if ((m_str_DLMode == DOWNLOAD_MODE_REDL) || (m_str_DLMode == DOWNLOAD_MODE_REDL_OSDL))
	{
		bool b_wait_fastboot = false;
		int nLimitTime = 0 ; 

		CString cs_Qphone;
		cs_Qphone.Format(_T("QPHONE%d"), m_i_COMPort);
		if (bGetADB(m_i_COMPort)){
			AddMsg("Get Adb Success.", None, 10);

			bADB_to_Fastboot(m_i_COMPort);
			AddMsg("adb to fastboot ok", None, 10);

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

			b_result = postCmd();

		}
	}	else if (m_str_DLMode == DOWNLOAD_MODE_OSDL1)
	{
		bool b_wait_fastboot = false;
		int nLimitTime = 0 ; 
		m_str_multiDLFlag = _T("0") ;//slingle dl only

		CString cs_Qphone; 
		cs_Qphone = _T("QPHONE");
		if ( i_idtype == 2){ //dell id , from adb to fastboot
			if (bGetADB_SINGLE()){
				AddMsg("Get Adb Success.", None, 10);

				bADB_to_Fastboot_SINGLE();
				AddMsg("adb to fastboot ok", None, 10);

				while ( !b_wait_fastboot){
					if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"),output,ErrorCode, cs_Qphone) ){
						b_wait_fastboot = true;
						AddMsg("Get Fastboot Success.", None, 10);
					}
					Sleep(1000);
					nLimitTime ++;
					if ( nLimitTime > 60 ) break;
				}
			}
		}
		else if ( i_idtype == 1)// picasso 
		{
			b_wait_fastboot = true;
		}

		if ( b_wait_fastboot){
			b_result = FastbootEXE_Download_SINGLE();
		}
		else AddMsg("Fail can't find fastboot", DownloadStep::None, 100);

		if (b_result){
			b_result = postCmd();
		}
		else
		{
			AddMsg("Fail cFastbootEXE_Download_SINGLE", DownloadStep::None, 10);
		}

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



bool CDownload8994::bADB_to_Fastboot_SINGLE( ){ 
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];

	bool bGetAdb = false; 
	CString csPhone;
	CString csCmd;

//	csPhone.Format("QPHONE");
	csCmd.Format(_T("reboot bootloader"));

	bGetAdb = bCallAdbFastbootCMD(_T("adb.exe"), csCmd,output,ErrorCode, DNULL);

	Sleep(1000);
	
	delete [] output;
	delete [] ErrorCode;
	return bGetAdb;
}

bool CDownload8994::bADB_to_Fastboot(int nPhone){ 
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];
	//int nRetryTime = 60;
	bool bGetAdb = false; 
	CString csPhone;
	CString csCmd;

	csPhone.Format("QPHONE%d", nPhone);
	csCmd.Format(_T("reboot bootloader -s QPHONE%d"), nPhone);

	bGetAdb = bCallAdbFastbootCMD(_T("adb.exe"), csCmd,output,ErrorCode, DNULL);

	Sleep(1000);
	
	delete [] output;
	delete [] ErrorCode;
	return bGetAdb;
}

bool CDownload8994::postCmd(){
	bool b_result = false;
	std::string std_Msg;
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];
	CString cs_FindString("on");
	//fastboot oem adb Qon
	//fastboot oem root Qon
	//fastboot oem permissive Qon
	//fastboot oem ftd Qon

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("oem adb Qon"),output,ErrorCode, DREAD) )){
			AddMsg("oem adb Qon fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("oem adb Qon pass", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("oem root Qon"),output,ErrorCode, DREAD) )){
			AddMsg("oem root Qon fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("oem root Qon pass", None, 10);
		}
		Sleep(200);

		if ( !(  b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("oem permissive Qon"),output,ErrorCode, DREAD) )){
			AddMsg("oem permissive Qon fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("oem permissive Qon pass", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("format drm"),output,ErrorCode, DREAD) )){
			AddMsg("format drm fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("format drm pass", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("oem ftd Qon"),output,ErrorCode, DREAD) )){
			AddMsg("oem ftd Qon fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("oem ftd Qon pass", None, 10);
		}
		Sleep(200);

		/*check variable */

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("getvar ftd"),output,ErrorCode, cs_FindString) )){
			AddMsg("getvar ftd fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("getvar ftd ok", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("getvar permissive"),output,ErrorCode, cs_FindString) )){
			AddMsg("getvar permissive fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("getvar permissive ok", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("getvar adb"),output,ErrorCode, cs_FindString) )){
			AddMsg("getvar adb fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("getvar adb ok", None, 10);
		}
		Sleep(200);

		if ( !( b_result = bCallAdbFastbootCMD(_T("fastboot.exe"), _T("getvar root"),output,ErrorCode, cs_FindString) )){
			AddMsg("getvar root fail", None, 10);
			goto Exit_ShowResult;
		}else
		{
			AddMsg("getvar root ok", None, 10);
		}
		Sleep(200);

Exit_ShowResult:
	if ( !b_result) {

	}
	else
	{
	//	m_strErrorCode = "-";
	//	m_strResult = "PASS";
	}

	delete[] output ;
	delete[] ErrorCode;
	return b_result;
}


bool CDownload8994::bGetADB_SINGLE(){ 
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];
	int nRetryTime = 60;
	bool bGetAdb = false; 
	CString csPhone;
	CString csCmd;

	//csCmd = _T("devices ");
	csPhone.Format("QPHONE");

	for(int i =0; i <nRetryTime; i++){
		//if ( bGetAdb = bAdbCMD(csCmd, output, ErrorCode, nPhone))
		if (bCallAdbFastbootCMD(_T("adb.exe"), _T("devices "),output,ErrorCode, csPhone))
		{
			bGetAdb = true;
			Sleep(1000);
			break;
		}
	}

	delete[] output;
	delete[] ErrorCode ;	
	return bGetAdb;
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
		if (bCallAdbFastbootCMD(_T("adb.exe"), _T("devices "),output,ErrorCode, csPhone))
		{
			bGetAdb = true;
			Sleep(1000);
			//bAdbCMD(csCmd, output, ErrorCode, 0);
			
			break;
		}
	}

	delete[] output;
	delete[] ErrorCode ;	
	return bGetAdb;
}

bool CDownload8994::FastbootEXE_Download_SINGLE(){
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
		csExePath.Format(_T("\"%s\" flash %s \"%s\" "),str_FastbootPath, csImgType, csImgPath);

		
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
		csExePath.Format(_T("%s flash %s %s"),str_FastbootPath, csImgType, csImgPath);
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

		csExePath.Format(_T("\"%s\" format \"%s\" "),str_FastbootPath, csImgType);
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

		csExePath.Format(_T("\"%s\" erase \"%s\""),str_FastbootPath, csImgType);
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


	//for (int i = 0 ; i < mapDL_ALL_CHECK1_IMGS.size() ; i++){
	//	iter = mapDL_ALL_CHECK1_IMGS.find(i); 
	//	if(iter != mapDL_ALL_CHECK1_IMGS.end()){
	//		csImgPath = m_str_imageFilePath + "\\" + iter->second;

	//	}
	//}
	//csExePath.Format(_T("%s flash %s %s -s"),str_FastbootPath, csImgType, csImgPath);
	//bFastbootDL(csExePath);
	//return true;
}


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

bool CDownload8994::ReadIMG_qfil( CString imagePath )
{
	char buff[100];
	CString csMsg;
	sprintf_s(buff, sizeof(buff),"%s\\emmc_appsboot.mbn", imagePath );
	if (_taccess(buff , 0) != 0) 
	{
		CString csMsg;
		csMsg.Format("%s access fail", buff );
		AddMsg(csMsg, None, 10);
		return false;
	}
	st_QFILE.m_emmc_appsboot = (char*) buff;


	sprintf_s(buff, sizeof(buff),"%s\\prog_emmc_firehose_8994_lite.mbn", imagePath );
	if (_taccess(buff , 0) != 0) 
	{
		csMsg.Format("%s access fail", buff );
		AddMsg(csMsg, None, 10);
		return false;
	}
	else
	{
		csMsg.Format("%s access ok", buff );
		AddMsg(csMsg, None, 10);
	}
	st_QFILE.m_firehose_8994_lite = (char*) buff;


	sprintf_s(buff, sizeof(buff),"%s\\patch0_fastbootimage.xml", imagePath );
	if (_taccess(buff , 0) != 0) 
	{
		csMsg.Format("%s access fail", buff );
		AddMsg(csMsg, None, 10);
		return false;
	}
	else
	{
		csMsg.Format("%s access ok", buff );
		AddMsg(csMsg, None, 10);
	}
	st_QFILE.m_patch0_fastbootimage = (char*) buff;


	sprintf_s(buff, sizeof(buff),"%s\\rawprogram0_fastbootimage.xml", imagePath );
	if (_taccess(buff , 0) != 0) 
	{
		csMsg.Format("%s access fail", buff );
		AddMsg(csMsg, None, 10);
		return false;
	}
	else
	{
		csMsg.Format("%s access ok", buff );
		AddMsg(csMsg, None, 10);
	}
	st_QFILE.m_rawprogram0_fastbootimage = (char*) buff;

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
	bool bRes = false;
	AddMsg("Hex Download", None, 100);
//	m_b_reOpenAfterReset = b_reOpenAfterReset;
		
	CString cs_QMSL_MSVC10R = QMSL_MSVC10R;
	m_hQMSL_MSVC10R = ::LoadLibrary(cs_QMSL_MSVC10R);
	if(m_hQMSL_MSVC10R == NULL)
	{
		AddMsg(false, "Load QMSL_MSVC10R Fail", FireHoseProtocol);
		return false;
	}

	if ( SAHARADL() ){
		if ( FireHose()){
			AddMsg("Sahara , FireHose PASS , reboot ", None, 20);
			bRes = true;
		}
		else
		{
			AddMsg("Hex Download Fail.", None, 100);
		}

	}
	else
	{
		AddMsg("Hex Download Fail.", None, 100);
	//	return false;
	}

	FreeLibrary(m_hQMSL_MSVC10R);
	AddMsg("Hex Download Success.", None, 100);
	return bRes;
}


bool CDownload8994::FindQualcomDevice(int nComport)
{
	GUID GUIDQualcommDiagUsbClass1	= {0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
	GUID *pGUID = new GUID;
	CoCreateGuid(pGUID);
	pGUID = &GUIDQualcommDiagUsbClass1;

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
	delete sz_id;
	delete pGUID;
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return bFound;
}


bool CDownload8994::FindADBdevice(int nADBPort)
{
	GUID GUIDAndroidUsbClassID1		= {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};
	GUID *pGUID = new GUID;
	//DEFINE_GUID (pGUID, 
	//CoCreateGuid(pGUID);
	//pGUID = &GUIDQualcommDiagUsbClass1;
//	GUID *pGUID = &GUIDAndroidUsbClassID1;
	pGUID = &GUIDAndroidUsbClassID1;

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
				if ( atoi( sz_id) >-1 ){//get any port
					delete sz_id;
					return true;
				}
			//	return atoi( sz_id);
			}
			else
			{/*return specific port */
				if( nADBPort = atoi( sz_id)){
					delete sz_id;
					return true;
				}
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
	delete pGUID;
	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	return -1;
}

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


	//QLIB_QPHONEMS_SaharaConfigureCallback SWDL_QPHONEMS_CB = QMSLCallBack_8994;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_8994;

	if(ConnectServer_Sahara != NULL)
	{
		m_hPhone = ConnectServer_Sahara(iPort, &saharaVer, szMsmid, szBGetMsmId,n_mode, n_timeout, SWDL_QPHONEMS_CB);
		AddMsg(true, "ConnectServer_Sahara", SaharaProtocol);
	}
	else
	{
		AddMsg(true, "ConnectServer_Sahara fail 0", SaharaProtocol);
		return false;
	}

	if(saharaVer == 0){
		AddMsg("ConnectServer_Sahara_fail", DownloadStep::None, 100);
		return false;
	}

	if(Sahara_FlashProgrammer != NULL)
	{
		char cHexPath[256] = {0};
		
		strncpy(cHexPath, st_QFILE.m_firehose_8994_lite.c_str(), st_QFILE.m_firehose_8994_lite.length());
	//	strncpy(cHexPath, m_str_hex.c_str(), m_str_hex.length());
		cret = Sahara_FlashProgrammer(m_hPhone, cHexPath);

		if(cret) //1
		{
			AddMsg("Flash Programmer  ok", DownloadStep::None, 100);
		}
		else         //0
		{
			AddMsg("Flash Programmer  Fail", DownloadStep::None, 100);
			return false;
		}
	}
	else
	{
		AddMsg("Flash Sahara_FlashProgrammer  null", DownloadStep::None, 100);
		return false;
	}
    Sleep(1000);

	if (m_hPhone != NULL && DisconnectServer != NULL)
	{
		DisconnectServer(m_hPhone);
		m_hPhone = NULL;
	}

//	FreeLibrary(m_hQMSL_MSVC10R);
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
//
//void swdlCallBack_8994(
//   HANDLE hContextID,
//   char * sMsg,
//   unsigned short iMsgSize)
//{
//
//   printf("\n%s", sMsg);
//
//	FILE *pFile = NULL;
//	pFile = fopen("C:\\FireHoseQMSLCallBack_8916.log", "wb");
//	fseek(pFile, 0, SEEK_END);
//	fwrite(sMsg, 1, sizeof(sMsg)-1, pFile);
//	fclose(pFile);
//}


bool CDownload8994::FlashProgrammer()
{
	unsigned int iPort = m_i_COMPort;
	HANDLE hPhone = NULL;
	swdlQPHONEMSCB SWDL_QPHONEMS_CB = QMSLCallBack_FireHose;

	HMODULE     m_hQMSL_MSVC10R; 
	CString cs_QMSL_MSVC10R = QMSL_MSVC10R;
	m_hQMSL_MSVC10R = ::LoadLibrary(cs_QMSL_MSVC10R);
	if(m_hQMSL_MSVC10R == NULL)
	{
		AddMsg(false, "Load QMSL_MSVC10R Fail", FireHoseProtocol);
		return false;
	}


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
			AddMsg("Connect Sahara Server Fail", DownloadStep::None, 100);
			return false;
		}
		else
		{
			AddMsg("Connect Sahara Server ok", DownloadStep::None, 100);
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
			AddMsg("Flash Programmer ok", DownloadStep::None, 100);
		}
		else         //0
		{
			AddMsg("Flash Programmer fail", DownloadStep::None, 100);
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
		AddMsg("Disconnect Sahara Server  ok", DownloadStep::None, 100);
	}
	hPhone = NULL;

	//FreeLibrary(m_hQMSL_MSVC10R);
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

	/*loading Lib*/
	pQLIB_QPHONEMS_ConnectServer_FireHose ConnectServer_FireHose = (pQLIB_QPHONEMS_ConnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_ConnectServer_FireHose");
	pQLIB_DisconnectServer_FireHose DisconnectServer_FireHose = (pQLIB_DisconnectServer_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_DisconnectServer_FireHose");
	pQLIB_QPHONEMS_FireHosePower FireHose_Reboot = (pQLIB_QPHONEMS_FireHosePower)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHosePower");


	//Connect Server
	hPhone = ConnectServer_FireHose(iPort, swdlCallBack);
	if (hPhone == 0)
	{
		AddMsg("Connect Firehose Server Fail", DownloadStep::None, 100);
		return false;
	}
	else
	{
		AddMsg("Connect Firehose Server ok", DownloadStep::None, 100);
	}

	//Ping device
	pQLIB_QPHONEMS_FireHoseNOP FireHose_NOP = (pQLIB_QPHONEMS_FireHoseNOP)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_FireHoseNOP");
	szResult = FireHose_NOP(hPhone);
	if(szResult) //1
	{
		AddMsg("Ping Firehose device ok", DownloadStep::None, 100);
	}
	else         //0
	{
		AddMsg("Ping Firehose device fail", DownloadStep::None, 100);
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
		AddMsg("Configure Firehose ok", DownloadStep::None, 100);
	}
	else         //0
	{
		AddMsg("Configure Firehose fail", DownloadStep::None, 100);
		return false;
	}

	//Download image
	char szRawProgramFile[MAX_PATH] = {0};
	char szPatchFile[MAX_PATH] = {0};
	float imageSizeInMB = 0.0;
	float throughput = 0.0;


	sprintf_s(szRawProgramFile, MAX_PATH, "%s", st_QFILE.m_rawprogram0_fastbootimage.c_str());
	sprintf_s(szPatchFile, MAX_PATH, "%s", st_QFILE.m_patch0_fastbootimage.c_str());

	pQLIB_QPHONEMS_UploadEmmcImage_FireHose UploadEmmcImage_FireHose = (pQLIB_QPHONEMS_UploadEmmcImage_FireHose)GetProcAddress(m_hQMSL_MSVC10R, "QLIB_QPHONEMS_UploadEmmcImage_FireHose");
	szResult = UploadEmmcImage_FireHose(hPhone, szRawProgramFile, szPatchFile, &imageSizeInMB, &throughput);
	if(szResult) //1
	{
		AddMsg("Firehose Download image Success ok", DownloadStep::None, 100);
	}
	else         //0
	{
		AddMsg("Firehose Download image Success fail", DownloadStep::None, 100);
		return false;
	}

	//Reboot device

	szResult = FireHose_Reboot(hPhone, _T("reset"));
	if(szResult) //1
	{
		AddMsg("Firehose  reset ok", DownloadStep::None, 100);
	}
	else         //0
	{
		AddMsg("Firehose  reset fail", DownloadStep::None, 100);
		return false;
	}

    printf("\n----------------QLIB_DisconnectServer_FireHose---------------------------\n");
    DisconnectServer_FireHose(hPhone);

	return true;
}

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
				AddMsg("Fail to find QUALCOMM Comport", DownloadStep::None, 100);
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
	AddMsg("CDownload8994 Read FA Success.", DownloadStep::None, 100);
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

	int i_length = FALENGTH;
	//bool b_result = false;

	AddMsg("CDownload8994 Write FA Success.", DownloadStep::None, 100);
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
bool CDownload8994::ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idtype) 
{
	if (i_idtype == 2 )
	{
		strcpy(sz_sectorData, _T("Dan6673Dan,01,5555555555,Undefined,Undefined,GBROB1A,0000000000000000000000,,"));
		if ( ReadId() ){/*get scalar id ok*/
			if(m_szId.empty() || m_szId.length() != ID_SIZE)
			{	
				m_szErrMsg = "get scalar id Fail  = ";//  + std_ScalarId.c_str();
				m_szErrMsg = m_szErrMsg + m_szId.c_str();
				AfxMessageBox( m_szErrMsg.c_str());
			}
			else
			{
				//	m_szId = "a1234567899";
				m_szId = m_szId + ",";
				std::string str_faData = sz_sectorData;
				str_faData.replace(14, 12, m_szId);
				sprintf_s( sz_sectorData , 512, "%s", str_faData.c_str());
			}
		}

	}
	else if  (( i_idtype == 0)||( i_idtype == 1)){// read picasso 
			bool b_result = false;
			char* output = new char[BUFFER_SIZE];
			char* ErrorCode =new char[BUFFER_SIZE];
			char sz_buffer[512] = {0};
			bool b_wait_fastboot = false;
			int  nLimitTime = 0;
			CString cs_Qphone;
			
			cs_Qphone = _T("QPHONE");
			for(int i = 0 ; i < 60; i++) //detect adb for 60s
			{
				if (bGetADB_SINGLE()){
					AddMsg("ReadFASector Get Adb Success.", None, 10);

					bADB_to_Fastboot_SINGLE();
					AddMsg("ReadFASector adb to fastboot ok", None, 10);

					while ( !b_wait_fastboot){
						if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"),output,ErrorCode, cs_Qphone) ){
							b_wait_fastboot = true;
							AddMsg("ReadFASector Get Fastboot Success.", None, 10);
						}
						Sleep(1000);
						nLimitTime ++;
						if ( nLimitTime > 60 ) break;
					}
				}
				
				if ( b_wait_fastboot ) break; 
				AddMsg("ReadFASector Get Adb ... ..", DownloadStep::None, 10);
				Sleep(1000);
			}
		/*get fastboot*/

			if ( !b_wait_fastboot){
				AddMsg("ReadFASector Get fastboot Fail.", None, 10);
				return false;
			}

			AddMsg("ReadFASector Get fastboot ok.", DownloadStep::None, 100);
			Sleep(300);

			/* dump protocol --- "dump:%s"  example:"dump:otpfa0x0+512" */
			char sz_sectorSize[32] = {0};
			char sz_command[1024] = {0};

			sprintf(sz_command, "flash passport passport_FactoryDLTool");
			if ( !(bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD) )){
			//	b_result = false;
				AddMsg("ReadFASector flash passport passport_FactoryDLTool fail", None, 10);
				return false;
			}else
			{
				memcpy(sz_sectorData, output, i_sectorSize);
				AddMsg("ReadFASector flash passport passport_FactoryDLTool ok", None, 10);
			}
			Sleep(300);

			sprintf(sz_sectorSize, "%d+%d", i_sectorNum * 512, i_sectorSize);
			sprintf(sz_command, "dump:%s%s", "Qfa@", sz_sectorSize);
			if ( !(bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD) )){
				AddMsg("ReadFASector dump fail ", None, 10);
				return false;
			}else
			{
				memcpy(sz_sectorData, output, i_sectorSize);
				AddMsg("ReadFASector dump ok", None, 10);
			}
			Sleep(300);

			sprintf(sz_command, "upload:200 picasso.bin");
			if ( !(bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD) )){
				//b_result = false;
				AddMsg("ReadFASector fail,  upload:200 picasso.bin", None, 10);
				return false;
			}else
			{
				memcpy(sz_sectorData, output, i_sectorSize);
				AddMsg("ReadFASector ok,  upload:200 picasso.bin", None, 10);
			}
			Sleep(300);

			sprintf(sz_command, "getvar picasso");
			if ( !(bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD) )){
				b_result = false;
				AddMsg("ReadFASector fail, getvar picasso", None, 10);
			}else
			{
				memcpy(sz_sectorData, output, i_sectorSize);
				AddMsg("ReadFASector ok,  getvar picasso", None, 10);
			}
			Sleep(300);
		
			std::string strPicasso;
			char * pch;

			pch = strstr (output, "finished");
			strncpy (pch,"\0",1);
			pch = output;

			strPicasso = output;
			CString csPicasso = strPicasso.c_str();
			csPicasso.Replace(_T("picasso: "), _T("")); 
			strPicasso = csPicasso.GetBuffer(0); //get piccsso finally
			strPicasso = 	strPicasso + ",";
			csPicasso.ReleaseBuffer();

			strcpy(sz_sectorData, _T("Dan6673Dan,01,5555555555,Undefined,Undefined,GBROB1A,0000000000000000000000,,"));
			std::string str_faData = sz_sectorData;
			str_faData.replace(14, 11, strPicasso);
			sprintf_s( sz_sectorData , 512, "%s", str_faData.c_str());

			delete[] output;
			delete[] ErrorCode ;	
	}

	return true;
}


bool CDownload8994::ReadId (void)
{
	char szBuf[20]="";
	CString str_dllIIC = IICDLL;
	HINSTANCE hDll;
	hDll = LoadLibrary(str_dllIIC);
	if (hDll != NULL )
	{
		typedef void(_stdcall *lpReadId)(char*,int);
		lpReadId ReadId;
		ReadId =(lpReadId)::GetProcAddress(hDll,"ReadId");
		if( ReadId != NULL)
		{
			ReadId(szBuf,20);
			FreeLibrary(hDll);
			m_szErrMsg = "";
			//		m_str_message = _T("Can not find cable loss xml!");
			m_szId = szBuf;
			//AfxMessageBox(m_szId.c_str());
			return true;
		}
		else
		{	FreeLibrary(hDll);
			m_szErrMsg = "Load address of ReadId fail.";
			m_szId = szBuf;
			AfxMessageBox(m_szErrMsg.c_str());
			return false;
		}
	}
	else
	{	m_szErrMsg = "Load I2C.DLL fail.";
		m_szId = szBuf;
		AfxMessageBox(m_szErrMsg.c_str());
		return false;
	}
}
std::string CDownload8994::GetId(void)
{	
	return m_szId;
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

	bool b_speedUp = false;
	bool b_reOpenAfterReset = false;
	bool b_result = false;
	char* output = new char[BUFFER_SIZE];
	char* ErrorCode =new char[BUFFER_SIZE];

	int  i_length = i_sectorSize;
	char sz_buffer[512] = {0};

	int  nLimitTime = 0;
	bool b_wait_fastboot = false;
	CString cs_Qphone;

	/*get fastboot*/
	cs_Qphone.Format("QPHONE%d", m_i_COMPort);
	DownloadProtocol nDLPROTOCOL = QTT_FASTDOWNLOAD;

	while ( !b_wait_fastboot){
		if (bCallAdbFastbootCMD(_T("fastboot.exe"), _T("devices"),output,ErrorCode, cs_Qphone) ){
			b_wait_fastboot = true;
			AddMsg("Get Fastboot Success.", None, 10);
		}
		Sleep(1000);
		nLimitTime ++;
		if ( nLimitTime > 60 ) break;
	}

	AddMsg("WriteFASector Write FA.", DownloadStep::None, 100);


	/* fastboot oem write Qfa offset length value */
	char sz_command[1024] = {0};
	int offset = i_sectorNum * 512;


	if(i_sectorNum == 0){
		sprintf(sz_command, "oem write Qfa %d %d %s", offset, 255, sz_sectorData);
		if ( !(bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD) )){
			b_result = false;
			AddMsg("WriteFASector fail", None, 10);
		}else
		{
			sprintf(sz_command, "oem write Qfa %d %d %s", offset+256, 512-1, sz_sectorData+256);
			b_result = (bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD));
			//memcpy(sz_sectorData, output, i_sectorSize);
			AddMsg("WriteFASector pass", None, 10);
		}
	}
	else{
		sprintf(sz_command, "oem write Qfa %d %d %s", offset, offset+i_sectorSize-1, sz_sectorData);
		b_result = (bCallAdbFastbootCMD(_T("fastboot.exe"), sz_command, output, ErrorCode,DREAD));
		AddMsg("WriteFASector pass", None, 10);
	//	b_result = runWriteFASector(sz_command);
	}


	delete[] output;
	delete[] ErrorCode ;	
	return b_result;
}


bool CDownload8994::runWriteFASector(char *sz_sectorData){

	AddMsg("CDownload8994 Write FA Success.", DownloadStep::None, 100);
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

	DWORD dwRead;
	CHAR chBuf[4096]; 
	bool bSuccess = FALSE;

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
			if ( (cs_FindData.Find(DREAD) != -1) )// neet to read result
			{
				bSuccess = ReadFile( hRead, chBuf, BUFFER_SIZE, &dwRead, NULL);
				chBuf[dwRead] = '\0';
				strncpy(output, chBuf, dwRead);
				std::string	 std_Buf = chBuf;
				if ( std_Buf.find("FAILED") !=  string::npos ){
					bSuccess = false; 
				}
				if ( bSuccess ) 
				{
					strcpy(ErrorCode, _T("bCallAdbFastbootCMD exec ok"));
					isOk = true; //don't not need to check
				}
				else
				{
					strcpy(ErrorCode, _T(" bCallAdbFastbootCMD exec fail "));
					isOk = false; //don't not need to check
				}
			}
			else if ( ( cs_FindData.Find(DNULL) == -1) )  //need to check return payload
			{
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

				isOk = true; //don't not need to check
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


bool CDownload8994::bFastbootDL(CString folderPath){

	CString csExeCMD;
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
