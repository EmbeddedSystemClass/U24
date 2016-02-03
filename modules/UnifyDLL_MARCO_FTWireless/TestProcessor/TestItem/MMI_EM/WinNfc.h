/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

    WinNfc

Abstract:

    This header file provides the definitions and symbols necessary for an
    Application to access the Smartcard via Near Field Communication (NFC).

Environment:

    Win32

Notes:

--*/
#ifndef _WINNFC_H_
#define _WINNFC_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "NfcErr.h"

#ifdef __cplusplus
extern "C" {


#endif

#ifdef WINNFC_EXPORTS
#define WINNFC_API __declspec(dllexport)
#else
#define WINNFC_API __declspec(dllimport)
#endif

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif
#ifndef _LPCVOID_DEFINED
#define _LPCVOID_DEFINED
typedef const VOID *LPCVOID;
#endif

#ifndef ULONG_PTR
typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif

typedef ULONG_PTR NFCCONTEXT;
typedef NFCCONTEXT *PNFCCONTEXT, *LPNFCCONTEXT;

typedef ULONG_PTR NFCHANDLE;
typedef NFCHANDLE *PNFCHANDLE, *LPNFCHANDLE;

WINNFC_API LONG WINAPI
NfcEstablishContext(
    IN  DWORD dwScope,
    IN  LPCVOID pvReserved1,
    IN  LPCVOID pvReserved2,
    OUT LPNFCCONTEXT phContext);


WINNFC_API LONG WINAPI
NfcReleaseContext(
    IN      NFCCONTEXT hContext);


WINNFC_API LONG WINAPI
NfcConnect(
    IN      NFCCONTEXT hContext,
    IN      LPCSTR szReader,
    IN      DWORD dwShareMode);


WINNFC_API LONG WINAPI
NfcDisconnect(
    IN      NFCCONTEXT hContext,
    IN      DWORD dwDisposition);

///////////////////////////////////////////////////////////////////////////////
//
//  NFC parameter definitions
//
#define NFC_PARAMETER_READER_NAME		0x00000001 
#define NFC_PARAMETER_ESCAPE_COMMAND	0x00000002

//add by roman from v1.7.1 2013/06/11
WINNFC_API LONG WINAPI
NfcGetParameter(
    IN      NFCCONTEXT hContext,
    IN      DWORD dwType,
	IN OUT  void* value,
	IN OUT  DWORD* dwLength);


///////////////////////////////////////////////////////////////////////////////
//
//  Card Type definitions
//
#define NFC_CARDTYPE_ACTIVE_106		0x00000001  /**<  Active operating mode at 106kbps.  */
#define NFC_CARDTYPE_ACTIVE_212     0x00000002  /**<  Active operating mode at 212kbps. */
#define NFC_CARDTYPE_ACTIVE_424     0x00000004  /**<  Active operating mode at 424kbps. */
#define NFC_CARDTYPE_PASSIVE_106    0x00000008  /**<  Passive operating mode using the ISO14443-3 Type A startup at 106 kbps.*/
#define NFC_CARDTYPE_PASSIVE_212    0x00000010  /**<  Passive operating mode using NFC polling startup at 212k. To perform a successful poll in this mode you will have to corretly set the NfcPollPayload in \ref phHal_sDevInputParam  */
#define NFC_CARDTYPE_PASSIVE_424    0x00000020  /**<  Passive operating mode using NFC polling startup at 424k. To perform a successful poll in this mode you will have to corretly set the NfcPollPayload in \ref phHal_sDevInputParam */
#define NFC_CARDTYPE_MIFARE		    0x00000040  /**<  Mifare operating mode. */
#define NFC_CARDTYPE_ISO14443_4A	0x00000080  /**<  ISO14443-4 Type A operating Mode. */
#define NFC_CARDTYPE_FELICA_212	    0x00000100  /**<  Felica operating mode at 212k. */
#define NFC_CARDTYPE_FELICA_424		0x00000200  /**<  Felica operating mode at 424k. */
#define NFC_CARDTYPE_ISO14443_4B    0x00000400  /**<  ISO14443-4 Type B operating Mode. */
#define NFC_CARDTYPE_JEWEL			0x00000800  /**<  Innovision Jewel tag operating Mode. */

#define NFC_CARDTYPE_PAY_PASS		0x00001000  /**<  not support */
#define NFC_CARDTYPE_VISA_WAVE		0x00002000  /**<  not support */


WINNFC_API LONG WINAPI
NfcSelectCard(
    IN      NFCCONTEXT hContext,
    IN      DWORD dwCardTypes,
	IN		PBYTE pbInitData,
	IN		DWORD dwInitDataLen);


WINNFC_API LONG WINAPI
NfcDeSelectCard(
    IN      NFCCONTEXT hContext);

#define LED_RED		0x01
#define LED_BLUE	0x02
#define LED_ALL		0x03
#define LED_OFF		0x00

///////////////////////////////////////////////////////////////////////////////
//
//  LED And Buzzer Control
//

WINNFC_API LONG WINAPI
NfcLEDControl(
    IN NFCCONTEXT hContext,
    IN BYTE bState);

#define BUZZER_ON		0x00
#define BUZZER_OFF		0x08

WINNFC_API LONG WINAPI
NfcBuzzerControl(
    IN NFCCONTEXT hContext,
    IN BYTE bState);


/*************************************************************************************************************

	PR533 only functions
	TODO:

**************************************************************************************************************/
WINNFC_API LONG WINAPI
NFC_ConnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwShareMode, 
	DWORD dwPreferredProtocols);

