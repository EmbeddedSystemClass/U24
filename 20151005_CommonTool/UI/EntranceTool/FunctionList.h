#pragma once

#define  INIFILEPASH           _T(".\\INI\\setup.ini")
#define  DIRECTORYPATH         _T("C:\\Image\\")
#define  TOOLPATH              _T("C:\\TOOL\\")
#define  TOOLPATHCONFG         _T(".\\INI\\Tool_Path.ini")
#define  ERRORNUMBER           512

#include "..\..\Modules\DBAccess\DBAccess.h"
#include "..\..\Modules\VMS_DBAccess\VMS_DBAccess.h"

#include "MD5_Check.h"
#include "afx.h" 
#include "Filename_resolution.h"
#include "DirectoryDispose.h"


class FunctionList
{
public:
	FunctionList(void);
	~FunctionList(void);

	bool GetComputername_LT(TCHAR* ch_computername);
	TCHAR* CString2TCHAR(CString &str);
	CString GetFileName();
	char* THCAR2char(TCHAR* tchStr); 
    bool GetEmployeeIdOnly();

	bool ProjectName_Return(TCHAR* sz_modelprojList,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg);

	CString m_strToolVersion;
	CString m_strFImageVersion;
	CString m_strOImageVersion;
	CString m_strEntranceVersion;
	CString m_strFImageFixTag;
	CString m_strEntranceFixTag;
	CString m_strSOModel;
	CString m_strSOPartNo;
	CString m_strSOType;
	CString m_strSegment;
	CString m_strFImageList;

