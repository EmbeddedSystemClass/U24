#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{

/*****************************************************************************/
//
//  Gauge IC
//
/*****************************************************************************/
	class RM_CLASS_API CGaugeICRequest: public CDiagRequestBase
	{
	public:
		CGaugeICRequest(int nTyep, int& nValue,bool& bSuccess);
		virtual ~CGaugeICRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nValue;
		int m_nType;
	};
	class RM_CLASS_API CGasGaugeDeviceNameRequest: public CDiagRequestBase
	{
	public:
		CGasGaugeDeviceNameRequest (std::string &devName);
		virtual ~CGasGaugeDeviceNameRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		std::string& m_strDeviceName;
	};

	class RM_CLASS_API CGasGaugeICDeviceIDRequest: public CDiagRequestBase
	{
	public:
		CGasGaugeICDeviceIDRequest (BYTE& ID);
		virtual ~CGasGaugeICDeviceIDRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		BYTE &m_ChargeIC_ID;
	};



/*****************************************************************************/
//
//  Charging mode
//
/*****************************************************************************/

	class RM_CLASS_API CChargingStatusRequest: public CDiagRequestBase
	{
	public:
		CChargingStatusRequest(bool& bIsCharging,bool& bSuccess);
		virtual ~CChargingStatusRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bIsCharging;
		bool& m_bSuccess;
	};

	class RM_CLASS_API CChargingSwitchRequest: public CDiagRequestBase
	{
	public:
		CChargingSwitchRequest(short Mode_ID,bool bEnable,bool& bSuccess);
		virtual ~CChargingSwitchRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CChargingSwitchRequestWithVol: public CDiagRequestBase
	{
	public:
		CChargingSwitchRequestWithVol(bool bEnable,bool& bSuccess,int iVol);
		virtual ~CChargingSwitchRequestWithVol();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CReadBatteryVoltage : public CDiagRequestBase 
	{
	public:
		CReadBatteryVoltage(int* nVoltage, bool& bSuccess);
		virtual ~CReadBatteryVoltage();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		//bool Encode(CBufferPtr& ptBufferSend);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		int* m_pnVoltage;
		bool& m_bSuccess;
	};


/*****************************************************************************/
//
//  Coin Cell
//
/*****************************************************************************/

	class RM_CLASS_API CReadCoincellVoltageRequest: public CDiagRequestBase
	{
	public:
		CReadCoincellVoltageRequest(int& nVoltage,bool& bSuccess);
		virtual ~CReadCoincellVoltageRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nVoltage;
	};

	/*****************************************************************************/
	//
	//  key event
	//
	/*****************************************************************************/

	class RM_CLASS_API CPowerKeyEventRequest: public CDiagRequestBase
	{
	public:
		CPowerKeyEventRequest ();
		virtual ~CPowerKeyEventRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived)
		{
			CDiagRequestBase::OnDataReceiver(ptBufferReceived);
		}
	};


} //PLATFORM_TOOL






