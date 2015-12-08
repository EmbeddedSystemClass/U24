// 1.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FA_Operator.h"
#include "assert.h"

#include <atlenc.h>

#include <string>
#include <vector>
#include <algorithm>
using std::find;
using std::string;
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CMy1App

bool GetDllPath(CString& path)
{
	CString m_Dll_File_Path("");
	TCHAR Temp_Global[256]={0};
	CString Message;

	TCHAR TempBuff[150];
	CString strTemp;
	CString strPath;
	CFileFind filefindctrl;
	/*GetCurrentDirectory(sizeof(TempBuff), TempBuff);
	strPath.Format(_T("%s"), TempBuff);*/
	CString pthToolDir;
	::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
	pthToolDir.ReleaseBuffer();
	pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));
	strPath = pthToolDir;

	strPath += _T("\\DLLPath_FAData\\DLLPath_FAData.ini");

	if(filefindctrl.FindFile(strPath))
	{
		m_Dll_File_Path = strPath;
	}
	else
	{		
		m_Dll_File_Path = "";
		return false;
	}

	filefindctrl.Close();

	::GetPrivateProfileString(_T("DLLPath_FAData"), _T("DLLPath_FAData"), _T(" "), Temp_Global, sizeof(Temp_Global), m_Dll_File_Path);
	path.Format(_T("%s"),Temp_Global);
	return true;
}

bool ExportDllFunction(char* szErrCode,
					   char* szErrMsg)
{
	CString dllPath("");
	if(GetDllPath(dllPath))
	{
		if(hFADll == NULL)
		{
			hFADll = LoadLibrary(dllPath);
		}
		else
		{
			FreeLibrary(hFADll);
			hFADll = NULL;
			hFADll = LoadLibrary(dllPath);
		}
		if(hFADll != NULL)
		{
			if(pfnReadFA == NULL)
			{
				pfnReadFA = (ReadFA)GetProcAddress(hFADll, "ReadFA");
			}
			else
			{
				pfnReadFA = NULL;
				pfnReadFA = (ReadFA)GetProcAddress(hFADll, "ReadFA");
			}
			if(pfnWriteFA == NULL)
			{
				pfnWriteFA = (WriteFA)GetProcAddress(hFADll, "WriteFA");			
			}
			else
			{
				pfnWriteFA = NULL;
				pfnWriteFA = (WriteFA)GetProcAddress(hFADll, "WriteFA");			
			}
		}
	}
	else
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5017"));
		_stprintf_s(szErrMsg,  strlen(_T("Can't find dll(contain FA Data)!")) + 1, _T("%s"), _T("Can't find dll(contain FA Data)!"));
		return false;
	}
	if((pfnReadFA != NULL) && (pfnWriteFA != NULL))
		return true;
	else
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,   strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
}

bool freeDllLibrary()
{	
	if(FreeLibrary(hFADll))
	{
		hFADll = NULL;
		pfnReadFA = NULL;
		pfnWriteFA = NULL;
		return true;
	}
	else
	{
		hFADll = NULL;
		pfnReadFA = NULL;
		pfnWriteFA = NULL;
		return false;
	}
}

bool IsStationSetup(unsigned char* faData)
{
	int stationIndex = (byte)faData[128];					
	int temp = faData[128 + 3];	
	if((stationIndex == 240) && (temp == 1))
	{			
		return true;
	}
	else
	{
		return false;
	}
}

bool IsInSetupStationList(char* szStationIndex, unsigned char* faData)
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


bool SearchTargetStation(unsigned char* faData, int index, int& nTestResult)
{
	int stationIndex = (byte)faData[128];
	int stationLen = (byte)faData[128 + 2];
	int totalSearchLength = 128 + 3 + stationLen;	
	bool findTarget = false;//flag for do while loop
	do 
	{
		stationIndex = (byte)faData[totalSearchLength];
		stationLen = (byte)faData[totalSearchLength + 2];

		if(stationIndex == index)
		{						
			int temp = faData[totalSearchLength + 3];
			nTestResult = ((temp & 0xF0) >> 4);
			findTarget = true;
			break;
		}
		else
		{
			totalSearchLength += (stationLen + 3);	
		}		
		if(stationIndex == 0)
		{
			break;
		}
	} while (!findTarget);
	return findTarget;
}

bool SearchTargetStation(unsigned char* faData, int index, int& nTargetPosition, int& nTargetStationInfoLength)
{
	int stationIndex = (byte)faData[128];
	int stationLen = (byte)faData[128 + 2];
	int totalSearchLength = 128 + 3 + stationLen;	
	bool findTarget = false;//flag for do while loop
	do 
	{
		stationIndex = (byte)faData[totalSearchLength];
		stationLen = (byte)faData[totalSearchLength + 2];

		if(stationIndex == index)
		{						
			nTargetPosition = totalSearchLength;
			nTargetStationInfoLength = stationLen;
			findTarget = true;
			break;
		}
		else
		{
			totalSearchLength += (stationLen + 3);	
		}		
		if(stationIndex == 0)
		{
			break;
		}
	} while (!findTarget);
	return findTarget;
}
unsigned char MaybeStore0(unsigned char inData)
{
	if(inData == 0)
	{
		return '0';
	}
	else
	{
		return inData;
	}
}
unsigned char MaybeRestore0(unsigned char inData)
{
	if(inData == '0')
	{
		return 0;
	}
	else
	{
		return inData;
	}
}
unsigned char MaybeRestore_(unsigned char inData)
{
	if(inData == 0)
	{
		return '|';
	}
	else
	{
		return inData;
	}
}
bool WriteFA_SpecialForStore0(char* szPort, unsigned char* faData, char* szErrCode, char* szErrMsg)
{
	for(int i = 127; i < 512; i++)
	{
		faData[i] = MaybeStore0(faData[i]);
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ReadFA_SpecialForRetore0(char* szPort, unsigned char* faData, char* szErrCode, char* szErrMsg)
{
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		for(int i = 127; i < 512; i++)
		{
			faData[i] = MaybeRestore0(faData[i]);
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool ReadWriteFA_SpecialForWolf(char* szPort)
{
	unsigned char faData[512] = {0};
	char szErrCode[512] = {0};
	char szErrMsg[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		for(int i = 0; i < 512; i++)
		{
			faData[i] = MaybeRestore_(faData[i]);
		}		
	}
	else
	{
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ReadWriteMoblileID_SpecialForWolf(char* szPort, char* MobileID)
{
	unsigned char faData[512] = {0};
	char szErrCode[512] = {0};
	char szErrMsg[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		for(int i = 9; i < 20; i++)
		{
			faData[i] = MobileID[i - 9];
		}		
	}
	else
	{
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void GetErrorCode(int nIndexStation, char* szErrCode)
{
	switch(nIndexStation)
	{
	case 1:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1130"));
		}break;
	case 2:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1131"));
		}break;
	case 3:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1132"));
		}break;
	case 4:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1133"));
		}break;
	case 5:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1134"));
		}break;
	case 6:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1135"));
		}break;
	case 7:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1136"));
		}break;
	case 8:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1137"));
		}break;
	case 9:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1138"));
		}break;
	case 10:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1139"));
		}break;
	case 11:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1140"));
		}break;
	case 12:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1141"));
		}break;
	case 13:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1142"));
		}break;
	case 14:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1143"));
		}break;
	case 15:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1144"));
		}break;
	case 16:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1145"));
		}break;
	case 240:
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1146"));
		}break;
	}
}

