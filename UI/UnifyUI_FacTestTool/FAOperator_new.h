/***************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name   : FA related functions   
* File name     : FAOperator_new.h             
* Summary       : define fa related functions                                                
* Version     Author             Date                Abstract                      
* 1.0         Luffy.Liu          2011/11/22          add
****************************************************************************/

#pragma once


#define SECTOR_SIZE		256
#define PICASSO_SIZE	10
#define IMEI_LENGTH     15
#define MAGICNUMBER		_T("Dan6673Dan")
#define FA_VERSION      _T("01")
#define BASICNUM        8
#define POINTERNUM      2
#define RECORDNUM       17
#define FA_SIZE         512
#define MOBILEID_RD		_T("0000000000000000000000")


#include <string>
#include <vector>
#include <algorithm>
#include <afxpriv.h>
#include "CommDef.h"

using std::find;
using std::string;
using std::vector;

#ifndef _UNICODE
#define _UNICODE
#endif

class CFAOperator_new
{
public:
	CFAOperator_new(void);
	~CFAOperator_new(void);


	bool GetRecordData(char* sz_sectorData);
	bool GetBasicData(char* sz_sectorData);
	bool SetSectorData(const char* sz_sectorData);
	bool EraseFactoryArea();

//for basic infomation
	bool IsValidePicasso();

	CString GetPicassoNumber();
	bool SetPicassoNumber(CString str_picasso);

	CString GetMobileID();
	bool SetMobileID(int i_slot);

	CString GetImeiNumber();
	bool SetImeiNumber(CString str_imei);

	bool SetModelName(CString str_modelname);
	bool SetServiceID(CString str_serviceid);

	CString GetNAL();
	bool SetNAL(CString str_NAL);

//for sector pointer
	bool IncRecordNum();
	int  GetPointer();

//for record information
	bool SetRecordType(CString str_recordtype);
	bool SetPartNum(CString str_partnum);
	bool SetSO(CString str_so);
	bool SetSOType(CString str_sotype);
	bool SetSegment(CString str_segment);
	bool SetProcess(CString str_process);
	bool SetLine(CString str_line);

	bool SetTestResult(CString str_result);
	bool SetErrorcode(CString str_errorcode);
	bool SetEmployeeID(CString str_employeeid);
	bool SetToolVersion(CString str_toolversion);
	bool SetFactoryVersion(CString str_factoryversion);
	bool SetUserVersion(CString str_userversion);
	bool SetPcba(CString str_pcba);

private:
	bool GetBasicSectorData(char* sz_sectorData);
	bool GetPointerSectorData(char* sz_sectorData);
	bool GetRecordSectorData(char* sz_sectorData);

	bool SetBasicData(char* sz_sectorData);
	bool SetPointerData(char* sz_sectorData);

	bool SetWorkstation();
	bool SetTestTime();

	bool SetMagicNum(CString str_magicnum);
	bool SetFAVersion(CString str_FAversion);

private:
	std::map<int, CString> m_map_BasicData;	
	std::map<int, int>     m_map_PointerData;
	std::map<int, CString> m_map_RecordData;
};
