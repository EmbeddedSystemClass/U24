
#pragma once

#include <Shlwapi.h>
#include "afxmt.h"
#define MAX_BUFFER_SIZE                        512

/*==================== Structure ==========================*/
/* Main UI display ID for all station use */
#pragma pack (1)
struct st_UIIDparameter
{
	CString str_station;             // Station (DL,FT...)
	CString str_modelName;           // Model Name
	CString str_line;                // Line (T01,FA05,P07...)

	CString str_so;
	CString str_partNumber;
	CString str_toolVer;
	CString str_employee;
	CString str_factoryVer;
	CString str_userLoadVer;
	CString str_imagePath;
	CString str_soType;              // so type is normal or rework
	CString str_segment;             //TLINE,FA,PACKAGE,FQC,OOB,etc.
	CString str_CRType;              //C,D or normal SO.

	CString str_idtype;           //for pline tool, id may be imei or others
	int		i_idLength;

	CString str_SDstatus;         //define in PreDB, 'IN':read shipping os auto test result from inner SD
	                              //                 'OUT':read shipping os auto test result external SD
	CString str_APKTestItem;      //shipping os auto test item define in PreDB
	bool    b_isReadIMEI;         //true: Read imei from phone in shipping os
	                              //false: scan imei in shipping os

};
#pragma pack()

CString ConvertString(const char* sz_buffer);  //char to CString
void CStringToChar(CString str_In, char *sz_Out);