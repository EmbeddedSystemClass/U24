/***************************************************************************************
* Copyright (C) : QCS/WGC18                                      
* Module name	: Common  
* File name     : BaseProject.h             
* Summary       : All download project base clase.                                                  
* Version		Author             Date                Abstract                      
* 1.0           Alex.Chen          2011/07/04          First version  
****************************************************************************************/

#pragma once

#include "../../../Modules/UnifyInterface/FacTestToolInterface.h"
#include "../IniAccess.h"
#include "../DLError.h"

//#include "../../../Modules/QisdaNVModule/NVItem.h"
#include "../../../Modules/QisdaQualcommDownloadModule/Download.h"
//#include "../../../modules/QcnToCnvDll/QcnToCnvDll.h"
#include "../../SPU/SPU/ISPU_CALLBACK.h"
//#include "../../SPU/SPU_Loader/LoaderExport.h"


/*================================= Structure ==============================*/
#pragma pack (1)
typedef struct
{
	std::string   str_imagePath;     // Image FilePath
	std::string   str_imageType;     // Image Type
	std::string   str_imageName;     // Image FileName
	bool          b_isValid;
}st_Image;
#pragma pack()
/*================================= Structure ==============================*/


/*================================= Const ==================================*/
#define   BUFSIZE                 4096
#define   SWVERSION_LENGTH        6
#define   HWVERSION_LENGTH        5
#define   DOWNLOAD_MODE_REDL      _T("ReDL")
#define   DOWNLOAD_MODE_REDL_OSDL _T("ReDL_OSDL")
/*================================= Const ==================================*/

using namespace QSYNC_LIB;
class CBaseProject : public CNotifyBase_ANSI, public INotify_ANSI, public ISPU_CALLBACK
{
public:
	CBaseProject();
	virtual ~CBaseProject();

public:
	/* CNotifyBase_ANSI */
	virtual bool Register(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Fire(const std::string& strEvent, long nParam);

	/* INotify_ANSI */
	virtual void Event (const std::string& strEvent, long nParam);

	/* ISPU_CALLBACK */
	virtual bool CheckBreak();
	virtual bool WaitInput(const TSTRING& strName, TCHAR* szBuffer, int nBufferSize);
	virtual bool Print(const TSTRING& strName, const TSTRING& strOutput);

public:
	void SetSlotNumber(int i_slot);
	void SetImageVector(std::vector<st_Image> vector_image);
	void SetPicassoList(std::map<int, CString> map_picassoList);
	void SetCOMPicasso(std::map<int, CString> map_COMPicasso);
	void SetSWVerion(CString str_SWVeriosn);
	void SetHWVerion(CString str_HWVeriosn);
	void SetToolVerion(CString str_ToolVeriosn);
	void SetFactoryVerion(CString str_fatoryVersion);
	void SetCustomerSwVerion(CString str_CustomerSwVersion);
	void SetMobileId(CString str_MobileId);
	void SetDLLIniFileName(const char* sz_iniFileName);
	void SetDLMode(const char* sz_DLMode);
	void GetError(char* sz_errorCode, int i_size);
	virtual bool Run() = 0;
	virtual bool PostRun(int i_slot) = 0;
	virtual bool ReadFA(char* sz_FAData) = 0;
	virtual bool WriteFA(char* sz_FAData) = 0;
	virtual bool ReadFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;
	virtual bool WriteFASector(int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;
	virtual bool LogUpload(char *str_modelname) = 0;
	virtual bool LogUploadByBAT(char *str_modelname) = 0;

protected:
	void SendMessageToUI(const char* sz_message);
	void SetProgress(const int i_mainProgress, const int i_subProgress);
	void SetError(const char* sz_errorCode);
	std::string SearchImageFileByType(const char* sz_type);
	void SetAllPICSData(void);
	CString m_str_fatoryVersion;
	CString m_str_fatoryVersionDevice;
private:
	void SetPICSData(const char* sz_key, const char* sz_value);

protected:
	bool GetHWPlatform(char* sz_outBuffer, DWORD dw_size);
	bool GetSWVersion(char* sz_outBuffer, DWORD dw_size);
	bool GetHWVersion(char* sz_outBuffer, DWORD dw_size);
	bool GetToolVersion(char* sz_outBuffer, DWORD dw_size);

protected:
	int                           m_i_slot;                       // Slot
	std::vector<st_Image>         m_vImage;                       // Image
	TCHAR                         m_sz_iniFile[MAX_PATH];         // INI file name
	std::map<int, CString>	      m_map_COMPicasso;               // COM,Picasso
	CString                       m_str_DLMode;
	std::map<CString, CString>	  m_map_parameterList;

private:
	TCHAR                         m_sz_errorCode[BUFSIZE];        // ErrorCode
	std::map<int, CString>	      m_map_picassoList;              // Slot,Picasso
	CString                       m_str_SWVersion;                // SW version
	CString                       m_str_HWVersion;                // HW version
	CString                       m_str_ToolVersion;              // Tool version
	CString                       m_str_CustomerSwVersion;
/*======================================= Lepord / Wolf ==============================*/
protected:
	char* GetImagePath(char* sz_imagePath, DWORD dw_size);
	void GetCurrentPath(void);
	int GetCOMport(void);
	char  m_sz_currentPath[BUFSIZE];
/*======================================= Lepord / Wolf ==============================*/

};
