#include "stdafx.h"
#include "QisdaShopFloor.h"
#include "Shlwapi.h"
#include "DBAccess.h"
#include <direct.h>
#include "XML\MyMSXML.h"
#pragma comment(lib, "DBAccess.lib")

CQisdaShopFloor::CQisdaShopFloor()
{
	this->m_cstrDBInfo.SetSize(DBI_LAST);

	DWORD nNameLength = MAX_COMPUTERNAME_LENGTH;
	//::GetComputerNameA(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(nNameLength), &nNameLength);
	this->m_cstrDBInfo[DBI_PCNAME] = "F08V01";

	//this->m_cstrDBInfo[DBI_PCNAME].ReleaseBuffer();

	this->m_cstrPICSdata.SetSize(DefaultDeviceInput);
}

CQisdaShopFloor::~CQisdaShopFloor()
{
#ifdef FACTORYMODE
	Disconnect();
#endif
}

const bool CQisdaShopFloor::GetDBInfo(dbinfo_e nItem, char *szItemBuffer, char *szErrCode, char *szErrMsg)
{
	if(nItem < DBI_LAST && DBI_SO <= nItem){
		if(0 != strcpy_s(szItemBuffer, DefaultStringLength, this->m_cstrDBInfo[nItem].GetString())){
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
	cstrXMLPath.Format("%s%s%s.xml", XMLPATHFILE,
									 this->m_cstrDBInfo[DBI_MODEL].GetString(),
									 this->m_cstrDBInfo[DBI_PROCESS].GetString());
	
	if(ERROR_SUCCESS != cStationXML.Load(CString(cstrXMLPath))){
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
	
	if(!StationNodeList){
		::strcpy_s(szErrCode, DefaultStringLength, ERROR_XMLNODELISTMISSING);
		::sprintf_s(szErrMsg, DefaultStringLength, "%s node list missing", cstrRootName.GetString());
		
		return false;
	}
	
	//Store PICS item names in m_cstrPICSItem
	else{
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

	if(bRet){
		bRet = WriteToPicsFile(res, szInErrcode, nSlot);

	#ifdef FACTORYMODE
		if("10" != this->m_cstrDBInfo[DBI_EMPLOYEELV]){
			bRet = WriteToQisdaShopFloor(res, szInErrcode, szErrCode, szErrMsg, nSlot);
		}
	#endif
	}

	this->m_cstrPICSdata[nSlot].clear();

	return bRet;
}

const bool CQisdaShopFloor::QueryDB(const char *szSO, const char *szEmployee, char *szErrCode, char *szErrMsg)
{
	bool bRet = true;
	
	//copy to member
	this->m_cstrDBInfo[DBI_SO].Format("%s", szSO);
	this->m_cstrDBInfo[DBI_EMPLOYEEID].Format("%s", szEmployee);

#ifdef FACTORYMODE

	bRet = QueryStation_Info(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(),
		this->m_cstrDBInfo[DBI_LINE].GetBuffer(DefaultStringLength),
		this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(DefaultStringLength),
		this->m_cstrDBInfo[DBI_SEGMENT].GetBuffer(DefaultStringLength),
		szErrCode,
		szErrMsg);

	this->m_cstrDBInfo[DBI_LINE].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_PROCESS].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_SEGMENT].ReleaseBuffer();
	/*
	//QuerySO_Info	
	bRet = QuerySO_Info(this->m_cstrDBInfo[DBI_SO].GetBuffer(),
						this->m_cstrDBInfo[DBI_MODEL].GetBuffer(MODEL_LENGTH),
						this->m_cstrDBInfo[DBI_PN].GetBuffer(PN_LENGTH),
						this->m_cstrDBInfo[DBI_SOTYPE].GetBuffer(SOTYPE_LENGTH),
						szErrCode,
						szErrMsg);
	
	this->m_cstrDBInfo[DBI_MODEL].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_PN].ReleaseBuffer();
	this->m_cstrDBInfo[DBI_SOTYPE].ReleaseBuffer();

	//QueryEmployeeLV
	if (bRet){
		bRet = QueryEmployeeID_Right(this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
									 this->m_cstrDBInfo[DBI_EMPLOYEELV].GetBuffer(DefaultStringLength),
									 szErrCode,
									 szErrMsg);
	
		this->m_cstrDBInfo[DBI_EMPLOYEELV].ReleaseBuffer();
		
		//QueryStation_Info
		if (bRet){
			bRet = QueryStation_Info(this->m_cstrDBInfo[DBI_PCNAME].GetBuffer(),
									 this->m_cstrDBInfo[DBI_LINE].GetBuffer(DefaultStringLength),
									 this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(DefaultStringLength),
									 this->m_cstrDBInfo[DBI_SEGMENT].GetBuffer(DefaultStringLength),
									 szErrCode,
									 szErrMsg);
			
			this->m_cstrDBInfo[DBI_LINE].ReleaseBuffer();
			this->m_cstrDBInfo[DBI_PROCESS].ReleaseBuffer();
			this->m_cstrDBInfo[DBI_SEGMENT].ReleaseBuffer();
		}
	}
	*/
#else
	this->m_cstrDBInfo[DBI_MODEL] = "RD_Model";
	this->m_cstrDBInfo[DBI_PN] = "RD_PN";
	this->m_cstrDBInfo[DBI_SOTYPE] = "RD_SOTYPE";

	this->m_cstrDBInfo[DBI_LINE] = "RD_LINE";
	this->m_cstrDBInfo[DBI_PROCESS] = "RD_PROCESS";
	this->m_cstrDBInfo[DBI_SEGMENT] = "RD_SEGMENT";
#endif

	return bRet;
}

const bool CQisdaShopFloor::WriteToQisdaShopFloor(bool res, char *szInErrcode, char *szErrCode, char *szErrMsg, int nSlot)
{
	bool bRet;

	//Set each item to PreDB
	for(int i = 0;i < this->m_cstrPICSItem.GetSize();i++){
		bRet = SetValue(this->m_cstrPICSItem[i].GetString(),this->m_cstrPICSdata[nSlot][i].GetString());

		if(!bRet){
			::sprintf_s(szErrMsg, DefaultStringLength, "Insert %s to PreDB fail", this->m_cstrPICSItem[i].GetString());
			::strcpy_s(szErrCode, DefaultStringLength, ERROR_SETTOPREDBFAIL);
			break;
		}
	}

	//Insert Pass/Fail
	if(bRet){
			res ? bRet = InsertPASS_data(this->m_cstrPICSdata[nSlot][0].GetBuffer(),
										 this->m_cstrDBInfo[DBI_SO].GetBuffer(),
										 this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
										 szErrCode,
										 szErrMsg) :
				  bRet = InsertFAIL_data(this->m_cstrPICSdata[nSlot][0].GetBuffer(),
										 this->m_cstrDBInfo[DBI_SO].GetBuffer(),
										 this->m_cstrDBInfo[DBI_EMPLOYEEID].GetBuffer(),
										 szInErrcode,
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
	CStringA cstrPICSPath;
	::GetModuleFileNameA(NULL, cstrPICSPath.GetBuffer(MAX_PATH), MAX_PATH);
	cstrPICSPath.ReleaseBuffer();
	int pos = cstrPICSPath.ReverseFind('\\') + 1;
	cstrPICSPath = cstrPICSPath.Left(pos);
	cstrPICSPath += "LocalPICS\\"; 
	::_mkdir(cstrPICSPath.GetString());
	cstrPICSPath += this->m_cstrPICSdata[nSlot][0] + "_" + cstrCurrentTime + ".pics";

	if (cPICSFile.Open(CString(cstrPICSPath), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite)){
		//First 15 necessary items in PICS, Sunlight's DLL create automatically
		cstrPICS += "ID\t" + this->m_cstrPICSdata[nSlot][0] + "\n";
		cstrPICS += "Model\t" + this->m_cstrDBInfo[DBI_MODEL] + "\n";
		cstrPICS += "PartNo\t\n";//PartNo info unknown
		cstrPICS += "SO\t" + this->m_cstrDBInfo[DBI_SO] + "\n";
		cstrPICS += "SOType\t" + this->m_cstrDBInfo[DBI_SOTYPE] + "\n";
		cstrPICS += "Segment\t" + this->m_cstrDBInfo[DBI_SEGMENT] + "\n";
		cstrPICS += "Process\t" + this->m_cstrDBInfo[DBI_PROCESS] + "\n";
		cstrPICS += "Line\t" + this->m_cstrDBInfo[DBI_LINE] + "\n";
		cstrPICS += "WrkStation\t" + this->m_cstrDBInfo[DBI_PCNAME] + "\n";
		cstrPICS += "TSDATE\t" + cstrCurrentTime + "\n";
		cstrPICS += "TSTIME\t" + cstrCurrentTime + "\n";
		res ? cstrPICS += "GDND\tY\n" : cstrPICS += "GDND\tN\n";
		
		cstrTemp.Format("Errcode\t%s\n", szInErrcode);
		cstrPICS += cstrTemp;
		
		cstrPICS += "Employee\t" + this->m_cstrDBInfo[DBI_EMPLOYEEID] + "\n";
		cstrPICS += "Serial\n";

		for(int i = 0;i < this->m_cstrPICSItem.GetSize();i++){
			cstrPICS += this->m_cstrPICSItem[i] + "\t" + this->m_cstrPICSdata[nSlot][i] + "\n";
		}
		cPICSFile.WriteString(CString(cstrPICS));
		cPICSFile.Close();

		return true;
	}

	else{
		return false;
	}
}

const bool CQisdaShopFloor::CheckRoute(char *szID, char *szErrCode, char *szErrMsg)
{
#ifdef FACTORYMODE
	CSingleLock singleLock(static_cast<CSyncObject *>(&this->m_CritSection_CheckRoute),TRUE);
	
	if("10" != this->m_cstrDBInfo[DBI_EMPLOYEELV]){
		return CheckRouting(szID,
							this->m_cstrDBInfo[DBI_MODEL].GetBuffer(),
							this->m_cstrDBInfo[DBI_PROCESS].GetBuffer(),
							szErrCode,
							szErrMsg);

	}

	else
		return true;
#else
	return true;

#endif
}

const bool CQisdaShopFloor::Connect() const
{
#ifdef FACTORYMODE
	return ConnectDB();

#else
	return true;

#endif
}

void CQisdaShopFloor::Disconnect() const
{
#ifdef FACTORYMODE
	CloseDB();
#endif
}