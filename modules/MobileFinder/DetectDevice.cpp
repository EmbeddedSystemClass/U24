#include "stdafx.h"
#include "DetectDevice.h"
#include "..\Common\CommonUtil\LogPolicy.h"
#include "..\Common\CommonUtil\unicodeutility.h"

using namespace QSYNC_LIB;

const UINT WM_MODEM_FOUND = 9001 ;
const UINT WM_DIAG_FOUND = 9002 ;
const UINT WM_ADB_FOUND = 9003 ;

#define ANDROID_USB_CLASS_ID \
{0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};
#define QUALCOMM_DIAG_USB_CLASS_ID \
{0x86e0d1e0L, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}};
#define QUALCOMM_MODEM_USB_CLASS_ID \
{0x2c7089aa, 0x2e0e,0x11d1,{0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};
//Qualcomm DIAG port setup class GUID {4D36E978-E325-11CE-BFC1-08002BE10318}
//Qualcomm DIAG port interface class GUID {0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}
//Qualcomm Modem port setup class GUID {4D36E96D-E325-11CE-BFC1-08002BE10318}
//Qualcomm Modem port interface class GUID {0x2c7089aa, 0x2e0e,0x11d1,0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}
//Android setup class GUID {F72FE0D4-CBCB-407d-8814-9ED673D0DD6B}
//Android interface class GUID {0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};
//USB Composite Device setup class GUID {36FC9E60-C465-11CF-8056-444553540000}

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

const bool CDectectDevice::IsADBDLDevice(const TSTRING& strSymbolicName)
{	
    TRACE_FILE_LINE_INFO();

    bool bRes = false;
    HANDLE hComm = ::CreateFile(strSymbolicName.c_str(),
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);
    bRes = (hComm != INVALID_HANDLE_VALUE);
	if(bRes)
	{
		DWORD dwWritten = 0;
		//const char szSendBuffer[] = {"serialno"};
		const char szSendBuffer[] = {"version-baseband"};
		//const char szSendBuffer[] = {"signature"};
		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(overlapped));
		overlapped.hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
		bRes = TRUE == ::WriteFile(hComm,szSendBuffer,strlen(szSendBuffer),&dwWritten,&overlapped);
		DWORD dwWaitResult = ::WaitForSingleObject(overlapped.hEvent,1000);
		::Sleep(100);
		if(dwWaitResult != WAIT_TIMEOUT)
		{
			DWORD dwRead = 0;
			char szReadBuffer[1024] = {0};
			OVERLAPPED overlapped;
			ZeroMemory(&overlapped, sizeof(overlapped));
			BOOL bReadSuccess = ::ReadFile(hComm,szReadBuffer,1024,&dwRead,&overlapped);
			//if(::strcmp(szReadBuffer,"FAILsignature not 256 bytes long") == 0)
			if(::strcmp(szReadBuffer,"FAILinvalid command") == 0)
				bRes = true;
		};
	}
    if(hComm) 
		::CloseHandle(hComm);
    return bRes;
};


const bool CDectectDevice::IsDiag(const TSTRING& strCOMPort)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);

    bool bRes = false;

    //若不等一下，port剛找到時也許還問不到其他資訊，例如modal name.
    ::Sleep(300);

    CSerialPort comport;

    if(m_bEnable && comport.OpenUartPort(strCOMPort,CSerialPort::CSPDIAGCommandMode))
//	if(m_bEnable && comport.OpenPort(strCOMPort,CSerialPort::CSPDIAGCommandMode))
   {
        CBufferPtr ptBuffer = new CBuffer;

		BYTE szProbe[] = {0x61,0x74,0x0d,0x1c,0xd5,0x7e,0x06,0x4E,0x95,0x7E};
//		char szProbe[] = {	0x7e,0x06,0x4E,0x95,0x7E};
#define	DETECT_DIAG_RSP_6250		("\x13\x61\x74\x0D")
#define   DETECT_DIAG_RSP_6280      ("\x13\x60\x00\x68\xC9")
#define	DETECT_DL					                ("\x7E\x02\x6A\xD3\x7E")
#define   DETECT_DL2					            ("\x7E\x03\x00\x06\x9E\x4C\x7E")
#define   DETECT_AT					                ("\x0D\x0A\x4F\x4B\x0D\x0A")
#define	ADB_FAIL_CMD							("\x46\x41\x49\x4C\x69\x6E\x76\x61\x6C\x69\x64\x20\x63\x6F\x6D\x6D\x61\x6E\x64")

		CBufferPtr ptBufferSend = new CBuffer(10,szProbe);

		if(comport.WriteAndRead((const BYTE *)ptBufferSend->GetData(),ptBufferSend->GetLength(),ptBuffer,1000))		
		{
			if(::memcmp(DETECT_DIAG_RSP_6250,ptBuffer->GetData(),4) == 0)
				bRes = true;//m_bIsDiagMode = true;
			else if(::memcmp(DETECT_DIAG_RSP_6280,ptBuffer->GetData(),5) == 0)
				bRes = true;//m_bIsDLMode = true;
			else if(::memcmp(DETECT_DL,ptBuffer->GetData(),5) == 0)
				bRes = true;//m_bIsDLMode = true;
			else if(::memcmp(DETECT_DL2, ptBuffer->GetData(),7) == 0)
				bRes = true;//m_bIsDLMode = true;
			else if(::memcmp(DETECT_AT, ptBuffer->GetData(),6) == 0)
				bRes = false;//m_bIsATMode = true;
			//else if(::memcmp(szModemReplyBuf, ptBuffer->GetData(),9) == 0)
			//	m_bIsATMode = true;
			else if(::memcmp(ADB_FAIL_CMD, ptBuffer->GetData(),19) == 0)
				bRes = false;//m_bIsADBMode = true;
		}

        comport.ClosePort();
    }

    return bRes;
}

