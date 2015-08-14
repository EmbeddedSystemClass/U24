#include "stdafx.h"
#include "..\CMDModuleExport.h"

namespace Berlin
{
	namespace DiagCmd
    {
        /*
        // SAMPLE CODE 1 :: Qualcomm DIAG command Sample {
        //

        // TODO : 1. MODIFY API NAME, SAMPLE to YOUR API NAME.
		CMDMODULEEXPORT_API bool DiagCmd_SAMPLE_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szResponse, char* szErrMsg)
        {
            // TODO : 2. PUT YOUR COMMAND HERE.
            BYTE Cmd[] = {0x4B, 0x0F, 0x0, 0x0} ;       // 0x4B (hex) or 75 (dec) is fine.
            unsigned int iTimeOut = 5000 ;


			CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

			bool bExeRet, bCheckRet = false;

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				CCmdPtr CmdObj = new CQualcommCmdContainer(Cmd, sizeof Cmd, true, iTimeOut);

                if (! CmdModule.ExeDiag(CmdObj) )
                    return false ;
                
                bExeRet = Cmd[0] == (const BYTE)*(CmdObj->GetResponse()+0) &&
                          Cmd[1] == (const BYTE)*(CmdObj->GetResponse()+1) ;
                if (! bExeRet ) {
                    _stprintf(szErrMsg, _T("ERROR: Resp. not begin with 0x%02X 0x%02X (0x%02X 0x%02X)"),
                              Cmd[0], Cmd[1], (const BYTE)*(CmdObj->GetResponse()), (const BYTE)*(CmdObj->GetResponse()+1) ) ;
                    // keep retry.
                }

				if ( bExeRet )
                {
                    PrintDiagCmd(CmdObj->GetResponse(), CmdObj->GetResponseSize(), szResponse, ",") ;

                    // TODO : 3. CHECK RESULT HERE.
                    bCheckRet = true ;


                    break ;
                }

				::Sleep(atoi(szRetryPeriod)) ;
			}

			return bExeRet && bCheckRet ;
        } // DiagCmd_SAMPLE_SPU }
        // TODO : 4. MODIFY HEADER FILE.
        // TODO : 5. COPY and REWRITE SCRIPT.

        //
        // SAMPLE CODE 1 :: Qualcomm DIAG command Sample  }
        */


