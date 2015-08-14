// ErrCodeGenerator.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ErrCodeGenerator.h"
#include <Shlwapi.h>
#include "io.h"
//#import "msxml2.dll"
#include "MyMSXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CErrCodeGeneratorApp, CWinApp)
END_MESSAGE_MAP()

CErrCodeGeneratorApp::CErrCodeGeneratorApp()
{
	::CoInitialize(NULL);
}

CErrCodeGeneratorApp::~CErrCodeGeneratorApp()
{
	::CoUninitialize();
}

CErrCodeGeneratorApp theApp;

BOOL CErrCodeGeneratorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


bool QueryErrorCode(char* sz_errInput, char* sz_genErrcode, char* sz_genErrMsg,char* sz_errMessage,bool &b_inCIM)
{
	if (m_map_errcode.size() == 0 || m_map_errmsg.size()==0)
	{
		sprintf(sz_errMessage,"The csv file is empty");
		return false;
	}

	CString str_errInput = sz_errInput;

	str_errInput.Trim();
	str_errInput.MakeLower();

	std::map<CString,CString>::iterator it = m_map_errcode.find(str_errInput);
	
	if (it != m_map_errcode.end())
	{
		sprintf(sz_genErrcode,"%s",it->second);
		sprintf(sz_genErrMsg,"%s",m_map_errmsg[it->second]);

		if (m_map_errInCIM[it->second] == "N")
		{
			b_inCIM = false;
		}
		else
		{
			b_inCIM = true;
		}
	}
	else
	{
		sprintf(sz_errMessage,"Find no this error define in the csv file");
		return false;
	}

	return true;
}
bool MapCSV(char* sz_errMessage)
{
	m_map_errcode.clear();
	m_map_errmsg.clear();
	m_map_errInCIM.clear();

	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	CString str_csvPath;
	str_csvPath.Format("%s\\MD5\\ErrorCodeDef", sz_currentPath);

	/* Check MD5\\ErrorCodeDef exist */
	if (::_taccess(str_csvPath, 0) != 0) {
		sprintf(sz_errMessage,"Find no this path \n没有发现指定的路径 :%s",str_csvPath);
		return false;
	}

	if (!PreCheckMD5(str_csvPath))
	{
		sprintf(sz_errMessage,"Check MD5 fail \n检查 MD5 失败");
		return false;
	}
	
	str_csvPath = str_csvPath + _T("\\*.csv");

	BOOL Binfind = FALSE;
	CFileFind find;
	Binfind = find.FindFile(str_csvPath); 
	if (!Binfind)
	{
		sprintf(sz_errMessage,"没有找到定义错误的csv文件");
		return false;
	}

	while(Binfind)
	{
		Binfind = find.FindNextFile();
		CString filepath = find.GetFileName();
		if(filepath.CompareNoCase(".") == 0 || filepath.CompareNoCase("..") == 0)	   
		{
			continue;
		}
		filepath = find.GetFilePath();
	//	CString filename = 	find.GetFileName();

		CStdioFile file;
		if (!file.Open(filepath, CFile::modeRead))
		{
			sprintf(sz_errMessage,"打开 csv 文件失败");
			return false;
		}

		BOOL Flag = TRUE;

		while(Flag)
		{       
			CString str_content;

			Flag = file.ReadString(str_content);

			if(Flag)
			{
				if (str_content.Find("//**")==-1)
				{
					char seps[] = ",";
					char *token = NULL;
					int i = 0;
					CString str_tempArray[100];

					token = strtok( (char*)str_content.GetBuffer(1024), seps );

					CString str_key = token;//A:Error Message

					str_key.Trim();
					str_key.MakeLower();

					CString str_value;

					if(m_map_errcode.count(str_key)!=0)
					{
						sprintf(sz_errMessage,"Find repeat define\n重复错误定义: %s",str_key);
						return false;
					}
					/*while( token != NULL )
					{
						token = strtok( NULL, seps );
						str_value = token;
						m_map_errcode[str_key]=str_value;
						break;
					}*/

					while( token != NULL )
					{
						token = strtok( NULL, seps );
						str_tempArray[i]= token;
						str_tempArray[i].Trim();
						i++;
					}

					str_tempArray[0].MakeUpper();//B:Error Code
					str_tempArray[4].MakeUpper();//F:Is in CIM

					if(m_map_errmsg.count(str_tempArray[0])!=0)
					{
						sprintf(sz_errMessage,"Find repeat define\n重复错误定义: %s",str_tempArray[0]);
						return false;
					}

					if ((str_key!=_T("")) && str_tempArray[0]!=_T(""))
					{
						m_map_errcode[str_key]=str_tempArray[0];//B:Error Code
						m_map_errmsg[str_tempArray[0]]=str_tempArray[2];//D:Error description in Chinese
						m_map_errInCIM[str_tempArray[0]]=str_tempArray[4];//F:Is in CIM
					}
					
				}
			}
		}

		file.Close();

	}

	return true;
}