const bool CDectectDevice::IsMODEM(const TSTRING& strCOMPort,TSTRING& strModule)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);

    //若不等一下，port剛找到時也許還問不到其他資訊，例如modal name.
    ::Sleep(300);

    bool bRes = false;

    CSerialPort comport;

    if(m_bEnable && comport.OpenPort(strCOMPort,CSerialPort::CSPATCommandMode))
    {
        CBufferPtr ptBuffer = new CBuffer;

        /*
        000075: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +1.7811474
        Pipe Handle: 0x8a23a2d4 (Endpoint Address: 0x2)
        Send 0x9 bytes to the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000076: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +0.0
        Pipe Handle: 0x8a23a2b4 (Endpoint Address: 0x82)
        Get 0x9 bytes from the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000077: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +0.0
        Pipe Handle: 0x8a23a2b4 (Endpoint Address: 0x82)
        Get 0x16 bytes from the device:
        0D 0A 2B 43 50 49 4E 3A 20 52 45 41 44 59 0D 0A   ..+CPIN: READY..
        0D 0A 4F 4B 0D 0A                                 ..OK..
        000078: Bulk or Interrupt Transfer (UP), 20.02.2008 
        */
        /*
        000436: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6688500 +0.0
        Pipe Handle: 0x8946efdc (Endpoint Address: 0x2)
        Send 0x9 bytes to the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000437: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6844741 +0.0156241
        Pipe Handle: 0x8946efbc (Endpoint Address: 0x82)
        Get 0x9 bytes from the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000438: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6844741 +0.0
        Pipe Handle: 0x8946efbc (Endpoint Address: 0x82)
        Get 0x1b bytes from the device:
        0D 0A 2B 43 4D 45 20 45 52 52 4F 52 3A 20 53 49   ..+CME ERROR: SI
        4D 20 66 61 69 6C 75 72 65 0D 0A                  M failure..
        */
        if (comport.WriteAndRead("AT+CPIN?\r",ptBuffer,1000))
            bRes = CSerialPort::IsCompleteATPackage(ptBuffer);

        if (bRes && comport.WriteAndRead("AT+CGMM\r",ptBuffer,1000))
        {
            if(CSerialPort::IsCompleteATPackage(ptBuffer))
            {								
                CBufferPtr ptDataBuffer = new CBuffer;
                CSerialPort::DecodeATPackage(ptBuffer,ptDataBuffer);
                string str;
                for(ULONGLONG i=0;i<ptDataBuffer->GetLength();i++)
                {
                    const char *pData = ptDataBuffer->GetData();
                    str += pData[i];
                }
#ifdef _UNICODE
                wchar_t* wszBuffer = new wchar_t[str.size()+1];
                memset(wszBuffer,0,sizeof(wchar_t)*(str.size()+1));
                AnsiToUnicodeBuffer(str.c_str(),wszBuffer,(str.size()+1)*sizeof(wchar_t));
                strModule = wszBuffer;
                delete [] wszBuffer;
#else
                strModule = str;
#endif
                bRes = true;
            }

        }
        comport.ClosePort();
    }

    return bRes;
}

BOOL CDectectDevice::OnDeviceChange(UINT nEventType,DWORD dwData)
{
    PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)dwData;
    TCHAR szMsg[80] = {0};

    CString strMsg;
    switch(nEventType)
    {
    case DBT_DEVICEARRIVAL:
    case DBT_DEVICEREMOVECOMPLETE:
        switch(lpdb->dbch_devicetype)
        {
#if(WINVER >= 0x040A)
    case DBT_DEVTYP_DEVICEINTERFACE:
        GetDevice((PDEV_BROADCAST_DEVICEINTERFACE)lpdb,nEventType);
        break;		
    case DBT_DEVTYP_HANDLE:
        strMsg = _T("DBT_DEVTYP_OEM");
        break;
#endif
    case DBT_DEVTYP_OEM:
        strMsg = _T("DBT_DEVTYP_HANDLE");
        break;
    case DBT_DEVTYP_PORT:
        {
            /*PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
            if (lpdbv)
            {
            DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_PORT);
            ptdevData->m_strPort = lpdbv->dbcp_name;
            if(nEventType == DBT_DEVICEREMOVECOMPLETE)
            {
            RemoveDevice(ptdevData);
            }
            else if(nEventType == DBT_DEVICEARRIVAL)
            {
            ptdevData->m_strPort = lpdbv->dbcp_name;
            if(IsMODEM(lpdbv->dbcp_name,ptdevData->m_strExtName))
            AddDevice(ptdevData);
            }
            }*/
        }
        break;
    case DBT_DEVTYP_VOLUME:
        {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
            if (lpdbv)
            {
                //DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_VOLUME);
                //wsprintf (szMsg, _T("%c:"),FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                //ptdevData->m_strDriver = szMsg;
                //if(nEventType == DBT_DEVICEARRIVAL)
                //	AddDevice(ptdevData);
                //if(nEventType == DBT_DEVICEREMOVECOMPLETE)
                //	RemoveDevice(ptdevData);
            }
        }
        break;
    default:
        strMsg = _T("DBT_DEVICEARRIVAL or DBT_DEVICEREMOVECOMPLETE");
        break;
        };
        break;
    case DBT_DEVICEQUERYREMOVE:   
        strMsg = _T("Permission to remove a device is requested. Any application can deny this request and cancel the removal. ");
        break;
    case DBT_DEVICEQUERYREMOVEFAILED:   
        strMsg = _T("Request to remove a device has been canceled. ");
        break;
    case DBT_DEVICEREMOVEPENDING:   
        strMsg = _T("Device is about to be removed. Cannot be denied. ");
        break;
    case DBT_DEVICETYPESPECIFIC:   
        strMsg = _T("Device-specific event. ");
        break;
    case DBT_CONFIGCHANGED:   
        strMsg = _T("Current configuration has changed. ");
        break;
    case DBT_DEVNODES_CHANGED:   
        strMsg = _T("Device node has changed. ");
        break;
    default:
        strMsg = _T("Unknow");
        break;
    };
    TRACE(strMsg + CString(_T("\n")));
    return TRUE;
}

DWORD CDectectDevice::LocatePort(HANDLE hUSBHUB, HDEVINFO hDeviceInfoSet, PSP_DEVINFO_DATA pspDeviceInfoData)
{
    TRACE_FILE_LINE_INFO();

    TCHAR TraceBuffer[256] = {0};
    DWORD nPort = 0, dwType, dwRequiredSize = 0, nBytesReturned = 0;

    TSTRING sDriverClassGUID;

    // Getting SPDRP_SERVICE
    SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, pspDeviceInfoData, SPDRP_DRIVER , &dwType, NULL, 0, &dwRequiredSize);
    if(dwRequiredSize > 0)
    {
        dwRequiredSize += sizeof(TCHAR);
        PBYTE pbyDriver = new BYTE[dwRequiredSize];
        if(SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, pspDeviceInfoData, SPDRP_DRIVER , &dwType, pbyDriver, dwRequiredSize, &dwRequiredSize))
        {
            ::_tcsupr_s((LPTSTR) pbyDriver,dwRequiredSize/sizeof(TCHAR));
            sDriverClassGUID.assign((LPTSTR) pbyDriver);
        }
        delete [] pbyDriver;
    }

    //Get Node Information from USB HUB to know how many ports are installed
    USB_NODE_INFORMATION NodeInformation;
    ZeroMemory(&NodeInformation, sizeof(NodeInformation));
    if(!DeviceIoControl(hUSBHUB, IOCTL_USB_GET_NODE_INFORMATION, &NodeInformation, sizeof(NodeInformation), &NodeInformation, sizeof(NodeInformation), &nBytesReturned, NULL))
    {
        TRACE_FILE_LINE_INFO();
        return 0;
    }

    for(DWORD PortIndex = 1; PortIndex <= NodeInformation.u.HubInformation.HubDescriptor.bNumberOfPorts; PortIndex++)
    {
        //////////////////////////////////////////////////////////////////////////
        // Get Driver Key Name to compare with sDriverClassGUID
        //////////////////////////////////////////////////////////////////////////
        USB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyName;
        ZeroMemory(&driverKeyName, sizeof(driverKeyName));
        driverKeyName.ConnectionIndex = PortIndex;

        if(!DeviceIoControl(hUSBHUB, 
            IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME, 
            &driverKeyName,	
            sizeof(driverKeyName), 
            &driverKeyName, 
            sizeof(driverKeyName), 
            &nBytesReturned, 
            NULL))
        {
            TRACE_FILE_LINE_INFO();
            continue;
        }
        else
        {
            nBytesReturned = driverKeyName.ActualLength;

            //check if necessary memory is valid
            if(nBytesReturned < sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME))
                continue;

            PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME) new BYTE[nBytesReturned];
            driverKeyNameW->ConnectionIndex = PortIndex;
            if(!DeviceIoControl(hUSBHUB, 
                IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME, 
                driverKeyNameW,	
                nBytesReturned, 
                driverKeyNameW, 
                nBytesReturned, 
                &nBytesReturned, 
                NULL))
            {
                TRACE_FILE_LINE_INFO();
                delete [] driverKeyNameW;
                continue;
            }
            else
            {
                USES_CONVERSION;
                //Compare the ClassGUID to check if the devices are the same.
                if(sDriverClassGUID == W2CT(driverKeyNameW->DriverKeyName))
                {
                    nPort = PortIndex;
                    delete [] driverKeyNameW;
                    break;
                }
            }
            delete [] driverKeyNameW;
        }
    }
    return(nPort);
}

