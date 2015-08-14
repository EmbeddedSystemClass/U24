#include "stdafx.h"
#include "stdlib.h"
#include "..\CMDModuleExport.h"
#include "..\EnumCOMPort.h"

namespace Berlin
{
	namespace Detect_COMPort
	{
		CMDMODULEEXPORT_API bool Detect_COMPortExist_SPU(char* szPort, char* szRetryTime, char* szRetryPeriod, char* szKeyword, char* szReturnPort)
		{	
			int iInputComport = atoi(szPort);
			int iDetectCOMPort = -1;
			bool bRes = false;

			std::vector<USBDEVICE_STRUCT> QualDevs;
			std::vector<USBDEVICE_STRUCT> ADBDevs;

			CEnumCOMPort EnumObject;
			std::string m_USB_Root_Name;

			for(int i = 0; i < atoi(szRetryTime); i++)
			{
				// 1. Find all Qualcomm devices and ADB devices
				if (! (EnumObject.FindQualcomDevice(QualDevs) && EnumObject.FindADBdevice(ADBDevs)))
				{
					sprintf(szReturnPort, "%d", iDetectCOMPort);
					return bRes;
				}

				// 2. Find USB port number by keyword
				if (iInputComport == 0)
				{
					for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = QualDevs.begin(); iterQual != QualDevs.end(); ++iterQual)
					{
						iInputComport = EnumObject.GetComportFromUSBByKeyword(*iterQual, szKeyword);
						if(iInputComport != -1)
						{
							break;
						}
					}
				}
				
				for (std::vector<USBDEVICE_STRUCT>::iterator iterQual = QualDevs.begin(); iterQual != QualDevs.end(); ++iterQual)
				{
					iDetectCOMPort = EnumObject.GetComportFromUSB(*iterQual);
					if ( iInputComport == iDetectCOMPort)
					{
						m_USB_Root_Name = (*iterQual).chParentName;
						break;
					}
				}

				// 3. Find ADB interface that under the same USB root 
				for (std::vector<USBDEVICE_STRUCT>::iterator iterADB = ADBDevs.begin(); iterADB != ADBDevs.end(); ++iterADB)
				{
					if (strcmp(m_USB_Root_Name.c_str() , (*iterADB).chParentName )==0 )
					{
						bRes = true;
						break;
					}
				}

				if(bRes)
					break;

				::Sleep(atoi(szRetryPeriod));
			}

			sprintf(szReturnPort, "%d", iDetectCOMPort);
			return bRes;		
		}
	} // DetectCOMPort


} // namespace Berlin