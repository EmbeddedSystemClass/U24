#include "StdAfx.h"
#include "FAOperator_new.h"

CFAOperator_new::CFAOperator_new(void)
{
}

CFAOperator_new::~CFAOperator_new(void)
{
}

bool CFAOperator_new::GetRecordData( char* sz_sectorData )
{
	return GetRecordSectorData(sz_sectorData);
}

bool CFAOperator_new::GetBasicData( char* sz_sectorData )
{
	char sz_basicdata[260] = {0};
	char sz_pointerdata[260] = {0};

	GetBasicSectorData(sz_basicdata);
	GetPointerSectorData(sz_pointerdata);

	memcpy_s(sz_sectorData,SECTOR_SIZE,sz_basicdata,SECTOR_SIZE);
	memcpy_s(sz_sectorData+SECTOR_SIZE,SECTOR_SIZE,sz_pointerdata,SECTOR_SIZE);

	return true;
}

bool CFAOperator_new::GetBasicSectorData( char* sz_sectorData )
{
	SetMagicNum(MAGICNUMBER);
	SetFAVersion(FA_VERSION);

	CString str_data = _T("");
	for (int i = 1; i<= BASICNUM; i++)
	{
		str_data += m_map_BasicData[i] + _T(",");
	}
	//str_data += m_map_BasicData[BASICNUM];

	CStringToChar(str_data,sz_sectorData);

	return true;
}

bool CFAOperator_new::GetPointerSectorData( char* sz_sectorData )
{
	CString str_data = _T("");
	str_data.Format(_T("%d,%d"),m_map_PointerData[1],m_map_PointerData[2]);
	CStringToChar(str_data,sz_sectorData);
	return true;
}

bool CFAOperator_new::GetRecordSectorData( char* sz_sectorData )
{
	SetWorkstation();
	SetTestTime();

	CString str_data = _T("");
	for (int i = 1; i<= RECORDNUM; i++)
	{
		str_data += m_map_RecordData[i] + _T(",");
	}
	//str_data += m_map_RecordData[RECORDNUM];

	CStringToChar(str_data,sz_sectorData);

	return true;
}

bool CFAOperator_new::SetSectorData( const char* sz_sectorData )
{
	char sz_basicdata[260]={0};
	char sz_pointerdata[260]={0};

	memcpy_s(sz_basicdata,SECTOR_SIZE,sz_sectorData,SECTOR_SIZE);
	memcpy_s(sz_pointerdata,SECTOR_SIZE,sz_sectorData+SECTOR_SIZE, SECTOR_SIZE);

	if (!SetBasicData(sz_basicdata))
	{
		return false;
	}
	if (!SetPointerData(sz_pointerdata))
	{
		return false;
	}
	return true;
}

bool CFAOperator_new::EraseFactoryArea()
{
	m_map_BasicData.clear();
	m_map_PointerData.clear();
	m_map_RecordData.clear();
	return true;
}

////////////////////////////for basic information/////////////////////////////

bool CFAOperator_new::SetMagicNum( CString str_magicnum )
{
	if (str_magicnum == MAGICNUMBER)
	{
		m_map_BasicData[1] = str_magicnum;
		return true;
	}
	else
	{
		return false;
	}	
}

bool CFAOperator_new::SetFAVersion( CString str_FAversion )
{
	if (str_FAversion == FA_VERSION)
	{
		m_map_BasicData[2] = FA_VERSION;
		return true;
	}
	else
	{
		return false;
	}
}

CString CFAOperator_new::GetPicassoNumber()
{
	if (m_map_BasicData.find(3) != m_map_BasicData.end())
	{
		return m_map_BasicData[3];
	}
	else
	{
		return _T("");
	}
}

bool CFAOperator_new::SetPicassoNumber( CString str_picasso )
{
	if (str_picasso.GetLength() != PICASSO_LENGTH)
	{
		return false;
	}

	char sz_picasso[15]={0};
	CStringToChar(str_picasso,sz_picasso);

	for (int i=0; i<PICASSO_LENGTH; i++)
	{
		if (!isalnum((int)sz_picasso[i]))
		{
			return false;
		}
	}

	m_map_BasicData[3] = str_picasso;

	return true;
}