bool CDectectDevice::IsVaildVIDPID(USHORT idVendor,USHORT idProduct)
{
    bool bRes = false;

    if( idVendor== 0x1D45 
        || idVendor == 0x04a5 
        || idVendor == 4597 
        || idVendor == 0x22B8 //Motorola
		|| idVendor == 0x1DA5 //Qisda		
        || // Siemens device Vid_11f5
		( idVendor == 0x413C && idProduct == 0xB007 ) ||//Dell Athena project
        ( idVendor == 0x05C6 && idProduct == 0x6000 ) ||
        ( idVendor == 0x05C6 && idProduct == 0x9004 ) ||
		( idVendor == 0x05C6 && idProduct == 0x3199 ) ||
		( idVendor == 0x05C6 && idProduct == 0x900E ) ||
        ( idVendor == 0x05C6 && idProduct == 0x9002 ) ||//Qualcomm's download port
        ( idVendor == 0x22B8 && idProduct == 0x2D50 ) ||
        ( idVendor == 0x22B8 && idProduct == 0x2D53 ) ||
        ( idVendor == 0x1DA5 && idProduct == 0x4527 ) ||
		( idVendor == 0x1DA5 && idProduct == 0x4529 ) ||
		( idVendor == 0x05C6 && idProduct == 0x9018 )
        ) //Qualcomm device Vid_05c6&Pid_6000
    {
        bRes =  true;
    }

    return bRes;
}

void CDectectDevice::AddUSBNotify(const TSTRING& strDeviceClass,const TSTRING& strEvent)
{
	DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_DEVNODE,strDeviceClass);
	ptdevData->m_strDeviceClass = strDeviceClass;

	AddDevice(ptdevData,strEvent);
}

void CDectectDevice::AddPortNotify(const TSTRING& strPort,const TSTRING& strDeviceClass,const TSTRING& strEvent)
{
    if(!strPort.empty())
    {
        DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_PORT,strPort);
        ptdevData->m_strDeviceClass = strDeviceClass;

        //Sepecially to get module name for QSyncer
        if(strEvent == DEVICE_CONNECT)//Modem port only
            IsMODEM(strPort,ptdevData->m_strExtName);

        AddDevice(ptdevData,strEvent);
    }
}

//dave
void CDectectDevice::AddStorageNotify(const TSTRING& strDeviceClass, const TSTRING& strEvent)
{
	if(!strDeviceClass.empty())
    {
        DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_VOLUME, strDeviceClass);
        AddDevice(ptdevData,strEvent);
    }
}
//dave

void CDectectDevice::ParserDBCCName(TCHAR dbcc_name[1],USHORT& idVendor,USHORT& idProduct,TSTRING& strPara)
{
    // \\?\usb#vid_22b8&pid_2d01#6&.......
    TCHAR *szToken;
    TCHAR *szNextToken;
    ::_tcstok_s(dbcc_name,_T("_&"),&szNextToken);
    szToken = ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
    idVendor = (USHORT)HEX2Ineger(szToken);
    ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
    szToken = ::_tcstok_s(szNextToken,_T("_&#"),&szNextToken);
    idProduct = (USHORT)HEX2Ineger(szToken);
    szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);
    strPara = szToken;
}

void CDectectDevice::AddDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv)
{
    TRACE_FILE_LINE_INFO();

    if(lpbv)
    {
        HDEVINFO  hDeviceInfoSet = ::SetupDiCreateDeviceInfoList(NULL,NULL);
        if(hDeviceInfoSet)
        {
            SP_DEVICE_INTERFACE_DATA spDeviceInterfaceData;
            ZeroMemory(&spDeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
            spDeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

            if(::SetupDiOpenDeviceInterface(hDeviceInfoSet, lpbv->dbcc_name, 0, &spDeviceInterfaceData))
            {
                SP_DEVINFO_DATA spDeviceInfoData;
                ZeroMemory(&spDeviceInfoData, sizeof(SP_DEVINFO_DATA));
                spDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

                DWORD dwDeviceInterfaceDetailDataSize = (offsetof( SP_DEVICE_INTERFACE_DETAIL_DATA, DevicePath) + sizeof(TCHAR));
                DWORD dwRequiredSize = 0;

                BOOL bRes = ::SetupDiGetDeviceInterfaceDetail(hDeviceInfoSet, &spDeviceInterfaceData, NULL, 0, &dwRequiredSize, &spDeviceInfoData);

                PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA) new BYTE[dwRequiredSize];
                ZeroMemory(pDeviceInterfaceDetail, dwRequiredSize);
                pDeviceInterfaceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
                bRes = ::SetupDiGetDeviceInterfaceDetail(hDeviceInfoSet, &spDeviceInterfaceData, pDeviceInterfaceDetail, dwRequiredSize, &dwRequiredSize, &spDeviceInfoData);

                USHORT idVendor;
                USHORT idProduct;
                TSTRING strPara;
                ParserDBCCName(pDeviceInterfaceDetail->DevicePath,idVendor,idProduct,strPara);

                if(IsVaildVIDPID(idVendor,idProduct))
				{
					//For Modem port notify	
					TSTRING strDeviceSymbolic;
					TSTRING strUSBModemPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Modem"),_T("Device Parameters"),_T("PortName"),strDeviceSymbolic);
					AddPortNotify(strUSBModemPort,strDeviceSymbolic,DEVICE_CONNECT);

					//For Diag port notify
					TSTRING strUSBDiagPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Ports"),_T("Device Parameters"),_T("PortName"),strDeviceSymbolic);
					//if(IsDiag(strUSBDiagPort))
						AddPortNotify(strUSBDiagPort,strDeviceSymbolic,DEVICE_DIAG_CONNECT);

					//For ADB notify
					TSTRING strUSB = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("USB"),_T("Control"),_T("ActiveService"),strDeviceSymbolic);
					if(strUSB == _T("androidusb"))
						AddUSBNotify(strDeviceSymbolic,DEVICE_ADB_CONNECT);
				}

				BYTE* pTemp = (BYTE*)pDeviceInterfaceDetail;
				delete [] pTemp;
			}
			::SetupDiDestroyDeviceInfoList(hDeviceInfoSet);

			//dave
			CString cstr_device_path = lpbv->dbcc_name;
			
			if(-1 < cstr_device_path.Find("Qualcomm") &&
			   -1 < cstr_device_path.Find("MMC_Storage"))
			{
				TSTRING str_device_path = cstr_device_path.GetBuffer();
				AddStorageNotify(str_device_path, DEVICE_STORAGE_CONNECT);
			}
			
			//dave
		}
	}
}