FA_OPERATOR_API bool FA_RWFA_Wolf (char* szPort)
{
	char szErrCode[50] = {0};
	char szErrMsg[50] = {0};
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);
	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}

	if(ReadWriteFA_SpecialForWolf(szPort))
	{
		return true;
	}
	else
	{
		return false;
	}

}
FA_OPERATOR_API bool FA_RWMobileID_Wolf (char* szPort, char* szMobileID)
{
	char szErrCode[50] = {0};
	char szErrMsg[50] = {0};
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);
	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}

	if(ReadWriteMoblileID_SpecialForWolf(szPort, szMobileID))
	{
		return true;
	}
	else
	{
		return false;
	}

}

FA_OPERATOR_API bool FA_ReadFactoryArea(char* szPort, 
										char* szFAData_Base64, 
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,   5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	bool bRes = false;
	unsigned char szFAData_u[512] = {0};
	if(!pfnReadFA(szPort, szFAData_u, szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("4203"));
		_stprintf_s(szErrMsg, strlen(_T("Read FA fail")) + 1, _T("%s"), _T("Read FA fail"));
		freeDllLibrary();
		return false;
	}
	else
	{
		/*int nDataLen = Base64EncodeGetRequiredLength(512);
		if(nDataLen > 1024)
			bRes = false;
		else
		{
			memset(szFAData_Base64, 0, nDataLen);
			bRes = Base64Encode(szFAData_u, 512, szFAData_Base64, &nDataLen) == TRUE;
			bRes = true;
		}*/
		memcpy(szFAData_Base64, szFAData_u, 512);
		freeDllLibrary();
		return true;
	}	
}

FA_OPERATOR_API bool FA_WriteFactoryArea(char* szPort, 
										 char* szFAData, 
										 char* szErrCode,
										 char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	bool bRes = false;
	////size_t nOgnDataBufLen = strlen(szFAData_Base64);
	////int nDecLen = Base64DecodeGetRequiredLength((int)nOgnDataBufLen);
	///*if (nDecLen <= 1024)
	//{
	//	BYTE* FSNBuf = new BYTE[nDecLen];
	//	memset(FSNBuf, 0, nDecLen);*/

	//	bRes = Base64Decode(szFAData_Base64, (int)nOgnDataBufLen, FSNBuf, &nDecLen) == TRUE;
	//	if (bRes)
	//		bRes = pfnWriteFA(szPort, FSNBuf, szErrCode, szErrMsg);

	//	delete [] FSNBuf;
	//}
	unsigned char szFAData_u[512] = {0};
	for(int i = 0; i < 512; i++)
	{
		szFAData_u[i] = szFAData[i];
	}
	bRes = pfnWriteFA(szPort, szFAData_u, szErrCode, szErrMsg);
	if(bRes)
	{
		freeDllLibrary();
		return bRes;
	}
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("4204"));
		_stprintf_s(szErrMsg,  strlen(_T("Write FA fail")) + 1, _T("%s"), _T("Write FA fail"));
		freeDllLibrary();
		return bRes;
	}
}

FA_OPERATOR_API bool FA_EraseFactoryArea(char* szPort, 
										 char* szErrCode,
										 char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,   5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char szTempDRMKey[512] = {0};
	memset(szTempDRMKey, '~', 512);
	if(!pfnWriteFA(szPort, szTempDRMKey, szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5000"));
		_stprintf_s(szErrMsg,  strlen( _T("Erase FA Data fail")) + 1, _T("%s"),  _T("Erase FA Data fail"));
		freeDllLibrary();
		return false;	
	}	
	else
	{
		freeDllLibrary();
		return true;
	}	
}

FA_OPERATOR_API bool FA_EraseTargetStationInfo(char* szPort,
											   char* szIndex,
											   char* szErrCode,
											   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		int nTargetPosition;
		int nTargetLength;
		if(SearchTargetStation(faData, atoi(szIndex), nTargetPosition, nTargetLength))
		{
			for(int i = 0; i < nTargetLength; i++)
			{
				faData[nTargetPosition + 3 + i] = '0';
			}
		}/*
		else
		{
			_stprintf_s(szErrCode, 5, _T("%s"), _T("5018"));
			_stprintf_s(szErrMsg, strlen(_T("Can¡¯t find this station!")) + 1, _T("%s"),  _T("Can¡¯t find this station!"));
			freeDllLibrary();
			return false;	
		}*/
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(!pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5000"));
		_stprintf_s(szErrMsg,  strlen(_T("Erase FA Data fail")) + 1, _T("%s"),  _T("Erase FA Data fail"));
		freeDllLibrary();
		return false;	
	}	
	else
	{
		freeDllLibrary();
		return true;
	}	
}

FA_OPERATOR_API bool FA_WriteMobileID  (char* szPort, 
										char* szSlot,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	pfnReadFA(szPort, faData, szErrCode, szErrMsg);

	char szComputer[1024] = {0};
	DWORD dwSize = 1024;
	GetComputerNameA(szComputer,&dwSize);
	if(strlen(szComputer) == 6)
	{
		szComputer[3] = 0;
		szComputer[6] = 0;
		char* szT = szComputer + 1;
		char* szD = szComputer + 4;
		faData[9+0] = (atoi(szT));
		faData[9+1] = (atoi(szD));
		faData[9+2] = '0';
	}
	else
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5012"));
		_stprintf_s(szErrMsg, strlen( _T("Computer name's length is not 6!")) + 1, _T("%s"),  _T("Computer name's length is not 6!"));
		freeDllLibrary();
		return false;
	}

	faData[9+3] = 0;//Adapter
	faData[9+4] = (atoi(szSlot));//Slot

	CTime tCurrent = CTime::GetCurrentTime();
	faData[9+5] = (tCurrent.GetDay());
	faData[9+6] = (tCurrent.GetMonth());
	faData[9+7] = (tCurrent.GetYear() % 100);
	faData[9+8] = (tCurrent.GetHour());
	faData[9+8] = MaybeStore0(faData[9+8]);
	faData[9+9] = (tCurrent.GetMinute());
	faData[9+9] = MaybeStore0(faData[9+9]);
	faData[9+10] = (tCurrent.GetSecond());
	faData[9+10] = MaybeStore0(faData[9+10]);

	if(!pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return false;
	}
	else
	{
		freeDllLibrary();
		return true;
	}
}