CString CFAOperator_new::GetImeiNumber()
{
	if (m_map_BasicData.find(4) != m_map_BasicData.end())
	{
		return m_map_BasicData[4];
	}
	else
	{
		return _T("");
	}
}

bool CFAOperator_new::SetImeiNumber( CString str_imei )
{
	//if (str_imei.GetLength() != IMEI_LENGTH)
	//{
	//	return false;
	//}
	m_map_BasicData[4] = str_imei;
	return true;
}

CString CFAOperator_new::GetMobileID()
{
	if (m_map_BasicData.find(7) != m_map_BasicData.end())
	{
		return m_map_BasicData[7];
	}
	else
	{
		return _T("");
	}
}

bool CFAOperator_new::SetMobileID( int i_slot )
{

#ifndef RD_MODE
	TCHAR sz_computer[30]={0};
	DWORD dw_size;
	GetComputerName(sz_computer,&dw_size);
	CString str_computer;
	str_computer.Format(_T("%s"),sz_computer);

	str_computer = str_computer + _T("000000");

	str_computer = str_computer.Left(6);

	CTime tCurrent = CTime::GetCurrentTime();
	CString str_datetime;
	str_datetime.Format(_T("%s%02d%4d%02d%02d%02d%02d%02d"),str_computer,i_slot,tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(),tCurrent.GetHour(),tCurrent.GetMinute(),tCurrent.GetSecond());
	m_map_BasicData[7] = str_datetime;
#else
	m_map_BasicData[7] = MOBILEID_RD;
#endif

	return true;
}

bool CFAOperator_new::SetModelName(CString str_modelname)
{
	if (str_modelname == _T(""))
	{
		return false;
	}

	m_map_BasicData[6] = str_modelname;

	return true;
}

bool CFAOperator_new::SetServiceID( CString str_serviceid )
{
	m_map_BasicData[5] = str_serviceid;
	return true;
}

CString CFAOperator_new::GetNAL()
{
	if (m_map_BasicData.find(8) != m_map_BasicData.end())
	{
		return m_map_BasicData[8];
	}
	else
	{
		return _T("");
	}
}

bool CFAOperator_new::SetNAL( CString str_NAL )
{
	if (str_NAL == _T(""))
	{
		return false;
	}

	m_map_BasicData[8]= str_NAL;
	return true;

}
////////////////////////////////////////for pointer data//////////////////////////////////////

bool CFAOperator_new::IncRecordNum()
{
	if (m_map_PointerData.size() == 0)
	{
		m_map_PointerData[1] = 1;
		m_map_PointerData[2] = 1;
	}
	m_map_PointerData[2] = m_map_PointerData[2] + 1;

	return true;
}

int CFAOperator_new::GetPointer()
{
	if (m_map_PointerData.size() == 0)
	{
		return 1;
	}
	else
	{
		return m_map_PointerData[2];
	}
}



//////////////////////////////////////for record data/////////////////////////////////////
bool CFAOperator_new::SetRecordType( CString str_recordtype )
{
	if (str_recordtype == _T("F") || str_recordtype == _T("S") ||str_recordtype == _T("O"))
	{
		m_map_RecordData[1] = str_recordtype;
		return true;
	}
	else
	{
		return false;
	}
}

bool CFAOperator_new::SetPartNum( CString str_partnum )
{
	if (str_partnum == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[2] = str_partnum;
		return true;
	}	
}

bool CFAOperator_new::SetSO( CString str_so )
{
	if (str_so == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[3] = str_so;
		return true;
	}
}

bool CFAOperator_new::SetSOType( CString str_sotype )
{
	if (str_sotype == _T("N") || str_sotype == _T("R"))
	{
		m_map_RecordData[4] = str_sotype;
		return true;
	}
	else
	{
		return false;
	}
}

bool CFAOperator_new::SetSegment( CString str_segment )
{
	if (str_segment == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[5] = str_segment;
		return true;
	}
}