void CDectectDevice::RemoveDevice(const TSTRING& strDriverClass)
{
    TRACE_FILE_LINE_INFO();

    for(DeviceMapIter Iter = m_Devices.begin();Iter != m_Devices.end();Iter++)
    {
        if(Iter->second->m_strDeviceClass == strDriverClass)
        {
            RemoveDevice(Iter->second,DEVICE_DISCONNECT);
            break;
        }
    }
}

const TSTRING CDectectDevice::GetCOMPort(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData,const TSTRING& sSymbolicName)
{
    //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_22b8&Pid_2d01\5&15143e4d&0&2
    TRACE_FILE_LINE_INFO();

    TSTRING sUSBPortName;

    HANDLE hUSBHUB = ::CreateFile(sSymbolicName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(hUSBHUB != INVALID_HANDLE_VALUE)
    {
        int nDevAddress = 0;
        if(!::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, SPDRP_ADDRESS , NULL, (unsigned char *)&nDevAddress, 4, NULL))
        {
            nDevAddress = LocatePort(hUSBHUB, hDeviceInfoSet, &spDeviceInfoData);
        }											
        else
        {												
            //Get Descriptors from USB HUB on port specified in nDevAddress
            DWORD nBytesReturned;
            USB_NODE_CONNECTION_INFORMATION ConnectionInformation;
            ZeroMemory(&ConnectionInformation, sizeof(ConnectionInformation));
            ConnectionInformation.ConnectionIndex = nDevAddress;

            if(::DeviceIoControl(hUSBHUB, IOCTL_USB_GET_NODE_CONNECTION_INFORMATION, &ConnectionInformation,	sizeof(ConnectionInformation), &ConnectionInformation, sizeof(ConnectionInformation), &nBytesReturned, NULL) 
                && ConnectionInformation.ConnectionStatus == DeviceConnected)
            {
                if(IsVaildVIDPID(ConnectionInformation.DeviceDescriptor.idVendor,ConnectionInformation.DeviceDescriptor.idProduct ))
                {							
                    sUSBPortName = GetQualcommPort(ConnectionInformation.DeviceDescriptor.idProduct,ConnectionInformation.DeviceDescriptor.idVendor);
                    if(sUSBPortName.size() == 0)
                    {
                        HKEY hKey = ::SetupDiOpenDevRegKey(hDeviceInfoSet, &spDeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_READ);
                        if(hKey != INVALID_HANDLE_VALUE)
                        {
                            DWORD dwType = 0, dwRequiredSize = 0;
                            TSTRING sSymbolicName = GetKeyValue(hKey, _T("SymbolicName"),dwType,dwRequiredSize);
                            sUSBPortName = GetKeyValue(hKey, _T("PortName"),dwType,dwRequiredSize);
                            ::RegCloseKey(hKey);
                        }
                    }
                }
            }
        }
        ::CloseHandle(hUSBHUB);
    }

	if(sUSBPortName.empty())
	{
		TSTRING strReg = _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\");		

		TCHAR* dbcc_name = new TCHAR[sSymbolicName.size() + 1];
		_tcscpy(dbcc_name,sSymbolicName.c_str());
		// \\?\usb#vid_22b8&pid_2d01#6&.......
		TCHAR *szToken;
		TCHAR *szNextToken;
		szToken = ::_tcstok_s(dbcc_name,_T("#"),&szNextToken);
		szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);
		strReg += szToken;
		strReg += _T("\\");
		szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);
		strReg += szToken;
		strReg += _T("\\Device Parameters");
		delete [] dbcc_name;

		AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);
		sUSBPortName = hKey.GetValue(_T("PortName"));
	}
	return sUSBPortName;
}

bool CDectectDevice::IsReallyCOMPort(const TSTRING& strPortName)
{	
    TRACE_FILE_LINE_INFO();

    bool bRes = false;
    TSTRING strPort;
    strPort= _TC("//./") +strPortName;
    HANDLE hComm = ::CreateFile(strPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);
    bRes = (hComm != INVALID_HANDLE_VALUE);
    if(hComm) ::CloseHandle(hComm);
    return bRes;
};

TSTRING CDectectDevice::GetQualcommPortName(HKEY hKey,const TSTRING& strSubKey)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strPort;
    TSTRING strDeviceReg = strSubKey + _T("\\Device Parameters");
    HKEY hDeviceKey = NULL;
    if(::RegOpenKey(hKey,strDeviceReg.c_str(),&hDeviceKey) == ERROR_SUCCESS)
    {
#define MAX_VALUE_LENGTH 16383
        TCHAR szBuffer[MAX_VALUE_LENGTH] = {0};
        DWORD dwBufLen=MAX_VALUE_LENGTH*sizeof(TCHAR);
        LONG lRet = ::RegQueryValueEx( hDeviceKey, _T("PortName"), NULL, NULL,(LPBYTE) szBuffer, &dwBufLen);
        if( (lRet == ERROR_SUCCESS) && (dwBufLen < MAX_VALUE_LENGTH*sizeof(TCHAR)) )
            strPort = szBuffer;
        ::RegCloseKey( hDeviceKey );
    }
    return strPort;
}

bool IsWinXP()
{
    bool bIsWinXP = false;

    DWORD dwVersion = ::GetVersion();

    DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwWindowsMajorVersion == 5)
    {
        if(dwWindowsMinorVersion == 1)
        { //Windows XP
            bIsWinXP =  true;
        }
    }
    return bIsWinXP;
}

bool IsWin2000()
{
    bool bIsWin2000 = false;

    DWORD dwVersion = ::GetVersion();

    DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwWindowsMajorVersion == 5)
    {
        if(dwWindowsMinorVersion == 0)
        { //Windows 2000
            bIsWin2000 =  true;
        }
    }

    return bIsWin2000;
}

