#include "stdafx.h"
#include <cstring>
#include <sstream>
#include <fstream>
#include "../NeptuneCtrl.h"
#include "GeneralDLLApi.h" // GeneralCmd
#include "io.h"
#include "../Utility.h"
#include <regex>

using namespace std;

extern CNeptuneCtrlApp theApp;



NEPDLLAPI int __stdcall SendDataToLog(int nPort, int nTimeout, char* szLog)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	TRACER_STREAM("Port:" << nPort << ",timeout:" << nTimeout << ",log:" << szLog << "\r\n");
	return HS_OK;
}

NEPDLLAPI int __stdcall InitDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->InitDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}
	/////////////////////////////////////////////////////////////////////
	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	/////////////////////////////////////////////////////////////////////
	return nRetCode;
}

NEPDLLAPI int __stdcall FreeDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->FreeDevice(nPort);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall OpenDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->open(nPort, nTimeout);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall CloseDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->close(nPort, nTimeout);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall ExactOpenDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return OpenDevice(nPort, nTimeout);
}

NEPDLLAPI int __stdcall ExactCloseDevice(int nPort, int nTimeout)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	return CloseDevice(nPort, nTimeout);
}

//110420 gilpin add for receive serial command such as touch panel pixel and key value
NEPDLLAPI int __stdcall ReceiveCmdOut(int nPort, int nTimeout, char* GenCommand, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->ReceiveCmdOut(nPort, nTimeout, GenCommand, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("[Port:" << nPort << "] => RetCode:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall GeneralCmd(int nPort, int nTimeout, char* GenCommand)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmd(nPort, nTimeout, GenCommand);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall GeneralCmdOut(int nPort, int nTimeout, char* GenCommand, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdOut(nPort, nTimeout, GenCommand, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall GeneralCmdIn(int nPort, int nTimeout, char* GenCommand, char* InBuffer, size_t nInLength)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdIn(nPort, nTimeout, GenCommand, InBuffer, nInLength);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

NEPDLLAPI int __stdcall GeneralCmdInOut(int nPort, int nTimeout, char* GenCommand, char* InBuffer, size_t nInLength, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	TRACE(_T("g in out:%d,%d,%s\n"),nPort,nTimeout, InBuffer);
	int nRetCode = theApp.m_pCmdCtrl[nPort]->GeneralCmdInOut(nPort, nTimeout, GenCommand, InBuffer, nInLength, OutBuffer);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB;
		}
	}

	TRACER_STREAM("port:" << nPort << "=>code:" << nRetCode << "\r\n");
	return nRetCode;
}

// 2009-10-27 Mick: power on to DT UART packet mode
// send ASCII 'A' for 5 second, recieve zzz is success
NEPDLLAPI int __stdcall PowerOnToUARTPacketMode(int nPort, int nTimeout, int DTRFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	if (theApp.m_pCmdCtrl[nPort] == NULL) {
		theApp.m_pCmdCtrl[nPort] = new CmdCtrl;
	}
	int nRetCode = theApp.m_pCmdCtrl[nPort]->NormalCmdPowerOnToUARTPacketMode(nPort, nTimeout, DTRFlag);
	if (nRetCode != HS_OK) {
		if (nPort <= QTH_PORT_COM_MAX) {
			nRetCode += P21_ERR_TYPE_UART ;
		}
		else {
			nRetCode += P21_ERR_TYPE_USB ;
		}
	}
	return nRetCode;
}

//compatible
NEPDLLAPI int __stdcall FB_PassOrFail(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	return HW_PassRange(nPort, nTimeout, InBuffer, OutBuffer);
}

// TODO: remove 2, or write new format
// FIXME: syntax error would be crashed
NEPDLLAPI int __stdcall HW_PassRange(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	CString logpath;
	::GetModuleFileName(NULL, logpath.GetBuffer(MAX_PATH), MAX_PATH);
	logpath.ReleaseBuffer(MAX_PATH);
	logpath = logpath.Left(logpath.ReverseFind('\\')) + _T("\\Instrument.log");
	TRACE("%s log path: %s\n", __FUNCTION__, logpath);
	if (!IsPathExist(logpath)) {
		::AfxMessageBox(_T("Instrument.log: file not found!"));
		return -1*ERROR_FILE_NOT_FOUND;
	}
	if (InBuffer == NULL) {
		::AfxMessageBox(_T("no pass/fail judge input"));
		return -1*ERROR_INVALID_DATA;
	}

	// parameter list ==> "Critical;NumberOfRanges;DiffLow1~DiffUp1;...;DiffLowX~DiffUpX;Tag"
	// 1st stage parsing: parsing parameters
	const int MAX_CHECK = 2;
	char seps[] = ";\t\r\n";
	char* token = NULL;
	char* parameter = new char[strlen(InBuffer) + 1]; // used in szTag
	strncpy(parameter, InBuffer, strlen(InBuffer));
	parameter[strlen(InBuffer)] = '\0';
	token = strtok(parameter, seps);
	bool isCritical = false;
	if (_stricmp("critical", token) == 0) {
		isCritical = true;
	}
	else if (_stricmp("uncritical", token) == 0) {
		isCritical = false;
	}
	else {
		::AfxMessageBox(_T("Invalid parameter format: Critical/Uncritical should be specified!"));
		return -1*ERROR_INVALID_FIELD;
	}

	// get type to check
	//  0 only check 1 value in ranges
	//  1,  2 check delta in ranges
	// -1, -2 check array value in ranges
	// but -1 is meaningless
	token = strtok(NULL, seps);
	int nRowCheck = -1;
	int nLineCheck = atoi(token);
	if (nLineCheck > MAX_CHECK) {
		::AfxMessageBox(_T("Invalid parameter format: Too many ranges specified!"));
		return -1*ERROR_INVALID_FIELD;
	}
	else if (nLineCheck < 0) {
		nRowCheck = -1*nLineCheck;
		nLineCheck = -1;
		if (nRowCheck > MAX_CHECK) {
			::AfxMessageBox(_T("Invalid parameter format: Too many ranges specified!"));
			return -1*ERROR_INVALID_FIELD;
		}
	}

	// get ranges from parameter list respectively
	char* szRange[MAX_CHECK];
	for (int n = 0; ; n++) {
		if (n != 0 && (n == nLineCheck || n == nRowCheck)) { // 2, 1, -1, -2
			break;
		}
		szRange[n] = strtok(NULL, seps);
		if (nLineCheck == 0 && n == 0) { // if 0 only 1 range
			break;
		}
	}

	// get tag
	char* szTag = strtok(NULL, seps);
	char* szCmpTag = szTag;
	if (*szCmpTag == '[') {
		for (; *szCmpTag != ']'; szCmpTag++);
		szCmpTag++;
	}
	//!1st stage parsing

	// 2nd stage parsing: parsing ranges
	double nLowerBound[MAX_CHECK] = { 0.0 };
	double nUpperBound[MAX_CHECK] = { 0.0 };
	double nMeasuredLower[MAX_CHECK] = { 0.0 };
	double nMeasuredUpper[MAX_CHECK] = { 0.0 };

	for (int n = 0; ;n++) {
		if (n != 0 && (n == nLineCheck || n == nRowCheck)) { // 2, 1, -1, -2
			break;
		}
		char* LowerBound = strtok(szRange[n], "~");
		char* UpperBound = strtok(NULL, "~");
		nLowerBound[n] = atof(LowerBound);
		nUpperBound[n]  = atof(UpperBound);
		if (nLineCheck == 0 && n == 0) { // if 0 only 1 range
			break;
		}
	}
	//!2nd stage parsing
	//parameter parsing done

	// read Instrument.log
	int nCount = 0;
	ifstream fin;
	fin.open(logpath.GetBuffer(MAX_PATH), ios::in | ios::binary);
	logpath.ReleaseBuffer();
	// read lower
	if (fin.is_open()) {
		while (!fin.eof()) {
			char szLine[MAX_PATH];
			fin.getline(szLine, sizeof(szLine));

			//YTT 110119 special control for cable loss
			if (strncmp(szLine,"FB_SET_CABLE_LOSS",17) == 0) {
				continue;//do nothing
			}

			if (strstr(szLine, szCmpTag) != NULL) {
				for (int i = 0; szLine[i] != NULL; i++) {
					if (szLine[i] == ',') {
						szLine[i] = ' ';
					}
				}
				istringstream iss(szLine);
				if (nLineCheck >= 0) { // 0, 1, 2 get value from each line
					iss >> nMeasuredLower[nCount];
					if (nLineCheck == 0) { // if 0 only 1 value, nCount == 0
						break;
					}
					nCount++; // nCount would always > 0
					if (nCount == nLineCheck) { // 1, 2
						break;
					}
				}
				else { // nLineCheck == -1
					iss >> nMeasuredLower[0] >> nMeasuredLower[1];
					break;
				}
			}
		}
	}
	else {
		::AfxMessageBox(_T("File not found: Instrument.log #1"));
		return -1*ERROR_FILE_NOT_FOUND;
	}

	if (nLineCheck != -1 && nCount != nLineCheck) {
		::AfxMessageBox(_T("Instrument.log parsing fail to read 1st value!"));
		fin.close();
		return -1*ERROR_INVALID_FIELD;
	}

	// read upper
	nCount = 0;
	if (fin.is_open()) {
		while (!fin.eof()) {
			if (nLineCheck <= 0) { // if -2, -1, 0 only 1 value
				break;
			}

			char szLine[MAX_PATH];
			fin.getline(szLine, sizeof(szLine));

			//YTT 110119 special control for cable loss
			if(strncmp(szLine,"FB_SET_CABLE_LOSS",17)==0) {
				//do nothing
				continue;
			}

			if (strstr(szLine, szCmpTag) != NULL) {
				istringstream iss(szLine);
				if (nLineCheck > 0) { // 1, 2 get value from each line
					iss >> nMeasuredUpper[nCount];
					nCount++; // nCount would always > 0
					if (nCount == nLineCheck) { // 1, 2
						break;
					}
				}
			}
		}
		fin.close();
	}

	//YTT 110117 forSMIQ cable loss
	double nCableLoss[MAX_CHECK] = { 0.0 };
    //YTT read cable loss
	char* szCableLossCmd = NULL;

	//          cmd      ;   tag cmd           ; param count ; cable loss val
	//,FB_SET_CABLE_LOSS ;NVDA_MODEM_GPSCWTest ;     0       ;      3         ;
	char seps2[] = ";\t\r\n ";
	ifstream fin_cableloss;
	fin_cableloss.open(logpath.GetBuffer(), ios::in | ios::binary);
	logpath.ReleaseBuffer();
	if (fin_cableloss.is_open()) {
		while (!fin_cableloss.eof()) {
			char szLine[MAX_PATH];
			szLine[MAX_PATH-1]=0;
			fin_cableloss.getline(szLine, sizeof(szLine));
			if(strncmp(szLine,"FB_SET_CABLE_LOSS",17)==0) {
				for (int i = 0; szLine[i] != NULL; i++) {
					if (szLine[i] == ',') {
						szLine[i] = ' ';
					}
				}
				szCableLossCmd = strtok(szLine,seps2);//,FB_SET_CABLE_LOSS
                szCableLossCmd = strtok(NULL, seps2);// NVDA_MODEM_GPSCWTest

                if(strcmp(szCmpTag,szCableLossCmd)==0) {
					szCableLossCmd = strtok(NULL, seps2);// 0
					szCableLossCmd = strtok(NULL, seps2);// 3
					nCableLoss[0] = atof(szCableLossCmd);
				}
			}
		}
		fin_cableloss.close();
	}

	if (nLineCheck == 0) {
		nMeasuredUpper[nCount] = nMeasuredLower[nCount];
		nMeasuredLower[nCount] = 0;
	}
	else if (nLineCheck == -1){
		for (int i = 0; i < nRowCheck; i++) {
			nMeasuredUpper[i] = nMeasuredLower[i];
			nMeasuredLower[i] = 0;
		}
	}
	if (nLineCheck != -1 && nCount != nLineCheck) {
		::AfxMessageBox(_T("Instrument.log parsing fail to read 2nd value!"));
		return -1*ERROR_INVALID_FIELD;
	}

	// remove measured records by szTag
	CString FileContent;
	fin.open(logpath.GetBuffer(), ios::in | ios::binary);
	logpath.ReleaseBuffer();
	if (fin.is_open()) {
		while (!fin.eof()) {
			char szLine[MAX_PATH];
			fin.getline(szLine, sizeof(szLine));
			//YTT 110119 special control for cable loss
			if (strncmp(szLine, "FB_SET_CABLE_LOSS", 17) == 0) {
				continue;//do nothing
			}

			if (strstr(szLine, szCmpTag) == NULL) {
				FileContent += szLine;
				FileContent += "\n";
			}
		}
		fin.close();
	}
	else {
		::AfxMessageBox(_T("File not found: Instrument.log #2"));
		return -1*ERROR_FILE_NOT_FOUND;
	}

	ofstream fout;
	fout.open(logpath.GetBuffer(), ios::trunc | ios::binary);
	logpath.ReleaseBuffer();
	if (fout.is_open()) {
		fout << FileContent;
		fout.close();
	}

	// determine test item PASS or FAIL
	for (int m = 0; ; m++) {
		if (m != 0 && (m == nLineCheck || m == nRowCheck)) { // 2, 1, -1, -2
			break;
		}

		double value = nMeasuredUpper[m] - nMeasuredLower[m];
        //YTT 100701 cable loss
        if (m == 0) {
			value += nCableLoss[0];
		}

		TRACE("PassFail: %lf\n", value);
		if (nLowerBound[m] <= value && value <= nUpperBound[m]) {
			sprintf(OutBuffer, "%s:PASS:%f", szTag, value);
		}
		else { // fail case
			sprintf(OutBuffer, "%s:FAIL:%f:%d", szTag, value, m);
			if (isCritical) {
				return -1*ERROR_INVALID_FIELD;
			}
			else {
				break; // fail but not stop
			}
		}

		if (nLineCheck == 0 && m == 0) { // if 0 only 1 range
			break;
		}
	}
	delete[] parameter;

	return HS_OK;
}

NEPDLLAPI int __stdcall HW_PassStr(int nPort, int nTimeout, char* InBuffer, char* OutBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACER_FUNCTION_IN();

	CString logpath;
	::GetModuleFileName(NULL, logpath.GetBuffer(MAX_PATH), MAX_PATH);
	logpath.ReleaseBuffer(MAX_PATH);
	logpath = logpath.Left(logpath.ReverseFind('\\')) + _T("\\Instrument.log");
	TRACE("%s log path: %s\n", __FUNCTION__, logpath);
	if (_access(logpath, 0) == -1) {
		::AfxMessageBox(_T("Instrument.log: file not found!"));
		return -1*ERROR_FILE_NOT_FOUND;
	}
	if (InBuffer == NULL) {
		::AfxMessageBox(_T("no HW_PassStr judge input"));
		return -1*ERROR_INVALID_DATA;
	}
	// parameter list ==> "Critical;Pattern;Tag"
	char seps[] = ";\t\r\n";
	char* parameter = new char[strlen(InBuffer) + 1]; // used in szTag
	strncpy(parameter, InBuffer, strlen(InBuffer));
	parameter[strlen(InBuffer)] = '\0';

	// check critical
	char* token = strtok(parameter, seps);
	bool isCritical = false;
	if (_stricmp("critical", token) == 0) {
		isCritical = true;
	}
	else if (_stricmp("uncritical", token) == 0) {
		isCritical = false;
	}
	else {
		::AfxMessageBox(_T("Invalid parameter format: Critical/Uncritical should be specified!"));
		return -1*ERROR_INVALID_FIELD;
	}

	// get pattern
	char* szPattern = strtok(NULL, seps);

	// get tag
	char* szTag = strtok(NULL, seps);
	char* szCmpTag = szTag;
	if (*szCmpTag == '[') {
		for (; *szCmpTag != ']'; szCmpTag++);
		szCmpTag++;
	}
	// parameter parsing done

	// read Instrument.log
	ifstream fin;
	fin.open(logpath.GetBuffer(MAX_PATH), ios::in | ios::binary);
	logpath.ReleaseBuffer();

	// check pattern
	char szLine[MAX_PATH];
	if (fin.is_open()) {
		while (!fin.eof()) {
			fin.getline(szLine, sizeof(szLine));
			if (strstr(szLine, szCmpTag) != NULL) {
				for (int i = 0; szLine[i] != NULL; i++) {
					if (szLine[i] == ',') {
						szLine[i] = ' ';
					}
				}
				break;
			}
		}
		fin.close();
	}
	else {
		::AfxMessageBox(_T("File not found: Instrument.log #str"));
		return -1*ERROR_FILE_NOT_FOUND;
	}
	if (strstr(szLine, szCmpTag) == NULL) {
		::AfxMessageBox(_T("Instrument.log parsing fail to read #str value!"));
		return -1*ERROR_INVALID_FIELD;
	}

	// remove records by szTag
	CString FileContent;
	fin.open(logpath.GetBuffer(), ios::in | ios::binary);
	logpath.ReleaseBuffer();
	if (fin.is_open()) {
		while (!fin.eof()) {
			char szLine[MAX_PATH];
			fin.getline(szLine, sizeof(szLine));
			if (strstr(szLine, szCmpTag) == NULL) {
				FileContent += szLine;
				FileContent += "\n";
			}
		}
		fin.close();
	}
	else {
		::AfxMessageBox(_T("File not found: Instrument.log #2"));
		return -1*ERROR_FILE_NOT_FOUND;
	}

	ofstream fout;
	fout.open(logpath.GetBuffer(), ios::trunc | ios::binary);
	logpath.ReleaseBuffer();
	if (fout.is_open()) {
		fout << FileContent;
		fout.close();
	}

	// determine test item PASS or FAIL
	TRACE("str: %s\n", szLine);
	// 20110217 Evonne: use regular expression to check pass or fail
	CString item = szLine;
	item.Replace(szCmpTag,"");
	item.Replace("NVDA","");
	item.Replace("Target","");
	item.Trim();

	std::tr1::cmatch what;
	std::tr1::regex expression(szPattern);
	if(regex_match(item.GetBuffer(), what, expression)) {
		sprintf(OutBuffer, "%s:PASS:%s", szTag, szLine);
	}
	else { // fail case
		sprintf(OutBuffer, "%s:FAIL", szTag);
		if (isCritical) {
			return -1*ERROR_INVALID_FIELD;
		}
		else {
			// fail but not stop
		}
	}

	item.ReleaseBuffer();

	//if (strstr(szLine, szPattern) != NULL) {
	//	sprintf(OutBuffer, "%s:PASS:%s", szTag, szLine);
	//}
	//else { // fail case
	//	sprintf(OutBuffer, "%s:FAIL", szTag);
	//	if (isCritical) {
	//		return -1*ERROR_INVALID_FIELD;
	//	}
	//	else {
	//		// fail but not stop
	//	}
	//}

	delete[] parameter;

	return HS_OK;
}