FA_OPERATOR_API bool FA_ReadMobileID (char* szPort, 
									  char* szMobileID,
									  char* szErrCode,
									  char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	char* szLine = (char*)(malloc(sizeof(char) * 3));
	char* szStation = (char*)(malloc(sizeof(char) * 3));
	char* szAdapter = (char*)(malloc(sizeof(char) * 3));
	char* szNotUsed = (char*)(malloc(sizeof(char) * 3));
	/***110627 gilpin modify assign value way***/
	szNotUsed[0] = '0';
	szNotUsed[1] = '0';
	/***110627 gilpin modify assign value way***/
	char* szDutNum = (char*)(malloc(sizeof(char) * 3));
	char* szDay = (char*)(malloc(sizeof(char) * 3));
	char* szMonth = (char*)(malloc(sizeof(char) * 3));
	char* szYear = (char*)(malloc(sizeof(char) * 3));
	char* szHour = (char*)(malloc(sizeof(char) * 3));
	char* szMinute = (char*)(malloc(sizeof(char) * 3));
	char* szSecond = (char*)(malloc(sizeof(char) * 3));

	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{		
		
		_itoa_s(faData[9], szLine, 3, 10);
		if(strlen(szLine) == 1)
		{
			*(szLine + 2) = '\0';
			*(szLine + 1) = *szLine;
			*szLine = '0';
		}
		else if(strlen(szLine) == 3)
		{
			_stprintf_s(szErrCode,  5, _T("%s"),  _T("5018"));
			_stprintf_s(szErrMsg,   strlen(_T("Computer name is wrong!")) + 1, _T("%s"), _T("Computer name is wrong!"));
			freeDllLibrary();
			return false;
		}
		
		_itoa_s(faData[10], szStation, 3, 10);
		if(strlen(szStation) == 1)
		{
			*(szStation + 2) = '\0';
			*(szStation + 1) = *szStation;
			*szStation = '0';
		}
			
		faData[12] = MaybeRestore0(faData[12]);
		_itoa_s(faData[12], szAdapter, 3, 10);		
		if(strlen(szAdapter) == 1)
		{
			*(szAdapter + 2) = '\0';
			*(szAdapter + 1) = *szAdapter;
			*szAdapter = '0';
		}
		
		_itoa_s(faData[13], szDutNum, 3, 10);
		if(strlen(szDutNum) == 1)
		{
			*(szDutNum + 2) = '\0';
			*(szDutNum + 1) = *szDutNum;
			*szDutNum = '0';
		}
		
		_itoa_s(faData[14], szDay, 3, 10);
		if(strlen(szDay) == 1)
		{
			*(szDay + 2) = '\0';
			*(szDay + 1) = *szDay;
			*szDay = '0';
		}

		
		_itoa_s(faData[15], szMonth, 3, 10);
		if(strlen(szMonth) == 1)
		{
			*(szMonth + 2) = '\0';
			*(szMonth + 1) = *szMonth;
			*szMonth = '0';
		}
		
		_itoa_s(faData[16], szYear, 3, 10);
		if(strlen(szYear) == 1)
		{
			*(szYear + 2) = '\0';
			*(szYear + 1) = *szYear;
			*szYear = '0';
		}
		
		faData[17] = MaybeRestore0(faData[17]);
		_itoa_s(faData[17], szHour, 3, 10);
		if(strlen(szHour) == 1)
		{
			*(szHour + 2) = '\0';
			*(szHour + 1) = *szHour;
			*szHour = '0';
		}
		
		faData[18] = MaybeRestore0(faData[18]);
		_itoa_s(faData[18], szMinute, 3, 10);
		if(strlen(szMinute) == 1)
		{
			*(szMinute + 2) = '\0';
			*(szMinute + 1) = *szMinute;
			*szMinute = '0';
		}
		
		faData[19] = MaybeRestore0(faData[19]);
		_itoa_s(faData[19], szSecond, 3, 10);
		if(strlen(szSecond) == 1)
		{
			*(szSecond + 2) = '\0';
			*(szSecond + 1) = *szSecond;
			*szSecond = '0';
		}

		CString szTempMobileID = szLine;
		szTempMobileID += szStation;
		szTempMobileID += szNotUsed;
		szTempMobileID += szAdapter;
		szTempMobileID += szDutNum;
		szTempMobileID += szDay;
		szTempMobileID += szMonth;
		szTempMobileID += szYear;
		szTempMobileID += szHour;
		szTempMobileID += szMinute;
		szTempMobileID += szSecond;

		strcpy_s(szMobileID, strlen(szTempMobileID) + 1, szTempMobileID.GetBuffer());
		//sprintf_s(szMobileID, 23, _T("%s%s%s%s%s%s%s%s%s%s%s"), szLine, szStation, szAdapter, szSlot, szDutNum, szDay, szMonth, szYear, szHour, szMinute, szSecond);		
		free(szLine);
		free(szStation);
		free(szAdapter);
		free(szNotUsed);
		free(szDutNum);
		free(szDay);
		free(szMonth);
		free(szYear);
		free(szHour);
		free(szMinute);
		free(szSecond);
		freeDllLibrary();
		return true;
	}
	else
	{
		free(szLine);
		free(szStation);
		free(szAdapter);
		free(szNotUsed);
		free(szDutNum);
		free(szDay);
		free(szMonth);
		free(szYear);
		free(szHour);
		free(szMinute);
		free(szSecond);
		freeDllLibrary();
		return false;
	}
	
	
}

FA_OPERATOR_API bool FA_ReadModelName (char* szPort, 
									   char* szModelname,
									   char* szErrCode,
									   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szModelname[0] = faData[2];
		szModelname[1] = faData[3];
		szModelname[2] = faData[4];
		szModelname[3] = faData[5];
		szModelname[4] = faData[6];
		szModelname[5] = faData[7];
		szModelname[6] = faData[8];
		szModelname[7] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	
}

FA_OPERATOR_API bool FA_WriteModelName (char* szPort, 
										char* szModelName,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen( _T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szModelName) > 7)
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5001"));
		_stprintf_s(szErrMsg, strlen(_T("Model Name¡¯s length is more than 7 !")) + 1, _T("%s"),  _T("Model Name¡¯s length is more than 7 !"));
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{		
		for(int i = 0; i < (int)strlen(szModelName); i++)
			faData[i + 2] = szModelName[i];
		/*faData[2] = szModelName[0];
		faData[3] = szModelName[1];
		faData[4] = szModelName[2];
		faData[5] = szModelName[3];
		faData[6] = szModelName[4];
		faData[7] = szModelName[5];
		faData[8] = szModelName[6];*/
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}	
}

FA_OPERATOR_API bool FA_WriteFAVersion (char* szPort, 
										char* szFAVersion,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen( _T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};	
	std::string tempString = szFAVersion;
	if(tempString[0] == 'v' || tempString[0] == 'V')
	{
		tempString = tempString.substr(1, tempString.length() - 1);
		faData[0] = tempString[0];
	}
	std::string strFA1 = tempString.substr(0, tempString.find("."));
	std::string strFA2 = tempString.substr(tempString.find(".") + 1, 1);
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		int temp1 = ((atoi(strFA1.c_str())) & 0x0F);		
		temp1 |= ((atoi(strFA2.c_str()) << 4) & 0xF0);
		faData[1] = (byte)temp1;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	
}

FA_OPERATOR_API bool FA_ReadFAVersion (char* szPort, 
									   char* szFAVersion,
									   char* szErrCode,
									   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szFAVersion[0] = 'V';
		byte versionFirstPart = (faData[1] & 0x0F);
		_itoa_s(versionFirstPart, szFAVersion + 1, 2, 10);
		szFAVersion[2] = '.';
		byte versionSecondPart = ((faData[1] & 0xF0) >> 4);
		_itoa_s(versionSecondPart, szFAVersion + 3, 2, 10);
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	
}

