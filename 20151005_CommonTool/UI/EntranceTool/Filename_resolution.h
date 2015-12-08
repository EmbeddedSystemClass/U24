#pragma once

#define  INIPASH           _T(".\\INI\\config.ini")

class Filename_resolution
{
public:
	Filename_resolution(void);
	~Filename_resolution(void);



public:
	bool GetFilename(TCHAR* Filename);
	bool ReadINI(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Getfileattributes(TCHAR* Filename, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool Getfileversion(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool GetSWversion(TCHAR* sz_version, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	bool GetModule(TCHAR* sz_model,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg);
	char* THCAR2char(TCHAR* tchStr); 


private:
	TCHAR*       m_c_filename; 
	CStringArray  m_stra_SWEP;
	CStringArray  m_stra_SWFP;
	CStringArray  m_stra_TLEU;
	CStringArray  m_stra_TLFT;
	CStringArray  m_stra_TLCS;
	CStringArray  m_stra_SDCT;
	CStringArray  m_stra_QCNP;
	CStringArray  m_stra_PLMN;
	CStringArray  m_stra_SIML;
	CStringArray  m_stra_APPV;

	CStringArray  m_stra_SWEP_value;
	CStringArray  m_stra_SWFP_value;
	CStringArray  m_stra_TLEU_value;
	CStringArray  m_stra_TLFT_value;
	CStringArray  m_stra_TLCS_value;
	CStringArray  m_stra_SDCT_value;
	CStringArray  m_stra_QCNP_value;
	CStringArray  m_stra_PLMN_value;
	CStringArray  m_stra_SIML_value;
	CStringArray  m_stra_APPV_value;

	CStringArray  m_stra_fileattributes;
	CString       m_stra[10];
	CStringArray  m_stra_value[10];
	CStringArray  m_stra_version[10];
	CStringArray  m_stra_name[10];

	bool          m_szbcheck[20];
	int           m_i_SWEP;                    //Record the number in [SWEP] section
	int           m_i_SWFP;
	int           m_i_TLEU;
	int           m_i_TLFT;
	int           m_i_TLCS;
	int           m_i_SDCT;
	int           m_i_QCNP;
	int           m_i_PLMN;
	int           m_i_SIML;
	int           m_i_APPV;
};
