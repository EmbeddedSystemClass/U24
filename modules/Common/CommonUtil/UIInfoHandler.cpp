#include "stdafx.h"
#include "UIInfoHandler.h"
#include "../FA_Operator/FA_Operator.h"
#include "../PICS_FILE/PICS_FILE.h"
#include "../PCEnv/PCEnv.h"


//PICS
bool CPICS::WritePICS(PICSinfo *Info, char *szErrCode, char *szErrMsg)
{
	bool ret;

	// Initial
	ret = Initial_PICSData();

	if (ret)
	{
		// Insert PICS items
		const int PICSLen = 512;
		char chPICS [PICSLen];

		// SetValue return false does not mean can skip next item.

		// <L1>SO</L1>
		ret = this->SetPICSValue("SO", Info->cstrSONumber.GetString());

		// <L2>MODEL</L2>
		ret = this->SetPICSValue("MODEL", Info->cstrModelName.GetString());

		// <L4>MOBILE_ID</L4>	
		sprintf_s (chPICS, PICSLen,"%s", Info->cstrMobileID.GetString());
		ret = this->SetPICSValue("MOBILE_ID", chPICS);

		// <L3>PICASSO</L3>
		ret = this->SetPICSValue("PICASSO", Info->cstrPicasso.GetString());

		// <L5>LINE</L5>
		// <L7>SLOT</L7>
		char szTemp[MAX_COMPUTERNAME_LENGTH + 1] ={0};
		char cTemp[3];
		DWORD dwLen = MAX_COMPUTERNAME_LENGTH;
		::GetComputerNameA(szTemp, &dwLen); //T01L01

		strncpy (cTemp, &szTemp[1], 2);
		cTemp[2] = 0;
		sprintf_s (chPICS, PICSLen, "%s", cTemp);
		ret = this->SetPICSValue("LINE", chPICS);

		strncpy (cTemp, &szTemp[4], 2);
		cTemp[2] = 0;
		sprintf_s (chPICS, PICSLen, "%s", cTemp);
		ret = this->SetPICSValue("SLOT", chPICS);												

		// <L6>STATION</L6>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrStation.GetString());
		ret = this->SetPICSValue("STATION", chPICS);

		// <L8>GDNG</L8>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrGDND.GetString());
		ret = this->SetPICSValue("GDNG", chPICS);

		// <L9>ERRORCODE</L9>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrErrCode.GetString());
		ret = this->SetPICSValue("ERRORCODE", chPICS);

		// <L10>TIME</L10>
		CTime curtime = CTime::GetCurrentTime();
		CStringA timestring(curtime.Format ("%Y/%m/%d %H:%M:%S"));  
		sprintf_s (chPICS, PICSLen, "%s", timestring.GetString());
		ret = this->SetPICSValue("TIME", chPICS);

		// <L11>EMPLOYEEID</L11>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrEmployeeID.GetString());
		ret = this->SetPICSValue("EMPLOYEEID", chPICS);
		
		// <L12>PCBA_VERSION</L12>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrPCBA.GetString());
		ret = this->SetPICSValue("PCBA_VERSION", chPICS);

		// <L13>SW_VERSION</L13>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrSWver.GetString());
		ret = this->SetPICSValue("SW_VERSION", chPICS);
		
		// <L14>TOOL_VERSION</L14>
		sprintf_s (chPICS, PICSLen, "%s", Info->cstrToolver.GetString());
		ret = this->SetPICSValue("TOOL_VERSION", chPICS);

		// <L15>CFG_VERSION</L15>
		ret = this->SetPICSValue("CFG_VERSION", "N/A");

		// <L16>DT_VERSION</L16>
		ret = this->SetPICSValue("DT_VERSION", "N/A");

		// Generate PICS file
		ret = this->GenPICSFile(Info->nStationNum, "", szErrCode, szErrMsg);
	}
	else
	{
		sprintf(szErrMsg, "Initial_PICSData fail");
	}

	return ret;
}

bool CPICS::SetPICSValue(const char* key, const char* value)
{
	return SetValue(key, value);
}

