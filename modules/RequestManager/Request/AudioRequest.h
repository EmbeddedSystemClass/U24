#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "stdio.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif


namespace PLATFORM_TOOL
{
	class RM_CLASS_API CSpeakerAmpRequest  : public CDiagRequestBase
	{
	public:
		CSpeakerAmpRequest (bool &bSuccess);
		virtual ~CSpeakerAmpRequest(){}
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}

		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);

	private:
		bool &m_bSuccess;
	};



    class RM_CLASS_API CIsHandsetPlugged : public CDiagRequestBase
    {
    public:
        CIsHandsetPlugged (bool& bSuccess);
        virtual ~CIsHandsetPlugged(){}
    protected:
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }

    private:
        bool& m_bIsPlugged ;
    };



    class RM_CLASS_API CVibratorOnOff : public CDiagRequestBase
    {
    public:
        CVibratorOnOff (bool &bSuccess, bool bOn);
        virtual ~CVibratorOnOff(){}
    protected:
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) ;
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }

    private:
        bool &m_bSuccess;
        bool m_bOn;
    };

}
