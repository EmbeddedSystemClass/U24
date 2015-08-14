#include "stdafx.h"
#include "QualcommCmdModule.h"

/****************************************************************
CQualcommCmdModule
*****************************************************************/
HANDLE CQualcommCmdModule::m_hResourceContext = NULL;

CQualcommCmdModule::CQualcommCmdModule(UINT Port, bool UseQPST) 
: m_UseQPST( UseQPST ), 
  m_Port( Port )
{
	this->ConnectDevice();
}

CQualcommCmdModule::~CQualcommCmdModule(void)
{
	this->DisConnectDevice();
}

bool CQualcommCmdModule::ConnectDevice()
{
	if( !this->m_hResourceContext )
	{
		QLIB_SetLibraryMode( this->m_UseQPST ? QLIB_LIB_MODE_QPST : QLIB_LIB_MODE_QPHONEMS );

		this->m_hResourceContext = QLIB_ConnectServer( this->m_Port == -1 ? QLIB_COM_AUTO_DETECT : this->m_Port );

		if( this->m_hResourceContext && QLIB_IsPhoneConnected( this->m_hResourceContext ) )
		{
			QLIB_FlushRxBuffer( this->m_hResourceContext );
		}
	}

	return NULL != this->m_hResourceContext;
}

bool CQualcommCmdModule::DisConnectDevice()
{
	if( this->m_hResourceContext )
	{
		QLIB_DisconnectServer( this->m_hResourceContext );

		QLIB_DisconnectAllServers();

		this->m_hResourceContext = NULL;
	}

	return true;
}

bool CQualcommCmdModule::IsConnected()
{
	if( this->m_hResourceContext )
	{
		return 0 != QLIB_IsPhoneConnected( this->m_hResourceContext );
	}
	else
	{
		return this->ConnectDevice();
	}
}

bool CQualcommCmdModule::PreExe()
{
	return true;
}

bool CQualcommCmdModule::ExeCommand(CCmdPtr &Cmd)
{
	short ResponseSize = DIAG_MAX_PACKET_SIZE ;

    bool bRet = ( 0 != QLIB_SendSync( this->m_hResourceContext,
                                      (short)Cmd->GetCmdSize(),
                                      (unsigned char *)Cmd->GetCmd(),
                                      &ResponseSize,
                                      (unsigned char *)Cmd->GetResponse(),
                                      (unsigned long)Cmd->GetTimeOut() ) ) ;

    Cmd->SetResponseBufferSize(ResponseSize) ;

	return bRet ;
}

bool CQualcommCmdModule::PostExe()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
// QLib Function
//////////////////////////////////////////////////////////////////////////

bool CQualcommCmdModule::GetPhoneMode(unsigned long* piMode)
{
    if( this->m_hResourceContext )
    {
        return 0 != QLIB_GetPhoneOperatingMode( this->m_hResourceContext, piMode ) ;
    }
    else
    {
        return this->ConnectDevice() ;
    }
}

bool CQualcommCmdModule::SetPhoneMode(int iMode)
{
    if( this->m_hResourceContext )
    {
        return 0 != QLIB_DIAG_CONTROL_F( this->m_hResourceContext, iMode ) ;
    }
    else
    {
        return this->ConnectDevice() ;
    }
}

bool CQualcommCmdModule::SetBootMode(unsigned char bFTMmode, unsigned char bReset)
{
    if( this->m_hResourceContext )
    {
        return 0 != QLIB_ChangeFTM_BootMode( this->m_hResourceContext, bFTMmode, bReset ) ;
    }
    else
    {
        return this->ConnectDevice() ;
    }
}