bool CPICS::GenPICSFile(int szStation,char *PICSPath,char *szErrorCode,char *szErrorString)
{
	return GeneratePICSFile(szStation, PICSPath, szErrorCode, szErrorString);
}

//FA
CFA::CFA():m_strStationInfoPath(""), 
	   m_nCurrentStation(0)
{
}

bool CFA::GetIniHandle(const char *szIni, char *szErrCode, char *szErrMsg)
{
	this->m_strStationInfoPath.assign(szIni);

	// Check is INI Exist.
	this->m_cStationInitInfoini.LoadIni("Setup", this->m_strStationInfoPath);
	if ( !m_cStationInitInfoini.IsIniExist()){
		strcpy(szErrMsg, "[FA] Station INFO ini is not exist.");
		return false;
	}

	return true;
}

bool CFA::CheckPrevStation(char *szPort, const char *szIni, char *szErrCode, char *szErrMsg)
{
	if(!this->GetIniHandle(szIni, szErrCode, szErrMsg))
		return false;

	// Read Current Station from ini.
	this->m_nCurrentStation = this->m_cStationInitInfoini.GetValue("CurrentStationOrder", 0) ;
	if ( 0 == this->m_nCurrentStation ){
		strcpy(szErrMsg, "[FA] Wrong Current Station Order Setting.");
		return false ;
	}
	
	if ( 1 != this->m_nCurrentStation ){
		char tmp[MAX_PATH] ;
		sprintf_s(tmp, MAX_PATH, "%i", this->m_nCurrentStation) ;
	
		if (!this->CheckStationInfo(szPort, tmp, szErrCode, szErrMsg)){
			return false ;
		}
	}
	
	return true ;
}

bool CFA::FAHandler(char *szPort, char *szResult, char *szErrCode, char *szErrMsg, FAinfo *pFA)
{	
	// Read Station Info.	
	if ( 1 == this->m_nCurrentStation ){
		// Hide Option, Default: EraseAll Data.
		int isEraseAll = this->m_cStationInitInfoini.GetValue("EraseAll", 1) ;

		if (1 == isEraseAll){
			if(!this->EraseFactoryArea(szPort, szErrCode, szErrMsg))
				return false;
		}
		
		// Initialize FAData.
		if(!this->InitializeFAStationInfo(szPort, szErrCode, szErrMsg))
			return false;
		
		if(pFA){
			if(!this->InitializeFAPhoneInfo(szPort, szErrCode, szErrMsg, pFA))
				return false;
		}

		else{
			strcpy(szErrMsg, "[FA] No FA info for initialize FA Phone Info");
			return false;
		}
	}
	
	return this->WriteFAStationInfo(szPort, szResult, szErrCode, szErrMsg) ;
}

bool CFA::WriteFAStationInfo(char *szPort, char *szResult, char *szErrCode, char *szErrMsg)
{	
	// Chang Section.
	char szSection[MAX_PATH] = {""};
	sprintf(szSection, "Station%d", this->m_nCurrentStation);
	this->m_cStationInitInfoini.SwitchSection( szSection ) ;
	
	// Read StationID.
	int stationID = this->m_cStationInitInfoini.GetValue("StationID", 0) ;
	if ( 0 == stationID ){
		sprintf(szErrMsg, "[FA] StationID cannot be zero. (%s)", szSection);
		return false ;
	}
	
	// Read StationId.
	std::string strTemp("");
	char szStationIndex[MAX_PATH] = {""};
	if ( !this->m_cStationInitInfoini.GetValue("StationID", strTemp) ){
		strcpy(szErrMsg, "[FA] Read station id fail");	
		return false ;
	}
	
	sprintf(szStationIndex, "%s", strTemp.c_str());
	
	// Read LineType
	strTemp = "";
	char szStationLineType[MAX_PATH] = {""};
	if ( !this->m_cStationInitInfoini.GetValue("LineType", strTemp) ){
		strcpy(szErrMsg, "[FA] Read line type fail");
		return false ;
	}
	
	sprintf(szStationLineType, "%s", strTemp.c_str());
	
	// Read Extern Length
	strTemp = "";
	char szStationExternLength[MAX_PATH] = {""};
	if ( !this->m_cStationInitInfoini.GetValue("ExternLength", strTemp) ){
		strcpy(szErrMsg, "[FA] Read extern length fail");
		return false ;
	}
	
	sprintf(szStationExternLength, "%s", strTemp.c_str());

	// Write to FA.
	return this->WriteStationInfo(szPort,
				      szStationIndex, 
				      szStationLineType, 
				      szStationExternLength, 
				      szResult, 
				      "", 
				      "", 
				      szErrCode, 
				      szErrMsg ) ;
}

