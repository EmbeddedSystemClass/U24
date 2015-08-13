#ifndef __DL_EXPORT__H__
#define __DL_EXPORT__H__

#include "..\..\Modules\QisdaQualcommDownloadModule\Download.h"

#pragma pack(push, 1)
struct tagDLSetting7x27
{
	IMAGE szHex;
	IMAGE szPartition;
	IMAGE szQcsbl;
	IMAGE szQcsblhd;
	IMAGE szOemsbl; 
	IMAGE szOemsblhd;
	IMAGE szAmss; 
	IMAGE szAmsshd; 
	IMAGE szAppsboot;
	IMAGE szAppsboothd;
	IMAGE szCefs;
	char szBoot[256];
	char szSystem[256];
	char szUserdata[256];
};
#pragma pack(pop)

#endif //__DL_EXPORT__H__