bool PreCheckMD5(CString str_filePath)
{
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);

	CString str_MD5LocalPath;
	str_MD5LocalPath.Format("%s\\MD5\\Errorcode_MD5.xml", sz_currentPath);
	if (_taccess(str_MD5LocalPath, 0) != 0) 
	{
		return false;
	}

	/* Replace xml path */
	if (ConvertXML(str_MD5LocalPath, str_filePath) != true)
	{
		return false;
	}

	if (_taccess("C:\\MD5_LOG", 0) != 0) 
	{
		BOOL BRt = CreateDirectory("C:\\MD5_LOG",NULL);
		if (!BRt)
		{
			return false;
		}
	}

	CString str_logName;
	str_logName = "C:\\MD5_LOG\\MD5.log";

	if (MD5Verify(str_filePath, str_MD5LocalPath, str_logName) != true)
	{
		return false;
	}

	if (CheckVerifyResult(str_logName) != true)
	{
		return false;
	}

	return true;
}

bool ConvertXML(CString str_XMLFilePathName, CString str_newName)
{
	/* Check input */
	if(str_XMLFilePathName == _T("") || str_newName == _T(""))
	{
		return false;
	}

	CMyMSXML pXML;

	if (pXML.Load(str_XMLFilePathName) != ERROR_SUCCESS)
	{
		return false;
	}

	XMLNode pNode = pXML.SearchNode(_T("//FCIV"));
	if (pNode == NULL)
	{
		return false;
	}
	
	XMLNode pItem = pXML.GetFirstChild(pNode);

	while (pItem)
	{
		XMLNode pNode2;

		pNode2 = pItem->selectSingleNode("name");

		if(pNode2 == NULL)
		{
			return false;
		}
		CString str_nameValue;
		str_nameValue = (char *)pNode2->text;
		CString str_repaceName = str_newName + "\\" + str_nameValue.Mid(str_nameValue.ReverseFind('\\')+1);

		/*if (str_nameValue.Mid(str_nameValue.ReverseFind('\\')+1) == str_newName.Mid(str_newName.ReverseFind('\\')+1))
		{*/
			str_nameValue.Replace(str_nameValue,str_repaceName);
			pNode2->Puttext((_bstr_t)(const char *)str_nameValue);
		//}

		pItem = pItem->GetnextSibling();
	}

	pXML.Save((LPCTSTR)str_XMLFilePathName);


	/*MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument26));
	if(FAILED(hr)) { 
		return FALSE;
	}

	if(_taccess(str_XMLFilePathName, 0) != 0)
	{
		return false;
	}
	pDoc->load((LPCTSTR)str_XMLFilePathName);
	if(pDoc == NULL)
	{
		return false;
	}*/

	//MSXML2::IXMLDOMNodePtr pNode;
	//pNode = pDoc->selectSingleNode("//FCIV");
	//if(pNode == NULL) {
	//	return false;
	//}

	//MSXML2::IXMLDOMNodePtr pItem;
	//pItem = pNode->GetfirstChild();
	//while(NULL != pItem)
	//{
	//	MSXML2::IXMLDOMNodePtr pNode;
	//	pNode = pItem->selectSingleNode("name");
	//	if(pNode == NULL)
	//	{
	//		return false;
	//	}
	//	CString str_nameValue;
	//	str_nameValue = (char *)pNode->text;
	//	CString str_repaceName = str_newName + "\\" + str_nameValue.Mid(str_nameValue.ReverseFind('\\')+1);

	//	/*if (str_nameValue.Mid(str_nameValue.ReverseFind('\\')+1) == str_newName.Mid(str_newName.ReverseFind('\\')+1))
	//	{*/
	//		str_nameValue.Replace(str_nameValue,str_repaceName);
	//		pNode->Puttext((_bstr_t)(const char *)str_nameValue);
	//	//}

	//	pItem = pItem->GetnextSibling();
	//}

	//pDoc->save((LPCTSTR)str_XMLFilePathName);

	return true;
}