bool CFA::InitializeFAStationInfo(char *szPort, char *szErrCode, char *szErrMsg)
{	
	// Read amount of station from ini.
	int iStationAmount = this->m_cStationInitInfoini.GetValue("StationAmount", 0) ;
	if ( 0 >= iStationAmount ) 
		return false ;

	// Read Each Station Index.
	char* szExternData = new char [iStationAmount+1] ;
	::ZeroMemory(szExternData, iStationAmount+1) ;
	for (int i=1; i<=iStationAmount; ++i) {
		// Chang Section.
		CStringA section ;
		section.Format("Station%d", i) ;
		this->m_cStationInitInfoini.SwitchSection( section.GetBuffer() ) ;

		// Read StationID.
		int stationID = this->m_cStationInitInfoini.GetValue("StationID", 0) ;
		if ( 0==stationID ) {
			sprintf(szErrMsg, "[FA] StationID cannot be zero. (%s)", section);
			return false ;
		}

		// Append StationID
		szExternData[i-1] = stationID ;
	}

	// Write to FA.
	char szLength[3] ;
	sprintf_s(szLength, 3, "%i", iStationAmount) ;

	bool ret = this->WriteStationInfo(szPort, "240", "N", szLength, "", "", szExternData, szErrCode, szErrMsg ) ;
	delete [] szExternData ;

	if (!ret) {
		return false ;
	}


	// Read and Write each Station Info.
	for (int i=1; i<=iStationAmount; ++i) {
	       // Change Section.
	        CStringA section ;
		section.Format("Station%d", i) ;
		this->m_cStationInitInfoini.SwitchSection( section.GetBuffer() ) ;

		std::string strTemp ;

		// Read StationId.
		CStringA stationID ;
		if ( !this->m_cStationInitInfoini.GetValue("StationID", strTemp) ){
			sprintf(szErrMsg, "[FA] Read stationID of station%d fail", i);			
			return false ;
		}
		
		stationID.Format("%s", strTemp.c_str()) ;

		// Read LineType
		CStringA stationLineType ;
		if ( !this->m_cStationInitInfoini.GetValue("LineType", strTemp) ){
			sprintf(szErrMsg, "[FA] Read Linetype of station%d fail", i);			
			return false ;
		}

		stationLineType.Format("%s", strTemp.c_str()) ;

		// Read Extern Length
		CStringA stationExternLength ;
		if ( !this->m_cStationInitInfoini.GetValue("ExternLength", strTemp) ){
			sprintf(szErrMsg, "[FA] Read Externlength of station%d fail", i);			
			return false ;
		}

		stationExternLength.Format("%s", strTemp.c_str()) ;

		// Write to FA.
		ret = this->WriteStationInfo(szPort, 
					     stationID.GetBuffer(), 
					     stationLineType.GetBuffer(), 
					     stationExternLength.GetBuffer(), 
					     "0", 
					     "", 
					     "", 
					     szErrCode, 
					     szErrMsg ) ;
		if (!ret) {
			sprintf(szErrMsg, "[FA] Write Station%i Info Fail (%s, %s)", i, szErrCode, szErrMsg);
			return false ;
		}

		// Clean Station Info.
		ret = this->EraseTargetStationInfo(szPort, 
						   stationID.GetBuffer(),
						   szErrCode, 
						   szErrMsg) ;
		if (!ret) {
			return false ;
		}
	}

	return true;
}

