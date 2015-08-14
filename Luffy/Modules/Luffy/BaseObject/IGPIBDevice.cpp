#include "stdafx.h"
#include "IGPIBDevice.h"
#include "NI488\inc\ni488.h"
//#include "NI488\lib\GPIB-32.OBJ"

int IGPIBDevice::g_iGPIBInterfaceID = 0;
int IGPIBDevice::g_iGPIBCardAddress = 0;
std::vector<std::string> IGPIBDevice::g_vDeviceName;
std::vector<int> IGPIBDevice::g_vNI488Address;
std::vector<int> IGPIBDevice::g_vPrimaryAddress;
std::vector<int> IGPIBDevice::g_vSecondaryAddress;


GP_IMPLEMENT_DYNAMIC(IGPIBDevice)

bool IGPIBDevice::DetectGPIBDevice(int iGPIBInterfaceID, int iGPIBCardAddress)
{
	g_iGPIBInterfaceID = iGPIBInterfaceID;
	g_iGPIBCardAddress = iGPIBCardAddress;
	g_vDeviceName.clear();
	g_vNI488Address.clear();
	g_vPrimaryAddress.clear();
	g_vSecondaryAddress.clear();

	int iUD = 0;

	Addr4882_t instruments[32];
	Addr4882_t result[32];
	Addr4882_t deviceFound[32];
	int iListnerCount = 0;

	// Initial variables
	for (int i = 0; i < 32; ++i)
	{
		instruments[i] = i;
		result[i] = 0;
		deviceFound[i] = 0;
	}
	instruments[31] = NOADDR;

	// Initial GPIB interface
	CString cstrGPIBInterface;
	cstrGPIBInterface.Format(_T("GPIB%d"), g_iGPIBInterfaceID);
	//CT2A szGPIBInterface(cstrGPIBInterface);

	iUD = ibfind(cstrGPIBInterface);
	if (ibsta & ERR)
		return false;

	ibrsc(iUD, 1);
	if (ibsta & ERR)
		return false;

	ibsic(iUD);
	if (ibsta & ERR)
		return false;

	ibsre(iUD, 1);
	if (ibsta & ERR)
		return false;

	ibconfig(iUD, IbcTIMING, 1);
	if (ibsta & ERR)
		return false;

	// Find GPIB devices
	FindLstn(g_iGPIBInterfaceID, instruments, result, 31);
	if (ibsta & ERR)
		return false;

	// Calculate GPIB device counts
	for (int i = 0; i < ibcntl; ++i)
	{
		if (result[i] > 1)
		{
			deviceFound[iListnerCount] = result[i];
			++iListnerCount;
		}
	}

	// Get GPIB device name, PAD, SAD
	for (int i = 0; i < iListnerCount; ++i)
	{
		char szDeviceName[128];
		memset(szDeviceName, 0, 128);

		int iPAD = GetPAD(deviceFound[i]);
		int iSAD = GetSAD(deviceFound[i]);

		int iDD = ibdev(0, iPAD, iSAD, T10s, 1, 0);
		if (ibsta & ERR)
			return false;

		ibclr(iDD);
		if (ibsta & ERR)
			return false;

		ibwrt(iDD, (void*)"*IDN?", 5);
		if (ibsta & ERR)
			return false;

		ibrd(iDD, (void*)szDeviceName, 127);
		if (ibsta & ERR)
			return false;

		ibonl(iDD, 0);
		if (ibsta & ERR)
			return false;

		bool bExist = false;
		std::string strDeviceName(szDeviceName);

		for (int j = 0; j < (signed)g_vDeviceName.size(); ++j)
		{
			if (g_vDeviceName[j] == strDeviceName && g_vPrimaryAddress[j] == iPAD)
			{
				bExist = true;
				break;
			}
		}

		if (! bExist)
		{
			g_vDeviceName.push_back(strDeviceName);
			g_vNI488Address.push_back(deviceFound[i]);
			g_vPrimaryAddress.push_back(iPAD);
			g_vSecondaryAddress.push_back(iSAD);
		}
	}

	return true;
}


bool IGPIBDevice::GetDeviceList(std::vector<std::string>& vDeviceList)
{
	vDeviceList = g_vDeviceName;
	return true;
}

int IGPIBDevice::GetNI488Address(int iIndex)
{
	if (iIndex < (signed)g_vNI488Address.size())
		return g_vNI488Address[iIndex];

	return -1;
}

int IGPIBDevice::GetPrimaryAddress(int iIndex)
{
	if (iIndex < (signed)g_vPrimaryAddress.size())
		return g_vPrimaryAddress[iIndex];

	return -1;
}

int IGPIBDevice::GetSecondaryAddress(int iIndex)
{
	if (iIndex < (signed)g_vSecondaryAddress.size())
		return g_vSecondaryAddress[iIndex];

	return -1;
}

IGPIBDevice::IGPIBDevice()
{
	m_strDeviceName.clear();
	m_iNI488Address = -1;
	m_iPAD = -1;
	m_iSAD = -1;
	m_iDD = -1;
	m_pLog=NULL;
}

bool IGPIBDevice::InitData(std::string strDeviceName, int iNI488Address, int iPAD, int iSAD)
{
	m_strDeviceName = strDeviceName;
	m_iNI488Address = iNI488Address;
	m_iPAD = iPAD;
	m_iSAD = iSAD;

	m_iDD = ibdev(g_iGPIBInterfaceID, m_iPAD, m_iSAD, T1s, 1, 0);
	if (ibsta & ERR)
		return false;

	return true;
}

