#include "stdafx.h"
#include <cstddef> // size_t

#include "../CmdType.h"
#include "GeneralDLLApi.h" // GeneraCmd
#include "SapporoDLLApi.h"
#include "io.h"
#include "../Utility.h"
#include "../FactoryBlock.h"


static const int CMD_MAX_RETRY = 1;

static int MobileCmd(int port, int timeout_ms, byte ftd_cmd_main, byte ftd_cmd_sub,
					 char* in_buf, char* out_buf, const char* func_name)
{
	byte ftd_cmd[4] = {ftd_cmd_main, ftd_cmd_sub, 0, 0};
	int ret_code = -1*ERROR_FUNCTION_FAILED;
	for (int nTry = 0; nTry < CMD_MAX_RETRY; nTry++) {
		ret_code = GeneralCmdInOut(port, timeout_ms, ftd_cmd, in_buf, strlen(in_buf), out_buf);
		TRACE(_T("%s(%d)\n"), func_name, ret_code);
		if (ret_code == HS_OK) {
			break;
		}
		Sleep(500);
	}
	return ret_code;
}

//05:audio
NEPDLLAPI int __stdcall HeadsetButtonCount(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_AU, 14, in_buf, out_buf, __FUNCTION__);
}

//22:SmartCover
NEPDLLAPI int __stdcall Sapporo_SCoverDetect(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_SCOVER, 1, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Sapporo_SCoverInterrupt(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_SCOVER, 2, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Sapporo_SCoverEnable3V(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_SCOVER, 3, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Sapporo_SCoverCheckI2C(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_SCOVER, 4, in_buf, out_buf, __FUNCTION__);
}

NEPDLLAPI int __stdcall Sapporo_SCoverSwitch5V(int port, int timeout_ms, char* in_buf, char* out_buf)
{

	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_SCOVER, 5, in_buf, out_buf, __FUNCTION__);
}

//92 factory

NEPDLLAPI int __stdcall Sapporo_FAC_FSGProtect(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_FAC, 9, in_buf, out_buf, __FUNCTION__);
}

//93

NEPDLLAPI int __stdcall Sapporo_ftdStartEnd(int port, int timeout_ms, char* in_buf, char* out_buf)
{
	TRACER_FUNCTION_IN();
	return MobileCmd(port, timeout_ms, Sapporo_FT_MISC, 1, in_buf, out_buf, __FUNCTION__);
}


//Special Control Functions

NEPDLLAPI int __stdcall Sapporo_CheckDevelopMode(char* output, char* error_code)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	if(ExecAdbOut("pull /usr/bin/fingerterm dev_check.bin", output, error_code)) {
		if(strstr(output, "does not exist") != NULL) {
			ret_code = HS_OK;
		}
		else {
			strcpy(error_code, "ERROR: It's in develop mode");
			ret_code = -1 * ERROR_BAD_PROFILE;
		}
	}
	else {
		ret_code = -1*ERROR_BAD_COMMAND;
	}
	return ret_code;
}

NEPDLLAPI int __stdcall Sapporo_ReadSWversion(char* output, char* error_code)
{
	TRACER_FUNCTION_IN();

	int ret_code = -1 * ERROR_FUNCTION_FAILED;
	if(ExecAdbOut("shell cat /etc/os-release", output, error_code)) {
		if(strstr(output, "VERSION_ID") != NULL) {
			ret_code = HS_OK;
		}
		else {
			strcpy(error_code, "ERROR: No \"VERSION_ID\" exist");
			ret_code = -1 * ERROR_BAD_PROFILE;
		}
	}
	else {
		strcpy(error_code, "ERROR: Exec \"shell cat /etc/os-release\" fail");
		ret_code = -1*ERROR_BAD_COMMAND;
	}

	if ( ret_code == HS_OK )
	{
		CString version(output);
		version = version.Left(version.Find(_T("PRETTY_NAME")));
		version = version.Right(version.GetLength() - version.Find(_T("VERSION_ID=")) - 11);
		version.Trim();
		strcpy(output, version);
	}
	return ret_code;
}

//Special Control Functions SPU

NEPDLLAPI int Sapporo_CheckDevelopMode_SPU(char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return Sapporo_CheckDevelopMode(output, error_code);
}

NEPDLLAPI int Sapporo_ReadSWversion_SPU(char* output, char* error_code)
{
	TRACER_FUNCTION_IN();
	return Sapporo_ReadSWversion(output, error_code);
}