bool CFA::InitializeFAPhoneInfo(char *szPort, char *szErrCode, char *szErrMsg, FAinfo *pFA)
{
	// Write FA Version
	if(!this->WriteFAVersion(szPort, pFA->cstrFAver.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	// Write ModelName
	if(!this->WriteModelName(szPort, pFA->cstrModelName.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	// Write Mobile ID
	if(!this->WriteMobileID(szPort, pFA->cstrSlot.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	// Write MEVersion
	if(!this->WriteMEVersion(szPort,pFA->cstrMEver.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	// Write SWVersion	
	CStringA csTemp, csSWV ;
	// Handle SWVersion Format
	csTemp.Format("%s", pFA->cstrSWver) ;
	csTemp.Remove('V') ;
	csTemp.Remove('v') ;
	if ( -1!=csTemp.Find('.') ) {
		int index = 0 ;
		csSWV.Format("%02s", csTemp.Tokenize(".", index)) ;
		csSWV.AppendFormat("%-4s", csTemp.Tokenize(".", index)) ;
		csSWV.Replace(' ', '0') ;
	}
	
	if(!this->WriteSWVersion(szPort, csSWV.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	// Write HWVersion
	if(!this->WritePCBAVersion(szPort, pFA->cstrPCBA.GetBuffer(), szErrCode, szErrMsg))
		return false;

	// Write PICASSO
	if(!this->WritePicassoNumber(szPort, pFA->cstrPicasso.GetBuffer(), szErrCode, szErrMsg))
		return false;
	
	return true ;
}

bool CFA::ReadFactoryArea(char* szPort, char* szFAData, char* szErrCode, char* szErrMsg)
{
	return FA_ReadFactoryArea(szPort, szFAData, szErrCode, szErrMsg);
}

bool CFA::WriteFactoryArea(char* szPort, char* szFAData, char* szErrCode, char* szErrMsg)
{
	return FA_WriteFactoryArea(szPort, szFAData, szErrCode, szErrMsg);
}

bool CFA::EraseFactoryArea(char* szPort, char* szErrCode, char* szErrMsg)
{
	return FA_EraseFactoryArea(szPort, szErrCode, szErrMsg);
}

bool CFA::EraseTargetStationInfo(char* szPort, char* index, char* szErrCode, char* szErrMsg)
{
	return FA_EraseTargetStationInfo(szPort, index, szErrCode, szErrMsg);
}

bool CFA::WriteMobileID(char* szPort, char* szSlot, char* szErrCode, char* szErrMsg)
{
	return FA_WriteMobileID(szPort, szSlot, szErrCode, szErrMsg);
}

bool CFA::ReadMobileID(char* szPort, char* szMobileID, char* szErrCode, char* szErrMsg)
{
	return FA_ReadMobileID(szPort, szMobileID, szErrCode, szErrMsg);
}

bool CFA::ReadModelName (char* szPort, char* szModelname, char* szErrCode, char* szErrMsg)
{
	return FA_ReadModelName(szPort, szModelname, szErrCode, szErrMsg);
}

bool CFA::WriteModelName (char* szPort, char* szModelName, char* szErrCode, char* szErrMsg)
{
	return FA_WriteModelName(szPort, szModelName, szErrCode, szErrMsg);
}

bool CFA::WriteFAVersion (char* szPort, char* szFAVersion, char* szErrCode, char* szErrMsg)
{
	return FA_WriteFAVersion(szPort, szFAVersion, szErrCode, szErrMsg);
}

bool CFA::ReadFAVersion (char* szPort, char* szFAVersion, char* szErrCode, char* szErrMsg)
{
	return FA_ReadFAVersion(szPort, szFAVersion, szErrCode, szErrMsg);
}

bool CFA::WriteMEVersion (char* szPort, char* szMEVersion, char* szErrCode, char* szErrMsg)
{
	return FA_WriteMEVersion(szPort, szMEVersion, szErrCode, szErrMsg);
}

bool CFA::ReadMEVersion (char* szPort, char* szMEVersion, char* szErrCode, char* szErrMsg)
{
	return FA_ReadMEVersion(szPort, szMEVersion, szErrCode, szErrMsg);
}

bool CFA::WriteSWVersion (char* szPort, char* szSWVersion, char* szErrCode, char* szErrMsg)
{
	return FA_WriteSWVersion(szPort, szSWVersion, szErrCode, szErrMsg);
}

bool CFA::ReadSWVersion (char* szPort, char* szSWVersion, char* szErrCode, char* szErrMsg)
{
	return FA_ReadSWVersion (szPort, szSWVersion, szErrCode, szErrMsg);
}

bool CFA::WritePCBAVersion (char* szPort, char* szPCBAVersion, char* szErrCode, char* szErrMsg)
{
	return FA_WritePCBAVersion(szPort, szPCBAVersion, szErrCode, szErrMsg);
}

bool CFA::ReadPCBAVersion (char* szPort, char* szPCBAVersion, char* szErrCode, char* szErrMsg)
{
	return FA_ReadPCBAVersion (szPort, szPCBAVersion, szErrCode, szErrMsg);
}

bool CFA::WritePicassoNumber (char* szPort, char* szPicasso, char* szErrCode, char* szErrMsg)
{
	return FA_WritePicassoNumber(szPort, szPicasso, szErrCode, szErrMsg);
}

bool CFA::ReadPicassoNumber (char* szPort, char* szPicasso, char* szErrCode, char* szErrMsg)
{
	return FA_ReadPicassoNumber(szPort, szPicasso, szErrCode, szErrMsg);
}

bool CFA::WriteTranceiverNumber (char* szPort, char* szXCVR, char* szErrCode, char* szErrMsg)
{
	return FA_WriteTranceiverNumber(szPort, szXCVR, szErrCode, szErrMsg);
}

bool CFA::ReadTranceiverNumber  (char* szPort, char* szXCVR, char* szErrCode, char* szErrMsg)
{
	return FA_ReadTranceiverNumber(szPort, szXCVR, szErrCode, szErrMsg);
}

bool CFA::WriteAllVersion(char* szPort, 
			  char* szModelName, 
			  char* szSWVersion, 
			  char* szPCBAVersion, 
			  char* szFAVersion,
			  char* szErrCode,
			  char* szErrMsg)
{
	return FA_WriteAllVersion(szPort,
				  szModelName, 
				  szSWVersion, 
				  szPCBAVersion, 
				  szFAVersion,
				  szErrCode,
				  szErrMsg);
}

bool CFA::ReadAllVersion (char* szPort,
			  char* szModelName,
			  char* szMobileID,
			  char* szSWVersion,
			  char* szPCBAVersion,
			  char* szPicasso,
			  char* szFAVersion,
			  char* szErrCode,
			  char* szErrMsg)
{
	return FA_ReadAllVersion (szPort,
				  szModelName,
				  szMobileID,
				  szSWVersion,
				  szPCBAVersion,
				  szPicasso,
				  szFAVersion,
				  szErrCode,
				  szErrMsg);
}

bool CFA::ReadStationInfo(char* szPort,
			  char* szStationIndex,
			  char* szLineType,
			  char* szTestResult,
			  char* szTestCount,
			  char* szTestDateTime,
			  char* szTestLineStation,
			  char* szReserve,
			  char* szErrCode,
			  char* szErrMsg)
{
	return FA_ReadStationInfo(szPort,
				szStationIndex,
				szLineType,
				szTestResult,
				szTestCount,
				szTestDateTime,
				szTestLineStation,
				szReserve,
				szErrCode,
				szErrMsg);
}

bool CFA::WriteStationInfo(char* szPort,
			   char* szStationIndex,
			   char* szLineType,
			   char* szStationExternLen,
			   char* szTestResult,
			   char* szReserve,
			   char* szStationExternInfo,
			   char* szErrCode,
			   char* szErrMsg)
{
	return FA_WriteStationInfo(szPort,
				   szStationIndex,
				   szLineType,
				   szStationExternLen,
				   szTestResult,
				   szReserve,
				   szStationExternInfo,
				   szErrCode,
				   szErrMsg);
}

bool CFA::CheckStationInfo(char* szPort, 
			   char* szStationIndex,	
			   char* szErrCode,
			   char* szErrMsg)
{
	return FA_CheckStationInfo(szPort, 
				   szStationIndex,	
				   szErrCode,
				   szErrMsg);
}

bool CFA::CheckMobileID(char* szPort, 
			char* szMobileID,
			char* szErrCode,
			char* szErrMsg)
{
	return FA_CheckMobileID(szPort, 
				szMobileID,
				szErrCode,
				szErrMsg);
}

bool CFA::CheckUISWVersion(char* szPort,
			   char* szUISW,
			   char* szFASW,
			   char* szErrCode,
			   char* szErrMsg)
{
	return FA_CheckUISWVersion(szPort,
				   szUISW,
				   szFASW,
				   szErrCode,
				   szErrMsg);
}

bool CFA::CheckUIPCBAVersion (char* szPort,
			      char* szUIPCBA,
			      char* szFAPCBA,
			      char* szErrCode,
			      char* szErrMsg)
{
	return FA_CheckUIPCBAVersion (szPort,
				      szUIPCBA,
				      szFAPCBA,
				      szErrCode,
				      szErrMsg);
}

bool CFA::CheckUIMEVersion(char* szPort, 
			   char* szUIME,
			   char* szFAME,
			   char* szErrCode,
			   char* szErrMsg)
{
	return FA_CheckUIMEVersion(szPort, 
				   szUIME,
				   szFAME,
				   szErrCode,
				   szErrMsg);
}

bool CFA::CheckUIPicasso(char* szPort, 
			 char* szUIPicasso,
			 char* szFAPicasso,
			 char* szErrCode,
			 char* szErrMsg)
{
	return FA_CheckUIPicasso(szPort, 
				 szUIPicasso,
				 szFAPicasso,
				 szErrCode,
				 szErrMsg);
}

bool CFA::CheckUITranceiver (char* szPort, 
			     char* szUIXCVR,
			     char* szFAXCVR,
			     char* szErrCode,
			     char* szErrMsg)
{
	return FA_CheckUITranceiver (szPort, 
				     szUIXCVR,
				     szFAXCVR,
				     szErrCode,
				     szErrMsg);
}

bool CFA::CheckUIModelName  (char* szPort, 
			     char* szUIModelName,
			     char* szFAModelName,
			     char* szErrCode,
			     char* szErrMsg)
{
	return FA_CheckUIModelName  (szPort, 
				     szUIModelName,
				     szFAModelName,
				     szErrCode,
				     szErrMsg);
}

bool CFA::CheckPicassoRule  (char* szPort, 
			     char* szPicasso,
			     char* szModelID,
			     char* szErrCode,
			     char* szErrMsg)
{
	return FA_CheckPicassoRule  (szPort, 
				     szPicasso,
				     szModelID,
				     szErrCode,
				     szErrMsg);
}



bool CPCEnv::TestPCEqiupment(const char *szIni, char *szErrMsg)
{
    // Get Setting
    int cpuSpeedSetting = -1, ramSizeSetting = -1 ;
    CIniAccess SetupIni("PCEnv", szIni) ;
    cpuSpeedSetting = SetupIni.GetValue ("CPU_LIMIT", 0) ;
    ramSizeSetting  = SetupIni.GetValue ("RAM_LIMIT", 0) ;
    if ( 0==cpuSpeedSetting || 0==ramSizeSetting )
    {
        // No Setting, bypass.
        return true ;
    }
    else if ( -1==cpuSpeedSetting || -1==ramSizeSetting )
    {
        sprintf(szErrMsg, "Get CPU Speed / RAM Size Setting Fail.") ;
        return false;
    }

    // Get PC equipment
    int cpuSpeed = -1, ramSize = -1 ;
    cpuSpeed = GetCPUSpeed() ;
    ramSize = GetRAMSize() ;
    if ( 0>=cpuSpeed || 0>=ramSize ) {
        sprintf(szErrMsg, "Get CPU Speed / RAM Size Fail.") ;
        return false;
    }

    // Compare
    if ( cpuSpeed<cpuSpeedSetting||ramSize<ramSizeSetting )
    {
        sprintf(szErrMsg, "Please Upgrade your PC HW.") ;
        return false;
    }

    return true ;
}

int CPCEnv::GetPCCPUSpeed()
{
    return GetCPUSpeed() ;
}

int CPCEnv::GetPCRAMSize ()
{
    return GetRAMSize() ;
}