#include "stdafx.h"
#include "QisdaShopFloor.h"
#include "Shlwapi.h"
#include <direct.h>
#include <stdlib.h>
#include "XML/MyMSXML.h"

CQisdaShopFloor::CQisdaShopFloor() : m_hDBAccess(NULL)
{
	this->m_cstrDBInfo.SetSize(DBI_LAST);

	//Get pcname
	DWORD nNameLength = MAX_COMPUTERNAME_LENGTH;
	::GetComputerNameA(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(nNameLength), &nNameLength);
	//strcpy_s(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(),7,_T("P06U02"));
	this->m_cstrDBInfo[DBI_PCNAME].ReleaseBuffer();

	this->m_cstrPICSdata.SetSize(DefaultDeviceInput);

	//Get module path
	::GetModuleFileNameA(NULL, this->m_ModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	this->m_ModulePath.ReleaseBuffer();
	::PathRemoveFileSpecA(this->m_ModulePath.GetBuffer(MAX_PATH));
	this->m_ModulePath.ReleaseBuffer();
}

CQisdaShopFloor::~CQisdaShopFloor()
{
	if (this->m_hDBAccess != NULL)
	{
		FreeLibrary (this->m_hDBAccess);
		this->m_hDBAccess = NULL;
	}
}

const bool CQisdaShopFloor::GetDBInfo(dbinfo_e nItem, char *szItemBuffer, char *szErrCode, char *szErrMsg)
{
	if(nItem < DBI_LAST && DBI_SO <= nItem)
	{
		if(0 != strcpy_s(szItemBuffer, DefaultStringLength, this->m_cstrDBInfo[nItem].GetString()))
		{
			::strcpy_s(szErrCode, DefaultStringLength, "Write to string fail");
			::strcpy_s(szErrMsg, DefaultStringLength, ERROR_WRITESTRINGFAIL);

			return false;
		}

		return true;
	}

	::strcpy_s(szErrCode, DefaultStringLength, "Out of data range");
	::strcpy_s(szErrMsg, DefaultStringLength, ERROR_OUTOFRANGEFAIL);

	return false;
}

const bool CQisdaShopFloor::ParsingXML(char *szErrCode, char *szErrMsg)
{
	CMyMSXML cStationXML;
	
	//XML naming rule:Model+station.xml
	CStringA cstrXMLPath;
	cstrXMLPath.Format("%s%s%s.xml", this->m_ModulePath.GetString(),
									   XMLPATHFILE,
									   this->m_cstrDBInfo[DBI_PROCESS].GetString());
	
	if(ERROR_SUCCESS != cStationXML.Load(CString(cstrXMLPath)))
	{
		::strcpy_s(szErrCode, DefaultStringLength, ERROR_XMLMISSING);
		::sprintf_s(szErrMsg, DefaultStringLength, "%s missing", cstrXMLPath.GetString());
		
		return false;
	}

	//Root name, PICS
	XMLNode rootnode = cStationXML.GetRoot();
	CStringA cstrRootName = CStringA(cStationXML.GetNodeName(rootnode));

	XMLNodeList StationNodeList = cStationXML.GetChildNodes(CString(cstrRootName));
	int nNodeSize = cStationXML.GetNodeListSize(StationNodeList);
	
	this->m_cstrPICSItem.RemoveAll();
	this->m_cstrPICSItem.SetSize(nNodeSize - 1);//No first node <ColType>
	
	if(!StationNodeList)
	{
		::strcpy_s(szErrCode, DefaultStringLength, ERROR_XMLNODELISTMISSING);
		::sprintf_s(szErrMsg, DefaultStringLength, "%s node list missing", cstrRootName.GetString());
		
		return false;
	}
	
	//Store PICS item names in m_cstrPICSItem
	else
	{
		for(int i = 0;i < this->m_cstrPICSItem.GetSize();i++){
			XMLNode node = cStationXML.GetNodeListItem (StationNodeList, i + 1);
			this->m_cstrPICSItem[i] = cStationXML.GetNodeText(node).GetString();
		}
	}

	return true;
}

void CQisdaShopFloor::SetPICS(const char *szValue, int nSlot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_SetPICS),TRUE);

	this->m_cstrPICSdata[nSlot].push_back(szValue);
}