        CMDMODULEEXPORT_API bool DiagCmd_Common_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szRequest, char* szResponse, char* szErrMsg)
        {
            CString cstrCMD = szRequest ;
            TCHAR szDelimiter[] = _T(" ") ;

            int iPos = 0 ;
            unsigned int iReqCmdSize = 0 ;
            bool bIsHEX = true ;

            TCHAR szTmp[MAX_PATH] ;
			TCHAR szTmpALL[MAX_PATH];
            ::ZeroMemory(szTmp, MAX_PATH) ;
			::ZeroMemory(szTmpALL, MAX_PATH) ;

            // Determine HEX or DEC format.
            if ( -1 == cstrCMD.Find( _T("0x") ) )
                bIsHEX = false ;

            cstrCMD.Replace( _T("0x"), _T("") ) ;

            // Check and Re-format raw data.
            for ( unsigned int i = 0; ; ++i)
            {
                int iPosTmp = iPos ;
                int iSize = cstrCMD.Tokenize(" ", iPosTmp).GetLength() ;
                if ( -1 == iPosTmp )
                    break ;

                if ( 0 > iSize || iSize > 2 ) {
                    _stprintf(szErrMsg, _T("ERROR: Wrong Command") ) ;
                    return false ;
                }
                else if ( 1 == iSize && bIsHEX )
                    _stprintf(szTmpALL, _T("%s%s0%s"), szTmp, szDelimiter, cstrCMD.Tokenize(" ", iPos).GetBuffer() ) ;
                else if ( 2 == iSize && bIsHEX )
                    _stprintf(szTmpALL, _T("%s%s%s"),  szTmp, szDelimiter, cstrCMD.Tokenize(" ", iPos).GetBuffer() ) ;
                else if ( 0 == iSize )
                    continue ;
                else {
                    _stprintf(szTmpALL, _T("%s%s%02X"),  szTmp, szDelimiter, atoi(cstrCMD.Tokenize(" ", iPos).GetBuffer()) ) ;
                }

                ++iReqCmdSize ;
            }


            // Covert HEX to BYTE array.
            cstrCMD = szTmpALL ;
            cstrCMD.Replace( szDelimiter, _T("") ) ;
            BYTE* szReqCmd = new BYTE[iReqCmdSize] ;
            ::ZeroMemory(szReqCmd, iReqCmdSize) ;
            CommonFunction::HexStringToByteArray(cstrCMD.GetBuffer(), szReqCmd, iReqCmdSize) ;

            CQualcommCmdModule CmdModule(atoi(szPort), 1 == atoi(szUseQPST));

            if (! CmdModule.IsConnected() ) {
                _stprintf(szErrMsg, _T("ERROR: Check DUT connection") ) ;
                return false ;
            }

            bool bExeRet, bCheckRet = false;

            for(int i = 0;i < atoi(szRetryTime);i++)
            {
                CCmdPtr CmdObj = new CQualcommCmdContainer(szReqCmd, sizeof szReqCmd, true, 5000);

                CmdModule.ExeDiag(CmdObj) ;

                // Check Response data.
                bExeRet = szReqCmd[0] == (const BYTE)*(CmdObj->GetResponse()+0) &&
                          szReqCmd[1] == (const BYTE)*(CmdObj->GetResponse()+1) ;
                if (! bExeRet ) {
                    _stprintf(szErrMsg, _T("ERROR: Resp. not begin with 0x%02X 0x%02X (0x%02X 0x%02X)"),
                              szReqCmd[0], szReqCmd[1], (const BYTE)*(CmdObj->GetResponse()), (const BYTE)*(CmdObj->GetResponse()+1) ) ;
                    // keep retry.
                }

                // Format response Data.
                PrintDiagCmd(CmdObj->GetResponse(), CmdObj->GetResponseSize(), szResponse, ",") ;

                if ( bExeRet )
                    break ;

                ::Sleep(atoi(szRetryPeriod)) ;
            }

            return bExeRet ;
        } // DiagCmd_Common_SPU }


        CMDMODULEEXPORT_API bool DiagCmd_GetSpecifyByte_SPU (const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char* szRequestWithIdx, char* szResponse, char* szErrMsg)
        {
            // Parameters :
            // - szRequestWithIdx [input] : format as "DIAG_COMMAND;WANTED_BYTE_INDEX"
            //                              ex. "0x4B 0x0F 0x00 0x0; 9" or "75 15 0 0 ;2"
            //                              DIAG_COMMAND is fine to use HEX or DEC, and WANTED_BYTE_INDEX count form 1.
            // - szResponse       [output]: return WANTED_BYTE_INDEX-th byte as HEX. (like 0x01)
            //

            CString cstrReq = szRequestWithIdx ;
            if ( -1== cstrReq.Find( _T(";"), 0) ) {
                _stprintf(szErrMsg, _T("ERROR: Request didnot contain the wanted byte index.") ) ;
                return false ;
            }

            int iPos = cstrReq.Find( _T(";"), 0) + 1 ;
            int iByteIdx = atoi( cstrReq.Tokenize(",", iPos) ) - 1 ;
            cstrReq = cstrReq.Left( cstrReq.Find( _T(";"), 0) ) ;
                
            if (! DiagCmd_Common_SPU(szPort, szUseQPST, szRetryTime, szRetryPeriod, cstrReq.GetBuffer(), szResponse, szErrMsg) )
                return false ;

            CString cstrTmp = szResponse ;
            iPos = 0 ;
            for (int i = 0; i < iByteIdx; ++i ) {
                cstrTmp.Tokenize(",", iPos) ;
            }

            _stprintf(szResponse, _T("%s"), cstrTmp.Tokenize(",", iPos).GetBuffer() ) ;

            return true ;

        } // DiagCmd_GetSpecifyByte_SPU }
        

        //////////////////////////////////////////////////////////////////////////
        //  ADD YOUR NEW API HERE
        //////////////////////////////////////////////////////////////////////////

	}
}