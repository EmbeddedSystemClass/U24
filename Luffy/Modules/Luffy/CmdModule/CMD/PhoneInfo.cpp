#include "stdafx.h"
#include "..\CMDModuleExport.h"


namespace Berlin
{
	namespace PhoneInfo
	{
		CMDMODULEEXPORT_API bool FA_Read_SPU( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer )
		{
			CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

			bool ret;

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				ret = CmdModule.ReadNVItem(2497, 2500, (unsigned char *)szFABuffer);

				if( ret )
				{
					break;
				}

				::Sleep(atoi(szRetryPeriod));
			}

			return ret;
		}

	}

	CMDMODULEEXPORT_API bool FA_Write_SPU( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer )
	{
		CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

		bool ret;

		for(int i = 0;i < atoi(szRetryTime);i++)
		{
			ret = CmdModule.WriteNVItem(2497, 2500, (unsigned char *)szFABuffer);

			if( ret )
			{
				break;
			}

			::Sleep(atoi(szRetryPeriod));
		}

		return ret;
	}

	CMDMODULEEXPORT_API bool ReadIMEI_SPU(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char returnIMEI[16])
	{
		CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

		bool ret = false;
		BYTE m_cTempIMEIResp[128] = {0};
		char m_sDigitIMEI[128*2+1] = {0};

		for(int i = 0;i < atoi(szRetryTime);i++)
		{
			ret = CmdModule.ReadNVItem(550, 550, m_cTempIMEIResp);

			if( true == ret )
			{
				int m_iresult = CommonFunction::HexBcdToDigit(m_cTempIMEIResp, 128, m_sDigitIMEI, 128*2+1);
				for(int i=0; i<15; i++)
					returnIMEI[i] = m_sDigitIMEI[i+3];

				break;
			}
			else
				strcpy_s(returnIMEI, 16, _T("000000011234560"));

			::Sleep(atoi(szRetryPeriod));
		}
		returnIMEI[15]='\0';

		return ret;
	}
}