const bool CQisdaShopFloor::Write(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_WritePICS),TRUE);

	bool bRet = true;

	bRet = this->ParsingXML(szErrCode, szErrMsg);
	
	if(bRet)
	{
		bRet = WriteToPicsFile(res, szInErrcode, nSlot);

		if("10" != this->m_cstrDBInfo[DBI_EMPLOYEELV]){
			bRet = WriteToQisdaShopFloor(res, szInErrcode, szErrCode, szErrMsg, nSlot);
		}
	}

	this->m_cstrPICSdata[nSlot].clear();

	return bRet;
}

const bool CQisdaShopFloor::CheckSO(char *szPicaso, char *szErrCode, char *szErrMsg)
{
	bool bRet = true;

	DWORD dwSize = sizeof(m_sz_PCname);
    ::GetComputerName(m_sz_PCname,&dwSize);

	memset(m_sz_PCname, 0, sizeof(m_sz_PCname));
	memset(m_sz_line, 0, sizeof(m_sz_line));
	memset(m_sz_process, 0, sizeof(m_sz_process));
	memset(m_sz_segment, 0, sizeof(m_sz_segment));
	char sz_process[10];
	//= 'RUN_IN';
	char sz_count[10];
	memset(sz_process, 0, sizeof(sz_process));
	memset(sz_count, 0, sizeof(sz_count));
	CString str_process = _T("RUN_IN");
	strcpy_s(sz_process,sizeof(sz_process),str_process.GetBuffer());
	str_process.ReleaseBuffer();

	int n_processcount = 0;
	int n_socount = 0;

	n_socount = atoi(this->m_cstrDBInfo[DBI_QTY].GetBuffer(QTY_LENGTH+1));
	this->m_cstrDBInfo[DBI_QTY].ReleaseBuffer();

	CString str_CRtype = _T("");
	str_CRtype.Format(m_cstrDBInfo[DBI_CRTYPE].GetBuffer(CRTYPE_LENGTH+1));
	m_cstrDBInfo[DBI_CRTYPE].ReleaseBuffer();

	if(!str_CRtype.IsEmpty())
	{
		bRet = this->m_pQuerySOCount_Control(szPicaso,this->m_cstrDBInfo[DBI_SO].GetBuffer(),
			sz_process,sz_count,szErrCode,szErrMsg);
		this->m_cstrDBInfo[DBI_SO].ReleaseBuffer();

		if(bRet)
		{
			n_processcount = atoi(sz_count);
			if(n_socount < n_processcount)
			{
				CString str_temp = _T("通过SO查询出来的工单数小于Picasso查询出来的工单数");
				strcpy_s(szErrMsg,512,str_temp.GetBuffer());
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	//bRet = m_QueryStation_Info(m_sz_PCname,m_sz_line,m_sz_process,m_sz_segment,szErrCode,szErrMsg);
	bRet = m_QueryStation_Info(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(),m_sz_line,m_sz_process,m_sz_segment,szErrCode,szErrMsg);
	this->m_cstrDBInfo[DBI_PCNAME].ReleaseBuffer();
	if(bRet)
	{
		//AfxMessageBox("m_CheckSO begin", MB_OK);
		bRet = m_CheckSO(this->m_cstrDBInfo[DBI_SO].GetBuffer(),
			this->m_cstrDBInfo[DBI_CRTYPE].GetBuffer(),
			szPicaso,
			this->m_cstrDBInfo[DBI_MODEL].GetBuffer(MODEL_LENGTH+1),
			m_sz_process,m_sz_segment,szErrCode,szErrMsg);
	}

	return bRet;
}

const bool CQisdaShopFloor::QueryDB(const char *szSO, const char *szEmployee, char *szErrCode, char *szErrMsg)
{
	bool bRet = true;
	
	//copy to member
	this->m_cstrDBInfo[DBI_SO].Format("%s", szSO);
	this->m_cstrDBInfo[DBI_EMPLOYEEID].Format("%s", szEmployee);

	memset(m_sz_toolVer, 0, sizeof(m_sz_toolVer));
	memset(m_sz_facImageVer, 0, sizeof(m_sz_facImageVer));
	memset(m_sz_userImageVer, 0, sizeof(m_sz_userImageVer));
	memset(m_sz_ETtoolVer, 0, sizeof(m_sz_ETtoolVer));
	memset(m_sz_starttime, 0, sizeof(m_sz_starttime));
	memset(m_sz_endtime, 0, sizeof(m_sz_endtime));
	memset(m_sz_Status_factory, 0, sizeof(m_sz_Status_factory));
	memset(m_sz_sz_Qty, 0, sizeof(m_sz_sz_Qty));
	memset(m_sz_CRType, 0, sizeof(m_sz_CRType));
	memset(m_sz_PCname, 0, sizeof(m_sz_PCname));
	memset(m_sz_model, 0, sizeof(m_sz_sz_Qty));
	memset(m_sz_partNo, 0, sizeof(m_sz_CRType));
	memset(m_sz_soType, 0, sizeof(m_sz_PCname));


	CString str_CRType = _T("");

	//(char* sz_so, char* sz_model, char* sz_partNo, char* sz_soType, char * sz_toolVer, char * sz_facImageVer, char * sz_userImageVer,
	//char * sz_EntranceToolVer,char * sz_Qty,char * sz_CRtype,char* sz_errorCode, char* sz_errorMsg);
	//bRet = this->m_QuerySO_Info_V3(szSO,m_sz_model,m_sz_partNo,m_sz_soType,
	//						m_sz_toolVer,m_sz_facImageVer,m_sz_userImageVer,
	//						m_sz_ETtoolVer,m_sz_sz_Qty,m_sz_CRType,
	//						szErrCode,
	//						szErrMsg);
	/*if(!bRet)
	{
		return false;
	}*/


//#ifdef FACTORYMODE	
	/*
	this->m_cstrDBInfo[DBI_MODEL] = "GWOLB1A";
	this->m_cstrDBInfo[DBI_SO] = "123456";
	this->m_cstrDBInfo[DBI_EMPLOYEEID] = "1007237";
	this->m_cstrDBInfo[DBI_PROCESS] = "RUN_IN";
	*/

	//QuerySO_Info	
	//AfxMessageBox("QuerySO", MB_OK);
	bRet = this->m_QuerySO_Info_V3( this->m_cstrDBInfo[DBI_SO].GetBuffer(),
									this->m_cstrDBInfo[DBI_MODEL].GetBuffer(MODEL_LENGTH+1),
									this->m_cstrDBInfo[DBI_PN].GetBuffer(PN_LENGTH+1),
									this->m_cstrDBInfo[DBI_SOTYPE].GetBuffer(SOTYPE_LENGTH+1),
									this->m_cstrDBInfo[DBI_TOOLVERSION].GetBuffer(TOOLVERSION_LENGTH+1),
									this->m_cstrDBInfo[DBI_FACTORYIMAGEVERSION].GetBuffer(FACTORYIMAGEVERSION_LENGTH+1),
									this->m_cstrDBInfo[DBI_USERIMAGEVERSION].GetBuffer(USERIMAGEVERSION_LENGTH+1),
									this->m_cstrDBInfo[DBI_ENTRANCETOOLVERSION].GetBuffer(ENTRANCETOOLVERSION_LENGTH+1),
									this->m_cstrDBInfo[DBI_QTY].GetBuffer(QTY_LENGTH+1),
									this->m_cstrDBInfo[DBI_CRTYPE].GetBuffer(CRTYPE_LENGTH+1),
									szErrCode,
									szErrMsg);

	this->m_cstrDBInfo[DBI_SO].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_MODEL].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_PN].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_SOTYPE].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_TOOLVERSION].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_FACTORYIMAGEVERSION].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_USERIMAGEVERSION].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_ENTRANCETOOLVERSION].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_QTY].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_CRTYPE].ReleaseBuffer();
	
   //AfxMessageBox("QuerySO OK", MB_OK);

	//QueryEmployeeLV
	if (bRet)
	{
		bRet = this->m_QueryEmployeeID_Right(this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
											 this->m_cstrDBInfo[DBI_EMPLOYEELV].GetBuffer(DefaultStringLength),
											 szErrCode,
											 szErrMsg);
	
		this->m_cstrDBInfo[DBI_EMPLOYEELV].ReleaseBuffer();
	}


	//QueryStation_Info
	//TOM
	if (bRet)
	{
//		this->m_cstrDBInfo[DBI_PCNAME]="P06U02";
		bRet = this->m_QueryStation_Info(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(),
										 this->m_cstrDBInfo[DBI_LINE].GetBuffer(DefaultStringLength),
										 this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(DefaultStringLength),
										 this->m_cstrDBInfo[DBI_SEGMENT].GetBuffer(DefaultStringLength),
										 szErrCode,
										 szErrMsg);
			//bRet = this->m_QueryStation_Info("P06U02",
			//							 this->m_cstrDBInfo[DBI_LINE].GetBuffer(DefaultStringLength),
			//							 this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(DefaultStringLength),
			//							 this->m_cstrDBInfo[DBI_SEGMENT].GetBuffer(DefaultStringLength),
			//							 szErrCode,
			//							 szErrMsg);
			
		this->m_cstrDBInfo[DBI_LINE].ReleaseBuffer();
		this->m_cstrDBInfo[DBI_PROCESS].ReleaseBuffer();
		this->m_cstrDBInfo[DBI_SEGMENT].ReleaseBuffer();
	}

