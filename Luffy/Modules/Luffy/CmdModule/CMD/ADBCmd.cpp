#include "stdafx.h"
#include "stdlib.h"
#include "string"
#include <Windows.h>
#include "..\CmdModuleExport.h"

namespace Berlin
{
	namespace ADBCmd
	{
        /*
        // SAMPLE CODE 1 :: ADB with RESPONSE {
        //

        // TODO : 1. MODIFY API NAME, SAMPLE_HasReturn to YOUR API NAME.
        CMDMODULEEXPORT_API bool ADBCmd_SAMPLE_HasReturn_SPU(const char* szRetryTime, const char* szRetryPeriod, char* szResponse, char* szErrMsg)
        {
            // TODO : 2. PUT YOUR COMMAND HERE.
            char Cmd[] = "PUT_YOUR_CMD_HERE" ;
            UINT iTimeout = 5000 ;                      // or INFINITE
            bool bCheckDevice = true ;                  // Should check device exist first ?
            ADBModuleExeType myexeType = exeType_ADB ;  // execute as : exeType_ADB or exeType_FASTBOOT or exeType_SYS ?


            CADBCmdModule CmdModule ;
            CmdModule.ShouldConnectDevice(bCheckDevice) ;
            CmdModule.SetExecuteType(myexeType) ;

            bool bRet, bExeRet = false ;
            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)Cmd, sizeof Cmd, true, iTimeout) ;

                bExeRet = CmdModule.ExeDiag(CmdObj) ;

                if (bExeRet)
                {
                    std::string strBuffer( (char*)CmdObj->GetResponse() ) ;

                    sprintf(szResponse, _T("%s"), strBuffer.c_str()) ;

                    // TODO : 3. CHECK RESULT HERE.
                    bRet = true ;
                    

                    break ;
                }
                else
                    sprintf(szErrMsg, "ERROR: Fail to execute \"%s\"", CmdObj->GetCmd()) ;

                Sleep(atoi(szRetryPeriod));
            }

            return bRet && bExeRet ;
        } // ADBCmd_SAMPLE_HasReturn_SPU }
        // TODO : 4. MODIFY HEADER FILE.
        // TODO : 5. COPY and REWRITE SCRIPT.

        //
        // SAMPLE CODE 1 :: ADB with RESPONSE }
        */


        /*
        // SAMPLE CODE 2 :: ADB without RESPONSE {
        //

        // TODO : 1. MODIFY API NAME, SAMPLE to YOUR API NAME.
        CMDMODULEEXPORT_API bool ADBCmd_SAMPLE_SPU(const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg)
        {
            // TODO : 2. PUT YOUR COMMAND HERE.
            char Cmd[] = "PUT_YOUR_CMD_HERE" ;
            UINT iTimeout = 5000 ;                    // or INFINITE
            bool bCheckDevice = true ;                // Should check device exist first ?
            ADBModuleExeType exeType = exeType_ADB ;  // execute as : exeType_ADB or exeType_FASTBOOT or exeType_SYS ?


            CADBCmdModule CmdModule ;
            CmdModule.ShouldConnectDevice(bCheckDevice) ;
            CmdModule.SetExecuteType(exeType) ;

            bool bRet = false ;
            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)Cmd, sizeof Cmd, false, iTimeout) ;

                bRet = CmdModule.ExeDiag(CmdObj) ;

                if (bRet)
                    break ;
                else
                    sprintf(szErrMsg, "ERROR: Fail to execute \"%s\"", CmdObj->GetCmd()) ;

                Sleep(atoi(szRetryPeriod));
            }

            return bRet ;
        } // ADBCmd_SAMPLE_SPU }
        // TODO : 3. MODIFY HEADER FILE, AND UPPER LINE.
        // TODO : 4. COPY and REWRITE SCRIPT.
        
        //
        // SAMPLE CODE 2 :: ADB without RESPONSE }
        */


        /*
        // SAMPLE CODE 3 :: FASTBOOT without RESPONSE {
        //

        // TODO : 1. MODIFY API NAME, FASTBOOTSAMPLE to YOUR API NAME.
        CMDMODULEEXPORT_API bool ADBCmd_FASTBOOTSAMPLE_SPU(const char* szRetryTime, const char* szRetryPeriod, char* szErrMsg)
        {
            // TODO : 2. PUT YOUR COMMAND HERE.
            char Cmd[] = "PUT_YOUR_CMD_HERE" ;
            UINT iTimeout = 5000 ;                          // or INFINITE
            bool bCheckDevice = true ;                      // Should check device exist first ?
            ADBModuleExeType exeType = exeType_FASTBOOT ;   // execute as : exeType_ADB or exeType_FASTBOOT or exeType_SYS ?


            CADBCmdModule CmdModule ;
            CmdModule.ShouldConnectDevice(bCheckDevice) ;
            CmdModule.SetExecuteType(exeType) ;

            bool bRet = false ;
            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)Cmd, sizeof Cmd, false, iTimeout) ;

                bRet = CmdModule.ExeDiag(CmdObj) ;

                if (bRet)
                    break ;
                else
                    sprintf(szErrMsg, "ERROR: Fail to execute \"%s\"", CmdObj->GetCmd()) ;

                Sleep(atoi(szRetryPeriod));
            }

            return bRet ;
        } // ADBCmd_SAMPLE_SPU }
        // TODO : 3. MODIFY HEADER FILE, AND UPPER LINE.
        // TODO : 4. COPY and REWRITE SCRIPT.
        
        //
        // SAMPLE CODE 3 :: FASTBOOT without RESPONSE }
        */



