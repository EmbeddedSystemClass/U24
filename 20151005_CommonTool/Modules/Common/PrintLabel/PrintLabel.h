
#ifdef PRINTLABEL_EXPORTS
#define PRINTLABEL_API extern "C" __declspec(dllexport)
#else
#define PRINTLABEL_API extern "C" __declspec(dllimport)
#endif

#include "lppx2.h"

//#include "ComComm.h"

#pragma warning(disable:4996)

#define   OK   0
#define   NG   -1

class CPrintLabel 
{
public:
	CPrintLabel(void);
	virtual ~CPrintLabel(void);

public:
	int GetConnect(IApplication * p_app);
	void DisConnect(void);

private:
	int Connect(void);

private:
	IApplication *  m_p_labelSoftware;
	LPDISPATCH		m_p_disp;
	BOOL			m_b_attached;
	BOOL			m_b_load;
	COleException	m_p_error;

};


PRINTLABEL_API BOOL PrintIMEIPPID(char *sz_IMEI, char *sz_PPID, char *sz_outInfo);

PRINTLABEL_API BOOL PrintPPID(char *sz_PPID, char *sz_outInfo);

PRINTLABEL_API BOOL PrintPPIDID(char *sz_PPID, char *sz_outInfo);

PRINTLABEL_API BOOL PrintFATravelLabel(char *sz_Picasso, char *sz_Model, char *sz_HW, char *sz_SW, char *sz_SO, char *sz_SN, char *sz_PN, char *sz_Line,char *sz_outInfo);

PRINTLABEL_API BOOL PrintFAPCBALabel(char *sz_PCBA_barcode ,char *sz_Model,char *sz_outInfo);
//PRINTLABEL_API bool PrintLabelLPTPORT(char * sz_IMEI, char * sz_PPID, char *sz_outInfo);

PRINTLABEL_API BOOL PrintIMEI(char *sz_imei, char * sz_labelFile, char *sz_outInfo);
