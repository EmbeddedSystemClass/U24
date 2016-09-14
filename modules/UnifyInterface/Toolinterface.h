#ifndef _I_TOOL_INTERFACE_H_
#define _I_TOOL_INTERFACE_H_

#include "FacTestToolInterface.h"
//#include "..\..\ui\UnifyUI_FacTestTool\CommDef.h"

#define     DLL_NEW_TOOLINTERFACE	"NewToolInterface"
#define     DLL_DEL_TOOLINTERFACE	"DeleteToolInterface"

namespace ParameterKeyWord
{
	static const TCHAR	STATIONNAME[]					= _T("STATION");
	static const TCHAR	MODELNAME[]						= _T("MODEL_NAME");
	static const TCHAR	WORKINGDIR[]						= _T("WORKING_DIR");
	static const TCHAR	CONFIGDIR[]							= _T("CONFIG_DIR");
	static const TCHAR	FORCEPASS[]						= _T("FORCEPASS");
	static const TCHAR	LOGDIR[]								= _T("DLL_LOG_PATH");
	static const TCHAR	RFPROCESSORNAME[]			= _T("RF_PROCESSOR_NAME");
}

interface IToolInterface
{
public:
	enum ToolTestParts
	{
		TEST_BOX			= 10,
		POWERSUPPLY	= 20 ,
		PHONE				= 30 ,
		JIG					= 40,
		COMMON			= 100,
	};

	virtual ~IToolInterface(){};

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value) = 0;
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size) = 0;

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize, int i_idType) = 0;
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize) = 0;
	
	//Oninitial dialog
	virtual bool OnAPPLaunch(int i_slot = 1) = 0;

	//Each run
	virtual bool SetCableloss(const wchar_t *szType, const wchar_t *szBand, const wchar_t *szChannel, const wchar_t *szLoss) = 0;

	virtual bool SetTestPart(IToolInterface::ToolTestParts PartID, const wchar_t *szName, const wchar_t *szItem, const wchar_t *szContent, const wchar_t *szAttribute = L"") = 0;

	virtual bool SetTestItem(const wchar_t *szItem) = 0;//Must keep the same for factory
	virtual bool InitTestItem(const wchar_t *szItem) = 0;

	virtual bool SetParameterValue(const wchar_t* sz_keyword, const wchar_t* sz_value) = 0;
	virtual bool GetParameterValue(const wchar_t* sz_keyword, wchar_t* sz_value, int i_size) = 0;
	//virtual bool GetParameterValue(const wchar_t* sz_keyword, char* sz_value, int i_size) = 0;

	virtual bool OnRunInit(int i_slot = 1) = 0;
	
	virtual bool RunTestItem(int iItem, int i_slot = 1) = 0;
	
	virtual bool OnRunFinish(int i_slot = 1) = 0;
	
	virtual bool OnAPPEnd(int i_slot = 1) = 0;

	/* Response to Jig control  */
	virtual bool ResopnseToJig(bool b_wait) = 0;

	/* Response to show dialog  */
	virtual bool ResopnseToShowDlg(char* sz_showReslut) = 0;

};

typedef bool (__cdecl *p_fn_newToolInterface)   (IToolInterface**);
typedef bool (__cdecl *p_fn_deleteToolInterface)(IToolInterface*);

#endif//_I_TOOL_INTERFACE_H_
