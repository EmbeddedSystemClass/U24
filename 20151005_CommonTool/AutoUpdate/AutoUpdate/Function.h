#pragma once
#include "Clog.h"

class Function
{
public:
	TCHAR m_NewEntranceToolFolderPath[MAX_PATH];
	TCHAR m_NewEntranceToolFilePath[MAX_PATH];
	Clog m_Clog;
	Function(void);
	~Function(void);
	bool DoAutoUpdate(TCHAR*, TCHAR*);
	bool KillEntranceTool(void);
	bool UpOnEntranceTool(void);
	bool DoCopy(TCHAR*,TCHAR*);
	bool DoUnzip(TCHAR*,TCHAR*);
	bool DoMD5Check(void);
	bool FindCompressedFile(TCHAR* SearchPath,TCHAR* keyword,TCHAR* CompressedType, TCHAR* OutFileName );
	bool ExecuteEntranceTool();
	bool TraverseFolder(LPCTSTR lpPath);
};