		CMDMODULEEXPORT_API bool ADBCmd_Devices_SPU(char* szRetryTime, char* szRetryPeriod, char* szResponse) 
		{
			bool ret = false;

			char szADBcmd[] = "devices";

			CADBCmdModule CmdModule ;
            CmdModule.ShouldConnectDevice(false) ;

			for (int i = 0; i < atoi(szRetryTime); ++i)
			{
				CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)szADBcmd, sizeof szADBcmd, true, 5000);

				ret = CmdModule.ExeDiag(CmdObj) ;

                if (ret) {
                    std::string strBuffer = (char*)(CmdObj->GetResponse()) ;
                    sprintf(szResponse, _T("%s"), strBuffer.c_str()) ;

					break;
                }

				Sleep(atoi(szRetryPeriod));
			}
			return ret;
		}  // ADBCmd_Devices_SPU }


        CMDMODULEEXPORT_API bool ADBCmd_Common_SPU(char* szRetryTime, char* szRetryPeriod, char* szADBCmd, char* szResponse, char* szErrMsg) 
        {
            bool ret = false ;

            CADBCmdModule CmdModule ;
            int iLen = strlen(szADBCmd) + 1 ;

            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)szADBCmd, iLen, true, 5000);

                ret = CmdModule.ExeDiag(CmdObj);

                if (ret)
                {
                    std::string strBuffer = (char*)(CmdObj->GetResponse()) ;

                    sprintf(szResponse, _T("%s"), strBuffer.c_str()) ;

                    break;
                }

                Sleep(atoi(szRetryPeriod));
            }
            return ret;
        }  // ADBCmd_Common_SPU }


        CMDMODULEEXPORT_API bool ADBCmd_Common_NoResponse_SPU(char* szRetryTime, char* szRetryPeriod, char* szADBCmd, char* szErrMsg) 
        {
            bool ret = false ;

            CADBCmdModule CmdModule ;
            int iLen = strlen(szADBCmd) + 1 ;

            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)szADBCmd, iLen, false, 5000);

                ret = CmdModule.ExeDiag(CmdObj);

                if (ret)
                    break ;

                Sleep(atoi(szRetryPeriod));
            }

            return ret ;
        }  // ADBCmd_Common_NoResponse_SPU }


        CMDMODULEEXPORT_API bool ADBCmd_KillServer_SPU(const char* szRetryTime, const char* szRetryPeriod)
        {
            bool ret = false ;

            CADBCmdModule CmdModule ;
            CmdModule.ShouldConnectDevice(false) ;

            char szADBCmd[] = "kill-server" ;

            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)szADBCmd, sizeof szADBCmd, false, 5000);

                ret = CmdModule.ExeDiag(CmdObj);

                if (ret)
                    break;

                Sleep(atoi(szRetryPeriod));
            }

            return ret ;
        } // ADBCmd_KillServer_SPU }


        CMDMODULEEXPORT_API bool ADBCmd_Fastboot_Common_SPU(char* szRetryTime, char* szRetryPeriod, char* szFastbootCmd, char* szResponse, char* szErrMsg) 
        {
            bool ret = false ;

            CADBCmdModule CmdModule ;
            CmdModule.SetExecuteType( exeType_FASTBOOT ) ;
            CmdModule.ShouldConnectDevice(false) ;

            int iLen = strlen(szFastbootCmd) + 1 ;

            for (int i = 0; i < atoi(szRetryTime); ++i)
            {
                CCmdPtr CmdObj = new CADBCmdContainer((BYTE *)szFastbootCmd, iLen, true, 5000);

                ret = CmdModule.ExeDiag(CmdObj);

                if (ret)
                {
                    std::string strBuffer = (char*)(CmdObj->GetResponse());

                    if ( std::string::npos != strBuffer.find(_T("usage: fastboot")) ) {
                        sprintf(szErrMsg, _T("ERROR: wrong Fastboot Command"));
                        return false ;
                    }

                    sprintf(szResponse, _T("%s"), strBuffer.c_str());

                    break;
                }

                Sleep(atoi(szRetryPeriod));
            }
            return ret;
        }  // ADBCmd_Fastboot_Common_SPU }
		


        //////////////////////////////////////////////////////////////////////////
        //  ADD YOUR NEW API HERE
        //////////////////////////////////////////////////////////////////////////

	} //namespace ADBCmd }
} // namespace Berlin }