bool CFAOperator_new::SetProcess( CString str_process )
{
	if (str_process == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[6] = str_process;
		return true;
	}
}

bool CFAOperator_new::SetLine( CString str_line )
{
	if (str_line == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[7] = str_line;
		return true;
	}
}

bool CFAOperator_new::SetWorkstation()
{

	TCHAR sz_computer[30]={0};
	DWORD dw_size;
	GetComputerName(sz_computer,&dw_size);
	CString str_pcname;
	str_pcname.Format(_T("%s"),sz_computer);
	m_map_RecordData[8] = sz_computer;

	return true;
}

bool CFAOperator_new::SetTestTime()
{
	CTime tCurrent = CTime::GetCurrentTime();
	CString str_date,str_time;
	str_date.Format(_T("%04d%02d%02d"),tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay());
	str_time.Format(_T("%02d%02d%02d"),tCurrent.GetHour(),tCurrent.GetMinute(),tCurrent.GetSecond());
	m_map_RecordData[9] = str_date;
	m_map_RecordData[10] = str_time;

	return true;
}

bool CFAOperator_new::SetTestResult( CString str_result )
{
	if (str_result == _T("N") || str_result == _T("Y"))
	{
		m_map_RecordData[11] = str_result;
		return true;
	}
	else
	{
		return false;
	}
}

bool CFAOperator_new::SetErrorcode( CString str_errorcode )
{
	if (str_errorcode == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[12] = str_errorcode;
		return true;
	}
}

bool CFAOperator_new::SetEmployeeID( CString str_employeeid )
{
	if (str_employeeid == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[13] = str_employeeid;
		return true;
	}
}

bool CFAOperator_new::SetToolVersion( CString str_toolversion )
{
	if (str_toolversion == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[14] = str_toolversion;
		return true;
	}
}

bool CFAOperator_new::SetFactoryVersion( CString str_factoryversion )
{
	if (str_factoryversion == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[15] = str_factoryversion;
		return true;
	}
}

bool CFAOperator_new::SetUserVersion( CString str_userversion )
{
	if (str_userversion == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[16] = str_userversion;
		return true;
	}
}

bool CFAOperator_new::SetPcba( CString str_pcba )
{
	if (str_pcba == _T(""))
	{
		return false;
	}
	else
	{
		m_map_RecordData[17] = str_pcba;
		return true;
	}
}


bool CFAOperator_new::SetBasicData( char* sz_sectorData )
{
	CString str_srcString = ConvertString(sz_sectorData); 

	m_map_BasicData.clear();

	if (str_srcString.Find(_T(",")) == -1)
	{
		return true;
	}
	
	CString strTemp;
	int i_index = 0;
	while(str_srcString.GetLength() != 0)
	{
		i_index++;
		if (str_srcString.Find(_T(",")) == -1)
		{
			m_map_BasicData[i_index] = str_srcString;
			break;
		}
		else
		{
			strTemp = str_srcString.Left(str_srcString.Find(_T(",")));
			str_srcString = str_srcString.Mid(str_srcString.Find(_T(",")) + 1,str_srcString.GetLength());
			m_map_BasicData[i_index] = strTemp;
		}	
	}
	return true;
}

bool CFAOperator_new::SetPointerData( char* sz_sectorData )
{
	CString str_srcString = ConvertString(sz_sectorData); 

	m_map_PointerData.clear();

	if (str_srcString.Find(_T(",")) == -1)
	{
		m_map_PointerData[1] = 1;
		m_map_PointerData[2] = 1;
		return true;
	}

	m_map_PointerData[1] = StrToInt(str_srcString.Left(str_srcString.Find(_T(","))));
	str_srcString = str_srcString.Mid(str_srcString.Find(_T(",")) + 1,str_srcString.GetLength());
	m_map_PointerData[2] = StrToInt(str_srcString);
	return true;
}

bool CFAOperator_new::IsValidePicasso()
{
	if (GetPicassoNumber().GetLength() == PICASSO_LENGTH)
	{
		if (GetMobileID() != MOBILEID_RD)
		{
			return true;
		}
		
	}

	return false;

}













