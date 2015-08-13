#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "C7Request.h"



namespace PLATFORM_TOOL
{
	//
	// CC7NVBackupRequest
	CC7NVBackupRequest::CC7NVBackupRequest (bool &bSuccess)
		: CDiagRequestBase (_T("CC7NVBackupRequest"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
        
		BYTE szRAWCommand[] = {0x80, 0x37, 0x01, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CC7NVBackupRequest::~CC7NVBackupRequest()
	{
	}

	void CC7NVBackupRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
        BYTE szBuffer[12] = {0};
        memcpy(szBuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

        if ( (0==szBuffer[4])&&(0==szBuffer[5])&&(0==szBuffer[6])&&(0==szBuffer[7]) )
            m_Success = true ;
        else
            m_Success = false ;

        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	//
	// CC7NVRestoreRequest
	CC7NVRestoreRequest::CC7NVRestoreRequest (bool &bSuccess)
		: CDiagRequestBase (_T("CC7NVRestoreRequest"))
		, m_Success (bSuccess)
	{

		bSuccess = false;

		BYTE szRAWCommand[] = {0x80, 0x37, 0x02, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CC7NVRestoreRequest::~CC7NVRestoreRequest()
	{
	}

	void CC7NVRestoreRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE szBuffer[12] = {0};
		memcpy(szBuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if ( (0==szBuffer[4])&&(0==szBuffer[5])&&(0==szBuffer[6])&&(0==szBuffer[7]) )
			m_Success = true ;
		else
			m_Success = false ;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

//CC7DRMBackupRequest
	CC7DRMBackupRequest::CC7DRMBackupRequest (bool &bSuccess)
		: CDiagRequestBase (_T("CC7DRMBackupRequest"))
		, m_Success (bSuccess)
	{

		bSuccess = false;
        
		BYTE szRAWCommand[] = {0x80, 0x37, 0x05, 0x00};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CC7DRMBackupRequest::~CC7DRMBackupRequest()
	{
	}

	void CC7DRMBackupRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
        BYTE szBuffer[12] = {0};
        memcpy(szBuffer,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

        if ( (0==szBuffer[4])&&(0==szBuffer[5])&&(0==szBuffer[6])&&(0==szBuffer[7]) )
            m_Success = true ;
        else
            m_Success = false ;

        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

}

