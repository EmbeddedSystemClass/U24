
#ifdef SPU_EXTEND_UI_EXPORTS
#define SPU_EXTEND_UI_API extern "C" __declspec(dllexport)
#else
#define SPU_EXTEND_UI_API extern "C" __declspec(dllimport)
#endif

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

SPU_EXTEND_UI_API bool SPU_WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize,HWND hParent = NULL);
SPU_EXTEND_UI_API bool SPU_Print(const TSTRING& strName,const TSTRING& strOutput,HWND hParent = NULL);

#ifdef _AFX
SPU_EXTEND_UI_API bool SPU_WaitInput_MFC(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize,CWnd* pParent = NULL);
SPU_EXTEND_UI_API bool SPU_Print_MFC(const TSTRING& strName,const TSTRING& strOutput,CWnd* pParent = NULL);
#endif//#ifdef _AFX