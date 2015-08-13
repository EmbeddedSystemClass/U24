#include "stdafx.h"
#include "CCMU200.h"


RF_IMPLEMENT_DYNCREATE(CCMU200)

CCMU200::CCMU200()
{
	m_piFunc_Dev = new CFuncDev_CMU(dynamic_cast<ITesterDevice*>(this));
	m_piFuncBT=new CFuncBT_CMU(dynamic_cast<ITesterDevice*>(this));
	m_piFuncWLAN = new CFuncWLAN_CMU(dynamic_cast<ITesterDevice*>(this));
	m_piFuncGPS = new CFuncGPS_CMU(dynamic_cast<ITesterDevice*>(this));

	m_bInternalReference = true;
}
 
 
CCMU200::~CCMU200()
{
	if (m_piFuncBT)
	{
		delete m_piFuncBT;
	}
	if (m_piFuncWLAN)
	{
		delete m_piFuncWLAN;
	}
	if (m_piFuncGPS)
	{
		delete m_piFuncGPS;
	}

}

bool CCMU200::SetInputLoss(int iMode, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:INPUT%d %8.3f; *OPC?", iMode, g_iRFInputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputLoss(int iMode, double dLoss)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	if (! GPIB_Write(std::string("0;CONFigure:USER:CORRection:LOSS:TABLe:ENABle OFF")))
		return false;

	sprintf_s(szCommand, 255, "%d;SOURce:CORRection:LOSS:OUTPUT%d %8.3f; *OPC?", iMode, g_iRFOutputPort, dLoss);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetInputPort(int iMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;INPut RF%d; *OPC?", iMode, g_iRFInputPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}

bool CCMU200::SetOutputPort(int iMode)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand, strResponse;

	sprintf_s(szCommand, 255, "%d;OUTPut RF%d; *OPC?", iMode, g_iRFOutputPort);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strResponse))
		return false;
	if (strstr(strResponse.c_str(), "1") == NULL)
		return false;

	return true;
}



/********************************************
			WCDMA Test
********************************************/

bool CCMU200::Initial_CMU200()
{
	if (! GPIB_OPComplete(std::string("*CLS; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("SYSTem:NONV:DISable; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("TRACe:REMote:MODE:DISPlay ON; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("CONFigure:SYNC:FREQ:REF:MODE INT; *OPC?")))
		return false;

	if (! GPIB_OPComplete(std::string("SYSTem:REMote:RDMode ON; *OPC?")))
		return false;

	return true;
}

bool CCMU200::Initial_WCDMA()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, MAP_COMMAND, _WCDMA19UEFDD_NSIG, IDSTR_WCDMA19UEFDD_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_WCDMA19UEFDD_NSIG))
		return false;
	if (! SetOutputPort(_WCDMA19UEFDD_NSIG))
		return false;

	// WCDMA Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _WCDMA19UEFDD_SIG, IDSTR_WCDMA19UEFDD_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_WCDMA19UEFDD_SIG))
		return false;
	if (! SetOutputPort(_WCDMA19UEFDD_SIG))
		return false;

	return true;
}
bool CCMU200::SetRFAnalyzerFreq(double dFreq)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;RFANalyzer:FREQuency %.3f MHz; *OPC?", g_iMode, dFreq);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;
	return true;
}

