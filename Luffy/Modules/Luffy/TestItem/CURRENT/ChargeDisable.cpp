#include "stdafx.h"
#include "ChargeDisable.h"


GP_IMPLEMENT_DYNCREATE(CChargeDisable)

CChargeDisable::CChargeDisable(void)
: m_pIPhone(NULL)
 ,m_pCGProc(NULL)

{
}

CChargeDisable::~CChargeDisable(void)
{
}

bool CChargeDisable::Run(int iItem)
{
	bool bRes = MainFunction();

	if (bRes)
	{
		FactoryLog(true, "", "", "", "", "", "", "", "", "Charger disable PASS");
		SetPICSData("ChargeDisable", "PASS");
	}
	else
	{
		FactoryLog(false, "", FunErr_Stop_Coin_Cell_Charge_Fail, "", "", "", "", "", "", m_strMsg);
		SetPICSData("ChargeDisable", "FAIL");
	}

	return bRes;
}

bool CChargeDisable::MainFunction()
{
	if (! m_pIPhone->Initial())
	{
		m_strMsg = "Fail to connect phone with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	if (! m_pIPhone->Charger(false))
	{
		m_strMsg = "Fail to disable charger with Qisda module";
		TraceLog(m_strMsg);
		return false;
	}

	return true;
}


bool CChargeDisable::InitData(std::map<std::string, std::string>& paramMap)
{
	m_pCGProc = dynamic_cast<CGPProcessor*>(this->m_pIProc);
	m_pIPhone = dynamic_cast<CAndroidPhone*>(m_pCGProc->GetPhoneByName());

	return true;
}