TSTRING CDectectDevice::GetUSBDeviceInfo(const USHORT idProduct,const USHORT idVendor,const TSTRING& strPara,const TSTRING& strClass,const TSTRING& strSubFolder,const TSTRING& strField,TSTRING& strSymbolic)
{
    TSTRING strValue;

    TSTRING strReg = _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%04x&Pid_%04x");

    CString strFormat;
    strFormat.Format(strReg.c_str(),idVendor,idProduct);

    strReg = strFormat;
    strReg += _T("\\") + strPara;

    AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);
    TSTRING strDeviceClass = hKey.GetValue(_T("Class"));
	TSTRING strDeviceClassGUID = hKey.GetValue(_T("ClassGUID"));
    TSTRING strDeviceDesc = hKey.GetValue(_T("DeviceDesc"));
    TSTRING strParentIdPrefix = hKey.GetValue(_T("ParentIdPrefix"));//If it's a composite device

	if(strDeviceClass == strClass)
	{
		if(!strSubFolder.empty())
			strReg += _T("\\") + strSubFolder;
		AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);

		strValue = hKey.GetValue(strField);		

		if(strValue.empty() == false)
		{
			//\\?\USB#Vid_413c&Pid_b007#5&1b8ed9a6&0&8#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
			//\\?\usb#vid_413c&pid_b007&mi_01#6&23c90afa&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}
			strSymbolic.clear();
			TCHAR szSymboilcFormat[] = {_T("\\\\?\\usb#vid_%04x&pid_%04x%s#%s%s#%s")};	
			CString strDeviceSymbolic;
			strDeviceSymbolic.Format(szSymboilcFormat
				,idVendor
				,idProduct
				,_T("")
				,strPara.c_str()
				,_T("")
				,strDeviceClassGUID.c_str());
			strSymbolic = strDeviceSymbolic;
		}
	}

    if(strValue.empty() && strParentIdPrefix.empty() != true)
    {		
        //Find other USB Composite Devices
        //USB\Vid_22b8&Pid_2d01&MI_00\7&105cb4db&0&0000
        for(int i=0;i<5;i++)
        {			
			TSTRING strComposite;
			CString strCompositeMI;
			CString strCompositeNumber;
			if(IsWin2000())
			{
				strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%d");
				strCompositeMI.Format(_T("&MI_%02d"),i);
				strCompositeNumber.Format(_T("&%d"),i);
			}
			else if(IsWinXP())
			{
				strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%04d");
				strCompositeMI.Format(_T("&MI_%02d"),i);
				strCompositeNumber.Format(_T("&%04d"),i);
			}
			else//Vista and latest
			{
				strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%04d");
				strCompositeMI.Format(_T("&MI_%02d"),i);
				strCompositeNumber.Format(_T("&%04d"),i);
			}

            CString strNewComposite;
            strNewComposite.Format(strComposite.c_str(),i,i);
            TSTRING strReg = strFormat + strNewComposite;

            AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);
            TSTRING strDeviceClass = hKey.GetValue(_T("Class"));
			TSTRING strDeviceClassGUID = hKey.GetValue(_T("ClassGUID"));
            if(strDeviceClass == strClass)
            {
                if(!strSubFolder.empty())
                    strReg += _T("\\") + strSubFolder;
                AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);

                strValue = hKey.GetValue(strField);

				//\\?\USB#Vid_413c&Pid_b007#5&1b8ed9a6&0&8#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
				//\\?\usb#vid_413c&pid_b007&mi_01#6&23c90afa&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}
				strSymbolic.clear();
				TCHAR szSymboilcFormat[] = {_T("\\\\?\\usb#vid_%04x&pid_%04x%s#%s%s#%s")};	
				CString strDeviceSymbolic;
				strDeviceSymbolic.Format(szSymboilcFormat
					,idVendor
					,idProduct
					,strCompositeMI
					,strParentIdPrefix.c_str()
					,strCompositeNumber
					,strDeviceClassGUID.c_str());
				strSymbolic = strDeviceSymbolic;

                break;
            }
        }
    }
	
    return strValue;
}

TSTRING CDectectDevice::GetQualcommPort(const USHORT idProduct,const USHORT idVendor)
{
    //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_22b8&Pid_2d01\5&15143e4d&0&2
    TRACE_FILE_LINE_INFO();

    ::Sleep(300);

    TSTRING strCOMPort;

    TSTRING strReg = _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%04x&Pid_%04x&MI_00");

    CString strFormat;
    strFormat.Format(strReg.c_str(),idVendor,idProduct);

    HKEY hKey;
    LONG lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,strFormat,0, KEY_READ, &hKey );	
    if( lRet == ERROR_SUCCESS )
    {
        int nTestIndex = 0;
        TSTRING strSubKey = GetSubKey(hKey,nTestIndex);
        while(strSubKey.size() > 0)
        {
            TSTRING strPort;					
            strPort = GetQualcommPortName(hKey,strSubKey);
            if(strPort.size() > 0)
            {
                if(IsReallyCOMPort(strPort))
                {
                    TSTRING strModule;
                    if(IsMODEM(strPort,strModule))
                    {
                        strCOMPort = strPort;
                        break;
                    }
                }
            }
            nTestIndex++;
            strSubKey = GetSubKey(hKey,nTestIndex);
        }
        ::RegCloseKey( hKey );
    }

    return strCOMPort;
}

TSTRING CDectectDevice::GetSubKey(HKEY hKey,int nIndex)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strKey;			
#define MAX_KEY_LENGTH 255
    TCHAR szBuffer[MAX_KEY_LENGTH] = {0};
    DWORD dwBufLen=MAX_KEY_LENGTH*sizeof(TCHAR);
    FILETIME ftLastWriteTime;
    if(::RegEnumKeyEx(hKey, nIndex,szBuffer,&dwBufLen,NULL,NULL,NULL,&ftLastWriteTime) == ERROR_SUCCESS)
        strKey = szBuffer;
    return strKey;
}
const TSTRING CDectectDevice::GetKeyValue(HKEY hKey,const TSTRING& strName,DWORD& dwType,DWORD& dwRequiredSize)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strValue;
    if(::RegQueryValueEx(hKey, strName.c_str(), NULL, NULL, NULL, &dwRequiredSize) == ERROR_SUCCESS)
    {
        PBYTE pbyPortName = new BYTE[dwRequiredSize+2];
        ::ZeroMemory(pbyPortName,dwRequiredSize+2);
        if(::RegQueryValueEx(hKey, strName.c_str(), NULL, &dwType, pbyPortName, &dwRequiredSize) == ERROR_SUCCESS)
            strValue.assign((LPTSTR) pbyPortName);
        delete [] pbyPortName;
    }
    return strValue;
}

char CDectectDevice::FirstDriveFromMask (ULONG unitmask)
{
    TRACE_FILE_LINE_INFO();

    char i = 0;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}

bool CDectectDevice::DetectExistingPorts()
{
    TRACE_FILE_LINE_INFO();

	bool bRes = false;

	GUID qualcomm_modem_class_id = QUALCOMM_MODEM_USB_CLASS_ID;
	bRes = EnumerateDeviceInterfaces(qualcomm_modem_class_id);
	GUID qualcomm_diag_class_id = QUALCOMM_DIAG_USB_CLASS_ID;
	bRes = EnumerateDeviceInterfaces(qualcomm_diag_class_id);
	GUID android_class_id = ANDROID_USB_CLASS_ID;
	bRes = EnumerateDeviceInterfaces(android_class_id);

	return true;
}