FA_OPERATOR_API bool FA_WriteMEVersion (char* szPort, 
										char* szMEVersion,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szMEVersion) != 1)
	{		
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5002"));
		_stprintf_s(szErrMsg,  strlen(_T("ME version¡¯s length is not 1!")) + 1, _T("%s"), _T("ME version¡¯s length is not 1!"));
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		faData[20] = *szMEVersion;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadMEVersion (char* szPort, 
									   char* szMEVersion,
									   char* szErrCode,
									   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		*szMEVersion = faData[20];
		*(szMEVersion + 1) = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}	
}

FA_OPERATOR_API bool FA_WriteSWVersion (char* szPort, 
										char* szSWVersion,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szSWVersion) != 6)
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5003"));
		_stprintf_s(szErrMsg,  strlen(_T("SW version¡¯s length is not 6!")) + 1, _T("%s"), _T("SW version¡¯s length is not 6!")); 
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		faData[21] = szSWVersion[0];
		faData[22] = szSWVersion[1];
		faData[23] = szSWVersion[2];
		faData[24] = szSWVersion[3];
		faData[25] = szSWVersion[4];
		faData[26] = szSWVersion[5];
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadSWVersion (char* szPort, 
									   char* szSWVersion,
									   char* szErrCode,
									   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szSWVersion[0] = faData[21];
		szSWVersion[1] = faData[22];
		szSWVersion[2] = faData[23];
		szSWVersion[3] = faData[24];
		szSWVersion[4] = faData[25];
		szSWVersion[5] = faData[26];
		szSWVersion[6] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_WritePCBAVersion (char* szPort, 
										  char* szPCBAVersion,
										  char* szErrCode,
										  char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szPCBAVersion) != 5)
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5004"));
		_stprintf_s(szErrMsg,  strlen(_T("PCBA version¡¯s length is not 5!")) + 1, _T("%s"), _T("PCBA version¡¯s length is not 5!")); 
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		faData[27] = szPCBAVersion[0];
		faData[28] = szPCBAVersion[1];
		faData[29] = szPCBAVersion[2];
		faData[30] = szPCBAVersion[3];
		faData[31] = szPCBAVersion[4];
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadPCBAVersion (char* szPort, 
										 char* szPCBAVersion,
										 char* szErrCode,
										 char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szPCBAVersion[0] = faData[27];
		szPCBAVersion[1] = faData[28];
		szPCBAVersion[2] = faData[29];
		szPCBAVersion[3] = faData[30];
		szPCBAVersion[4] = faData[31];
		szPCBAVersion[5] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_WritePicassoNumber (char* szPort, 
											char* szPicasso,
											char* szErrCode,
											char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szPicasso) != 10)
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5005"));
		_stprintf_s(szErrMsg,  strlen(_T("Picasso Number¡¯s length is not 10!")) + 1, _T("%s"), _T("Picasso Number¡¯s length is not 10!"));
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		faData[32] = szPicasso[0];
		faData[33] = szPicasso[1];
		faData[34] = szPicasso[2];
		faData[35] = szPicasso[3];
		faData[36] = szPicasso[4];
		faData[37] = szPicasso[5];
		faData[38] = szPicasso[6];
		faData[39] = szPicasso[7];
		faData[40] = szPicasso[8];
		faData[41] = szPicasso[9];
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadPicassoNumber (char* szPort, 
										   char* szPicasso,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szPicasso[0] = faData[32];
		szPicasso[1] = faData[33];
		szPicasso[2] = faData[34];
		szPicasso[3] = faData[35];
		szPicasso[4] = faData[36];
		szPicasso[5] = faData[37];
		szPicasso[6] = faData[38];
		szPicasso[7] = faData[39];
		szPicasso[8] = faData[40];
		szPicasso[9] = faData[41];
		szPicasso[10] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_WriteTranceiverNumber (char* szPort, 
											   char* szXCVR,
											   char* szErrCode,
											   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szXCVR) != 10)
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5006"));
		_stprintf_s(szErrMsg, strlen(_T("Tranceiver Number¡¯s length is not 11!")) + 1, _T("%s"),  _T("Tranceiver Number¡¯s length is not 11!"));
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		faData[42] = szXCVR[0];
		faData[43] = szXCVR[1];
		faData[44] = szXCVR[2];
		faData[45] = szXCVR[3];
		faData[46] = szXCVR[4];
		faData[47] = szXCVR[5];
		faData[48] = szXCVR[6];
		faData[49] = szXCVR[7];
		faData[50] = szXCVR[8];
		faData[51] = szXCVR[9];
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadTranceiverNumber  (char* szPort, 
											   char* szXCVR,
											   char* szErrCode,
											   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) +1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szXCVR[0] = faData[42];
		szXCVR[1] = faData[43];
		szXCVR[2] = faData[44];
		szXCVR[3] = faData[45];
		szXCVR[4] = faData[46];
		szXCVR[5] = faData[47];
		szXCVR[6] = faData[48];
		szXCVR[7] = faData[49];
		szXCVR[8] = faData[50];
		szXCVR[9] = faData[51];
		szXCVR[10] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_WriteAllVersion   (char* szPort, 
										   char* szModelName,
										   char* szSWVersion,
										   char* szPCBAVersion,
										   char* szFAVersion,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	if(strlen(szModelName) != 7)
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5001"));
		_stprintf_s(szErrMsg,  strlen(_T("Model Name¡¯s length is not 7 !")) + 1, _T("%s"), _T("Model Name¡¯s length is not 7 !"));
		freeDllLibrary();
		return false;
	}
	if(strlen(szSWVersion) != 6)
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5003"));
		_stprintf_s(szErrMsg, strlen(_T("SW version¡¯s length is not 6!")) + 1, _T("%s"),  _T("SW version¡¯s length is not 6!")); 
		freeDllLibrary();
		return false;
	}
	if(strlen(szPCBAVersion) != 5)
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5004"));
		_stprintf_s(szErrMsg, strlen(_T("PCBA version¡¯s length is not 5!")) + 1, _T("%s"),  _T("PCBA version¡¯s length is not 5!")); 
		freeDllLibrary();
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		std::string tempString = szFAVersion;
		if(tempString[0] == 'v' || tempString[0] == 'V')
		{
			tempString = tempString.substr(1, tempString.length() - 1);
			faData[0] = tempString[0];
		}
		std::string strFA1 = tempString.substr(0, tempString.find("."));
		std::string strFA2 = tempString.substr(tempString.find(".") + 1, 1);		
		int temp1 = ((atoi(strFA1.c_str())) & 0x0F);		
		temp1 |= ((atoi(strFA2.c_str()) << 4) & 0xF0);
		faData[1] = (byte)temp1;
		

		faData[2] = szModelName[0];
		faData[3] = szModelName[1];
		faData[4] = szModelName[2];
		faData[5] = szModelName[3];
		faData[6] = szModelName[4];
		faData[7] = szModelName[5];
		faData[8] = szModelName[6];

		faData[21] = szSWVersion[0];
		faData[22] = szSWVersion[1];
		faData[23] = szSWVersion[2];
		faData[24] = szSWVersion[3];
		faData[25] = szSWVersion[4];
		faData[26] = szSWVersion[5];

		faData[27] = szPCBAVersion[0];
		faData[28] = szPCBAVersion[1];
		faData[29] = szPCBAVersion[2];
		faData[30] = szPCBAVersion[3];
		faData[31] = szPCBAVersion[4];
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadAllVersion (char* szPort, 
										char* szModelName,
										char* szMobileID,
										char* szSWVersion,
										char* szPCBAVersion,
										char* szPicasso,
										char* szFAVersion,
										char* szErrCode,
										char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		szFAVersion[0] = 'V';
		byte versionFirstPart = (faData[1] & 0x0F);
		_itoa_s(versionFirstPart, szFAVersion + 1, 2, 10);
		szFAVersion[2] = '.';
		byte versionSecondPart = ((faData[1] & 0xF0) >> 4);
		_itoa_s(versionSecondPart, szFAVersion + 3, 2, 10);

		szModelName[0] = faData[2];
		szModelName[1] = faData[3];
		szModelName[2] = faData[4];
		szModelName[3] = faData[5];
		szModelName[4] = faData[6];
		szModelName[5] = faData[7];
		szModelName[6] = faData[8];
		szModelName[7] = '\0';

		//read mobile id need test
		char* szLine = (char*)(malloc(sizeof(char) * 3));
		char* szStation = (char*)(malloc(sizeof(char) * 3));
		char* szAdapter = (char*)(malloc(sizeof(char) * 3));
		char* szSlot = (char*)(malloc(sizeof(char) * 3));
		char* szDutNum = (char*)(malloc(sizeof(char) * 3));
		char* szDay = (char*)(malloc(sizeof(char) * 3));
		char* szMonth = (char*)(malloc(sizeof(char) * 3));
		char* szYear = (char*)(malloc(sizeof(char) * 3));
		char* szHour = (char*)(malloc(sizeof(char) * 3));
		char* szMinute = (char*)(malloc(sizeof(char) * 3));
		char* szSecond = (char*)(malloc(sizeof(char) * 3));

			_itoa_s(faData[9], szLine, 3, 10);
			if(strlen(szLine) == 1)
			{
				*(szLine + 2) = '\0';
				*(szLine + 1) = *szLine;
				*szLine = '0';
			}

			_itoa_s(faData[10], szStation, 3, 10);
			if(strlen(szStation) == 1)
			{
				*(szStation + 2) = '\0';
				*(szStation + 1) = *szStation;
				*szStation = '0';
			}

			_itoa_s(faData[11], szAdapter, 3, 10);
			if(strlen(szAdapter) == 1)
			{
				*(szAdapter + 2) = '\0';
				*(szAdapter + 1) = *szAdapter;
				*szAdapter = '0';
			}

			_itoa_s(faData[12], szSlot, 3, 10);
			if(strlen(szSlot) == 1)
			{
				*(szSlot + 2) = '\0';
				*(szSlot + 1) = *szSlot;
				*szSlot = '0';
			}

			_itoa_s(faData[13], szDutNum, 3, 10);
			if(strlen(szDutNum) == 1)
			{
				*(szDutNum + 2) = '\0';
				*(szDutNum + 1) = *szDutNum;
				*szDutNum = '0';
			}

			_itoa_s(faData[14], szDay, 3, 10);
			if(strlen(szDay) == 1)
			{
				*(szDay + 2) = '\0';
				*(szDay + 1) = *szDay;
				*szDay = '0';
			}


			_itoa_s(faData[15], szMonth, 3, 10);
			if(strlen(szMonth) == 1)
			{
				*(szMonth + 2) = '\0';
				*(szMonth + 1) = *szMonth;
				*szMonth = '0';
			}

			_itoa_s(faData[16], szYear, 3, 10);
			if(strlen(szYear) == 1)
			{
				*(szYear + 2) = '\0';
				*(szYear + 1) = *szYear;
				*szYear = '0';
			}

			_itoa_s(faData[17], szHour, 3, 10);
			if(strlen(szHour) == 1)
			{
				*(szHour + 2) = '\0';
				*(szHour + 1) = *szHour;
				*szHour = '0';
			}

			_itoa_s(faData[18], szMinute, 3, 10);
			if(strlen(szMinute) == 1)
			{
				*(szMinute + 2) = '\0';
				*(szMinute + 1) = *szMinute;
				*szMinute = '0';
			}

			_itoa_s(faData[19], szSecond, 3, 10);
			if(strlen(szSecond) == 1)
			{
				*(szSecond + 2) = '\0';
				*(szSecond + 1) = *szSecond;
				*szSecond = '0';
			}
			
			CString szTempMobileID = szLine;
			szTempMobileID += szStation;
			szTempMobileID += szAdapter;
			szTempMobileID += szSlot;
			szTempMobileID += szDutNum;
			szTempMobileID += szDay;
			szTempMobileID += szMonth;
			szTempMobileID += szYear;
			szTempMobileID += szHour;
			szTempMobileID += szMinute;
			szTempMobileID += szSecond;

			strcpy_s(szMobileID, 25, szTempMobileID.GetBuffer());
			//sprintf_s(szMobileID, 25, _T("T%sD%s%s%s%s%s%s%s%s%s%s"), szLine, szStation, szAdapter, szSlot, szDutNum, szDay, szMonth, szYear, szHour, szMinute, szSecond);		
			free(szLine);
			free(szStation);
			free(szAdapter);
			free(szSlot);
			free(szDutNum);
			free(szDay);
			free(szMonth);
			free(szYear);
			free(szHour);
			free(szMinute);
			free(szSecond);

		szSWVersion[0] = faData[21];
		szSWVersion[1] = faData[22];
		szSWVersion[2] = faData[23];
		szSWVersion[3] = faData[24];
		szSWVersion[4] = faData[25];
		szSWVersion[5] = faData[26];
		szSWVersion[6] = '\0';

		szPCBAVersion[0] = faData[27];
		szPCBAVersion[1] = faData[28];
		szPCBAVersion[2] = faData[29];
		szPCBAVersion[3] = faData[30];
		szPCBAVersion[4] = faData[31];
		szPCBAVersion[5] = '\0';

		szPicasso[0] = faData[32];
		szPicasso[1] = faData[33];
		szPicasso[2] = faData[34];
		szPicasso[3] = faData[35];
		szPicasso[4] = faData[36];
		szPicasso[5] = faData[37];
		szPicasso[6] = faData[38];
		szPicasso[7] = faData[39];
		szPicasso[8] = faData[40];
		szPicasso[9] = faData[41];
		szPicasso[10] = '\0';
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_ReadStationInfo(char* szPort, 
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
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg, strlen(_T("Load FA DLL fail!")) + 1, _T("%s"),  _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	if(ReadFA_SpecialForRetore0(szPort, faData, szErrCode, szErrMsg))
	{
		if(!IsStationSetup(faData))
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1146"));
			_stprintf_s(szErrMsg, strlen(_T("Setup Station is not setup!")) + 1, _T("%s"),  _T("Setup Station is not setup!"));
			freeDllLibrary();
			return false;
		}
		if(atoi(szStationIndex) == 240)
		{
			char* szDay = (char*)(malloc(sizeof(char) * 3));
			char* szMonth = (char*)(malloc(sizeof(char) * 3));
			char* szYear = (char*)(malloc(sizeof(char) * 3));
			char* szHour = (char*)(malloc(sizeof(char) * 3));
			char* szMinute = (char*)(malloc(sizeof(char) * 3));			
			//read station setup
			*szLineType = faData[129];
			*(szLineType + 1) = '\0';
			int temp = faData[128 + 3];
			_itoa_s((temp & 0x0F), szTestResult, 3, 10);
			//*(szTestResult + 1) = '\0';
			_itoa_s((temp >> 4), szTestCount, 3, 10);
			//*(szTestCount + 1) = '\0';
			
			_itoa_s(faData[134], szDay, 3, 10);
			if(strlen(szDay) == 1)
			{
				*(szDay + 2) = '\0';
				*(szDay + 1) = *szDay;
				*szDay = '0';
			}


			_itoa_s(faData[135], szMonth, 3, 10);
			if(strlen(szMonth) == 1)
			{
				*(szMonth + 2) = '\0';
				*(szMonth + 1) = *szMonth;
				*szMonth = '0';
			}

			_itoa_s(faData[136], szYear, 3, 10);
			if(strlen(szYear) == 1)
			{
				*(szYear + 2) = '\0';
				*(szYear + 1) = *szYear;
				*szYear = '0';
			}

			_itoa_s(faData[133], szHour, 3, 10);
			if(strlen(szHour) == 1)
			{
				*(szHour + 2) = '\0';
				*(szHour + 1) = *szHour;
				*szHour = '0';
			}

			_itoa_s(faData[132], szMinute, 3, 10);
			if(strlen(szMinute) == 1)
			{
				*(szMinute + 2) = '\0';
				*(szMinute + 1) = *szMinute;
				*szMinute = '0';
			}
			CString szTempTestDateTime = szYear;
			szTempTestDateTime += szMonth;
			szTempTestDateTime += szDay;
			szTempTestDateTime += szHour;
			szTempTestDateTime += szMinute;
			strcpy_s(szTestDateTime, 11, szTempTestDateTime.GetBuffer());
			//sprintf_s(szTestDateTime, 11, _T("%s%s%s%s%s"), szYear, szMonth, szDay, szHour, szMinute);
			free(szDay);
			free(szMonth);
			free(szYear);
			free(szHour);
			free(szMinute);

			int nLineNum = faData[137] & 0x0F;
			char* cLineNum = (char*)(malloc(sizeof(char) * 3));
			_itoa_s(nLineNum, cLineNum, 3, 10);
			if(strlen(cLineNum) == 1)
			{
				*(cLineNum + 2) = '\0';
				*(cLineNum + 1) = *cLineNum;
				*cLineNum = '0';
			}

			int nStationNum = faData[137] >> 4;
			char* cStationNum = (char*)(malloc(sizeof(char) * 3));
			_itoa_s(nStationNum, cStationNum, 3, 10);
			if(strlen(cStationNum) == 1)
			{
				*(cStationNum + 2) = '\0';
				*(cStationNum + 1) = *cStationNum;
				*cStationNum = '0';
			}
			
			CString szTempTestLineStation = cLineNum;
			szTempTestLineStation += cStationNum;
			strcpy_s(szTestLineStation, 8, szTempTestLineStation.GetBuffer());
			//sprintf_s(szTestLineStation, 8, _T("%s%s"), cLineNum, cStationNum);			
			free(cLineNum);
			free(cStationNum);
			
			if((faData[130] - 8) == 0)
			{
				*szReserve = faData[138];//no exter info
				*(szReserve + 1) = '\0';
			}
			else
			{				
				*szReserve = faData[138];
				int i = 1;
				for(; i <= faData[130] - 8; i++)
					*(szReserve + i) = faData[138 + i];
				*(szReserve + i) = '\0';
			}//read exter info
		}
		else
		{
			//read other station info
			int stationIndex = (byte)faData[128];
			int stationLen = (byte)faData[128 + 2];
			int totalSearchLength = 128 + 3 + stationLen;
			bool findTarget = false;//flag for do while loop
			do 
			{
				stationIndex = (byte)faData[totalSearchLength];
				stationLen = (byte)faData[totalSearchLength + 2];
				if(stationIndex == atoi(szStationIndex))
				{	
					char* szDay = (char*)(malloc(sizeof(char) * 3));
					char* szMonth = (char*)(malloc(sizeof(char) * 3));
					char* szYear = (char*)(malloc(sizeof(char) * 3));
					char* szHour = (char*)(malloc(sizeof(char) * 3));
					char* szMinute = (char*)(malloc(sizeof(char) * 3));	

					*szLineType = faData[totalSearchLength + 1];
					*(szLineType + 1) = '\0';
					int temp = faData[totalSearchLength + 3];
					_itoa_s((temp & 0x0F), szTestCount, 2, 10);
					int szTestResultReplace = temp >> 4;//0,1,F(0xf0)
					if(szTestResultReplace == 15) {*szTestResult = 'F'; *(szTestResult + 1) = '\0';}
					if(szTestResultReplace == 0)  {*szTestResult = '0'; *(szTestResult + 1) = '\0';}
					if(szTestResultReplace == 1)  {*szTestResult = '1';	*(szTestResult + 1) = '\0';}

					_itoa_s(faData[totalSearchLength + 6], szDay, 3, 10);
					if(strlen(szDay) == 1)
					{
						*(szDay + 2) = '\0';
						*(szDay + 1) = *szDay;
						*szDay = '0';
					}


					_itoa_s(faData[totalSearchLength + 7], szMonth, 3, 10);
					if(strlen(szMonth) == 1)
					{
						*(szMonth + 2) = '\0';
						*(szMonth + 1) = *szMonth;
						*szMonth = '0';
					}

					_itoa_s(faData[totalSearchLength + 8], szYear, 3, 10);
					if(strlen(szYear) == 1)
					{
						*(szYear + 2) = '\0';
						*(szYear + 1) = *szYear;
						*szYear = '0';
					}

					_itoa_s(faData[totalSearchLength + 5], szHour, 3, 10);
					if(strlen(szHour) == 1)
					{
						*(szHour + 2) = '\0';
						*(szHour + 1) = *szHour;
						*szHour = '0';
					}

					_itoa_s(faData[totalSearchLength + 4], szMinute, 3, 10);
					if(strlen(szMinute) == 1)
					{
						*(szMinute + 2) = '\0';
						*(szMinute + 1) = *szMinute;
						*szMinute = '0';
					}
					CString szTempTestDateTime = szYear;
					szTempTestDateTime += szMonth;
					szTempTestDateTime += szDay;
					szTempTestDateTime += szHour;
					szTempTestDateTime += szMinute;
					strcpy_s(szTestDateTime, 11, szTempTestDateTime.GetBuffer());
					//sprintf_s(szTestDateTime, 11, _T("%s%s%s%s%s"), szYear, szMonth, szDay, szHour, szMinute);
					free(szDay);
					free(szMonth);
					free(szYear);
					free(szHour);
					free(szMinute);


					int nLineNum = faData[137] & 0x0F;
					char* cLineNum = (char*)(malloc(sizeof(char) * 3));
					_itoa_s(nLineNum, cLineNum, 3, 10);
					if(strlen(cLineNum) == 1)
					{
						*(cLineNum + 2) = '\0';
						*(cLineNum + 1) = *cLineNum;
						*cLineNum = '0';
					}

					int nStationNum = faData[137] >> 4;
					char* cStationNum = (char*)(malloc(sizeof(char) * 3));
					_itoa_s(nStationNum, cStationNum, 3, 10);
					if(strlen(cStationNum) == 1)
					{
						*(cStationNum + 2) = '\0';
						*(cStationNum + 1) = *cStationNum;
						*cStationNum = '0';
					}
					CString szTempTestLineStation = cLineNum;
					szTempTestLineStation += cStationNum;
					strcpy_s(szTestLineStation, 8, szTempTestLineStation.GetBuffer());
					//sprintf_s(szTestLineStation, 8, _T("%s%sD%s"), szLineType, cLineNum,cStationNum);
					free(cLineNum);
					free(cStationNum);
	
					

					if((stationLen - 8) == 0)
					{
						*szReserve = faData[totalSearchLength + 10];//no exter info
						*(szReserve + 1) = '\0';
					}
					else
					{
						*szReserve = faData[totalSearchLength + 10];
						int i = 1;
						for(; i <= stationLen - 8; i++)
							*(szReserve + i) = faData[totalSearchLength + 10 + i];
						*(szReserve + i) = '\0';
					}//read exter info



					findTarget = true;
				}
				if(stationIndex == 0)
				{
					_stprintf_s(szErrCode,  5, _T("%s"),  _T("5011"));
					_stprintf_s(szErrMsg,   strlen(_T("This station isn't test yet!")) + 1, _T("%s"), _T("This station isn't test yet!"));
					findTarget = false;
					freeDllLibrary();
					return false;
				}
				totalSearchLength += (stationLen + 3);
			} while (!findTarget);
		}
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}

FA_OPERATOR_API bool FA_WriteStationInfo(char* szPort, 
										 char* szStationIndex,
										 char* szLineType,
										 char* szStationExternLen,
										 char* szTestResult,
										 char* szReserve,
										 char* szStationExternInfo,
										 char* szErrCode,
										 char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5016"));
		_stprintf_s(szErrMsg,  strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	unsigned char faData[512] = {0};
	unsigned char szTestResultReplace;
	if(*szTestResult == 'F')  szTestResultReplace = 0x0f;
	if(*szTestResult == '0')  szTestResultReplace = 0x00;
	if(*szTestResult == '1')  szTestResultReplace = 0x01;
	if(pfnReadFA(szPort, faData, szErrCode, szErrMsg))
	{
		if(!IsInSetupStationList(szStationIndex, faData))
		{		
			_stprintf_s(szErrCode,  5, _T("%s"),  _T("5019"));
			_stprintf_s(szErrMsg,  strlen(_T("The station you want write isn't setup yet!")) + 1, _T("%s"),  _T("The station you want write isn't setup yet!"));
			freeDllLibrary();
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
					_stprintf_s(szErrCode,  5, _T("%s"),  _T("5018"));
					_stprintf_s(szErrMsg,  strlen(_T("Computer name is wrong!")) + 1, _T("%s"),  _T("Computer name is wrong!"));
					freeDllLibrary();
					return false;
				}
				temp |= bT;
				temp |= (bD<<4);

				faData[137] = temp;
			}
			else
			{
				_stprintf_s(szErrCode,  5, _T("%s"), _T("5012"));
				_stprintf_s(szErrMsg,  strlen(_T("Computer name's length is not 6!")) + 1, _T("%s"), _T("Computer name's length is not 6!"));
				freeDllLibrary();
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
							_stprintf_s(szErrCode,  5, _T("%s"), _T("5018"));
							_stprintf_s(szErrMsg, strlen(_T("Computer name is wrong!")) + 1, _T("%s"),  _T("Computer name is wrong!"));
							freeDllLibrary();
							return false;
						}						
						temp |= bT;
						temp |= (bD<<4);

						faData[totalSearchLength + 9] = temp;
					}
					else
					{
						_stprintf_s(szErrCode, 5, _T("%s"),  _T("5012"));
						_stprintf_s(szErrMsg,  strlen(_T("Computer name's length is not 6!")) + 1, _T("%s"), _T("Computer name's length is not 6!"));
						freeDllLibrary();
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
				if(stationIndex == '~')
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
							_stprintf_s(szErrCode, 5, _T("%s"),  _T("5018"));
							_stprintf_s(szErrMsg, strlen(_T("Computer name is wrong!")) + 1, _T("%s"),  _T("Computer name is wrong!"));
							freeDllLibrary();
							return false;
						}						
						temp |= bT;
						temp |= (bD<<4);

						faData[totalSearchLength + 9] = temp;
					}
					else
					{
						_stprintf_s(szErrCode,  5, _T("%s"), _T("5012"));
						_stprintf_s(szErrMsg, strlen(_T("Computer name's length is not 6!")) + 1, _T("%s"),  _T("Computer name's length is not 6!"));
						freeDllLibrary();
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
			} while (!writeDone);
		}
	}
	else
	{
		freeDllLibrary();
		return false;
	}
	if(pfnWriteFA(szPort, faData, szErrCode, szErrMsg))
	{
		freeDllLibrary();
		return true;
	}
	else
	{
		freeDllLibrary();
		return false;
	}
}


