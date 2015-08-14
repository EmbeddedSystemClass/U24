#ifndef _QISDA_API_H_
#define _QISDA_API_H_


// FA
typedef bool (*pFA_Read)(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer);
typedef bool (*pFA_Write)(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char *szFABuffer);

/* Only for test */
typedef bool (*pRead_IMEI)(const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod, char returnIMEI[16]);

#endif // End of #ifndef _QISDA_API_H_
