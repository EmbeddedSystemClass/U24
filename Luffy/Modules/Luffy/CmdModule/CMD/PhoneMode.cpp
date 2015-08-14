#include "stdafx.h"
#include "..\CMDModuleExport.h"

bool PhoneModeCommonMethod(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, BYTE *Cmd, UINT CmdSize, UINT CmdTimeOut = 5000)
{
	CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

	bool ret, success = false;

	for(int i = 0;i < atoi(szRetryTime);i++)
	{
		CCmdPtr CmdObj = new CQualcommCmdContainer(Cmd, CmdSize, true, CmdTimeOut);

		ret = CmdModule.ExeDiag(CmdObj);

		success = (const BYTE)*(CmdObj->GetResponse()) == 0x29;

		if( success && ret )
			break;

		::Sleep(atoi(szRetryPeriod));
	}

	return success && ret;
}

namespace Berlin
{
	namespace PhoneMode
    {
        //////////////////////////////////////////////////////////////////////////
        // via QLib
        //////////////////////////////////////////////////////////////////////////
        CMDMODULEEXPORT_API bool PhoneMode_QMSL_IsConnect_SPU ( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod )
        {
            CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

            ::Sleep(500);

            bool ret = false;

            for(int i = 0;i < atoi(szRetryTime);i++)
            {
                ret = CmdModule.IsConnected();

                if( ret )
                    break;

                ::Sleep(atoi(szRetryPeriod));
            }

            return ret;
        }

        CMDMODULEEXPORT_API bool PhoneMode_QMSL_SetMode_SPU ( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* chMode )
        {
            CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

            ::Sleep(500);

            bool ret = false ;
            int  iMode = atoi(chMode) ;

            for(int i = 0;i < atoi(szRetryTime);i++)
            {
                ret = CmdModule.SetPhoneMode(iMode);

                if( ret )
                    break ;

                ::Sleep(atoi(szRetryPeriod));
            }

            return ret;
        }

        CMDMODULEEXPORT_API bool PhoneMode_QMSL_GetMode_SPU ( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szModeType)
        {
            CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

            ::Sleep(500);

            bool ret = false;

            for(int i = 0;i < atoi(szRetryTime);i++)
            {
                unsigned long iMode = 999 ;
                ret = CmdModule.GetPhoneMode(&iMode);

                if (ret)
                {
                    switch(iMode)
                    {
                    case 0:
                        sprintf (szModeType, "%s", _T("PWROFF"));
                        break;
                    case 1:
                        sprintf (szModeType, "%s", _T("FTM Mode"));
                        break;
                    case 2:
                        sprintf (szModeType, "%s", _T("Offline Mode"));
                        break;
                    case 3:
                        sprintf (szModeType, "%s", _T("Offline_AMPS Mode"));
                        break;
                    case 4:
                        sprintf (szModeType, "%s", _T("Offline_CDMA Mode"));
                        break;
                    case 5:
                        sprintf (szModeType, "%s", _T("Online Mode"));
                        break;
                    case 6:
                        sprintf (szModeType, "%s", _T("Low Power Mode"));
                        break;
                    case 7:
                        sprintf (szModeType, "%s", _T("Reset Mode"));
                        break;
                    case 8:
                        sprintf (szModeType, "%s", _T("NET_TEST_GW Mode"));
                        break;
                    case 13:
                        sprintf (szModeType, "%s", _T("APQ no response"));
                        break;
                    default:
                        sprintf (szModeType, "%s", _T("Unknown Mode"));
                    }

                    break;
                }

                ::Sleep(atoi(szRetryPeriod));
            }

            return ret;
        }

