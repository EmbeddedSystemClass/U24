#include "StdAfx.h"
#include "FAOperator.h"

bool CFAOperator::IsStationSetup(char* faData)
{
	int stationIndex = (byte)faData[128];					
	int temp = (byte)faData[128 + 3];	
	if((stationIndex == 240) && (temp == 1))
	{			
		return true;
	}
	else
	{
		return false;
	}
}

bool CFAOperator::IsInSetupStationList(char* szStationIndex, char* faData)
{
	int stationNum = faData[130] - 8;
	int nStationIndex = atoi(szStationIndex);
	if(nStationIndex == 240)
		return true;
	for(int i = 0; i < stationNum; i++)
	{		
		int tempStationIndex = faData[139 + i];
		if( nStationIndex == tempStationIndex)
		{
			return true;
		}
	}
	return false;
}

CFAOperator::CFAOperator(void)
{
	m_str_errCode = _T("");
	m_str_errMsg = _T("");
	memset(m_sz_faData, 0, FA_SIZE);	
}

CFAOperator::~CFAOperator(void)
{
}

bool CFAOperator::SetFAData(char* sz_faData)
{
	for (int i=0; i<FA_SIZE; i++)
	{
		m_sz_faData[i] = sz_faData[i];
	}
	return true;
}

bool CFAOperator::GetFAData(char* sz_faData)
{
	for (int i=0; i<FA_SIZE; i++)
	{
		sz_faData[i] = m_sz_faData[i];
	}
	return true;
}
bool CFAOperator::SetFA(char* sz_faData)
{	
	for (int i=0; i<FA_SIZE; i++)
	{
		m_sz_faData[i] = (sz_faData[i]=='|')? 0:sz_faData[i];
	}
	return true;	
}

bool CFAOperator::GetFA(char* sz_faData)
{
	for (int i=0; i<FA_SIZE; i++)
	{
		sz_faData[i] = (m_sz_faData[i]==0)? '|':m_sz_faData[i];
	}	
	return true;
}

void CFAOperator::GetPicassoNumber(char* sz_picasso)
{
	for (int i=0; i< PICASSO_LENGTH; i++)
	{
		sz_picasso[i] = m_sz_faData[i+32];
	}
	sz_picasso[PICASSO_LENGTH] = '\0';

	return;
}

bool CFAOperator::SetPicassoNumber(const char* sz_picasso)
{
	if(strlen(sz_picasso) != PICASSO_LENGTH)
 	{
 		return false;
 	}

	for (int i=0; i< PICASSO_LENGTH; i++)
	{
		m_sz_faData[i+32] = sz_picasso[i];
	}

	return true;
}

void CFAOperator::GetModelName(char* sz_modelName)
{
	for (int i=0; i<7 ;i++)
	{
		sz_modelName[i] = m_sz_faData[i+2];
	}

	sz_modelName[7] = '\0';

	return;
}

bool CFAOperator::SetModelName(CString str_modelname)
{
	if(str_modelname.GetLength() > 7)
	{
		return false;
	}
	char sz_modelName[20]={0};
	CStringToChar(str_modelname,sz_modelName);

	for(UINT i = 0; i < strlen(sz_modelName); i++)
	{
		m_sz_faData[i + 2] = sz_modelName[i];		
	}

	return true;

}


void CFAOperator::GetSWVersion(char* sz_swVersion)
{
	for (int i=0;i<6;i++)
	{
		sz_swVersion[i] = m_sz_faData[i+21];
	}

	sz_swVersion[6] = '\0';
}

bool CFAOperator::SetSWVersion(CString str_swVersion)
{
	/*if(str_swVersion.GetLength() > 6)
	{
		return false;
	}
	char sz_swVersion[20]={0};
	CStringToChar(str_swVersion,sz_swVersion);

	for (UINT i=0; i< strlen(sz_swVersion); i++ )
	{
		m_sz_faData[i+21] = sz_swVersion[i];		
	}*/

	return true;
	
}


bool CFAOperator::EraseFactoryArea()
{
	char szTempDRMKey[512] = {0};
	memset(szTempDRMKey, 0x7E, 512);
	SetFAData(szTempDRMKey);		
	return true;
}

