/******************************************************************************
* Copyright (C) QCS/WGC18                                      
* Module name	: Entrance Tool   
* File name     : MD5_Check.h            
* Summary       : Do MD5 Check                                                              
* Version      Author            Date                 Abstract                      
* 1.0          Tom Li         2011/07/06           First version                        
*****************************************************************************/

#pragma once

#include "Clog.h"
#include "DirectoryDispose.h"
#include "../../Lib/MyMSXML/MyMSXML.h"
//#include "Funccheck.h"


#define  VERIFYSUCCEEDSTRING   _T("All files verified successfully")
#define  INIFILEPASH           _T(".\\INI\\setup.ini")
#define  DISKMAP               _T("Y:")
#define  ImagePath             _T("C:\\Image\\")
#define  ToolPath             _T("C:\\Tool\\")

//#import "MSXML.TLB" named_guids

//class CMyMSXML;
//
//#ifdef  _WIN32
//typedef CMyMSXML						XMLClass;
//typedef MSXML::IXMLDOMDocumentPtr		XMLDoc;
//typedef MSXML::IXMLDOMNodePtr			XMLNode;
//typedef MSXML::IXMLDOMNodeListPtr		XMLNodeList;
//typedef MSXML::IXMLDOMAttributePtr		XMLAttr;
//typedef MSXML::IXMLDOMNamedNodeMapPtr	XMLAttrList;
//typedef MSXML::IXMLDOMElementPtr		XMLElement;
//#endif

class MD5_Check
{
public:
	MD5_Check(void);
	~MD5_Check(void);



public:
	bool ConvertXML(CString str_XMLFilePathName, CString str_newName, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool GetToolfilespath(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool CheckVerifyResult(CString str_logFile, int& i_result, CString& str_errorfile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	void InsertStringIntoList(CString str_String);
	bool VerifyHash(CString str_dirPath, CString str_XMLFilePath, CString str_logFile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	//bool GenerateHash(CString str_dirPath, CString str_XMLFilePath, CString str_XMLFileName, CString str_logFile);
	bool ReadIniFile(CString& str_ModelName, CString& str_SoftVersion, CString& str_ServerPath);
	bool CheckServerPathValid(void);
	bool Do_MD5_Check(CString str_filecheck, CString str_Hashpath, CString str_Replace, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool CheckDiskMapServer(CString str_path, CString& str_destPath);
	bool AddMapDiskServer(CString str_disk, CString str_server);
	bool CancelMapDiskServer(CString str_disk);
	bool GenerateHash(CString str_dirPath, CString str_XMLFilePath, CString str_XMLFileName, CString str_logFile, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool VerifyGeneratedHash(CString str_Hash_Generate, CString str_Hash_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
    bool ReadUiXML(CString str_XMLFilePathName,CString& str_newName,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
private:
	CString       m_str_serverPath;      // Server path (SERVER_PATH)
	CString       m_str_dirPath;         // Server Directory path (DIRECTORY_PATH)
	CString       m_str_hashPath;        // Server Hash path (HASHFILE_PATH)
	CString       m_str_StationName;     // Station Name
	CString       m_str_ModelName;       // Model Name
	CString       m_str_swversion;       // Software version
	CStringArray  m_stra_Toolpath;       // TOOL files path 
	bool          m_b_checkFlag;


	//char[512]    m_sz_serverPath;
	//char[512]    m_sz_dirPath;
	//char[512]    m_sz_hashPath;
	//char[512]    m_sz_localPath;
	//char[512]    m_sz_StationName;


	DirectoryDispose obj_dirDispose;   // Dispose directory object
	Clog obj_logDispose;               // Write log object
	CMyMSXML m_obj_XML_GenerateHash;
	CMyMSXML m_obj_XML_ServerHash;
	Clog m_Clog;

	//std::map<std::string, std::string> m_map_generate;
	//std::map<std::string, std::string> m_map_Server;
};
