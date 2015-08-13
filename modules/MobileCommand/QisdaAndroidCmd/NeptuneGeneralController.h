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
	virtual int InitDevice(int port) = 0;
	virtual int FreeDevice(int port) = 0;
	virtual int GeneralCmd(int port, int timeout, byte* cmd) = 0;
    virtual int GeneralCmdOut(int port, int timeout, byte* cmd, char* out_buf) = 0;
	virtual int GeneralCmdIn(int port, int timeout, byte* cmd, char* in_buf, size_t in_size) = 0;
	virtual int GeneralCmdInOut(int port, int timeout, byte* cmd, char* in_buf, size_t in_size, char* out_buf) = 0;

    //FF:
	virtual int UnregisterCallBack(void* farCallBack) = 0;
	virtual int RegisterCallBack(void* farCallBack) = 0;
};

#endif // !defined(AFX_NEPTUNEGENERALCONTROLLER_H__4B0EF1C5_8F25_4E43_AF13_6BBB7041A3E8__INCLUDED_)