bool CFAOperator::FA_Station_Setup (char* szStationAmount, char* szStationList)
{
	int iAmount = atoi(szStationAmount) ;

	// Write Setup Infomation
	char szTmp[512] ;
	char szReserved[512] ;
	::memset(szTmp, 0, 512) ;
	::memset(szReserved, 0, 512) ;

	strncpy_s(szTmp, szStationList, iAmount) ;
	bool ret = WriteStationInfo ("240", "N", szStationAmount, "0", szReserved,szTmp) ;

	return ret ;
}
bool CFAOperator::WriteStationInfo(   char* szStationIndex,
										 char* szLineType,
										 char* szStationExternLen,
										 char* szTestResult,
										 char* szReserve,
										 char* szStationExternInfo
										 )
{
	char faData[512] = {0};
	char szTestResultReplace;
	if(*szTestResult == 'F')  szTestResultReplace = 0x0f;
	if(*szTestResult == '0')  szTestResultReplace = 0x00;
	if(*szTestResult == '1')  szTestResultReplace = 0x01;
	if(GetFAData(faData))
	{
		if(!IsInSetupStationList(szStationIndex, faData))
		{		
			m_str_errCode = _T("5019");
			m_str_errMsg = _T("The station you want write isn't setup yet!");
			return false;
		}
		if(atoi(szStationIndex) == 240)
		{
			//write setup station
			faData[128] = (byte)240;
			faData[129] = *szLineType;
			faData[130] = atoi(szStationExternLen) + 8;
			faData[131] = 0x01;

			CTime tCurrent = CTime::GetCurrentTime();	
			faData[132] = (tCurrent.GetMinute());
			faData[133] = (tCurrent.GetHour());
			faData[134] = (tCurrent.GetDay());
			faData[135] = (tCurrent.GetMonth());
			faData[136] = (tCurrent.GetYear() % 100);

			char szComputer[1024] = {0};
			DWORD dwSize = 1024;
			GetComputerNameA(szComputer,&dwSize);
			if(strlen(szComputer) == 6)
			{
				char* szT = szComputer + 1;
				char* szD = szComputer + 4;
				byte temp = 0;
				int bT = atoi(szT);
				int bD = atoi(szD);
				if(bT > 99 || bD > 99)
				{
					m_str_errCode = _T("5018");
					m_str_errMsg = _T("Computer name is wrong!");
					return false;
				}
				temp |= bT;
				temp |= (bD<<4);

				faData[137] = temp;
			}
			else
			{
				m_str_errCode = _T("5012");
				m_str_errMsg = _T("Computer name's length is not 6!");
				
				return false;
			}

			faData[138] = *szReserve; 
			if(atoi(szStationExternLen) != 0)
			{
				int nTempLength = (int)strlen(szStationExternInfo);
				for(int i = 0; i < nTempLength; i++)
				{
					faData[139 + i] = *(szStationExternInfo + i);
				}
			}			
		}
		else
		{			
			int stationIndex = (byte)faData[128];
			int stationLen = (byte)faData[128 + 2];
			int totalSearchLength = 128 + 3 + stationLen;
			bool writeDone = false;//flag for do while loop
			do 
			{
				stationIndex = (byte)faData[totalSearchLength];
				stationLen = (byte)faData[totalSearchLength + 2];
				if(stationIndex == atoi(szStationIndex))
				{
					//write on exist  need test count ++					
					faData[totalSearchLength + 1] = *szLineType;
					faData[totalSearchLength + 2] = atoi(szStationExternLen) + 8;
					int tempTestResultTestCount = 0;
					tempTestResultTestCount |= (szTestResultReplace << 4);//test result
					int tempTestCount = faData[totalSearchLength + 3];
					tempTestCount = tempTestCount & 0x0F;
					tempTestCount++;
					if(tempTestCount == 16) tempTestCount = 1;
					tempTestResultTestCount |= tempTestCount;//test count
					faData[totalSearchLength + 3] = tempTestResultTestCount;

					CTime tCurrent = CTime::GetCurrentTime();			
					faData[totalSearchLength + 4] = (tCurrent.GetMinute());
					faData[totalSearchLength + 5] = (tCurrent.GetHour());
					faData[totalSearchLength + 6] = (tCurrent.GetDay());
					faData[totalSearchLength + 7] = (tCurrent.GetMonth());
					faData[totalSearchLength + 8] = (tCurrent.GetYear() % 100);

					char szComputer[1024] = {0};
					DWORD dwSize = 1024;
					GetComputerNameA(szComputer,&dwSize);
					if(strlen(szComputer) == 6)
					{
						char* szT = szComputer + 1;
						char* szD = szComputer + 4;
						byte temp = 0;
						int bT = atoi(szT);
						int bD = atoi(szD);
						if(bT > 99 || bD > 99)
						{
							m_str_errCode = _T("5018");
							m_str_errMsg = _T("Computer name is wrong!");
							
							return false;
						}						
						temp |= bT;
						temp |= (bD<<4);

						faData[totalSearchLength + 9] = temp;
					}
					else
					{
						m_str_errCode = _T("5012");
						m_str_errMsg = _T("Computer name's length is not 6!");
						
						return false;
					}

					faData[totalSearchLength + 10] = *szReserve; 
					if(atoi(szStationExternLen) != 0)
					{						
						for(int i = 0; i < (int)strlen(szStationExternInfo); i++)
						{
							faData[totalSearchLength + 11 + i] = *(szStationExternInfo + i);
						}
					}
					writeDone = true;
				}
				if(stationIndex == 126)
				{
					//write on blank
					faData[totalSearchLength] = atoi(szStationIndex);
					faData[totalSearchLength + 1] = *szLineType;
					faData[totalSearchLength + 2] = atoi(szStationExternLen) + 8;
					int tempTestResultTestCount = 0;
					tempTestResultTestCount |= (szTestResultReplace << 4);//test result
					int tempTestCount = faData[totalSearchLength + 3];
					tempTestCount = tempTestCount & 0x0F;
					tempTestCount++;
					if(tempTestCount == 16) tempTestCount = 1;
					tempTestResultTestCount |= (tempTestCount);//test count
					faData[totalSearchLength + 3] = tempTestResultTestCount;

					CTime tCurrent = CTime::GetCurrentTime();			
					faData[totalSearchLength + 4] = (tCurrent.GetMinute());
					faData[totalSearchLength + 5] = (tCurrent.GetHour());
					faData[totalSearchLength + 6] = (tCurrent.GetDay());
					faData[totalSearchLength + 7] = (tCurrent.GetMonth());
					faData[totalSearchLength + 8] = (tCurrent.GetYear() % 100);

					char szComputer[1024] = {0};
					DWORD dwSize = 1024;
					GetComputerNameA(szComputer,&dwSize);
					if(strlen(szComputer) == 6)
					{
						char* szT = szComputer + 1;
						char* szD = szComputer + 4;
						byte temp = 0;
						int bT = atoi(szT);
						int bD = atoi(szD);
						if(bT > 99 || bD > 99)
						{
							m_str_errCode = _T("5018");
							m_str_errMsg = _T("Computer name is wrong!");
							
							return false;
						}						
						temp |= bT;
						temp |= (bD<<4);

						faData[totalSearchLength + 9] = temp;
					}
					else
					{
						m_str_errCode = _T("5012");
						m_str_errMsg = _T("Computer name's length is not 6!");
						
						return false;
					}

					faData[totalSearchLength + 10] = *szReserve; 
					if(atoi(szStationExternLen) != 0)
					{						
						for(int i = 0; i < (int)strlen(szStationExternInfo); i++)
						{
							faData[totalSearchLength + 11 + i] = *(szStationExternInfo + i);
						}
					}
					writeDone = true;
				}
				totalSearchLength += (stationLen + 3);

				//no place to write station result ,exit.
				if ((totalSearchLength + strlen(szStationExternInfo) + strlen(szReserve)) > FA_SIZE)
				{
					m_str_errCode = _T("5021");
					m_str_errMsg = _T("No enough space to write FA result!");

					return false;
				}
			} while (!writeDone);
		}
	}
	else
	{
		
		return false;
	}
	if(SetFAData(faData))
	{
		
		return true;
	}
	else
	{
		
		return false;
	}
}