bool MD5Verify(CString str_dirPath, CString str_XMLFilePathName, CString str_logFile)
{
	/* Check Input */
	if(str_dirPath == _T("") || str_XMLFilePathName == _T("")  || str_logFile == _T("")) 
	{
		return false;
	}

	str_dirPath.Replace(_T(" "),_T("\" \""));
	str_XMLFilePathName.Replace(_T(" "),_T("\" \""));
	str_logFile.Replace(_T(" "),_T("\" \""));

	/* Check fciv.exe exist */
	char sz_currentPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, sz_currentPath, MAX_PATH);
	::PathRemoveFileSpec(sz_currentPath);
	CString str_fcivPathName;
	str_fcivPathName.Format(_T("%s\\MD5\\fciv.exe"), sz_currentPath);
	if (_taccess(str_fcivPathName, 0) != 0) 
	{
		return false;
	}

	str_fcivPathName.Replace(_T(" "),_T("\" \""));

	/* Command */
	CString str_command;
	CString str_commandOne;
	CString str_commandTwo;
	CString str_commandThree;
	str_commandOne = _T("cmd.exe /c ");
	str_commandTwo = str_fcivPathName + _T(" -r -v ") + str_dirPath + _T(" -xml ") + str_XMLFilePathName;
	str_commandThree = str_commandThree + _T(" > ") + str_logFile;
	str_command = str_commandOne + str_commandTwo + str_commandThree;

	/* Run */
	TCHAR sz_commandLine[1024];
	memset(sz_commandLine, 0, sizeof(sz_commandLine));
	for (int i = 0; i < str_command.GetLength(); i ++) 
	{
		sz_commandLine[i] = str_command[i];
	}
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb          = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW;

	BOOL b_createRes = FALSE;	
	b_createRes = CreateProcess(NULL, sz_commandLine, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
	if(b_createRes == FALSE) 
	{
		return false;
	}
	DWORD dw_waitResult = 0;
	DWORD dw_timeout;
	dw_timeout = 10000;
	dw_waitResult = ::WaitForSingleObject(pi.hProcess, dw_timeout);
	switch(dw_waitResult)
	{
	case WAIT_FAILED:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_TIMEOUT:
		::CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
		return false;

	case WAIT_OBJECT_0:
		break;
	}
	::CloseHandle(pi.hProcess);
	pi.hProcess = NULL;

	return true;
}

bool CheckVerifyResult(CString str_logFile)
{
	/* Check Input */
	if(str_logFile == _T("")) 
	{
		return false;
	}

	/* Check file exist */
	int i_existentResult = 0;
	int i_retry = 10;
	while(1)
	{
		i_existentResult = _taccess(str_logFile, 0);
		if(i_existentResult != 0)
		{
			if(i_retry <= 0)
			{
				break;
			} 
			else 
			{
				Sleep(1000);
				i_retry--;
				continue;
			}
		} 
		else 
		{
			break;
		}
	}
	if(i_existentResult != 0) 
	{
		return false;
	}

	/* Check file Size */
	DWORD dw_fileSize = 0;
	i_retry = 10;
	while(1) 
	{
		HANDLE h_handle = NULL;
		h_handle = CreateFile(str_logFile, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if(h_handle != INVALID_HANDLE_VALUE)
		{
			dw_fileSize = GetFileSize(h_handle, NULL);
			CloseHandle(h_handle);
			if(dw_fileSize == 0)
			{
				if(i_retry <= 0) 
				{
					break;
				} 
				else 
				{
					Sleep(1000);
					i_retry--;
					continue;
				}
			} 
			else 
			{
				Sleep(1000);
				break;
			}
		} 
		else {
			break;
		}
	}
	if(dw_fileSize == 0)
	{
		return false;
	}

	/* Read */
	int i_succeedFlag = 0;
	CStdioFile obj_stdioFile;
	if (!obj_stdioFile.Open(str_logFile, CFile::modeRead|CFile::modeNoTruncate|CFile::shareDenyNone, NULL))
	{
		return false;
	} 
	try 
	{
		CString str_string;
		while(obj_stdioFile.ReadString(str_string)) 
		{
			if(str_string.Find(_T("All files verified successfully")) != -1) 
			{
				i_succeedFlag = 1;
				break;
			}
			str_string = _T("");
		}
	} 
	catch(CFileException *e) 
	{
		e->Delete();
	}

	obj_stdioFile.Close();

	if (i_succeedFlag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}