	CString m_strProject;
	CString m_strModel;
	CString m_strProcess;
	bool bChecSO_VRCMFinished;
	bool CheckSO_VRCM(CString strModel, CString strSO,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg);
	CString GetInformtionFromSO(CString VersionType);
//VersionType: ToolVersion/FImageVerison/OImageVersion/EntrnaceVersion/FImageF/EntranceF/SoType/SoCount/Segment/Model
	bool Entrance_Tool_Update(TCHAR * Entrance_toolver,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg);
	bool CheckSegment_Version(TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg);
	bool CheckStation_Process(TCHAR* szProcess,TCHAR * sz_ErrCode,TCHAR * sz_ErrMsg);
	bool CheckImployeeIDStatus(TCHAR* szProcess, TCHAR* szImployee, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Check_strLevel(CString strLevel,CString strProcess,CString strSegment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool CheckFDCSVersionStatus(bool bCommonTool, CString strProcess, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool ImageCopy(CString str_ModelProjectName, CString str_ProjectName, CString& str_ImagePath, TCHAR* sz_SO, CString str_ETVersion, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	


	bool PrCheck1(TCHAR* sz_SO,TCHAR* sz_employId, TCHAR* sz_Station, TCHAR* sz_Model, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool PrCheck3(CString str_ProjectName, TCHAR* sz_SO, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Query_ImployeeID(TCHAR* sz_employId, TCHAR* sz_Level,TCHAR* sz_status,TCHAR* sz_wrkstation,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Query_StationInfo(TCHAR* sz_line, TCHAR* sz_process, TCHAR* sz_segment,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Query_SOInfo(TCHAR* sz_SO, TCHAR* sz_Model, TCHAR* sz_PN, TCHAR* sz_SOType,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Query_PorcessList(TCHAR* sz_Model, TCHAR* sz_ProcessList, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool ToolCopy(CString str_ServerPath, CString str_LocPath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool DeleteExtrl_Tool(CString str_LocPath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool DoModuleCheck(TCHAR* sz_Model, TCHAR* sz_Model_get, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Recurse(LPCTSTR pstr, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Unziptool(CString str_Toolpath , CString str_destpath, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool UnzipImage(CString str_search, TCHAR* szImageVersion , TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool ImageDirectoryDelete(CString str_search, TCHAR* szImageVersion , TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool RunTool(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString str_process, CString str_ProjectName);
	bool ReadIniFile(CString& str_ServerPath, bool b_state);
	bool CreateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath, CString str_process, CString str_ProjectName);
	bool GetToolName(CString& str_Toolpath, CString str_process, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);

	bool DirectoryCopy(CString str_Dest, CString str_Source, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool DeleteDirectory(CString str_Directory, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool FDCSCheck(TCHAR* sz_SO, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Pre2_check(CString str_ETVersion, TCHAR* str_ErrCode, TCHAR* str_ErrMsg);
	bool DO_SD_Copy(CString str_projectName, bool b_state, TCHAR* szErrCode, TCHAR* szErrMsg);
	int FindInformation(CString str_dir,CString& filepath, CString str_Wildcard);
	int FindTooldirectory(CString str_dir,CString& str_directory, CString str_version);
	CString GetProjectName();
	CString GetProcessName();
	bool FcheckImageDirectory(TCHAR* szImageVersion, CString& str_locPath, bool b_state, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	

	bool ImageCopyCheck(TCHAR* szProject, TCHAR* szImageVersion, CString& str_replace, bool b_state, bool b_ImageType, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool MD5_CopyCheck(CString str_server_path, CString str_LOC_Path, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool ToolUpgrade(TCHAR* szProject, TCHAR* szToolVersion, TCHAR* szErrCode, TCHAR* szErrMsg);
	bool CheckCommonTool(CString str_Process, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool ExecuteCommonTool(CString str_path_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool GetCommonToolPath(CString str_Process, CString& str_path, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
    bool Update_ImployeeIdStatus(TCHAR* sz_employId,TCHAR* sz_status,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
    bool CreateUpdateBat(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg, CString& str_batpath);
    
    CString GetUnify_UI_Version(CString& lpszFileName);
    bool CreateDoMd5(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath);
	CString UiMD5_ReturnValue(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath);
	
    bool DO_UiMD5Check(TCHAR* sz_ErrCode,TCHAR* sz_ErrMsg,CString& str_batpath,CString& str_checkPath);
	bool KillAdbProcess(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	int FindInformationToolMD5(CString str_dir,CString& filepath, CString str_Wildcard);
	bool ExecuteFactoryResetTool(CString str_path_Server, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	CString GetFactoryResetPath();
	int FindInformationResetToolMD5(CString str_dir,CString& filepath, CString str_Wildcard);
	int FindResetTooldirectory(CString str_dir,CString& str_directory, CString str_version);
private:
	TCHAR m_sz_employId[15];
	TCHAR m_sz_Level[15];
    TCHAR m_sz_status[15];
    


	TCHAR m_sz_PCname[15];
    TCHAR m_sz_line[15]; 
    TCHAR m_sz_process[15]; 
	TCHAR m_sz_wrkstation[15];
    TCHAR m_sz_segment[15];

	TCHAR m_sz_SO[10];
	TCHAR m_sz_Model[15];
    //TCHAR m_sz_SOType[15];
    //TCHAR m_sz_PN[15];

	TCHAR  m_sz_version_get[50];          //get version from file name
	TCHAR  m_sz_model_get[15];          //get version from file name

	MD5_Check m_obj_MD5check;
	Filename_resolution m_file_ana;

	DirectoryDispose m_obj_DirectoryDispose;

	CString m_str_process;
	CString m_str_ProjectName;
	CString m_str_ImagePath;
	CString m_str_status;


	//Tom add 20120101
	char m_sz_model_pre2[50];
	char m_sz_partNo_pre2[50];
	char m_sz_soType_pre2[50];
	char m_sz_toolVer_pre2[50];
	char m_sz_facImageVer_SO_pre2[50];
	char m_sz_userImageVer_pre2[50];
	char m_sz_ETtoolVer_pre2[50];
	char m_sz_starttime_pre2[50];
	char m_sz_endtime_pre2[50];
	char m_sz_Status_factory_pre2[50];
	char m_sz_SWVersion_pre2[50];
	char m_sz_Status_Tool_pre2[50];
	char m_sz_Reserve1_pre2[50];
	char m_sz_toolVer_pr2[50];
	char m_sz_facImageVer_Tool_pre2[50];
	char m_sz_CRType_pre2[50];
	char m_sz_sz_Qty[50];
    char m_sz_segment_pre2[50];

   // CString str_ETtoolVer_pre2;

	//CString   add 201201
	CString m_str_SOType;
	CString m_str_CRType;
	CString m_str_ToolStatus;
	CString m_str_FactoryStatus;
	CString m_str_UseStatus;

	CString m_str_UserImage;
	CString m_str_FactoryImage;
	CString m_str_EntranceToolver;
	CString m_str_Toolver;
	CString m_str_Model;
	CString m_str_ImagePath_Factory_LOC;
	CString m_str_ImagePath_User;
	CString m_str_MD5Path_Factory_LOC;
	CString m_str_HashFilepath_Loc;
	CString m_str_ImagePath_User_LOC;
	CString m_str_MD5DirectoryPath_User_LOC;
	CString m_str_MD5_User_Server;
	CString m_str_MD5File_User_LOC;
	CString m_str_ModelProjectName;

	CString m_str_ToolFolderVersion;
	CString m_str_filename;
	CString m_str_CommonToolver;
	CString m_str_FacResetToolver;
	//MsgDlg m_obj_Errormessage;
	

private:
	bool m_bDBConnected;
	bool m_bMD5check;
	bool m_bMD5check_tool;
	bool m_bMD5check_tool_Pre;
	bool m_bImageCopy;
	bool m_bfactoryImagestate;
	bool m_buserImagestate;
	bool m_btoolstate;
	bool m_bcommonTool;
	bool m_bWin7;
	st_9JPNData st_PNinfo;
	Clog m_Clog;

};