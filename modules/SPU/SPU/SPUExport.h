#ifndef SPU_EXPORTS_H_
#define SPU_EXPORTS_H_

#ifdef SPU_EXPORTS
#define SPU_API extern "C" __declspec(dllexport)
#define SPU_CLASS_API __declspec(dllexport)
#else
#define SPU_API extern "C" __declspec(dllimport)
#define SPU_CLASS_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include "ISPU_CALLBACK.h"

SPU_API SPU_RESULT ExecuateSPUScript(const TSTRING& strScriptFile,LPISPU_CALLBACK pISPU_CALLBACK);
SPU_API SPU_RESULT Load(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK);
SPU_API SPU_RESULT LoadEx(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK,const TSTRING& strScriptFile);

#endif //#define SPU_EXPORTS_H_