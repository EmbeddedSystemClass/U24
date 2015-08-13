/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: fastboot mode read/write FA.
 * FileName   : FastbootRWFA.h
 * Version    : $Revision:   1.0  $
 * Author     : Alex.Chen
 * Time       : 2011/06/13
 * History    :
 *  NO   Date            Updated by        Content of change
 *  1    2011/06/13      Alex.Chen         Create.
 ********************************************************************************/

#pragma once

#pragma warning(disable:4996)
#pragma warning(disable:4482)

#ifdef FASTBOOTRWFA_EXPORTS
#define FASTBOOTRWFA_API extern "C" __declspec(dllexport)
#else
#define FASTBOOTRWFA_API extern "C" __declspec(dllimport)
#endif

#include "ADBDevice.h"

#define  FALENGTH  512


FASTBOOTRWFA_API bool  ReadFA (char* sz_port, unsigned char* sz_FAData, char* sz_errorCode, char* sz_errorMsg);
FASTBOOTRWFA_API bool  WriteFA(char* sz_port, unsigned char* sz_FAData, char* sz_errorCode, char* sz_errorMsg);