void CFAOperator::GetMobileID( char *sz_mobileID )
{
	for (int i=0; i < 11; i++)
	{
		sz_mobileID[i] = m_sz_faData[i+9];
	}
	sz_mobileID[11] = '\0';
}

bool CFAOperator::SetMobileID( const char *sz_mobileID)
{
	for (int i = 0; i< 11; i++)
	{
		m_sz_faData[i+9] = sz_mobileID[i];
	}

	return true;
}

void CFAOperator::GenerateMobileID( char * sz_mobileID ,int szSlot3)
{
	char szComputer[1024] = {0};

	DWORD dwSize = 1024;

	GetComputerNameA(szComputer,&dwSize);

	if(strlen(szComputer) == 6 && szComputer[0] == 'T' && szComputer[3] == 'D')

	{

		szComputer[3] = 0;

		szComputer[6] = 0;

		char* szT = szComputer + 1;

		char* szD = szComputer + 4;

		sz_mobileID[0] = TransferIntToHex_BCD(atoi(szT));

		sz_mobileID[1] = TransferIntToHex_BCD(atoi(szD));

		sz_mobileID[2] = 0;

	}

	else

	{//Fake data T01D01

		sz_mobileID[0] = 1;

		sz_mobileID[1] = 1;

		sz_mobileID[2] = 0;

	}

	sz_mobileID[3] = 0;//Adapter

	sz_mobileID[4] = TransferIntToHex_BCD(szSlot3);//Slot

	CTime tCurrent = CTime::GetCurrentTime();

	sz_mobileID[5] = TransferIntToHex_BCD(tCurrent.GetDay());

	sz_mobileID[6] = TransferIntToHex_BCD(tCurrent.GetMonth());

	sz_mobileID[7] = TransferIntToHex_BCD(tCurrent.GetYear() % 100);

	sz_mobileID[8] = TransferIntToHex_BCD(tCurrent.GetHour());

	sz_mobileID[9] = TransferIntToHex_BCD(tCurrent.GetMinute());

	sz_mobileID[10] = TransferIntToHex_BCD(tCurrent.GetSecond());

	sz_mobileID[11] = '\0';
}

