#include <windows.h>
class KPEFile {
  const char        * pModule;
  PIMAGE_DOS_HEADER pDOSHeader;
  PIMAGE_NT_HEADERS pNTHeader;

public:
  // 將 RVA 位址轉成 virtual address
  const char * RVA2Ptr(unsigned rva) {
    if ( (pModule!=NULL) && rva )
      return pModule + rva;
    else
      return NULL;
  }

  // 建構子: 取得 DOS Header 與 NTHeader
  KPEFile(HMODULE hModule);
  
  // 傳回目前執行檔中 指定的 Directory 位址
  const void * GetDirectory(int id);

  // 傳回指定 import DLL module 的 結構資訊描述子
  PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor(LPCSTR pDllName);

  // 傳回指定 imported function 的位址 ( __imp__xxx variable )
  const unsigned * GetFunctionPtr(PIMAGE_IMPORT_DESCRIPTOR
    pImport, LPCSTR pProcName);

  FARPROC SetImportAddress(LPCSTR pDllName, LPCSTR pProcName,
    FARPROC pNewProc);

  FARPROC SetExportAddress(LPCSTR pProcName, FARPROC pNewProc);
};
