#include "StdAfx.h"
#include "SPUExport.h"
#include "SPU.h"

SPU_API SPU_RESULT ExecuateSPUScript(const TSTRING& strScriptFile,LPISPU_CALLBACK pISPU_CALLBACK)
{
	SPU_RESULT SPU_RES = SPU_FAIL;
	CSPU spu(pISPU_CALLBACK,strScriptFile);
	
	std::vector<TSTRING> strMemory;

	CStdioFileEx cFile;
	if(cFile.Open(strScriptFile.c_str(), CStdioFile::modeRead))
	{
		SPU_RESULT nRet = SPU_SUCCESS;
		nRet = spu.LoadCommand(cFile,strMemory) ? SPU_SUCCESS : SPU_FAIL;
		if(nRet == SPU_FAIL && pISPU_CALLBACK)
			pISPU_CALLBACK->Print(_T(""),_T("Load Scription Failed!\n"));
		cFile.Close();
	}
	return spu.Run(strMemory,false,strScriptFile);
}

SPU_API SPU_RESULT LoadEx(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptFile)
{
	SPU_RESULT SPU_RES = SPU_FAIL;
	CSPU spu(pISPU_CALLBACK,strScriptFile);
	return spu.Run(vectMemory,false,strScriptFile);
}

SPU_API SPU_RESULT Load(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK pISPU_CALLBACK)
{
	SPU_RESULT SPU_RES = SPU_FAIL;
	CSPU spu(pISPU_CALLBACK);
	return spu.Run(vectMemory,false,_T(""));
}