#ifndef _C__SAMPLE_PHONE_H_
#define _C__SAMPLE_PHONE_H_


#include "../CommonUtility/BaseObject/IPhone.h"
#include "../../../lib/Qualcomm/inc/QLib.h"
#include "../../../lib/Qualcomm/inc/QLib_Defines.h"


class CSamplePhone : public IPhone
{
RF_DECLARE_DYNCREATE(CSamplePhone)


// Data members
public:
	HANDLE m_hPhone;
	unsigned short m_unCOMPortNumber;
	int m_iPhoneMode;

protected:
private:


// Member functions
public:
	CSamplePhone() {}
	virtual ~CSamplePhone() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap) { return true; }

	// Phone functions
	virtual bool Initial();
	virtual bool Disconnect();
	virtual bool IsConnected();
	virtual bool Reboot();
	virtual bool GetMode(int& iMode);
	virtual bool ChangeMode(int iMode);

protected:
private:
};


#endif // End of #ifndef _C__SAMPLE_PHONE_H_
