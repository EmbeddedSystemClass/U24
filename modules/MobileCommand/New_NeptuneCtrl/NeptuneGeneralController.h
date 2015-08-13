// NeptuneGeneralController.h: interface for the CNeptuneGeneralController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEPTUNEGENERALCONTROLLER_H__4B0EF1C5_8F25_4E43_AF13_6BBB7041A3E8__INCLUDED_)
#define AFX_NEPTUNEGENERALCONTROLLER_H__4B0EF1C5_8F25_4E43_AF13_6BBB7041A3E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNeptuneGeneralController  
{
public:
	CNeptuneGeneralController();
	virtual ~CNeptuneGeneralController();
	virtual int InitDevice(int nPort) = 0;
	virtual int FreeDevice(int nPort) = 0;
	virtual int GeneralCmd(int nPort, int Timeout, char* CtrlBuffer) = 0;
    virtual int GeneralCmdOut(int nPort, int Timeout, char* CtrlBuffer, char* OutBuffer) = 0;
	virtual int GeneralCmdIn(int nPort, int Timeout, char* CtrlBuffer, char* InBuffer, size_t nInLength) = 0;
	virtual int GeneralCmdInOut(int nPort, int Timeout, char* CtrlBuffer, char* InBuffer, size_t nInLength, char* OutBuffer) = 0;

	// The work buffer would be a data buffer or a file path. It just depends on implementation.
    virtual int GeneralDownload(int nPort, int Timeout, char* CtrlBuffer, char* WorkBuffer) = 0;
	virtual int GeneralUpload(int nPort, int Timeout, char* CtrlBuffer, char* WorkBuffer) = 0;

    //FF:
	virtual int UnregisterCallBack(void* farCallBack) = 0;
	virtual int RegisterCallBack(void* farCallBack) = 0;

public:
};

#endif // !defined(AFX_NEPTUNEGENERALCONTROLLER_H__4B0EF1C5_8F25_4E43_AF13_6BBB7041A3E8__INCLUDED_)
