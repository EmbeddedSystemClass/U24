/***************************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name	: Error code generator.   
* File name     : ErrCodeGenerator.h             
* Summary       : Generate error code                                                             
* Version     Author              Date                 Abstract                      
* 1.0      Nancy.Yang           2012/03/19           First version   
****************************************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#define ERRCODE_GEN_EXPORTS

#ifdef ERRCODE_GEN_EXPORTS
#define ERRCODE_GEN_API extern "C" __declspec(dllexport)
#define ERRCODE_GEN_CLASS_API __declspec(dllexport)
#else
#define ERRCODE_GEN_API extern "C" __declspec(dllimport)
#define DERRCODE_GEN_CLASS_API __declspec(dllimport)
#endif

#include "resource.h"		// main symbols
#include <map>
#pragma warning(disable:4996)

class CErrCodeGeneratorApp : public CWinApp
{
public:
	CErrCodeGeneratorApp();
	~CErrCodeGeneratorApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

std::map<CString,CString>  m_map_errcode; 
std::map<CString,CString>  m_map_errmsg; 
std::map<CString,CString>  m_map_errInCIM;

/* MD5 verify */
bool PreCheckMD5(CString str_filePath);
bool ConvertXML(CString str_XMLFilePathName, CString str_newName);
bool MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile);
bool CheckVerifyResult(CString str_logFile);

ERRCODE_GEN_API bool QueryErrorCode(char* sz_errInput, char* sz_genErrcode, char* sz_genErrMsg,char* sz_errMessage,bool &b_inCIM);
ERRCODE_GEN_API bool MapCSV(char* sz_errMessage);