WINNFC_API LONG WINAPI
NFC_GetATR(
	IN NFCCONTEXT hContext, 
	OUT PBYTE pAtr,
	OUT LPDWORD pLength,
	OUT LPDWORD pActiveProtocol);

WINNFC_API LONG WINAPI
NFC_SendAPDU(
    IN NFCCONTEXT hContext,
    IN LPCBYTE pbSendBuffer,
    IN DWORD cbSendLength,
    OUT LPBYTE pbRecvBuffer,
    IN OUT LPDWORD pcbRecvLength);

WINNFC_API LONG WINAPI
NFC_DisconnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwDisposition);

/*************************************************************************************************************

	ICC functions

**************************************************************************************************************/
#define SCARD_PROTOCOL_UNDEFINED    0x00000000  // There is no active protocol.
#define SCARD_PROTOCOL_T0           0x00000001  // T=0 is the active protocol.
#define SCARD_PROTOCOL_T1           0x00000002  // T=1 is the active protocol.
#define SCARD_PROTOCOL_RAW          0x00010000  // Raw is the active protocol.


WINNFC_API LONG WINAPI
ICC_ConnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwShareMode, 
	DWORD dwPreferredProtocols);

WINNFC_API LONG WINAPI
ICC_GetATR(
	IN NFCCONTEXT hContext, 
	OUT PBYTE pAtr,
	OUT LPDWORD pLength,
	OUT LPDWORD pActiveProtocol);

WINNFC_API LONG WINAPI
ICC_SendAPDU(
    IN NFCCONTEXT hContext,
    IN LPCBYTE pbSendBuffer,
    IN DWORD cbSendLength,
    OUT LPBYTE pbRecvBuffer,
    IN OUT LPDWORD pcbRecvLength);

WINNFC_API LONG WINAPI
ICC_DisconnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwDisposition);


/*************************************************************************************************************

	SAM functions

**************************************************************************************************************/
WINNFC_API LONG WINAPI
SAM_ConnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwShareMode, 
	DWORD dwPreferredProtocols);

WINNFC_API LONG WINAPI
SAM_GetATR(
	IN NFCCONTEXT hContext, 
	OUT PBYTE pAtr,
	OUT LPDWORD pLength,
	OUT LPDWORD pActiveProtocol);

WINNFC_API LONG WINAPI
SAM_SendAPDU(
    IN NFCCONTEXT hContext,
    IN LPCBYTE pbSendBuffer,
    IN DWORD cbSendLength,
    OUT LPBYTE pbRecvBuffer,
    IN OUT LPDWORD pcbRecvLength);

WINNFC_API LONG WINAPI
SAM_DisconnectCard(
	IN NFCCONTEXT hContext, 
	IN DWORD dwDisposition);

//
////////////////////////////////////////////////////////////////////////////////
//
//  ISO14443-4A/B functions
//

WINNFC_API LONG WINAPI
ISO14443_SendAPDU(
    IN NFCCONTEXT hContext,
    IN LPCBYTE pbSendBuffer,
    IN DWORD cbSendLength,
    OUT LPBYTE pbRecvBuffer,
    IN OUT LPDWORD pcbRecvLength);

//
////////////////////////////////////////////////////////////////////////////////
//
//  Mifare functions
//

#define MIFARE_UL_BLOCKS 16         // Mifare UL has 16 block with 4 bytes each memory

#define MIFARE_1K_SECTORS 16        // Mifare 1k has 16 sectors with 4 blocks of 16bytes each
#define MIFARE_1K_BLOCKS 4

#define MIFARE_4K_SECTORS 32        // Mifare 4k has 32 sectors with 4 blocks of 16bytes each +
#define MIFARE_4K_BLOCKS 4
#define MIFARE_4K_UPPER_SECTORS 8   // Mifare 4k has additional 8 sectors with 16 blocks of 16bytes each
#define MIFARE_4K_UPPER_BLOCKS 16


#define MAX_NFCID_LENGTH	32
WINNFC_API LONG WINAPI
Mifare_GetCardID(
    IN NFCCONTEXT hContext,
	IN LPSTR mszID);

#define KEY_TYPE_A 0x01
#define KEY_TYPE_B 0x02

WINNFC_API LONG WINAPI
Mifare_KeyAuthority(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
	IN BYTE bKeyType,
    IN LPCBYTE pbKey);


WINNFC_API LONG WINAPI
Mifare_ReadBlock(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
    IN OUT LPBYTE pbData,
	IN OUT LPDWORD pcbLength);

WINNFC_API LONG WINAPI
Mifare_WriteBlock(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
    IN LPCBYTE pbData,
	IN DWORD cbLength);

WINNFC_API LONG WINAPI
Mifare_ReadValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
	IN LPDWORD pcbValue);

WINNFC_API LONG WINAPI
Mifare_WriteValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
	IN DWORD pcbValue);

WINNFC_API LONG WINAPI
Mifare_IncreaseValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
	IN DWORD pcbValue);

WINNFC_API LONG WINAPI
Mifare_DecreaseValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock,
	IN DWORD pcbValue);

WINNFC_API LONG WINAPI
Mifare_TransferValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock);

WINNFC_API LONG WINAPI
Mifare_RestoreValue(
    IN NFCCONTEXT hContext,
	IN BYTE bBlock);

//int ChangeKeyAccess(int idx, int nSector, unsigned char* KeyA, unsigned char* KeyB, unsigned char* Access)


#ifdef __cplusplus
}
#endif
#endif // _WINNFC_H_