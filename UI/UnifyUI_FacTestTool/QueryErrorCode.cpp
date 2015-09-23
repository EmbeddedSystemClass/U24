#include "StdAfx.h"
#include "QueryErrorCode.h"

CQueryErrorCode::CQueryErrorCode(void)
{
	m_h_ErrModule = NULL;
}

CQueryErrorCode::~CQueryErrorCode(void)
{
	UnLoadErrCodeGeneratorDLLL();
}

bool CQueryErrorCode::UnLoadErrCodeGeneratorDLLL()
{

	if (m_h_ErrModule != NULL)
	{
		FreeLibrary (m_h_ErrModule);
		m_h_ErrModule = NULL;
	}
	return true;
}

bool CQueryErrorCode::LoadErrCodeGeneratorDLL()
{
	/* Load library */
	if (m_h_ErrModule == NULL)
	{
		m_h_ErrModule = ::LoadLibrary(_T("ErrCodeGenerator.dll"));

		if (m_h_ErrModule == NULL)
		{
			m_str_errorMsg = _T("Load ErrCodeGenerator.dll Fail!");
			return false;
		}
	}

	/*Get function : QueryErrorCode*/
	QueryErrorCode = (pQueryErrorCode)::GetProcAddress(m_h_ErrModule, "QueryErrorCode");
	if (QueryErrorCode == NULL)
	{
		m_str_errorMsg = _T("加载ErrCodeGenerator.dll函数失败!\nQueryErrorCode == NULL!");
		return false;
	}

	/*Get function : MapCSV*/
	MapCSV = (pMapCSV)::GetProcAddress(m_h_ErrModule, "MapCSV");
	if (MapCSV == NULL)
	{
		m_str_errorMsg = _T("加载ErrCodeGenerator.dll函数失败!\nMapCSV == NULL!");
		return false;
	}

	return true;
}

bool CQueryErrorCode::QueryErrCode(char* sz_errInput, char* sz_genErrcode, char* sz_genErrMsg, char* sz_errMessage,bool &b_inCIM)
{
	if (!QueryErrorCode(sz_errInput,sz_genErrcode,sz_genErrMsg,sz_errMessage,b_inCIM))
	{
		return false;
	}

	return true;
}

bool CQueryErrorCode::MapCSVFile(char* sz_errMessage)
{
	if(!MapCSV(sz_errMessage))
	{
		return false;
	}

	return true;
}

CString CQueryErrorCode::GetErrorMsg( )
{
	return  m_str_errorMsg; 
}

bool CQueryErrorCode::IsInitDLL()
{
	if (m_h_ErrModule != NULL)
	{
		return true;
	}

	if (!LoadErrCodeGeneratorDLL())
	{
		return false;
	}

	char sz_errmsg[200]={0};

	if (!MapCSVFile(sz_errmsg))
	{
		m_str_errorMsg = sz_errmsg;
		return false;
	}

	return true;
}
