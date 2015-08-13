// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QISDAEMMCDOWNLOADMODULE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QISDAEMMCDOWNLOADMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

#ifdef QISDAEMMCDOWNLOADMODULE_EXPORTS
#define QISDAEMMCDOWNLOADMODULE_API __declspec(dllexport)
#else
#define QISDAEMMCDOWNLOADMODULE_API __declspec(dllimport)
#endif


/*
// This class is exported from the QisdaEmmcDownloadModule.dll
class QISDAEMMCDOWNLOADMODULE_API CQisdaEmmcDownloadModule {
public:
	CQisdaEmmcDownloadModule(void);
	// TODO: add your methods here.
};

extern QISDAEMMCDOWNLOADMODULE_API int nQisdaEmmcDownloadModule;

QISDAEMMCDOWNLOADMODULE_API int fnQisdaEmmcDownloadModule(void);
*/


#include <vector>
#include <map>
#include "../QisdaQualcommDownloadModule/Download.h"
#include "XML/MyMSXML.h"



#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'CDownloadEmmc::m_ToBeDLfile' : class 'std::map<_Kty,_Ty>' needs to have dll-interface to be used by clients of class 'CDownloadEmmc'


#pragma pack (1)

typedef struct
{
	char   chFileName[256];
	UINT64 dwSectorOffset;
	DWORD  size; //bytes

	char   chLabel[32];
	UINT64 PartitionSectorNum;
	UINT64 PhysicalPartitionNum;
	UINT64 StartSector;
	bool   NeedToDL;
} DownloadList;



typedef struct
{
	BYTE bootable;
	BYTE CHS_FirstSector [3];
	BYTE partitionType;
	BYTE CHS_LastSector [3];
	DWORD LBA_FirstSector; //	LBA of first absolute sector in the partition
	DWORD NumberOfSector;
} PrimaryPartition_Struct;

typedef struct
{
	BYTE  CodeArea [440];
	DWORD DiskSignature;
	WORD  Reserved;

	PrimaryPartition_Struct partition[4];
	WORD MbrSignature;
} MBR_STRUCT;

typedef struct 
{
	char  order[32];
	char  type[8];
	bool  bootable;
	char  label[16];
	DWORD size;
	char  readonly[8];
	char  filename[32];
	DWORD offset;
	bool  needToDL;
} DL_STRUCT;

//
//typedef struct
//{
//	char   chHandleName  [MAX_PATH];
//	char   chInstanceName[MAX_PATH];
//	char   chFriendlyName[MAX_PATH];
//	char   chDeviceDescr [MAX_PATH];
//	char   chParentName  [MAX_PATH];
//	DWORD  dwDeviceNumber;
//
//} USBDEVICE_STRUCT;

typedef struct
{
	char   chDriveLetter[MAX_PATH]; // "X:"
	char   chHandleName [MAX_PATH]; // "\\\\.\\X:"
	UINT64 u64DiskSize;
	DWORD  dwSectorSize;
	DWORD  dwDeviceNumber;
	DWORD  dwDevInstance;

	// additional info
	DWORD  sizeG; //8G, 16G
	char   chEMMCname     [32];
	char   chSerialNumber [16];
} DRIVEDEVICE_STRUCT;

typedef struct
{
	USBDEVICE_STRUCT   usbDev;
	DRIVEDEVICE_STRUCT driveDev;

} EMMCDEVICE_STRUCT;

#pragma pack()





using namespace QSYNC_LIB;


class QISDAEMMCDOWNLOADMODULE_API CDownloadEmmc : public CNotifyBase_ANSI
{
public:
	CDownloadEmmc (int comport = 0);
	~CDownloadEmmc();
	void SetDownloadPath (const char* fullpath);
	void SetDownloadFile (const char* chFile, const char* chType);
	void SetCOMport (const int comport);
	bool IsConnected ();
	bool EmmcDownload ();

	//
	// CNotifyBase_ANSI
	// 
	virtual bool Register(INotify_ANSI* pNotify,const std::string& strEvent);
	virtual bool Register(EventFunc_ANSI* pEventFunc,const std::string& strEvent);
	virtual bool Unregister(INotify_ANSI* pNotify,const std::string& strEvent);
	virtual bool Unregister(EventFunc_ANSI* pEventFunc,const std::string& strEvent);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Fire(const std::string& strEvent,long nParam);




protected:
	int           m_COMport; // comport = 0 means use any connected device
	char          m_DLpath [MAX_PATH];
	HANDLE        hEvent;
	HANDLE        hVolume;
	DownloadList  *m_FileList;
	DWORD         SECTOR_SIZE;
	UINT64        DISK_SIZE;

	std::map<CString, CString>       m_ToBeDLfile;
	std::map<int, EMMCDEVICE_STRUCT> m_eMmcDevice; // map <ComPort, eMMC>


	// Device function
	bool   FindQualcomDevice (std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	bool   FindUsbMassStorageDevice(std::vector<EMMCDEVICE_STRUCT> &inEMMCdevice);
	bool   DetectDiskDevice (std::vector<USBDEVICE_STRUCT> &inUSBdevice);
	bool   DetectDrives (std::vector<DRIVEDEVICE_STRUCT> &inDriveDevice);
	int    GetComportFromUSB (USBDEVICE_STRUCT& dev);

	bool   CreateDeviceEvent (const char* chName);
	void   DestroyDeviceEvent();
	HANDLE OpenDeviceAndEvent();
	void   CloseDeviceAndEvent(HANDLE hHandle);


	// Download and Files
	bool   DownloadFile(HANDLE hDevice, const char* chFileName, const UINT64 dwOffset, const UINT64 sectorOffset);
	bool   DownloadMBR_EBR (HANDLE hDevice, DownloadList *pDLstruct);
	void   PatchMBR_EBR (const DownloadList *pDLstruct, BYTE* pBuffer);
	bool   ParsePartitionXML ();
	bool   HasFilesToDownload();

	// UI function
	void   UpdateProgress (const char* chMessage, const int nStep = 0, const int nProgress = 0);
	void   AddMessageWithLastError (const char* chMessage, ...);
	void   AddMessage (const char* chMessage, ...);



	// EVT1 function
	//DownloadList* PrimaryList;
	//DownloadList* ExtList;
	//int    gDownloadPartSearchIndex (const char* key, const char* value);
	//void   GetFileNameOnly (const char* fullpath, char* chBuffer, DWORD dwSize);
	//bool   WriteMBR (HANDLE hHandle, BYTE* pMBR, DWORD dwMBRsize);
	//bool   WriteEBR (HANDLE hHandle, UINT64 dwOffset, BYTE* pEBR, DWORD dwEBRsize);
	//bool   WritePrimaryPartitionFiles (HANDLE hHandle, int index);
	//bool   WriteExtendedPartitionFiles (HANDLE hHandle, int index, DWORD dwEBRsize);
	//bool   HasFilesToDownload(DWORD dwEBRsize);
	//bool   DownloadFile (HANDLE hDevice, const char* chFile, const UINT64 dwOffset);

	//UINT64 CalculateMBR (int devIndex, BYTE** pBuffer, DWORD* dwSize); // return offset for EBR
	//bool   CalculateEBR (int devIndex, BYTE** pBuffer, DWORD* dwSize, const UINT64 dwOffset);
};


#pragma warning( pop )