bool GetUsbDeviceDetails( HDEVINFO hardware_dev_info,PSP_DEVICE_INTERFACE_DATA dev_info_data,PSP_DEVICE_INTERFACE_DETAIL_DATA* dev_info_detail_data) 
{
	ULONG required_len = 0;

	// First query for the structure size. At this point we expect this call
	// to fail with ERROR_INSUFFICIENT_BUFFER error code.
	if (SetupDiGetDeviceInterfaceDetail(hardware_dev_info,
		dev_info_data,
		NULL,
		0,
		&required_len,
		NULL)) {
			return false;
	}

	if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
		return false;

	// Allocate buffer for the structure
	PSP_DEVICE_INTERFACE_DETAIL_DATA buffer =
		reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(required_len));

	if (NULL == buffer) {
		SetLastError(ERROR_OUTOFMEMORY);
		return false;
	}

	buffer->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	// Retrieve the information from Plug and Play.
	if (SetupDiGetDeviceInterfaceDetail(hardware_dev_info,
		dev_info_data,
		buffer,
		required_len,
		&required_len,
		NULL)) 
	{
			*dev_info_detail_data = buffer;
			return true;
	} 
	else 
	{
		// Free the buffer if this call failed
		free(buffer);

		return false;
	}
}

bool GetUsbDeviceName(HDEVINFO hardware_dev_info, PSP_DEVICE_INTERFACE_DATA dev_info_data, TSTRING& strName) 
{
	bool bRes = false;
	PSP_DEVICE_INTERFACE_DETAIL_DATA func_class_dev_data = NULL;
	bRes = GetUsbDeviceDetails(hardware_dev_info,dev_info_data,&func_class_dev_data);
	try 
	{
		strName = func_class_dev_data->DevicePath;
	} 
	catch (...) 
	{
		SetLastError(ERROR_OUTOFMEMORY);
	}

	free(func_class_dev_data);

	return !strName.empty() && bRes;
}

bool CDectectDevice::EnumerateDeviceInterfaces(GUID class_id,bool exclude_removed,bool active_only,ULONG flags)
{
	bool bRes = false;

	HDEVINFO hardware_dev_info = ::SetupDiGetClassDevs(&class_id, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	for (ULONG index = 0; hardware_dev_info != 0; index++) 
	{
		SP_DEVICE_INTERFACE_DATA interface_data = {0};
		interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		// SetupDiEnumDeviceInterfaces() returns information about device
		// interfaces exposed by one or more devices defined by our interface
		// class. Each call returns information about one interface. The routine
		// can be called repeatedly to get information about several interfaces
		// exposed by one or more devices.
		if (::SetupDiEnumDeviceInterfaces(hardware_dev_info,0,&class_id,index,&interface_data)) 
		{
			// Satisfy "exclude removed" and "active only" filters.
			if ((!exclude_removed || (0 == (interface_data.Flags & SPINT_REMOVED))) && (!active_only || (interface_data.Flags & SPINT_ACTIVE))) 
			{
					TSTRING dev_name;
					if (GetUsbDeviceName(hardware_dev_info, &interface_data, dev_name)) 
					{
						/*
						//Diag
						\\?\acpi#pnp0501#1#{4d36e978-e325-11ce-bfc1-08002be10318}
						\\?\usb#vid_22b8&pid_2d09&mi_01#6&6325ec2&0&0001#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
						//Modem
						\\?\usb#vid_22b8&pid_2d09&mi_00#6&6325ec2&0&0000#{2c7089aa-2e0e-11d1-b114-00c04fc2aae4}
						*/
						if(_tcslen(_T("\\\\?\\usb")) <= dev_name.size() && _tcsnccmp(_T("\\\\?\\usb"),dev_name.c_str(),7) == 0)
						{								
							SP_DEVINFO_DATA spDeviceInfoData;
							ZeroMemory(&spDeviceInfoData, sizeof(SP_DEVINFO_DATA));
							spDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
							TSTRING strPort = GetCOMPort(hardware_dev_info,spDeviceInfoData,dev_name);
							if(!strPort.empty())
							{								
								GUID qualcomm_diag_class_id = QUALCOMM_DIAG_USB_CLASS_ID;
								if(class_id == qualcomm_diag_class_id)
								{
									if(IsDiag(strPort))
									{
										DeviceData* ptdevData = new DeviceData(interface_data.InterfaceClassGuid,dev_name,interface_data.Flags,DBT_DEVTYP_PORT,strPort);
										::SendMessage(m_hWnd, WM_DIAG_FOUND, (WPARAM)ptdevData, 0);
									}
								}
								GUID qualcomm_modem_class_id = QUALCOMM_MODEM_USB_CLASS_ID;
								if(class_id == qualcomm_modem_class_id)
								{
									DeviceData* ptdevData = new DeviceData(interface_data.InterfaceClassGuid,dev_name,interface_data.Flags,DBT_DEVTYP_PORT,strPort);
									::SendMessage(m_hWnd, WM_MODEM_FOUND, (WPARAM)ptdevData, 0);
								}
							}
							
							GUID google_adb_class_id = ANDROID_USB_CLASS_ID;
							if(class_id == google_adb_class_id)
							{
								if(IsADBDLDevice(dev_name))
								{
									DeviceData* ptdevData = new DeviceData(interface_data.InterfaceClassGuid,dev_name,interface_data.Flags,DBT_DEVTYP_PORT,strPort);
									::SendMessage(m_hWnd, WM_ADB_FOUND, (WPARAM)ptdevData, 0);
								}
							}
						}
					} 
					else 
					{
						// Something went wrong in getting device name
						break;
					}
			}
		} 
		else 
		{
			if (ERROR_NO_MORE_ITEMS == GetLastError()) 
			{
				// There are no more items in the list. Enum is completed.
				bRes = true;
				break;
			} 
			else 
			{
				// Something went wrong in SDK enum
				break;
			}
		}
	}

	::SetupDiDestroyDeviceInfoList(hardware_dev_info);

	return bRes;
}

bool CDectectDevice::GetExistingPortNumbers(vector<TSTRING> *PortIDs)
{
    TRACE_FILE_LINE_INFO();

    LONG  lRet;
    DWORD nEntryCount = 0;
    DWORD nEntryIndex = 0;
    const unsigned int maxBufLen = 256;
    TCHAR sEntryName[maxBufLen];
    DWORD nNameLen = maxBufLen;
    CString strEntryName;
    DWORD nType;
    TCHAR sEntryData[maxBufLen];
    DWORD nDataLen = maxBufLen;

    memset(sEntryName,0,maxBufLen);
    memset(sEntryData,0,maxBufLen);

    HKEY  hkeySERIALCOMM;
    GetSerialPortHKEY( hkeySERIALCOMM );

    lRet = ::RegQueryInfoKey(hkeySERIALCOMM, NULL, NULL, NULL, NULL, NULL, NULL, &nEntryCount, NULL, NULL, NULL, NULL);
    if(lRet != ERROR_SUCCESS || nEntryCount==0)
        return false;

    for(nEntryIndex = 0; nEntryIndex < nEntryCount; nEntryIndex++)
    {
        nNameLen = maxBufLen;
        nDataLen = maxBufLen;

        lRet = ::RegEnumValue(hkeySERIALCOMM, nEntryIndex, sEntryName, &nNameLen,NULL, &nType, (BYTE*)sEntryData, &nDataLen);

        // if not successfully read value, jump to next entry. more data is OK to process.
        if (! (lRet== ERROR_SUCCESS || lRet==ERROR_MORE_DATA) )
        {
            continue;
        }

        strEntryName = CString(sEntryName,nNameLen);

        // Find serial cable registry
        if ( nType==REG_SZ && strEntryName.Find(_T("\\Device"),0)==0 ) //XP,2000:Entry Type must equal to REG_SZ and Entry Name must begin with "\Device\Serial"
        {
            TSTRING strPort = sEntryData;
            PortIDs->push_back(strPort);
        }
        // Find USB cable registry
        else if( (strEntryName.Find(_T("\\Device\\AITSerial"))==0)    //For AIT USB Driver
            || (strEntryName.Find(_T("\\Device\\QCUSB_")) ==0))  //For Qualcomm Platform
        {
            TSTRING strUSBPort = sEntryData;
            PortIDs->insert( PortIDs->begin(), strUSBPort); //USB ports are more likely used, therefore the priority is higher and insert to the beginning of the port List
        }
        else; // other string we do not deal with because it may take too much time
    }

    return true;
}

bool CDectectDevice::isPortExists(const TSTRING& strPort, vector<TSTRING> &vExistingPorts)
{
    TRACE_FILE_LINE_INFO();

    bool    bExist = false;
    TSTRING strCOMPort;
    vector <TSTRING>::iterator v_Iter;

    for ( v_Iter = vExistingPorts.begin() ; v_Iter != vExistingPorts.end() ; v_Iter++ )
    {
        strCOMPort = *v_Iter;
        if ( strPort == strCOMPort)
        {
            bExist = true;
            break;
        }
    }
    return bExist;
}

bool CDectectDevice::GetSerialPortHKEY(HKEY &hkeySERIALCOMM)
{
    TRACE_FILE_LINE_INFO();

    LONG lRet;
    HKEY hkeyHARDWARE;
    HKEY hkeyDEVICEMAP;

    lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE"), 0, KEY_QUERY_VALUE, &hkeyHARDWARE);
    if(lRet != ERROR_SUCCESS)  //fail to open HARDWARE Registry Entry
    {
        return false;
    }

    lRet = ::RegOpenKeyEx(hkeyHARDWARE, _T("DEVICEMAP"), 0, KEY_QUERY_VALUE, &hkeyDEVICEMAP);
    if(lRet != ERROR_SUCCESS)  //fail to open DEVICEMAP Registry Entry
    {
        return false;
    }

    lRet = ::RegOpenKeyEx(hkeyDEVICEMAP, _T("SERIALCOMM"), 0, KEY_QUERY_VALUE, &hkeySERIALCOMM);
    if(lRet != ERROR_SUCCESS)  //fail to open SERIALCOMM Registry Entry
    {
        return false;
    }

    RegCloseKey(hkeyHARDWARE);
    RegCloseKey(hkeyDEVICEMAP);

    return true;
}

const TSTRING CDectectDevice::GetDeviceProperty(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData, DWORD dwProperty)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strTemp;
    DWORD dwType = 0, dwRequiredSize = 0;
    ::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, dwProperty , &dwType, NULL, 0, &dwRequiredSize);
    if(dwRequiredSize > 0)
    {
        dwRequiredSize += sizeof(TCHAR);
        PBYTE pbyDeviceDesc = new BYTE[dwRequiredSize];
        if(::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, dwProperty , &dwType, pbyDeviceDesc, dwRequiredSize, &dwRequiredSize))
        {
            ::_tcsupr_s((LPTSTR) pbyDeviceDesc,dwRequiredSize/sizeof(TCHAR));
            strTemp.assign((LPTSTR) pbyDeviceDesc);
        }
        delete [] pbyDeviceDesc;
    }
    return strTemp;
}

