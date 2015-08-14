#include "stdafx.h"
#include "ITestItem.h"

using namespace StringUtility;

GP_IMPLEMENT_DYNAMIC(ITestItem)

ITestItem::ITestItem()
{
}

bool ITestItem::InitData( std::map<std::string, std::string>& paramMap )
{
	return true;
}


void ITestItem::TraceLog(std::string strMsg)
{
	if ( m_pIProc )
	{
		m_pIProc->TraceLog(strMsg.c_str());
	}
};

void ITestItem::FactoryLog(bool bPass, std::string strMsg)
{
	if ( m_pIProc  )
	{
		m_pIProc->FactoryLog(bPass,strMsg.c_str());
	}
};

void ITestItem::FactoryLog(bool bPass,
				std::string strItemCode,
				std::string strErrCode,
				std::string strBand,
				std::string strChannel,
				std::string strLower,
				std::string strUpper,
				std::string strMeasured,
				std::string strUnit,
				std::string strMsg
				)

{
	if ( m_pIProc)
	{
		m_pIProc->FactoryLog(bPass,strItemCode,strErrCode,strBand,strChannel,strLower,strUpper,strMeasured,strUnit,strMsg);
	}
};

void ITestItem::CPKLog(const char* szTech, const char* szItemName, const char* szBandClass, const char* szChannel, const char* szIdentifier1, 
					   const char* szIdentifier2, const char* szLowerSpec, const char* szUpperSpec, const char* szMeasValue, bool bPass, int iSlot)

{
	if ( m_pIProc)
	{
		m_pIProc->CPKLog(szTech,szItemName,szBandClass,szChannel,szIdentifier1,szIdentifier2,szLowerSpec,szUpperSpec,szMeasValue,bPass,iSlot);
	}
};

void ITestItem::SetPICSData(std::string strKey, std::string strValue)
{
	if ( m_pIProc)
	{
		m_pIProc->SetPICSData(strKey, strValue);
	}
};

void ITestItem::SetUIMessage(std::string strMsg)
{
	if (m_pIProc)
	{
		m_pIProc->SetUIMessage(strMsg);
	}
};

void ITestItem::SetUIProgress(int iMainProgress, int iSubProgress)
{
	if (m_pIProc)
	{
		m_pIProc->SetUIProgress(iMainProgress, iSubProgress);
	}
};

int ITestItem::GetCurrentSlot()
{
	int iSlot = -1;
	if ( m_pIProc)
	{
		iSlot = m_pIProc->GetCurrentSlot();
	}

	return iSlot;
}

void ITestItem::SetUiFailMsg(std::string strMsg)
{
}

p_fn_processor_callback ITestItem::GetProcessorCallback()
{
	p_fn_processor_callback pcallback = NULL;
	if ( m_pIProc)
	{
		pcallback = m_pIProc->GetProcessorCallback();
	}

	return pcallback;
}