FA_OPERATOR_API bool FA_CheckStationInfo(char* szPort, 
										 char* szStationIndex,	
										 char* szErrCode,
										 char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(!ExportDllFunction(szErrCode, szErrMsg))
	{
		_stprintf_s(szErrCode,  5, _T("%s"),  _T("5016"));
		_stprintf_s(szErrMsg,   strlen(_T("Load FA DLL fail!")) + 1, _T("%s"), _T("Load FA DLL fail!"));
		return false;
	}
	int nStationIndex = atoi(szStationIndex);
	unsigned char faData[512] = {0};
	vector<int> vecStationOrder;
	if(ReadFA_SpecialForRetore0(szPort, faData, szErrCode, szErrMsg))
	{
		if(!IsStationSetup(faData))
		{
			_stprintf_s(szErrCode,  5, _T("%s"), _T("1146"));
			_stprintf_s(szErrMsg, strlen(_T("Setup Station is not setup!")) + 1, _T("%s"),  _T("Setup Station is not setup!"));
			freeDllLibrary();
			return false;
		}
		//store all station index
		int indexExterInfoLen = faData[130] - 8;
		for(int i = 0; i < indexExterInfoLen; i++)
		{
			vecStationOrder.push_back(faData[139 + i]);
		}		
		
		if(find(vecStationOrder.begin(), vecStationOrder.end(), nStationIndex) == vecStationOrder.end())
		{
			//this station is not setup yet
			GetErrorCode(nStationIndex, szErrCode);
			_stprintf_s(szErrMsg, strlen(_T("The station you want check isn't in setup station list!")) + 1, _T("%s"),  _T("The station you want check isn't in setup station list!"));
			freeDllLibrary();
			return false;
		}

		for(int i = 0; i < (int)vecStationOrder.size(); i++)
		{
			int result = -1;
			//check whether is target station
			if(nStationIndex == vecStationOrder[i])
			{
				freeDllLibrary();
				return true;//the stations infront of it are all succeed	
			}

			//search the station which is in front of target station
			if(SearchTargetStation(faData, vecStationOrder[i], result))
			{				
					if(result == 15)
					{
						GetErrorCode(vecStationOrder[i], szErrCode);
						_stprintf_s(szErrMsg,  strlen(_T("This station is failed!")) + 1, _T("%s"), _T("This station is failed!"));
						freeDllLibrary();
						return false;//failed
					}
					if(result == 0)
					{
						GetErrorCode(vecStationOrder[i], szErrCode);
						_stprintf_s(szErrMsg, strlen( _T("This station isn't test yet!")) + 1, _T("%s"),  _T("This station isn't test yet!"));
						freeDllLibrary();
						return false;//not test yet
					}
			}
			else
			{
				//cannot find station info
				GetErrorCode(vecStationOrder[i], szErrCode);
				_stprintf_s(szErrMsg, strlen( _T("This station isn't test yet!")) + 1, _T("%s"),  _T("This station isn't test yet!"));
				freeDllLibrary();
				return false;//not test yet
			}			
		}		
	}
	else
	{
		freeDllLibrary();
		return false;
	}

	return false;
}