void IGPIBDevice::SetLog(CLogger* pLog)
{
	m_pLog = pLog;
}

void IGPIBDevice::CmdLog(std::string strMsg)
{
	char szHeader[256];
	memset(szHeader, 0, 256);

	sprintf_s(szHeader, 256, "GPIB_CMD %02d : ", m_iPAD);
	std::string strHeader(szHeader);
	std::string strLog = strHeader + strMsg;

	if( m_pLog )
	{
		m_pLog->CmdLog(strLog.c_str(), m_iSlot);
	}
}

bool IGPIBDevice::IsAvaliable()
{
	if (m_strDeviceName.empty() || m_iNI488Address == -1 || m_iPAD == -1 || m_iSAD == -1 || m_iDD == -1)
		return false;
	else
		return true;
}


bool IGPIBDevice::GetDeviceName(std::string& strDeviceName)
{
	if (! m_strDeviceName.empty())
	{
		strDeviceName = m_strDeviceName;
		return true;
	}

	return false;
}

int IGPIBDevice::GetNI488Address()
{
	return m_iNI488Address;
}

int IGPIBDevice::GetPrimaryAddress()
{
	return m_iPAD;
}

int IGPIBDevice::GetSecondaryAddress()
{
	return m_iSAD;
}


bool IGPIBDevice::GPIB_OPComplete(std::string strCommand)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);
	char szRead[256];
	memset(szRead, 0, 256);

	if (sprintf_s(szCmd, 4096, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iDD == -1)
		return false;

	ibwrt(m_iDD, (void*)szCmd, strlen(szCmd));

	CmdLog(strCommand);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		return false;
	}

	int iRetry = 100;
	do
	{
		ibrd(m_iDD, (void*)szRead, 15);
		if (ibsta & END)
			break;

		--iRetry;
		Sleep(100);
	} while (iRetry > 0);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		return false;
	}

	szRead[ibcntl] = '\0';
	if (szRead[ibcntl - 1] == '\n')
	{
		szRead[ibcntl - 1] = '\0';
	}

	CmdLog(std::string(szRead));

	if (strstr(szRead, "1") == NULL)
		return false;

	return true;
}

bool IGPIBDevice::GPIB_Query(std::string strCommand, std::string& strResponse)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);

	if (sprintf_s(szCmd, 4096, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iDD == -1)
		return false;

	ibwrt(m_iDD, (void*)szCmd, strlen(szCmd));

	CmdLog(strCommand);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		return false;
	}

	char* szRead = new char [524288];
	memset(szRead, 0, 524288);

	int iRetry = 100;
	do
	{
		ibrd(m_iDD, (void*)szRead, 524287);
		if (ibsta & END)
			break;

		--iRetry;
		Sleep(100);
	} while (iRetry > 0);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		delete [] szRead;
		return false;
	}

	szRead[ibcntl] = '\0';
	if (szRead[ibcntl - 1] == '\n')
	{
		szRead[ibcntl - 1] = '\0';
	}

	strResponse = szRead;
	CmdLog(strResponse);

	delete [] szRead;
	return true;
}

bool IGPIBDevice::GPIB_Query_BYTE(std::string strCommand, BYTE* pResponse, int& iSize)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);

	if (sprintf_s(szCmd, 4096, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iDD == -1)
		return false;

	ibwrt(m_iDD, (void*)szCmd, strlen(szCmd));

	CmdLog(strCommand);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		return false;
	}

	char *szRead = new char[524288];
	memset(szRead, 0, 524288);

	int iRetry = 100;
	do
	{
		ibrd(m_iDD, (void*)szRead, 524287);
		if (ibsta & END)
			break;

		--iRetry;
		Sleep(100);
	} while (iRetry > 0);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		delete [] szRead;
		return false;
	}

	szRead[ibcntl] = '\0';
	if (szRead[ibcntl - 1] == '\n')
	{
		szRead[ibcntl - 1] = '\0';
	}

	CmdLog(std::string(szRead));

	memset(pResponse, 0, iSize);
	if (iSize >= ibcntl)
		iSize = ibcntl;

	memcpy(pResponse, szRead, iSize);

	delete [] szRead;
	return true;
}

bool IGPIBDevice::GPIB_Write(std::string strCommand)
{
	char szCmd[4096];
	memset(szCmd, 0, 4096);

	if (sprintf_s(szCmd, 4096, "%s", strCommand.c_str()) == -1)
		return false;

	if (m_iDD == -1)
		return false;

	ibwrt(m_iDD, (void*)szCmd, strlen(szCmd));

	CmdLog(strCommand);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		return false;
	}

	return true;
}

bool IGPIBDevice::GPIB_Read(std::string& strResponse)
{
	if (m_iDD == -1)
		return false;

	char* szRead = new char[524288];
	memset(szRead, 0, 524288);

	int iRetry = 100;
	do
	{
		ibrd(m_iDD, (void*)szRead, 524287);
		if (ibsta & END)
			break;

		--iRetry;
		Sleep(100);
	} while (iRetry > 0);

	if (ibsta & ERR)
	{
		CmdLog(std::string("Error encountered"));
		delete [] szRead;
		return false;
	}

	szRead[ibcntl] = '\0';
	if (szRead[ibcntl - 1] == '\n')
	{
		szRead[ibcntl - 1] = '\0';
	}

	strResponse = szRead;
	CmdLog(strResponse);

	delete [] szRead;
	return true;
}