//#else
//	this->m_cstrDBInfo[DBI_MODEL] = "RD_Model";
//	this->m_cstrDBInfo[DBI_PN] = "RD_PN";
//	this->m_cstrDBInfo[DBI_SOTYPE] = "RD_SOTYPE";
//
//	this->m_cstrDBInfo[DBI_LINE] = "RD_LINE";
//	this->m_cstrDBInfo[DBI_PROCESS] = "RD_PROCESS";
//	this->m_cstrDBInfo[DBI_SEGMENT] = "RD_SEGMENT";
//#endif

	//AfxMessageBox("Query DB  SO Exit", MB_OK);
	return bRet;
}

const bool CQisdaShopFloor::WriteToQisdaShopFloor(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot)
{
	bool bRet;
	/*
	this->m_Clear_PICSData();
	
	bRet = SetValue("ID", this->m_cstrPICSdata[nSlot][0].GetString());

	bRet = SetValue("model",this->m_cstrDBInfo[DBI_MODEL].GetString());

	bRet = SetValue("partno", "1");

	bRet = SetValue("so",this->m_cstrDBInfo[DBI_SO].GetString());

	bRet = SetValue("sotype","1");

	bRet = SetValue("SEGMENT","1");

	bRet = SetValue("PROCESS",this->m_cstrDBInfo[DBI_PROCESS].GetString());

	bRet = SetValue("LINE",this->m_cstrDBInfo[DBI_LINE].GetString());

	bRet = SetValue("workstation",this->m_cstrDBInfo[DBI_PCNAME].GetString());
	
	bRet = SetValue("TSTDATE","1");

	bRet = SetValue("TSTTIME","2");

	if(res)
		SetValue("OKNG","PASS");
	else
		SetValue("OKNG","FAIL");

	bRet = SetValue("errcode",szInErrcode);

	bRet = SetValue("EMPLOYEE",this->m_cstrDBInfo[DBI_EMPLOYEEID].GetString());

	bRet = SetValue("Serial","");
	*/

	//Set each item to PreDB
	for(int i = 0;i < this->m_cstrPICSItem.GetSize();i++)
	{
		bRet = this->m_SetValue(this->m_cstrPICSItem[i].GetString(), this->m_cstrPICSdata[nSlot][i].GetString());

		if(!bRet){
			::sprintf_s(szErrMsg, DefaultStringLength, "Insert %s to PreDB fail", this->m_cstrPICSItem[i].GetString());
			::strcpy_s(szErrCode, DefaultStringLength, ERROR_SETTOPREDBFAIL);
			break;
		}
	}

	this->m_SetValue("ToolVersion","V2.007");
	//Insert Pass/Fail
	if(bRet){
			res ? bRet = this->m_InsertPASS_data(this->m_cstrPICSdata[nSlot][0].GetBuffer(),
												 this->m_cstrDBInfo[DBI_SO].GetBuffer(),
												 this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
												 szErrCode,
												 szErrMsg) :
			      bRet = this->m_InsertFAIL_data(this->m_cstrPICSdata[nSlot][0].GetBuffer(),
												 this->m_cstrDBInfo[DBI_SO].GetBuffer(),
												 this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
												 szInErrcode,
												 szErrCode,
												 szErrMsg);
	}

	if(res)
	{
		//(char * sz_picasso/*input*/,char * sz_IMEI/*input*/,char * sz_so/*input*/,
		//char * sz_soCRtype/*input*/, char * sz_model/*input*/,char * sz_process/*input*/, 
		//char * sz_segment/*input*/,char * sz_employID/*input*/,char * sz_ErrCode, char * sz_ErrMsg);
		//AfxMessageBox("Insert", MB_OK);
		bRet = m_pInsertCRHoldInfo(this->m_cstrPICSdata[nSlot][0].GetBuffer(), 
			                       "",
			                       this->m_cstrDBInfo[DBI_SO].GetBuffer(),
								   this->m_cstrDBInfo[DBI_CRTYPE].GetBuffer(CRTYPE_LENGTH+1),
								   this->m_cstrDBInfo[DBI_MODEL].GetBuffer(MODEL_LENGTH+1),
								   m_sz_process, m_sz_segment, 
								   this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
								   this->m_cstrDBInfo[DBI_PN].GetBuffer(PN_LENGTH+1),
								   szErrCode, 
								   szErrMsg);
	}
	
	return bRet;
}

