#ifndef _C_COMMON_PROCESSOR_H_
#define _C_COMMON_PROCESSOR_H_


#include "../../CommonUtility/BaseObject/ITestProcessor.h"
//#include "../../CommonUtility/BaseObject/ITestItem.h"


class CCommonProcessor : public ITestProcessor
{
RF_DECLARE_DYNCREATE(CCommonProcessor)


// Data members
public:
protected:
private:
	bool m_bStopped;


// Member functions
public:
	CCommonProcessor() {}
	virtual ~CCommonProcessor() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	// Test sequence
	//virtual bool Initial();
	//virtual bool IsConnected();
	/* Set/Get FA Data */
	virtual bool SetFAData(int i_slot, char* sz_value);
	virtual bool GetFAData(int i_slot, char* sz_value, int i_size) ;

	/*Get/Set FA Sector Data*/
	virtual bool GetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);
	virtual bool SetFASector(int i_slot, int i_sectorNum, char *sz_sectorData, int i_sectorSize);

	virtual bool Begin();
	virtual bool Run();
	virtual bool Stop();
	//virtual bool Disconnect();
	virtual bool End();
	virtual bool PreRun();
	virtual bool PostRun();

protected:
private:
};


#endif // End of #ifndef _C_COMMON_PROCESSOR_H_