const TSTRING CDectectDevice::GetsSymbolicName(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoDataHub)
{
    TRACE_FILE_LINE_INFO();

    TSTRING sSymbolicName;
    HKEY hKey = ::SetupDiOpenDevRegKey(hDeviceInfoSet, &spDeviceInfoDataHub, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
    if(hKey != INVALID_HANDLE_VALUE)
    {
        DWORD dwType=0, dwRequiredSize=0;
        sSymbolicName = GetKeyValue(hKey, _T("SymbolicName"),dwType,dwRequiredSize);				
        TSTRING sFrom(_T("\\??\\"));
        TSTRING::size_type nPos = 0;
        while ( TSTRING::npos != (nPos = sSymbolicName.find(sFrom)) )
            sSymbolicName.replace(nPos, sFrom.size(), _T("\\\\.\\"));
        ::RegCloseKey(hKey);
    }
    return sSymbolicName;
}

void CDectectDevice::AddDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);
    DeviceMapIter Iter = m_Devices.find(ptdevData->ID());
    if(Iter == m_Devices.end())
    {
        m_Devices.insert(DeviceMapPair(ptdevData->ID(),ptdevData));
        m_hDeviceConnectionStatusEvent.ResetEvent();

        ptdevData->SetEvent(true);
        if(m_bEnable)			
            Fire(strEvent,(long)&ptdevData);
    }
}

void CDectectDevice::RemoveDeviceFromMap(DeviceMapIter Iter)
{
    TRACE_FILE_LINE_INFO();

    if(Iter != m_Devices.end())
    {
        m_Devices.erase(Iter);
    }
}