bool CCMU200::SetCDMAChannel(int iChannel)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf(szCommand, "%d;RFANalyzer:FREQuency:UNIT CH", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	sprintf(szCommand, "%d;RFANalyzer:FREQuency %d;*OPC?", g_iMode, iChannel);
	strCommand = szCommand;
	if ( ! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::RFCDMASetTxInputLevelMaxPower(double dlPwrLvl)			////////////////////////////////
{
	char szCommand[256];
	memset(szCommand, 0, 256); 
	std::string strCommand;

	sprintf(szCommand, "%d;LEVel:MODE MANual;:LEVel:MAXimum %4.1f", g_iMode ,dlPwrLvl);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_InitMaxPowerMeasurement()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;DEFault:POWer:MAXimum:CONTrol ON; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	sprintf_s(szCommand, 255, "%d;CONFigure:POWer:MAXimum:CONTrol:REPetition SINGleshot,NONE,NONE; *OPC?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_OPComplete(strCommand))
		return false;

	return true;
}

bool CCMU200::WCDMA_FetchMaxPower(std::string& strMeasured)
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, "%d;READ:POWer:MAXimum?", g_iMode);
	strCommand = szCommand;
	if (! GPIB_Querry(strCommand, strMeasured))
		return false;

	return true;
}

int CCMU200::WCDMAFetchPower(float* f_MaxPwr_Result,UINT& iResultNum)
{
	char cCmd[100]="";
	String strReadBuffer;
	UINT uiTemp=MAX_RESULT_BUFFER_SIZE;
	float fTemp[MAX_RESULT_BUFFER_SIZE];
	int iRetryTimes=50;
	String szReadData;
	std::string strCommand;

	sprintf(cCmd, "%d;INITiate:NPOWer", g_iMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;
	do{
		sprintf(cCmd, "%d;FETCh:NPOWer:STATus?", g_iMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;
		Sleep(20);
	}while(NPOS(szReadData.find(_T("RDY"))) && --iRetryTimes>0);

	if(NPOS(szReadData.find(_T("RDY")))==false){ 
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

		sprintf(cCmd, "%d;SAMPle:NPOWer?", g_iMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

		strReadBuffer=szReadData;
		if(!GetFloatResultFromString(strReadBuffer,fTemp,uiTemp) ||iResultNum<1)
			return false;//ERR_ALLOCATE_MEMORY_NOT_ENOUGH;
		*f_MaxPwr_Result=fTemp[0];

		iResultNum=1;
		sprintf(cCmd, "%d;ABORt:NPOWer", g_iMode);
		strCommand = cCmd;
		if (GPIB_Write(strCommand) == false)

			return true;
	}

	return true;//ERR_FETCHNPOWERREZ_TIME_OUT;
}



/********************************************
			GSM Test
********************************************/
bool CCMU200::Initial_GSM()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	// GSM 900 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM900MS_NSIG, IDSTR_GSM900MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM900MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM900MS_NSIG))
		return false; 

	// GSM900 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM900MS_SIG, IDSTR_GSM900MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM900MS_SIG))
		return false;
	if (! SetOutputPort(_GSM900MS_SIG))
		return false;

	// GSM 1800 Non-signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1800MS_NSIG, IDSTR_GSM1800MS_NSIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1800MS_NSIG))
		return false;
	if (! SetOutputPort(_GSM1800MS_NSIG))
		return false; 

	// GSM1800 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _GSM1800MS_SIG, IDSTR_GSM1800MS_SIG);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_GSM1800MS_SIG))
		return false;
	if (! SetOutputPort(_GSM1800MS_SIG))
		return false;

	return true;
}

int CCMU200::CalGsmSetBand(UINT uiMode)
{
	switch(uiMode)
	{

	case 850:
		SetDeviceID(m_h_GSM850MS_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _GSM850MS_NSIG;
		return true;
		break;
	case 900:
		SetDeviceID(m_h_GSM_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _GSM900MS_NSIG;
		return true;
		break;
	case 1800:
		SetDeviceID(m_h_DCS_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _GSM1800MS_NSIG;
		return true;
		break;
	case 1900:
		SetDeviceID(m_h_PCS_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _GSM1900MS_NSIG;
		return true;
		break;
	default:
		SetDeviceID(m_h_RF_NSIG);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _RF_NSIG;
		break;
	}
	return false;
}

int CCMU200::GSMFetchPower(float *fTestPowerArray)
{
	int j = 0;
	char cCmd[100]="";
	char seps[] = ",";
	char *token = NULL;
	int iAvgBurst = 5;
	int iAvgBurstPower = 6;
	String szReadData;
	std::string strCommand;
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));

	sprintf(cCmd, "%d;INITiate:POWer:NORMal:GMSK; *OPC?", g_iMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	sprintf(cCmd, "%d;FETCh:SCALar:POWer:NORMal:GMSK?", g_iMode);	
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	String strRead = szReadData;
#ifdef _UNICODE
	string strReadAnsi = CStr::ToAnsiString(strRead);
#else
	string strReadAnsi = strRead;
#endif
	token = strtok( (char*)strReadAnsi.c_str(), seps );
	while( token != NULL )
	{
		if(j == iAvgBurst)
		{
			if((strstr(token,"MATC") != NULL))
				fTestPowerArray[j] = 1;
			else
				fTestPowerArray[j] = 0;

			token = strtok( NULL, seps );
			j++;
			continue;
		}
		if(j==iAvgBurstPower)
		{
			if(strstr(token,"NAN"))
			{				
				sprintf(cCmd, "%d;Abort:POWer:NORMal:GMSK;*OPC?", g_iMode);
				strCommand = cCmd;
				GPIB_Querry(strCommand, szReadData);
				return false;
			}

			fTestPowerArray[j]=(float)atof(token);

			token = strtok( NULL, seps );
			j++;
			continue;

		}
		fTestPowerArray[j]= (float)atof(token);
		token = strtok( NULL, seps );
		j++;
	}


	sprintf(cCmd, "%d;Abort:POWer:NORMal:GMSK;*OPC?",g_iMode);	
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return true;
}

bool CCMU200::SetGsmCMU(double dTestFrequency)
{
	char cCmd[100]="";
	std::string strCommand;
	ZeroMemory(cCmd, sizeof(cCmd)/sizeof(char));

	String strMsg = _T("");
	int m_iErrorCode = 0;

	sprintf(cCmd,"%d;RFANalyzer:CHANnel %4.1f MHz", g_iMode, dTestFrequency);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)	
		return false;

	sprintf(cCmd, "%d;LEVel:MODE MANual", g_iMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd, "%d;LEVel:MAXimum 38.00 dBm", g_iMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)
		return false;

	sprintf(cCmd,"%d;RFANalyzer:TSEQuence ANY", g_iMode);
	strCommand = cCmd;
	if (GPIB_Write(strCommand) == false)	
		return false;	

	return true;
}

bool CCMU200::GetFloatResultFromString(const String& strSource,float * fResult,UINT& iResultNum)
{
	StrVtr vtrStrTemp;
	StrVtrItr vtrItrTemp;
	int iIndex=0;

	CStr::ParseString(strSource,_T(","),vtrStrTemp);
	if(vtrStrTemp.size()>iResultNum)
		return false;
	else
		iResultNum=vtrStrTemp.size();

	for(vtrItrTemp=vtrStrTemp.begin();vtrItrTemp!=vtrStrTemp.end();++vtrItrTemp,++iIndex)
	{
		if(NPOS((*vtrItrTemp).find(_T("NAN")))==false)
			fResult[iIndex]=-99999;
		else

			fResult[iIndex]=CStr::StrToFloat(*vtrItrTemp);
	}

	return true;
}


/********************************************
			CDMA2000Test
********************************************/
bool CCMU200::FetchCDMA2000NPower(double *dlMeasuredPower)
{
	bool bRet = false;
	char cCmd[100]="";
	std::string strCommand;
	String szReadData;
	ZeroMemory(cCmd, sizeof(cCmd));
	char seps[] = ",";
	char *token = NULL;
	double dTempArray[20];
	int i = 0;

	sprintf(cCmd, "%d;ABORt:NPOW;*OPC?", g_iMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	sprintf(cCmd, "%d;INIT:NPOW;*OPC?", g_iMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	String strReadBuffer;
	int iRetryTimes=40;
	do
	{
		sprintf(cCmd, "%d;FETC:NPOW:STAT?", g_iMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;
		strReadBuffer = szReadData;
		--iRetryTimes;

		Sleep(50);

	} while(NPOS(strReadBuffer.find(_T("RDY"))) && --iRetryTimes>0);

	if(NPOS(strReadBuffer.find(_T("RDY")))==false){
		sprintf(cCmd, "%d;STAT:OPER:SYMB?", g_iMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

		sprintf(cCmd, "%d;FETCH:NPOW?", g_iMode);
		strCommand = cCmd;
		if (GPIB_Querry(strCommand, szReadData) == false)
			return false;

		strReadBuffer = szReadData;

#ifdef _UNICODE
		string strReadAnsi = CStr::ToAnsiString(strReadBuffer);
#else
		string strReadAnsi = strReadBuffer;
#endif
		token = strtok( (char*)strReadAnsi.c_str(), seps );
		while( token != NULL ){
			dTempArray[i]= (double)atof(token);
			token = strtok( NULL, seps );
			i++;
		}
		*dlMeasuredPower = dTempArray[3];

		return true;
	}
	sprintf(cCmd, "%d;ABORt:NPOW;*OPC?", g_iMode);
	strCommand = cCmd;
	if (GPIB_Querry(strCommand, szReadData) == false)
		return false;

	return bRet;
}

bool CCMU200::SetCDMA2000Band(UINT uiBand)
{
	switch(uiBand)
	{
	case PHONE_MODE_CDMA_800:
		SetDeviceID(m_h_CDMACellular_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _CDMA2KCellMS_NSig;
	//	m_chCMUModeStr = IDSTR_CDMA2KCellMS_NSig;
		return true;
		break;
	case PHONE_MODE_CDMA_1900:
		SetDeviceID(m_h_CDMAPCS_NSig);  //set device ID of IFunc_Dev::m_cGPIBInterface
		g_iMode = _CDMA2KPCSMS_NSig;
	//	m_chCMUModeStr = IDSTR_CDMA2KPCSMS_NSig;
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CCMU200::Initial_CDMA2000()
{
	char szCommand[256];
	memset(szCommand, 0, 256);
	std::string strCommand;

	sprintf_s(szCommand, 255, MAP_COMMAND, _CDMA2KCellMS_NSig, IDSTR_CDMA2KCellMS_NSig);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_CDMA2KCellMS_NSig))
		return false;
	if (! SetOutputPort(_CDMA2KCellMS_NSig))
		return false;

	// CDMA2000 Signaling
	sprintf_s(szCommand, 255, MAP_COMMAND, _CDMA2KCellMS_Sig, IDSTR_CDMA2KCellMS_Sig);
	strCommand = szCommand;
	if (! GPIB_Write(strCommand))
		return false;
	if (! SetInputPort(_CDMA2KCellMS_Sig))
		return false;
	if (! SetOutputPort(_CDMA2KCellMS_Sig))
		return false;

	return true;
}