FA_OPERATOR_API bool FA_CheckMobileID     (char* szPort, 
										   char* szMobileID,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	if(strlen(szMobileID) != 22 ||
		szMobileID == "0000000000000000000000" ||
		szMobileID == "FFFFFFFFFFFFFFFFFFFFFF")
	{		
		_stprintf_s(szErrCode,  5, _T("%s"), _T("5007"));
		_stprintf_s(szErrMsg,  strlen( _T("Mobile ID is all 0 or all F or its length is not 22!")) + 1, _T("%s"), _T("Mobile ID is all 0 or all F or its length is not 22!"));
		return false;
	}
	else
	{
		return true;
	}
}

FA_OPERATOR_API bool FA_CheckUISWVersion  (char* szPort, 
										   char* szUISW,
										   char* szFASW,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUISW(szUISW);
	string ssFASW(szFASW);
	if(ssUISW == ssFASW)
		return true;
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("1120"));
		_stprintf_s(szErrMsg, strlen(_T("UI SW Version in UI is not the same as in FA!")) + 1, _T("%s"),  _T("UI SW Version in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckUIPCBAVersion (char* szPort, 
											char* szUIPCBA,
											char* szFAPCBA,
											char* szErrCode,
											char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUIPCBA(szUIPCBA);
	string ssFAPCBA(szFAPCBA);
	if(ssUIPCBA == ssFAPCBA)
		return true;
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5008"));
		_stprintf_s(szErrMsg, strlen( _T("PCBA Version in UI is not the same as in FA!")) + 1, _T("%s"),  _T("PCBA Version in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckUIMEVersion  (char* szPort, 
										   char* szUIME,
										   char* szFAME,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUIME(szUIME);
	string ssFAME(szFAME);
	if(ssUIME == ssFAME)
		return true;
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("1122"));
		_stprintf_s(szErrMsg, strlen(_T("ME version in UI is not the same as in FA!")) + 1, _T("%s"),  _T("ME version in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckUIPicasso    (char* szPort, 
										   char* szUIPicasso,
										   char* szFAPicasso,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUIPicasso(szUIPicasso);
	string ssFAPicasso(szFAPicasso);
	if(ssUIPicasso == ssFAPicasso)
		return true;
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5015"));
		_stprintf_s(szErrMsg, strlen(_T("Picasso Number in UI is not the same as in FA!")) + 1, _T("%s"),  _T("Picasso Number in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckUITranceiver (char* szPort, 
										   char* szUIXCVR,
										   char* szFAXCVR,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUIXCVR(szUIXCVR);
	string ssFAXCVR(szFAXCVR);
	if(ssUIXCVR == ssFAXCVR)
		return true;
	else
	{
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5009"));
		_stprintf_s(szErrMsg, strlen(_T("Tranceiver in UI is not the same as in FA!")) + 1, _T("%s"),  _T("Tranceiver in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckUIModelName  (char* szPort, 
										   char* szUIModelName,
										   char* szFAModelName,
										   char* szErrCode,
										   char* szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);

	string ssUIModelName(szUIModelName);
	string ssFAModelName(szFAModelName);
	if(ssUIModelName == ssFAModelName)
		return true;
	else
	{	
		_stprintf_s(szErrCode, 5, _T("%s"),  _T("5010"));
		_stprintf_s(szErrMsg, strlen(_T("Model Name in UI is not the same as in FA!")) + 1, _T("%s"),  _T("Model Name in UI is not the same as in FA!"));
		return false;
	}
}

FA_OPERATOR_API bool FA_CheckPicassoRule  (char* szPort, 
										   char* szPicasso,
										   char* szModelID,
										   char* szErrCode,
										   char* szErrMsg)
{	
	CSingleLock singleLock(static_cast<CSyncObject *>(&CritSection_FA),TRUE);
	
	int nPicasso_Length = 10; //Its length is 10 usually
	if(strlen(szPicasso) != nPicasso_Length) 
	{
		strcpy_s(szErrCode, 5,  "1102");
		return false;
	}

	CString csPicasso = szPicasso;
	CString	csModelID = szModelID;

	CString strAB=csPicasso.Left(2);
	if(strAB != csModelID)
	{
		_stprintf_s(szErrMsg, 90, _T("%s"), ("The first part in Picasso Number is not %s, it is %s"), csModelID,strAB);
		strcpy_s(szErrCode, 5,  "1102");		
		return false;
	}
	CString strC1 = csPicasso.Mid(2,1).SpanIncluding(_T("12345678"));
	if(strC1.GetLength() == 0)
	{
		_stprintf_s(szErrMsg, 90, _T("%s"), ("The 3rd word in Picasso Number is illegal, it is %s"), csPicasso.Mid(2,1));
		strcpy_s(szErrCode, 5,  "1102");
		return false;
	}
	CString strC2 = csPicasso.Mid(3,1);
	if(strC2 != _T("0") && strC2 != _T("1"))
	{
		_stprintf_s(szErrMsg, 90, _T("%s"), ("The 4rd word in Picasso Number is not 0 or 1, it is %s") + strC2);
		strcpy_s(szErrCode, 5,  "1102");
		return false;
	}
	CString strC3 = csPicasso.Mid(4,1);
	if(strC3 != _T("0"))
	{
		_stprintf_s(szErrMsg,  90, _T("%s"), ("The 5rd word in Picasso Number must be 0, but it is %s"), strC3);
		strcpy_s(szErrCode, 5,  "1102");
		return false;
	}
	CString strDDDDE = csPicasso.Mid(5).SpanIncluding(_T("0123456789ABCDFGHJKLMNPQRSTUVWXYZ"));
	if(strDDDDE.GetLength() != 5)
	{
		_stprintf_s(szErrMsg, 90, _T("%s"),  ("The end part of Picasso Number is illegal, it is "), csPicasso.Mid(5));
		strcpy_s(szErrCode, 5,  "1102");
		return false;
	}
	_stprintf_s(szErrMsg, 90, _T("%s"), "Check Picasso from Scanner(%s) success.....", csPicasso);
	return true;
}