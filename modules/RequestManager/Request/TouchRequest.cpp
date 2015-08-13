#include "stdafx.h"
#include "TouchRequest.h"

namespace PLATFORM_TOOL{
	CDiagUpdateTouchFW::CDiagUpdateTouchFW()
		:CDiagRequestBase(5000, false)
	{
		BYTE szRAWCommand[] = {75, 250, 80, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDiagUpdateTouchFW::~CDiagUpdateTouchFW(){}

	CDiagCalibrateTouch::CDiagCalibrateTouch(bool &bSuccess)
		:CDiagRequestBase(_T("CDiagGetTouchStream"), 15000, true), m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 85, 0, 0, 0, 0, 0};

		bSuccess = false;
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagCalibrateTouch::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pIDACStart = (BYTE *)ptBufferReceived->GetData() + 6;
		
		m_bSuccess = 1 == *pIDACStart;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagCalibrateTouch::~CDiagCalibrateTouch(){}

	CDiagGetTouchStream::CDiagGetTouchStream (char *szTouchStream)
		: CDiagRequestBase (_T("CDiagGetTouchStream"))
		, m_TouchStream (szTouchStream)
	{
		BYTE szRAWCommand[] = {75, 250, 81, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagGetTouchStream::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			unsigned short x;
			unsigned short y;
			BYTE ID;	//1:first finger, 2:second finger, ...
			BYTE Flag;	//0x0A:Press, 0x09:move, 0x04:release
			BYTE Time1;
			BYTE Time2;
			BYTE Time3;
		} diag_TouchStream_response;
		#pragma pack(pop)
		
		BYTE *pTouchNum = (BYTE *)ptBufferReceived->GetData() + 8; 
		BYTE *pTouchStart = (BYTE *)ptBufferReceived->GetData() + 9;

		diag_TouchStream_response OnePoint;
		
		CStringA cstrTouchStream("");

		for(int i = 0;i < *pTouchNum;i++){
			Safememcopy((void*)&OnePoint,
						sizeof(diag_TouchStream_response),
						(void*)(pTouchStart + i * sizeof(diag_TouchStream_response)),
						sizeof(diag_TouchStream_response));
			
			//switch high and low byte of x, y
			BYTE *bp, btemp;
			bp = (BYTE*)&OnePoint.x;
			btemp = *bp;
			*bp = *(bp +1);
			*(bp +1) = btemp;

			bp = (BYTE*)&OnePoint.y;
			btemp = *bp;
			*bp = *(bp +1);
			*(bp +1) = btemp;
			
			CStringA cstrFlag;

			switch(OnePoint.Flag){
				case 0x0A://press
					cstrFlag = "T";
					break;

				case 0x09://move
					cstrFlag = "M";
					break;

				case 0x04://release
					cstrFlag = "R";
					break;

				default:
					cstrFlag = "N";
					break;
			}

			CStringA cstrTemp;
			cstrTemp.Format("%d,%d,%s,%d;", OnePoint.x, 
											OnePoint.y, 
											cstrFlag.GetString(), 
											OnePoint.ID);

			cstrTouchStream += cstrTemp;
		}

		::strcpy(m_TouchStream, cstrTouchStream.GetString());

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagGetTouchDelta::CDiagGetTouchDelta (char *szTouchDelta)
		: CDiagRequestBase (_T("CDiagGetTouchDelta"))
		, m_TouchDelta (szTouchDelta)
	{
		BYTE szRAWCommand[] = {75, 250, 83, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagGetTouchDelta::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pDeltaStart = (BYTE *)ptBufferReceived->GetData() + 8;

		CStringA cstrDeltas("");

		cstrDeltas.Format("%d", *pDeltaStart);

		for(int i = 1;i < 163;i++)
		{
			cstrDeltas.AppendFormat(";%d", *(pDeltaStart + i));
		}
		
		::strcpy(this->m_TouchDelta, cstrDeltas.GetString());

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagGetTouchIDAC::CDiagGetTouchIDAC (char *szTouchIDAC)
		: CDiagRequestBase (_T("CDiagGetTouchIDAC"))
		, m_TouchIDAC (szTouchIDAC)
	{
		BYTE szRAWCommand[] = {75, 250, 84, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagGetTouchIDAC::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pIDACStart = (BYTE *)ptBufferReceived->GetData() + 8;

		CStringA cstrIDACs("");

		cstrIDACs.Format("%d", *pIDACStart);

		for(int i = 1;i < 163;i++)
		{
			cstrIDACs.AppendFormat(";%d", *(pIDACStart + i));
		}

		::strcpy(this->m_TouchIDAC, cstrIDACs.GetString());

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagGetTouchBaseLine::CDiagGetTouchBaseLine (char *szTouchBaseLine)
		: CDiagRequestBase (_T("CDiagGetTouchBaseLine"))
		, m_TouchBaseLine (szTouchBaseLine)
	{
		BYTE szRAWCommand[] = {75, 250, 86, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagGetTouchBaseLine::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pBaseLineStart = (BYTE *)ptBufferReceived->GetData() + 8;

		CStringA cstrBaseLines("");

		cstrBaseLines.Format("%d", *pBaseLineStart);

		for(int i = 1;i < 163;i++)
		{
			cstrBaseLines.AppendFormat(";%d", *(pBaseLineStart + i));
		}

		::strcpy(this->m_TouchBaseLine, cstrBaseLines.GetString());

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagTouchEnableSensor::CDiagTouchEnableSensor(bool &bSuccess)
		:CDiagRequestBase(_T("CDiagTouchEnableSensor"), 5000, true), m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 71, 0, 0, 0, 13};

		bSuccess = false;
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagTouchEnableSensor::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pStatus = (BYTE *)ptBufferReceived->GetData() + 8;

		m_bSuccess = 1 == *pStatus;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagTouchDisableSensor::CDiagTouchDisableSensor(bool &bSuccess)
		:CDiagRequestBase(_T("CDiagTouchDisableSensor"), 5000, true), m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 71, 0, 0, 0, 14};

		bSuccess = false;
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagTouchDisableSensor::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pStatus = (BYTE *)ptBufferReceived->GetData() + 8;

		m_bSuccess = 1 == *pStatus;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagTouchEnterTestPage::CDiagTouchEnterTestPage()
		:CDiagRequestBase(_T("CDiagTouchEnterTestPage"), 0, false)
	{
		BYTE szRAWCommand[] = {75, 250, 87, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagTouchEnterTestPage::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CDiagTouchCheckVersion::CDiagTouchCheckVersion(char &TouchVersion, bool &bSuccess)
		:CDiagRequestBase(_T("CDiagTouchCheckVersion"))
		, m_TouchVersion (TouchVersion)
		, m_Success (bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 88, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagTouchCheckVersion::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{

		//sendrawrequest 75 250 88 00 00 00 00 00
		//return 0x4B FA 58 00 25 25 13 00 

		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code; //75
			BYTE  subsys_id;//250
			short mobile_id;//88
			BYTE a_id;
			BYTE b_id;
			BYTE THver;
			BYTE c_id;
		} diag_CheckVersion_response;
		#pragma pack(pop)
		
		diag_CheckVersion_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		m_Success = resp.THver != 0;
		
		m_TouchVersion = resp.THver;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

	CDiagTouchClearStreamBuffer::CDiagTouchClearStreamBuffer(bool &bSuccess)
		:CDiagRequestBase(_T("CDiagTouchClearStreamBuffer")),
		m_Success(bSuccess)
	{
		BYTE szRAWCommand[] = {75, 250, 89, 0, 0, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CDiagTouchClearStreamBuffer::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE *pStatus = (BYTE *)ptBufferReceived->GetData() + 6;

		this->m_Success = *pStatus == 0;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

}