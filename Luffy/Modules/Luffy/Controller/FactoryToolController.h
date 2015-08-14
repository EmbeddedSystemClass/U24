// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FACTORYTOOLCONTROLLER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FACTORYTOOLCONTROLLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef FACTORYTOOLCONTROLLER_EXPORTS
#define FACTORYTOOLCONTROLLER_API extern "C" __declspec(dllexport)
#else
#define FACTORYTOOLCONTROLLER_API extern "C" __declspec(dllimport)
#endif

#include "..\..\UnifyInterface\FacTestToolInterface.h"
#include "utils/Logger/CLogger.h"
#include "utils/xml/pugixml.hpp"
#include "Processor/GPProcessor.h"


/*================================= Structure ==============================*/
#pragma pack (1)
typedef struct
{
	int  iCOMPort;                     // COM Port
	std::string strErrorCode;          // ErrorCode
}DeviceStruct;
#pragma pack()


/**********************************************************
//Export functions in FactoryToolController.dll
***********************************************************/

FACTORYTOOLCONTROLLER_API bool __cdecl NewInterface(IFacTestToolInterface** ppIFacTestToolInterface);
FACTORYTOOLCONTROLLER_API bool __cdecl DeleteInterface(IFacTestToolInterface* pIFacTestToolInterface);

/**********************************************************
Global function and variable for callback notify
***********************************************************/
void NotifyToController (const char* szEvent, long nParam) ;
void NotifyToProcessor(const char* szEvent, long nParam, int iSlot);

/**********************************************************
CFactoryToolController
***********************************************************/
class CFactoryToolController :	public IFacTestToolInterface 
{
public:
	// Log
	CLogger m_log;


public:
	CFactoryToolController(void);
	virtual ~CFactoryToolController(void);

	/**********************************************************
										IFacTestToolInterface
	***********************************************************/
	/* Set/Get Parameter Value */
	virtual bool SetParameterValue(char* sz_keyword, char* sz_value, int i_slot = -1);
	virtual bool GetParameterValue(char* sz_keyword, char* sz_value, int i_size, int i_slot = -1);

	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetSerialNumber(int i_slot, char* sz_value, int i_size);
	virtual bool GetTimeStamp(int i_slot, char* sz_value, int i_size);
	virtual bool CreateAndInitPhone(const int i_slot);
	virtual bool SetIMEI(int i_slot,char* sz_value,int i_size);
	virtual bool SetSN(int i_slot,char* sz_value,int i_size);
	virtual bool ReadId(int i_slot, char* sz_value, int i_size);
	virtual bool GetFailItem(int i_slot, char* sz_value, int i_size);

	/* Get test items */
	virtual int GetItemCount();
	virtual bool GetItemByIndex(int i_index, char* sz_name, int i_size);

	virtual int GetRecoverItemCount();
	virtual bool GetRecoverItemByIndex(int i_index, char* sz_name, int i_size);

	/* Test sequence */
	virtual bool Begin(int i_slot = 1);
	virtual bool PreRun(int i_slot = 1);
	virtual bool Run(int i_slot = 1);
	virtual bool PostRun(int i_slot = 1);
	virtual bool Stop(int i_slot = 1);
	virtual bool End(int i_slot = 1);
	virtual bool LoadPhoneInfo(int i_slot = 1);

	/* Call back */
	virtual bool RegCallBack(p_fn_FacTestToolInterface_callback pCallback);

public:
	void SendEventToProcessor(const char* szEvent, long nParam, int iSlot);
	void Fire(const char* szEvent, long nParam);

protected:

	void InitDirParameter();
	bool InitXML();
	void GetCOMPortList();
	bool InitGPProcessor();
	void SetTestResult(int i_slot = 1);
	bool CreateProcessorForEachSlot();
	bool SetLogName();
	bool SetPICASSO(char* sz_keyword, char* sz_value, int i_slot);

protected:
	std::vector <std::string>		m_vEvent;
	bool							m_bForceToPass;

	std::vector<string>		m_vTestItemList;
	std::vector<string>		m_vRecoverItemList;

	std::map<std::string, std::string>	m_Parametermap;

	//MultiDevice
	std::map<int, CGPProcessor*>		m_mapProcessor;			// Slot <-> ToolInterface
	std::map<int, DeviceStruct>			m_mapDeviceStruct;		// Slot <-> COM port, ErrorCode
	std::map<int, std::string>			m_mapPicasso;			// Slot <-> Picasso
	std::map<int, size_t>				m_mapResult;				// Slot <-> Result

	// ITool DLL
	HMODULE							m_hDLL;

	//xml
	pugi::xml_document m_xmlTestItem;
	pugi::xml_document m_xmlConfig;


private:
	bool m_bProcessorReady;

	p_fn_FacTestToolInterface_callback m_pFacTestToolInterfaceCallback;
};


