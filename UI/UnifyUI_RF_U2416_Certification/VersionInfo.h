#ifndef _SERVICE_TOOL_VERSION_INFO_H_
#define _SERVICE_TOOL_VERSION_INFO_H_



// remember to modified makefile.inc
#define VERSION_NUM   1,0,1,9
#define VERSION_STR   "1.0.1.9"

#define COMPANY_NAME "Qisda Corporation"
#define LEGAL_COPY   "(c) 2012 Qisda Corporation. All rights reserved."

#ifdef _DEBUG
#define TOOL_EXT_DEBUG " (debug version)"
#define VERSION_TYPE  "D"
#else
#define TOOL_EXT_DEBUG
#define VERSION_TYPE  "R"
#endif // _DEBUG

#define TOOL_NAME "Service Tool"
#define TOOL_VERSION VERSION_STR VERSION_TYPE


#endif //_SERVICE_TOOL_LAUNCHER_VERSION_INFO_H_
