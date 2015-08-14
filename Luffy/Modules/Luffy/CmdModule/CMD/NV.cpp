#include "stdafx.h"
#include "..\CMDModuleExport.h"

namespace Berlin
{
	namespace NV
	{
		CMDMODULEEXPORT_API bool NV_ReadNV453_SPU(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* chOnlineMode)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

			::Sleep(1000);

			bool ret;
			BYTE m_bTempOnlineMode[128] = {0};

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				ret = CmdModule.ReadNVItem(453, 453, m_bTempOnlineMode);

				if( ret )
				{
					sprintf (chOnlineMode, "%d", m_bTempOnlineMode[0]);
					break;
				}
				else
					return false;

				::Sleep(atoi(szRetryPeriod));
			}
			
			return ret;
		} // Read NV453:To check Phone is online mode or not(FTM mode)!


		CMDMODULEEXPORT_API bool NV_ReadItem(const char* szPort, const char *szUseQPST,  const char* szRetryTime, const char* szRetryPeriod, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

			::Sleep(1000);

			bool ret = false;			
			BYTE m_bTempOnlineMode[128] = {0};

			for(int i = 0;i < atoi(szRetryTime);i++)
 			{
				ret = CmdModule.ReadNVItem(NVItem, NVItem, m_bTempOnlineMode);

				if( ret )
				{
					memcpy (pBuffer, &m_bTempOnlineMode, 128);
					break;
				}

				::Sleep(atoi(szRetryPeriod));
			}

			return ret;
		}

		CMDMODULEEXPORT_API bool NV_WriteNV453_SPU(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* chOnlineMode)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

			::Sleep(1000);

			bool ret;
			BYTE bWriteOnlineMode = (BYTE)atoi(chOnlineMode);

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				ret = CmdModule.WriteNVItem(453, 453, &bWriteOnlineMode);

				if( ret )
				{
					break;
				}

				::Sleep(atoi(szRetryPeriod));
			}
			
			return ret;
		} // Write NV453:To set Phone mode into online mode!

		CMDMODULEEXPORT_API bool NV_WriteItem(const char* szPort, const char *szUseQPST,  const char* szRetryTime, const char* szRetryPeriod, const WORD NVItem, BYTE* pBuffer, long BufferSize, WORD *pnv_stat)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );
	
			::Sleep(1000);

			bool ret = false ;
			//BYTE bWriteOnlineMode = (BYTE)pnv_stat;

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				ret = CmdModule.WriteNVItem(NVItem, NVItem, pBuffer);

				if( ret )
				{
					break;
				}

				::Sleep(atoi(szRetryPeriod));
			}

			return ret;
		}


		CMDMODULEEXPORT_API bool NV_WriteItem_Radix_SPU (const char* szPort, const char *szUseQPST, const char* NVItem, char* pBuffer, char* pradix, char* BufferSize, char *pnv_stat)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

			::Sleep(1000);

			int size = atoi(BufferSize);
			BYTE *pWriteData = new BYTE [size];

			if (! pWriteData)
				return false;

			::ZeroMemory(pWriteData, size);

			int iRadix = atoi(pradix);

			if((iRadix >= 2) && (iRadix <=36 ) && (iRadix != 1))
			{//MSDN: 2 <= iRadax <= 36, iRadix = 0 : base = 10
				*pWriteData = (BYTE)strtoul(pBuffer, NULL, iRadix);
			}
			else
			{
				delete [] pWriteData;
				return false;
			}
		
			//WORD nvstatus;
			BYTE bnvstatus = (BYTE)atoi(pnv_stat);

			bool ret = CmdModule.WriteNVItem(atoi(NVItem), atoi(NVItem), &bnvstatus);

			if (pnv_stat)
				wsprintf (pnv_stat, "%d", bnvstatus);

			delete [] pWriteData;
			return ret;
		}

		CMDMODULEEXPORT_API bool NV_ReadIMEI_SPU(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char returnIMEI[16])
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
		} // NV_ReadIMEI_SPU
	}
}