        CMDMODULEEXPORT_API bool PhoneMode_QMSL_SetBootMode_SPU ( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* bFTM, char* bReset )
        {
            CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

            ::Sleep(500);

            bool ret = false ;
            unsigned char bIsSetFTM = atoi(bFTM) ;
            unsigned char bIsReset = atoi(bReset) ;

            for(int i = 0;i < atoi(szRetryTime);i++)
            {
                ret = CmdModule.SetBootMode(bIsSetFTM, bIsReset) ;

                if( ret )
                    break ;

                ::Sleep(atoi(szRetryPeriod));
            }

            return ret;
        }


        //////////////////////////////////////////////////////////////////////////
        // via Diag CMD
        //////////////////////////////////////////////////////////////////////////
		CMDMODULEEXPORT_API bool PhoneMode_Diag_SetOffline_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod)
		{
			BYTE Cmd[] = {0x29, 0x01, 0x00};

			return PhoneModeCommonMethod(szPort, szUseQPST, szRetryTime, szRetryPeriod, Cmd, sizeof Cmd, 10000);
		}

		CMDMODULEEXPORT_API bool PhoneMode_Diag_SetFTM_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod)
		{
			BYTE Cmd[] = {0x29, 0x03, 0x00};

			return PhoneModeCommonMethod(szPort, szUseQPST, szRetryTime, szRetryPeriod, Cmd, sizeof Cmd, 10000);
		}

		CMDMODULEEXPORT_API bool PhoneMode_Diag_SetOnline_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod)
		{
			BYTE Cmd[] = {0x29, 0x04, 0x00};

			return PhoneModeCommonMethod(szPort, szUseQPST, szRetryTime, szRetryPeriod, Cmd, sizeof Cmd, 10000);
		}

		CMDMODULEEXPORT_API bool PhoneMode_Diag_SetLPM_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod)
		{
			BYTE Cmd[] = {0x29, 0x05, 0x00};

			return PhoneModeCommonMethod(szPort, szUseQPST, szRetryTime, szRetryPeriod, Cmd, sizeof Cmd, 10000);
		}

		CMDMODULEEXPORT_API bool PhoneMode_Diag_OfflineReset_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod)
		{
			BYTE Cmd[] = {0x29, 0x02, 0x00};

			return PhoneModeCommonMethod(szPort, szUseQPST, szRetryTime, szRetryPeriod, Cmd, sizeof Cmd, 10000);
		}

		CMDMODULEEXPORT_API bool PhoneMode_Diag_GetMode_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szModeType)
		{
			BYTE Cmd[] = {75, 15, 0, 0};

			CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

			bool ret, success = false;

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				CCmdPtr CmdObj = new CQualcommCmdContainer(Cmd, sizeof Cmd, true, 10000);

				ret = CmdModule.ExeDiag(CmdObj);

				success = (const BYTE)*(CmdObj->GetResponse()) == 0x4B;

				if( success && ret )
				{
					switch((const BYTE)*(CmdObj->GetResponse() + 8))
					{
					case 0:
						sprintf (szModeType, "%s", _T("PWROFF"));
						break;

					case 1:
						sprintf (szModeType, "%s", _T("FTM Mode"));
						break;

					case 2:
						sprintf (szModeType, "%s", _T("Offline Mode"));
						break;

					case 3:
						sprintf (szModeType, "%s", _T("Offline_AMPS Mode"));
						break;

					case 4:
						sprintf (szModeType, "%s", _T("Offline_CDMA Mode"));
						break;

					case 5:
						sprintf (szModeType, "%s", _T("Online Mode"));
						break;

					case 6:
						sprintf (szModeType, "%s", _T("Low Power Mode"));
						break;

					case 7:
						sprintf (szModeType, "%s", _T("Reset Mode"));
						break;

					case 8:
						sprintf (szModeType, "%s", _T("NET_TEST_GW Mode"));
                        break;

                    case 13:
                        sprintf (szModeType, "%s", _T("APQ no response"));
                        break;

					default:
						sprintf (szModeType, "%s", _T("Unknown Mode"));
					}

					break;
				}

				::Sleep(atoi(szRetryPeriod));
			}

			return success && ret;
		}
	}
}