const bool CQisdaShopFloor::WriteToPicsFile(bool res, char *szInErrcode, int nSlot)
{
	CStdioFile cPICSFile;

	CStringA cstrTemp, cstrPICS("");

	SYSTEMTIME systime;
	::GetLocalTime (&systime);
	CStringA cstrCurrentTime;
	cstrCurrentTime.Format("_%d%d%d_%02d%02d%02d", systime.wYear,
												   systime.wMonth,
												   systime.wDay,
												   systime.wHour,
												   systime.wMinute,
												   systime.wSecond);

	//Local PICS path: Moduelpath/LocalPICS/Picasso_Time.pics
	CStringA cstrPICSPath(this->m_ModulePath);
	cstrPICSPath += "\\LocalPICS\\"; 
	::_mkdir(cstrPICSPath.GetString());
	cstrPICSPath += this->m_cstrPICSdata[nSlot][0] + "_" + cstrCurrentTime + ".pics";

	if (cPICSFile.Open(CString(cstrPICSPath), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
	{
		//First 15 necessary items in PICS, Sunlight's DLL create automatically
		cstrPICS += "ID\t" +			this->m_cstrPICSdata[nSlot][0] + "\n";
		cstrPICS += "Model\t" +			this->m_cstrDBInfo[DBI_MODEL] + "\n";
		cstrPICS += "PartNo\t" +		this->m_cstrDBInfo[DBI_PN] + "\n";
		cstrPICS += "SO\t" +			this->m_cstrDBInfo[DBI_SO] + "\n";
		cstrPICS += "SOType\t" +		this->m_cstrDBInfo[DBI_SOTYPE] + "\n";
		cstrPICS += "Segment\t" +		this->m_cstrDBInfo[DBI_SEGMENT] + "\n";
		cstrPICS += "Process\t" +		this->m_cstrDBInfo[DBI_PROCESS] + "\n";
		cstrPICS += "Line\t" +			this->m_cstrDBInfo[DBI_LINE] + "\n";
		cstrPICS += "WrkStation\t" +	this->m_cstrDBInfo[DBI_PCNAME] + "\n";
		cstrPICS += "TSDATE\t" +		cstrCurrentTime + "\n";
		cstrPICS += "TSTIME\t" +		cstrCurrentTime + "\n";
		res ? cstrPICS += "GDND\tY\n" : cstrPICS += "GDND\tN\n";
		cstrTemp.Format("Errcode\t%s\n", szInErrcode);
		cstrPICS += cstrTemp;
		cstrPICS += "Employee\t" + this->m_cstrDBInfo[DBI_EMPLOYEEID] + "\n";
		cstrPICS += "Serial\n";

		//Definition items in xml
		for(int i = 0;i < this->m_cstrPICSItem.GetSize();i++)
		{
			cstrPICS += this->m_cstrPICSItem[i] + "\t" + this->m_cstrPICSdata[nSlot][i] + "\n";
		}

		cPICSFile.WriteString(CString(cstrPICS));
		cPICSFile.Close();

		return true;
	}

	else
	{
		return false;
	}
}

const bool CQisdaShopFloor::CheckRoute(char *szID, char *szErrCode, char *szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_CheckRoute),TRUE);

	//AfxMessageBox("CheckRoute  begin", MB_OK);
	
	if("10" != this->m_cstrDBInfo[DBI_EMPLOYEELV])
	{
		//AfxMessageBox("In  CheckRoute", MB_OK);
		return this->m_CheckRouting_New(szID,
									this->m_cstrDBInfo[DBI_MODEL].GetBuffer(),
									this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(),
									szErrCode,
									szErrMsg);

	}

	else
	{
		//AfxMessageBox("Out  CheckRoute", MB_OK);
		return true;
	}
}