int CFAOperator::TransferIntToHex_BCD( int nInt )
{
	// Warning: This function changes 16 to 0x16, not 0x10  !!
	int nHex = ((nInt/10) << 4) + (nInt % 10);

	return nHex;
}

bool CFAOperator::TransferHexToInt_BCD( const int nHex, char* sz )
{
	// Warning: This function changes 0x12 to "12", not "18"  !!
	bool bRes = ( (nHex >> 4) <= 9)	&& ((nHex & 0x0F) <= 9);

	if (bRes)
	{
		*sz = (nHex >> 4) + '0'; 
		*(sz+1) = (nHex & 0x0F) + '0';  
		*(sz+2) = 0;
	}

	return bRes;
}

void CFAOperator::GetMobileIDHexStr( char *szMobileID )
{
	char sz_mobileID[12] = {0};
	char sz_tmp[5] = {0};
	GetMobileID(sz_mobileID);
	int i;
	for(i=0; i < 11; i++)
	{
		TransferHexToInt_BCD((int)sz_mobileID[i], sz_tmp);
		szMobileID[2*i] = sz_tmp[0];
		szMobileID[2*i+1] = sz_tmp[1];
	}
	szMobileID[2*i] = '\0';
}


bool CFAOperator::IsValidePicasso( char *sz_picasso )
{
	for (int i=0; i<PICASSO_LENGTH; i++)
	{
		if ((sz_picasso[i] == 0)||!isalnum((int)sz_picasso[i]))
		{
			return false;
		}
	}
	return true;
}

bool CFAOperator::IsValideMobileID( char *szMobileID )
{
	char c = szMobileID[0];
	for (int i = 1; i< 11; i++)
	{
		if (szMobileID[i] != c)
		{
			return true;
		}
	}
	return false;
}

CString CFAOperator::GetErrorCode()
{
	return m_str_errCode;
}

CString CFAOperator::GetErrorMsg()
{
	return m_str_errMsg;
}