bool CQualcommCmdModule::ReadNVItem(unsigned short iStartNV, unsigned short iEndNV, unsigned char *pBuffer)
{	
	if ( this->IsConnected() )
	{
		unsigned short iNVCount = iEndNV - iStartNV + 1;
		unsigned short unStatus;

		for(unsigned short i = 0;i < iNVCount;i++)
		{
			if (! QLIB_DIAG_NV_READ_F( this->m_hResourceContext,
									   iStartNV + i, 
									   pBuffer + i * DIAG_NV_ITEM_DATA_SIZE, 
									   DIAG_NV_ITEM_DATA_SIZE, 
									   &unStatus) )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool CQualcommCmdModule::WriteNVItem(unsigned short iStartNV, unsigned short iEndNV, unsigned char *pBuffer)
{
	if( this->IsConnected() )
	{
		unsigned short iNVCount = iEndNV - iStartNV + 1;
		unsigned short unStatus;

		for(unsigned short i = 0;i < iNVCount;i++)
		{
			if (! QLIB_DIAG_NV_WRITE_F( this->m_hResourceContext,
										iStartNV + i, 
										pBuffer + i * DIAG_NV_ITEM_DATA_SIZE, 
										DIAG_NV_ITEM_DATA_SIZE, 
										&unStatus) )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool CQualcommCmdModule::WriteNVItemByIndex(unsigned short iNV, unsigned char *pBuffer, int iLength, int iIndex)
{
	if( this->IsConnected() )
	{
		unsigned short unStatus;
		unsigned char pBufferTemp[DIAG_NV_ITEM_DATA_SIZE] = {0};

		if ( QLIB_DIAG_NV_READ_F( this->m_hResourceContext,
								  iNV,
								  pBufferTemp,
								  DIAG_NV_ITEM_DATA_SIZE,
								  &unStatus) )
		{
			::memcpy_s(pBufferTemp + iIndex, iLength, pBuffer, iLength);

			return 0 != QLIB_DIAG_NV_WRITE_F( this->m_hResourceContext, iNV, pBufferTemp, DIAG_NV_ITEM_DATA_SIZE, &unStatus);
		}
	}

	return false;
}

bool CQualcommCmdModule::ReadNVItemByIndex(unsigned short iNV, unsigned char *pBuffer, int iLength, int iIndex)
{
	if( this->IsConnected() )
	{
		unsigned short unStatus;
		unsigned char pBufferTemp[DIAG_NV_ITEM_DATA_SIZE] = {0};

		if ( QLIB_DIAG_NV_READ_F( this->m_hResourceContext,
								  iNV,
								  pBufferTemp,
								  DIAG_NV_ITEM_DATA_SIZE,
								  &unStatus) )
		{
			::memcpy_s( pBuffer, iLength, pBufferTemp + iIndex, iLength );

			return true;
		}
	}

	return false;
}

bool CQualcommCmdModule::EFS2_SyncWithWait(unsigned char* sFilePath, unsigned long iTimeOut_ms, unsigned char* piExecutionStatus)
{
	if( this->IsConnected() )
		return 0 != QLIB_EFS2_SyncWithWait( this->m_hResourceContext, sFilePath, iTimeOut_ms, piExecutionStatus ) ;

	return false;
}

bool CQualcommCmdModule::EFS2_GoldenCopy()
{
	if( !this->IsConnected() )
	{
		return false;
	}

	if (this->m_hResourceContext != NULL)
	{
		unsigned long ulErrNo;
		char szFilePath[8];
		memset(szFilePath, 0, 8);
		sprintf(szFilePath, "/");

		if (! QLIB_EFS2_DIAG_MAKE_GOLDEN_COPY(this->m_hResourceContext, (unsigned char*)szFilePath, &ulErrNo))
		{
			printf("QLIB_EFS2_DIAG_MAKE_GOLDEN_COPY return FALSE");
			return false;
		}

		if (ulErrNo != 0)
		{
			char szMsg[256];
			memset(szMsg, 0, 256);
			sprintf(szMsg, "QLIB_EFS2_DIAG_MAKE_GOLDEN_COPY fail, Error number = %d", ulErrNo);
			printf(szMsg);

			return false;
		}

		
	}

	return true;
}

bool CQualcommCmdModule::WLAN_GEN6_START(int wlanID)
{
    if ( this->IsConnected() )
        return 0 != QLIB_FTM_WLAN_GEN6_START(this->m_hResourceContext, wlanID);

    return false ;
}

bool CQualcommCmdModule::WLAN_GEN6_GET_MAC_ADDR(unsigned char *macAddr)
{
    if ( this->IsConnected() )
        return 0 != QLIB_FTM_WLAN_GEN6_GET_MAC_ADDR(this->m_hResourceContext, macAddr);

    return false ;
}

bool CQualcommCmdModule::WLAN_GEN6_SET_MAC_ADDR(unsigned char *macAddr)
{
    if ( this->IsConnected() )
        return 0 != QLIB_FTM_WLAN_GEN6_SET_MAC_ADDR(this->m_hResourceContext, macAddr);

    return false ;
}




//////////////////////////////////////////////////////////////////////////
//  CQualcommCmdContainer
//////////////////////////////////////////////////////////////////////////

CQualcommCmdContainer::CQualcommCmdContainer(BYTE *Cmd, UINT CmdSize, bool HasReturnValue, UINT TimeOut)
:	m_Cmd( NULL ),
	m_TimeOut( TimeOut ),
	m_HasReturnValue( HasReturnValue ),
	m_CmdSize( CmdSize )
{
	if( 0 < this->m_CmdSize )
	{
		this->m_Cmd = new BYTE [this->m_CmdSize];
		memcpy((void *)this->m_Cmd, (void *)Cmd, this->m_CmdSize);
	}

	::ZeroMemory((void *)this->m_Response, DIAG_MAX_PACKET_SIZE);
}

CQualcommCmdContainer::~CQualcommCmdContainer()
{
	if(NULL != this->m_Cmd)
	{
		delete this->m_Cmd;
		this->m_Cmd = NULL;
	}
}

const BYTE *CQualcommCmdContainer::GetCmd() const
{
	return this->m_Cmd;
}

const BYTE *CQualcommCmdContainer::GetResponse() const
{
	return this->m_Response;
}

const UINT CQualcommCmdContainer::GetCmdSize() const
{
	return this->m_CmdSize;
}

bool CQualcommCmdContainer::HasReturnValue() const
{
	return this->m_HasReturnValue;
}

UINT CQualcommCmdContainer::GetTimeOut() const
{
	return this->m_TimeOut;
}

void CQualcommCmdContainer::SetResponseBufferSize(UINT size)
{
	m_ResponseSize = size ;
}

UINT CQualcommCmdContainer::GetResponseSize() const
{
    return m_ResponseSize / sizeof(_T('0x00')) ;
}