void CDectectDevice::RemoveDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);
    DeviceMapIter Iter = m_Devices.find(ptdevData->ID());
    while(Iter != m_Devices.end())
    {
        RemoveDeviceFromMap(Iter);
        Iter = m_Devices.find(ptdevData->ID());
    }

    m_hDeviceConnectionStatusEvent.SetEvent();

    ptdevData->SetEvent(false);
    if(m_bEnable)		
        Fire(strEvent,(long)&ptdevData);
}
int CDectectDevice::RegisterNotification()
{
    DEV_BROADCAST_DEVICEINTERFACE filter;
    memset(&filter,0,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    filter.dbcc_size         = sizeof(filter);
    filter.dbcc_devicetype   = DBT_DEVTYP_DEVICEINTERFACE;
    filter.dbcc_classguid    = GUID_CLASS_USB_DEVICE ;
	//dave
	//origin
	//m_hDevNotify = RegisterDeviceNotification(m_hWnd, &filter, 0);
	m_hDevNotify = RegisterDeviceNotification(m_hWnd, &filter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	//dave

    return 0;
}
int CDectectDevice::UnregisterNotification()
{
    UnregisterDeviceNotification(m_hDevNotify);
    m_hDevNotify = NULL;
    return 0;
}

bool CDectectDevice::RemoveNotify(const TSTRING& strSymboic,const TSTRING& strEvent)
{
	bool bFoundDevice = false;
	if(strSymboic.empty() == false)
	{
		for(DeviceMapIter Iter = m_Devices.begin();Iter != m_Devices.end();Iter++)
		{
			if(_tcsicmp(Iter->second->m_strDeviceClass.c_str(),strSymboic.c_str()) == 0)
			{
				RemoveDevice(Iter->second,strEvent);
				bFoundDevice = true;
				break;
			}
		}
	}
	return bFoundDevice;
}

bool CDectectDevice::RemovePortNotify(const TSTRING& strPort,const TSTRING& strEvent)
{
	bool bFoundDevice = false;
    if(!strPort.empty())
    {
        for(DeviceMapIter Iter = m_Devices.begin();Iter != m_Devices.end();Iter++)
        {
            if(Iter->second->m_strPort == strPort)
            {
                RemoveDevice(Iter->second,strEvent);
				bFoundDevice = true;
                break;
            }
        }
    }
	return bFoundDevice;
}

void CDectectDevice::GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType)
{
    if(lpbv)
    {
        if(nEventType == DBT_DEVICEREMOVECOMPLETE)
        {
            USHORT idVendor;
            USHORT idProduct;
            TSTRING strPara;
			TSTRING strSymboilc = lpbv->dbcc_name;
            ParserDBCCName(lpbv->dbcc_name,idVendor,idProduct,strPara);

			bool bFoundDevice = false;
            
			if(bFoundDevice == false)
			{//For Modem port notify
				TSTRING strSymbolic;
				TSTRING strUSBModemPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Modem"),_T("Device Parameters"),_T("PortName"),strSymbolic);
				if(strUSBModemPort.empty() == false)
					bFoundDevice = RemovePortNotify(strUSBModemPort,DEVICE_DISCONNECT);
			}
            
			//if(bFoundDevice == false)
			{//For Diag port notify
				TSTRING strSymbolic;
				TSTRING strUSBDiagPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Ports"),_T("Device Parameters"),_T("PortName"),strSymbolic);
				if(strUSBDiagPort.empty() == false)
					bFoundDevice = RemovePortNotify(strUSBDiagPort,DEVICE_DIAG_DISCONNECT);
			}

			//if(bFoundDevice == false)
			{//For ADB notify
				TSTRING strDeviceSymbolic;
				TSTRING strUSB = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("USB"),_T("Control"),_T("ActiveService"),strDeviceSymbolic);
				if(strUSB == _T("androidusb"))
				{
					bFoundDevice = RemoveNotify(strDeviceSymbolic,DEVICE_ADB_DISCONNECT);
				}
			}
        }
        else if(nEventType == DBT_DEVICEARRIVAL)
        {
            AddDevice(lpbv);
        }
    }
}


BOOL CDectectDevice::CreateGhostWnd()
{
    if(!IsWindow(m_hWnd))
    {
        if(RegisterWindow())
        {
            m_hWnd = ::CreateWindow(
                m_strClassName,m_strTitleName,WS_OVERLAPPEDWINDOW, 
                CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                NULL,NULL,::AfxGetApp()->m_hInstance,(void *)this);			
        }
    }	
    return IsWindow(m_hWnd);
}

BOOL CDectectDevice::RegisterWindow()
{
    WNDCLASSEX wcx;
    memset(&wcx,0,sizeof(WNDCLASSEX));

    wcx.cbSize = sizeof(WNDCLASSEX);						// Size of structure
    wcx.style = CS_HREDRAW | CS_VREDRAW;					// Redraw if size changes
    wcx.lpfnWndProc = CDectectDevice::stWinMsgHandler;		// Points to window procedure
    wcx.cbClsExtra = 0;										// No extra class memory
    wcx.cbWndExtra = 0;										// No extra window memory
    wcx.hInstance = ::AfxGetApp()->m_hInstance;				// Handle to instance
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// Predefined app. icon
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);				// Predefined arrow
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// White background brush
    wcx.lpszMenuName = NULL;								// Name of menu resource
    wcx.lpszClassName = m_strClassName.GetBuffer(0);		// Name of window class
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			// Small class icon

    return ::RegisterClassEx(&wcx) != NULL;
}

LRESULT CALLBACK CDectectDevice::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_DEVICECHANGE)
        OnDeviceChange(wParam, lParam);
    else if ( uMsg == WM_MODEM_FOUND )
    {
        /*
        While device connected, the default action is to sync with mobile, and pop up a dialog "Sync succeeded"
        If user click down some and hold it down before device connected, and loose it after the dialog pop
        there will be something wrong because the button captures the mouse.
        So we should release mouse capture here.*/
        ReleaseCapture();


        TRACE(_T("Receive Message WM_MODEM_FOUND\n"));
        DeviceDataPt ptdevData = (DeviceData*)(wParam);
        AddDevice(ptdevData,DEVICE_CONNECT);
    }
    else if ( uMsg == WM_DIAG_FOUND )
    {
        /*
        While device connected, the default action is to sync with mobile, and pop up a dialog "Sync succeeded"
        If user click down some and hold it down before device connected, and loose it after the dialog pop
        there will be something wrong because the button captures the mouse.
        So we should release mouse capture here.*/
        ReleaseCapture();


        TRACE(_T("Receive Message WM_DIAG_FOUND\n"));
        DeviceDataPt ptdevData = (DeviceData*)(wParam);
        AddDevice(ptdevData,DEVICE_DIAG_CONNECT);
    }
    else if ( uMsg == WM_ADB_FOUND )
    {
        /*
        While device connected, the default action is to sync with mobile, and pop up a dialog "Sync succeeded"
        If user click down some and hold it down before device connected, and loose it after the dialog pop
        there will be something wrong because the button captures the mouse.
        So we should release mouse capture here.*/
        ReleaseCapture();


        TRACE(_T("Receive Message WM_ADB_FOUND\n"));
        DeviceDataPt ptdevData = (DeviceData*)(wParam);
        AddDevice(ptdevData,DEVICE_ADB_CONNECT);
    }

    return ::DefWindowProc(m_hWnd,uMsg, wParam, lParam);
}

CDectectDevice* CDectectDevice::GetObjectFromWindow(HWND hWnd)
{
    return reinterpret_cast<CDectectDevice *>(GetWindowLong(hWnd, GWL_USERDATA));
}

LRESULT CALLBACK CDectectDevice::stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{			
    if (uMsg == WM_NCCREATE)
    {
        SetWindowLong(hwnd, GWL_USERDATA,(long)((LPCREATESTRUCT(lParam))->lpCreateParams));
        return TRUE;
    }
    CDectectDevice* pWnd = GetObjectFromWindow(hwnd);
    if (pWnd)
        return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
    else
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma warning(pop)