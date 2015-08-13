#ifndef _ENHANCE_CONTROLLER_H_
#define _ENHANCE_CONTROLLER_H_

#include "..\..\..\UnifyInterface\Toolinterface_Notify.h"
//For station name
#include "..\..\..\..\ui\UnifyUI_FacTestTool\CommDef.h"

typedef void (*p_fn_ECICallback)(const TCHAR *, LPARAM );

namespace ECIErrorCode
{
	namespace ToolLoader
	{
		static const size_t LOAD_DLL_FAIL										= 0x00010000;
		static const size_t FREE_DLL_FAIL										= 0x00010001;
	}

	namespace TestItemHdr
	{
		static const size_t LOAD_TESTITEM_FAIL							= 0x00020000;
		static const size_t TOOL_ONRUNINIT_FAIL							= 0x00020001;
		static const size_t TOOL_ONRUN_FAIL								= 0x00020002;
		static const size_t TOOL_ONRUNFINISH_FAIL						= 0x00020003;
		static const size_t TOOL_SET_TESTITEM_FAIL					= 0x00020004;
	}

	namespace TestBoxHdr
	{
		static const size_t LOAD_TESTBOX_SETTING_FAIL				= 0x00030000;
		static const size_t TOOL_SET_TESTBOX_FAIL						= 0x00030001;
	}

	namespace PowerSupplyHdr
	{
		static const size_t LOAD_PS_SETTING_FAIL						= 0x00040000;
		static const size_t TOOL_SET_PS_FAIL								= 0x00040001;
	}

	namespace PhoneHdr
	{
		static const size_t LOAD_PHONE_SETTING_FAIL									= 0x00050000;
		static const size_t LOAD_PHONE_SETTING_FROM_CONFIG_FAIL			= 0x00050001;
		static const size_t TOOL_SET_PHONE_FAIL										= 0x00050002;
	}

	namespace GPIBHdr
	{
		static const size_t LOAD_GPIB_SETTING_FAIL					= 0x00060000;
		static const size_t TOOL_SET_GPIB_FAIL							= 0x00060001;
	}

	namespace CablelossHdr
	{
		static const size_t LOAD_CABLELOSS_SETTING_FAIL			= 0x00070000;
		static const size_t TOOL_SET_CABLELOSS_FAIL					= 0x00070001;
	}

	namespace DownloadHdr
	{
		static const size_t LOAD_IMAGEFILE_FAIL			= 0x00080000;
	}
}


//Message for ECI to main export class
namespace ECIMsg
{
	static const TCHAR TestingItem[]													=_T("ECIMsg.TestingItem");
}

class CEnhanceController
{
public:
	CEnhanceController();
	CEnhanceController(CEnhanceController* pNextEC);
	virtual ~CEnhanceController();

	virtual size_t Enhance_OnAPPLaunch();
	
	virtual size_t Enhance_OnRunInit(int i_slot = 1);

	virtual size_t Enhance_RunTest(int i_slot = 1);
	
	virtual size_t Enhance_OnRunFinish(int i_slot = 1);
	
	virtual size_t Enhance_OnAPPEnd();

	void SetNextEC(CEnhanceController *pEC);
	void FreeNextEC();
protected:
	void SendMsg(const TCHAR *MsgType, LPARAM Msg);
	void SetECICallback(p_fn_ECICallback cb);

protected:
	CEnhanceController*						m_pNextCEC;
	static p_fn_ECICallback					m_ECICallback;
	static IToolInterface_Notify*			m_pITI;
	static std::vector<CString>			m_vTestItemList;
	static std::map<CString, CString>	m_Parametermap;
};

#endif