const bool CQisdaShopFloor::Connect() const
{
	return this->m_ConnectDB();

}

void CQisdaShopFloor::Disconnect() const
{
	this->m_CloseDB();
}

const bool CQisdaShopFloor::LoadDBAccess(char *szErrorMsg)
{
	if (this->m_hDBAccess != NULL)
	{
		FreeLibrary (this->m_hDBAccess);
		this->m_hDBAccess = NULL;
	}
	
	this->m_hDBAccess = LoadLibrary("DBAccess.dll");
	if (NULL == this->m_hDBAccess)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load DBAccess.dll fail");
		return false;
	}

	this->m_QuerySO_Info = (pQuerySO_Info)GetProcAddress(this->m_hDBAccess, "QuerySO_Info");
	if(!this->m_QuerySO_Info)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QuerySO_Info from dll fail");
		return false;
	}

	this->m_pQuerySOCount_Control = (pQuerySO_Info)GetProcAddress(this->m_hDBAccess, "QuerySOCount_Control");
	if(!this->m_QuerySO_Info)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QuerySOCount_Control from dll fail");
		return false;
	}

	this->m_QuerySO_Info_V3 = (pQuerySO_Info_All_V3)GetProcAddress(this->m_hDBAccess, "QuerySO_Info_All_V3");
	if(!this->m_QuerySO_Info_V3)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QuerySO_Info_V3 from dll fail");
		return false;
	}

	this->m_CheckSO = (pCheckSO)GetProcAddress(this->m_hDBAccess, "CheckSO");
	if(!this->m_CheckSO)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load CheckSO from dll fail");
		return false;
	}

	this->m_QueryStation_Info = (pQueryStation_Info)GetProcAddress(this->m_hDBAccess, "QueryStation_Info");
	if(!this->m_QueryStation_Info)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QueryStation_Info from dll fail");
		return false;
	}

	this->m_InsertFAIL_data = (pInsertFAIL_data)GetProcAddress(this->m_hDBAccess, "InsertFAIL_data");
	if(!this->m_InsertFAIL_data)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load InsertFAIL_data from dll fail");
		return false;
	}

	this->m_InsertPASS_data = (pInsertPASS_data)GetProcAddress(this->m_hDBAccess, "InsertPASS_data");
	if(!this->m_InsertPASS_data)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load InsertPASS_data from dll fail");
		return false;
	}

	this->m_SetValue = (pSetValue)GetProcAddress(this->m_hDBAccess, "SetValue");
	if(!this->m_SetValue)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load SetValue from dll fail");
		return false;
	}

	this->m_Clear_PICSData = (pClear_PICSData)GetProcAddress(this->m_hDBAccess, "Clear_PICSData");
	if(!this->m_Clear_PICSData)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load Clear_PICSData from dll fail");
		return false;
	}

	this->m_ConnectDB = (pConnectDB)GetProcAddress(this->m_hDBAccess, "ConnectDB");
	if(!this->m_ConnectDB)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load ConnectDB from dll fail");
		return false;
	}

	this->m_CloseDB = (pCloseDB)GetProcAddress(this->m_hDBAccess, "CloseDB");
	if(!this->m_CloseDB)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load CloseDB from dll fail");
		return false;
	}

	this->m_CheckRouting_New = (pCheckRouting_New)GetProcAddress(this->m_hDBAccess, "CheckRouting_New");
	if(!this->m_CheckRouting_New)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load CheckRouting_New from dll fail");
		return false;
	}

	this->m_QueryEmployeeID_Right = (pQueryEmployeeID_Right)GetProcAddress(this->m_hDBAccess, "QueryEmployeeID_Right");
	if(!this->m_QueryEmployeeID_Right)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QueryEmployeeID_Right from dll fail");
		return false;
	}

	this->m_pUpdateEmployeeID_Status = (pUpdateEmployeeID_Status)GetProcAddress(this->m_hDBAccess, "UpdateEmployeeID_Status");
	if(!this->m_pUpdateEmployeeID_Status)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load UpdateEmployeeID_Status from dll fail");
		return false;
	}

	this->m_pInsertCRHoldInfo = (pInsertCRHoldInfo)GetProcAddress(this->m_hDBAccess, "InsertCRHoldInfo");
	if(!this->m_pInsertCRHoldInfo)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load InsertCRHoldInfo from dll fail");
		return false;
	}

	this->m_pQueryHoldInfo_V2 = (pQueryholdInfo_V2)GetProcAddress(this->m_hDBAccess, "QueryholdInfo_V2");
	if(!this->m_pQueryHoldInfo_V2)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QueryholdInfo_V2 from dll fail");
		return false;
	}

	this->m_pQueryIMEI_ByPicasso = (pQueryIMEI_ByPicasso)GetProcAddress(this->m_hDBAccess, "QueryIMEI_ByPicasso");
	if(!this->m_pQueryIMEI_ByPicasso)
	{
		::strcpy_s(szErrorMsg, DefaultStringLength, "Load QueryIMEI_ByPicasso from dll fail");
		return false;
	}

	return true;
}

