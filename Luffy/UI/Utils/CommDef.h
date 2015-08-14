#include <map>


#define DLLNAME _T("TEST.dll")

#define PICASSO                    "PICASSO"
#define SW_VERSION                 "SW_VERSION"
#define FACTORY_VERSION			   "FACTORY_VERSION"
#define TOOL_VERSION               "TOOL_VERSION"
#define MODEL_NAME				   "MODEL_NAME"
#define STATION_NAME				"STATION"
#define LOGPATH						"UI_LOG_PATH"
#define TOOLMODE					"TOOL_MODE"

//#define   DEFAULT_SLOT			1

#define CONFIG_PATH                _T("C:\\PE_Config")

/*==================== Structure ==========================*/
/* Main UI display ID for all station use */
#pragma pack (1)
struct st_UIIDparameter
{
	int     i_parameterNum;          // Parameter num (NUM_TRANSFER_PARAMETER)
	bool    b_tsFlag;	             // TS_Flag
	CString str_firstProcess;		 // First process(station)
	CString str_station;             // Station (DL,FT...)
	CString str_modelName;           // Model Name
	CString str_line;                // Line (T-Line,A-Line...)
	CString str_pcba;
	CString str_so;
	CString str_partNumber;
	CString str_toolVer;
	CString str_employee;
	CString str_factoryVer;
	CString str_userLoadVer;
	CString str_imagePath;
	CString str_soType;
	CString str_segment;
	CString str_CRType;
};
#pragma pack()


#pragma pack (1)
struct st_UIControl
{
	bool b_ScanPicasso;
	bool b_PreScan;
	bool b_AutoRunPreScan;
	bool b_AutoRun;
	bool b_NAL;
	bool b_SQN;
	int  b_VKEnter;
	bool b_ReadFA;
	bool b_WriteFA;
	int  i_FAArea;
	CString str_NAL6;
};
#pragma pack()


CString ConvertString(const char* sz_buffer);
bool IsStringAlphaNumber(CString &str_text);
bool SpiltString(CString str_srcString, CString str_delimiter, CStringArray& stra_strArg);
void CStringToChar(CString str_In, char *sz_Out);
CString ConvertToVisibleString(char *sz_buf, int i_len);
