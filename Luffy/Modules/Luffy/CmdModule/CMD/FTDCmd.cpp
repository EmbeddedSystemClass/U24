#include "stdafx.h"
#include "..\CmdModuleExport.h"
#include "stdlib.h"

namespace Berlin
{
	namespace FTDCmd
    {
        /*
        // SAMPLE CODE 1 :: FTD with RESPONSE {
        //

        // TODO : 1. MODIFY API NAME, SAMPLE_HasReturn to YOUR API NAME.
        CMDMODULEEXPORT_API bool FTDCmd_SAMPLE_SPU (const char* szRetryTime, const char* szRetryPeriod, char* szResponse, char* szErrMsg)
        {
            // TODO : 2. PUT YOUR COMMAND HERE.
            char szCMD[4] = {0, 0, 0, 0} ;
            char* szExtraCmd = "" ;     // Extra Command
            UINT iTimeout = 5000 ;      // or INFINITE


            CFTDCmdModule CmdModule ;

            bool bRet = false ;
            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CFTDCmdContainer(szCMD, 4, true, iTimeout) ;
                CmdModule.SetExtraCmd(szExtraCmd) ;

                bRet = CmdModule.ExeDiag( CmdObj ) ;

                if (bRet)
                {
                    std::string strBuffer( (char*)CmdObj->GetResponse() ) ;
                    sprintf(szResponse, _T("%s"), strBuffer.c_str()) ;

                    // TODO : 3. CHECK RESULT HERE.

                    break ;
                }

                Sleep(atoi(szRetryPeriod)) ;
            }

            return bRet ;
        } // FTDCmd_SAMPLE_SPU }
        // TODO : 4. MODIFY HEADER FILE.
        // TODO : 5. COPY and REWRITE SCRIPT.

        //
        // SAMPLE CODE 1 :: FTD with RESPONSE }
        */



        CMDMODULEEXPORT_API bool FTDCmd_Common_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char *szFTDCmd, char* szOutput, char* szErrMsg)
        {
            // TODO: Identify ADB Device.
            if ( 0 == szADBDevice) {
                sprintf (szErrMsg, _T("ERROR: ADB COM Port is invalid"));
                return false;
            }

            CString cstrCMD = szFTDCmd;

            // Check CMD size
            unsigned int iSize = 1;
            int iPos = cstrCMD.Find(';');
            while ( -1 != iPos ) {
                ++iSize ;
                iPos = cstrCMD.Find(';', iPos+1);
            }
            
            if ( iSize > 5 ) {
                sprintf (szErrMsg, _T("ERROR: Wrong FTD Command Size."));
                return false ;
            }


			CFTDCmdModule CmdModule;
            const unsigned int iCmdSize = 4;
            char szCMD[iCmdSize] = {0, 0, 0, 0};

            iPos = 0 ;
            for ( unsigned int i = 0; i < iSize; ++i )
            {
				if (i == 4) // For NTC. 2->6 1 in console mode
				{
					CmdModule.SetExtraCmd( cstrCMD.Tokenize(";", iPos).GetBuffer() );
					break;
				}

                szCMD[i] = atoi( cstrCMD.Tokenize(";", iPos).GetBuffer() );
            }

            
            bool ret = false ;
            for(int i = 0;i < atoi( szRetryTime );i++)
            {
                CCmdPtr CmdObj = new CFTDCmdContainer(szCMD, iCmdSize, true, 1000);

                ret = CmdModule.ExeDiag( CmdObj ) ;

                char *chReturn = (char *) (CmdObj->GetResponse());
                sprintf (szOutput, "%s",  chReturn) ;

                if( ret )
                    break;

                ::Sleep( atoi( szRetryPeriod ) );
            }

            return ret;
        } // FTDCmd_Common }


        CMDMODULEEXPORT_API bool FTDCmd_Active_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg)
        {
            CString pthToolDir;
            ::GetModuleFileName(NULL, pthToolDir.GetBuffer(MAX_PATH), MAX_PATH);
            pthToolDir.ReleaseBuffer();
            pthToolDir = pthToolDir.Left(pthToolDir.ReverseFind('\\'));

            CString adb_path = pthToolDir + _T("\\adb.exe");
            CString param = _T("shell ftd");

            if (szADBDevice == 0)
            {
                sprintf (szErrMsg, _T("ERROR: ADB com port is invalid")) ;
                return false;
            }

            bool ret = false, success = false;

            for (int i = 0; i < atoi( szRetryTime ); ++i)
            {
                ret = Exec(adb_path, param, 5000, true);

                if (ret)
                {
                    if ( -1 != param.Find(_T("error: more than one device and emulator")) )
                    {
                        sprintf (szErrMsg, _T("More than one DUT, disconnect one DUT or assign a device SN"));
                        return false ;
                    }
                    else if ( -1 == param.Find(_T("system/core/ftd")) ) 
                    {
                        sprintf (szErrMsg, _T("Fail to enter FTD Mode"));
                        return false ;
                    }
                    else 
                    {
                        success = true;
                        break ;
                    }
                }
            }

            return ret && success;
        }  // FTDCmd_Active_SPU }

        CMDMODULEEXPORT_API bool FTDCmd_CheckIsFTD_SPU (const char* szADBDevice, const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg)
        {
            if (szADBDevice == 0)
            {
                sprintf (szErrMsg, _T("ERROR: ADB com port is invalid"));
                return false;
            }

            char *msgBuffer = new char[1024 * 1024 * 2];

            bool ret = false, success = false;

            for(int i = 0;i < atoi( szRetryTime );i++)
            {
                ret =  ExcuteADBOut("shell ps", msgBuffer, szErrMsg);

                if(ret) 
                {
                    CString outBuffer = msgBuffer;

                    if ( -1 != outBuffer.Find(_T("error: more than one device and emulator")) )
                    {
                        sprintf (szErrMsg, _T("More than one DUT, disconnect one DUT or assign a device SN")) ;
                        return false ;
                    }
                    else if ( -1 == outBuffer.Find(_T("ftd")) ) 
                    {
                        sprintf (szErrMsg, _T("Not in ftd mode")) ;
                        return false ;
                    }
                    else 
                    {
                        success = true;
                        break;
                    }
                }
            }

            return ret && success ;
        }  // FTDCmd_Active_SPU }

	} // namespace  FTDCmd }
} // namespace  Berlin }