const int CQisdaShopFloor::GetPicsItemCount()
{
	char szErrorCode[DefaultStringLength] = {""}, szErrorMsg[DefaultStringLength] = {""};

	bool bRet = this->ParsingXML(szErrorCode, szErrorMsg);

	if(!bRet)
		return 0;

	else
		return this->m_cstrPICSItem.GetSize();
}
const bool CQisdaShopFloor::GetPicsItem(int nItem, char *szItemBuffer) const
{
	if(nItem < this->m_cstrPICSItem.GetSize())
	{
		strcpy_s(szItemBuffer, DefaultStringLength, this->m_cstrPICSItem[nItem].GetString());
		return true;
	}

	else
		return false;
}

bool CQisdaShopFloor::SetPics(char *szPicaso)
{
	bool b_res = true;
	strcpy_s(m_sz_picasso,50,szPicaso);
	return b_res;
}

bool CQisdaShopFloor::UpdateEmployeeIDStatus(char *szErrCode, char *szErrMsg)
{
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_CheckRoute),TRUE);

	return m_pUpdateEmployeeID_Status(this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(), "Y","",szErrCode, szErrMsg);
	
}


bool CQisdaShopFloor::GetHoldInformation(char *sz_Id,char *sz_model,char *sz_process,char *sz_status,char *sz_holdreas,char *szErrCode,char *szErrMsg)
{
	bool b_res = false;
	CString strBoxErrMsg;
	char sz_IMEI[30]={0};


	b_res = m_pQueryHoldInfo_V2(sz_Id,sz_model,sz_process,sz_status,sz_holdreas,szErrCode,szErrMsg);

	CString str_status=_T("");
	str_status.Format(_T("%s"),sz_status);
	//AfxMessageBox(str_status);
	if (b_res==false)
	{   
		strBoxErrMsg=_T("");
		strBoxErrMsg.Format(_T("[%s]\n%s"),szErrCode,szErrMsg);
		AfxMessageBox(strBoxErrMsg);
		return false;
	}

	if (strcmp(str_status,"H")==0)
	{   
		strBoxErrMsg=_T("");
		strBoxErrMsg.Format(_T("%s"),sz_holdreas);
		AfxMessageBox(strBoxErrMsg);
		return false;
	}

	return b_res;

}
