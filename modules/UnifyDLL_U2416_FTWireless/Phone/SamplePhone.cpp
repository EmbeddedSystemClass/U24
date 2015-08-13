#include "stdafx.h"
#include "SamplePhone.h"


RF_IMPLEMENT_DYNCREATE(CSamplePhone)


// -------------------- Phone functions --------------------

bool CSamplePhone::Initial()
{
	bool bRes = false;

	QLIB_SetLibraryMode(false);

	m_hPhone = QLIB_ConnectServerWithWait(QLIB_COM_AUTO_DETECT, 5000);

	if (m_hPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hPhone))
		{
			QLIB_GetComPortNumber(m_hPhone, &m_unCOMPortNumber);
			QLIB_FlushRxBuffer(m_hPhone);
			bRes = true;
		}
	}

	return bRes;
}

bool CSamplePhone::Disconnect()
{
	if (m_hPhone != NULL)
		QLIB_DisconnectServer(m_hPhone);

	return true;
}

bool CSamplePhone::IsConnected()
{
	bool bRes = false;

	if (m_hPhone != NULL)
	{
		if (QLIB_IsPhoneConnected(m_hPhone))
			bRes = true;
	}
	else
		bRes = Initial();

	return bRes;
}

bool CSamplePhone::Reboot()
{
	bool bRes = false;

	if (m_hPhone != NULL)
	{
		if (QLIB_DIAG_CONTROL_F(m_hPhone, MODE_OFFLINE_D_F))
		{
			Sleep(300);
			if (QLIB_DIAG_CONTROL_F(m_hPhone, MODE_RESET_F))
				bRes = true;
		}
	}

	return bRes;
}

bool CSamplePhone::GetMode(int& iMode)
{
	bool bRes = false;

	if (m_hPhone != NULL)
	{
		unsigned char ucIsFTM = false;
		if (QLIB_IsFTM_Mode(m_hPhone, &ucIsFTM))
		{
			iMode = (signed int)ucIsFTM;
			bRes = true;
		}
	}

	return bRes;
}

bool CSamplePhone::ChangeMode(int iMode)
{
	bool bRes = false;

	if (m_hPhone != NULL)
	{
		if (QLIB_ChangeFTM_BootMode(m_hPhone, (unsigned char)iMode, true))
			bRes = true;
	}

	return bRes;
}
