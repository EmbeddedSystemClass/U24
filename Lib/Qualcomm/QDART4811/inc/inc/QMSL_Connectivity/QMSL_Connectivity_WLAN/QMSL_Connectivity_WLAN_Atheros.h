/******************************************************************************/
/**
Program: QMSL_WLAN

	$Id: //depot/HTE/QDART/QMSL6/inc/QMSL_Connectivity/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_Atheros.h#2 $

\code

	- Defines the "C" interface for all Factory Library functions.

	- Table of contents (text search can be done for the following sections)

			- General Information
			- Related Documents
			- Version History

			- WLAN FTM


	------------------------------------------------------


	----------------------
	General Information
	----------------------

	- All data types used in this interface are ANSI C compatible.

	- All returns that indicate "true if succeed, false if fail" are defined
		as:  1 = true = Success , 0 = false = failure

		Functions that are not entirely upper case do not correspond directly to
		an FTM or diagnostic command.

		For example, QLIB_ConnectServer() is not mapped directly to a diagnostic
		command.


	- Variable Names
		Variable names are prefixed in lower case to indicate the data type.
		The following are defined:

			p = Pointer
			i = Integer type, either char, short, or long
			d = Double or floating point
			e = Enumeration (possibly stored as a short, but representing
			                 a fixed list of possibilities)
			b = unsigned char, 0=false, 1=true

	----------------------
	Related Documents
	----------------------

	- The function name convention used in this document is as follows:

		QLIB_ indicates that this function corresponds to library function
		in the QLib project.

		From there, all diagnostic and FTM functions are named exactly as
		the command identifiers are defined in the QLib_Defines.h.

	- For a list of all document numbers referred to by QMSL, please see following section in the QDART Help file:
		1) QDART -> QDART Related Documents
	or  2) QDART -> QMSL -> Getting Started -> References.


\endcode


	-----------------------
	Version History
	-----------------------

 \b QUALCOMM  \b PROPRIETARY

	This document contains propriety information, and except with written
	permission of Qualcomm Technologies INC, such information shall not be
	published, or disclosed to others, or used for any purpose, and the
	document shall not be duplicated in whole or in part.


	Copyright (c) 2004-2012 QUALCOMM Technologies Incorporated.
	All Rights Reserved.
	Qualcomm Confidential and Proprietary

\version
Version information is stored in the file "QMSL_Release_Notes.txt"

\note
Compiler:  Microsoft Visual C++ v6.0 SP4
*******************************************************************************/

#if !defined(_QLIB_WLAN_ATHEROS_H)
#define _QLIB_WLAN_ATHEROS_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
	Establish whether the library is being compiled into a DLL (exporting),
	or being included from a client (importing)

	When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif


   /******************************************************************************
   						WLAN FTM

     NOTE: Please check the module manfucturer's documentation for specific information
           about the commands or parameters.  The documentation for the module-specific
   		commands is based sources and Qualcomm can give very little support about
   		individual commands and they can also not add new commands unless the
   		module vendor writes the corresponding firmware and provides the command
   		specification.

   *******************************************************************************/

   /******************************************************************************/
   /**

   Atheros WLAN command to set RX MAC ADDRESS

   \param aiMAC_Address = MAC Address

   \param aiBSS_ID = BSS ID

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_RX_MAC_ADDRESS
   (
      HANDLE hResourceContext, 
      unsigned char aiMAC_Address[6],
      unsigned char aiBss_Id[6]
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to set OTP MAC ADDRESS

   \param aiMAC_Address = MAC Address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_OTP_MAC_ADDRESS
   (
      HANDLE hResourceContext, 
      unsigned char aiMAC_Address[6] 
   );

	   /******************************************************************************/
   /**

   Atheros WLAN command to WRITE MAC ADDRESS

   \param aiMAC_Address = MAC Address
   
   \param location:		the location where the MAC Address is written to
						= 1 - flash; 
						= 5 - file
   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API	unsigned char QLIB_FTM_WLAN_Atheros_WRITE_MAC_ADDRESS
   ( 
		HANDLE hResourceContext,
		unsigned char aiMAC_Address[6], 
		int location
	);


   /******************************************************************************/
   /**

   Atheros WLAN command to READ MAC ADDRESS

   \param aiMAC_Address = MAC Address
   
   \param location:		the location where the MAC Address is written from
						= 1 - flash; 
						= 5 - file
   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API	unsigned char QLIB_FTM_WLAN_Atheros_READ_MAC_ADDRESS
   ( 
		HANDLE hResourceContext,
		int location, 
		unsigned char* aiMAC_Address
   );
	

   /******************************************************************************/
   /**

   Atheros WLAN command to start self init cal

   \param iChannel = WLAN channel frequency in MHz on which self init to be performed

   \param iChainMask = Chain Mask enum, 
                        1 = Chain 0,
                        2 = Chain 1,
                        3 = Both Chains (0, 1)

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SELF_INIT_CAL
   (
      HANDLE hResourceContext, 
      int iChannel, 
      int iChainMask
   );
   
   /******************************************************************************/
   /**

   Atheros WLAN command to get self init cal result

   \param iChannel = WLAN channel frequency in MHz on which self init to be performed

   \param iChainMask = Chain Mask enum, 
                        1 = Chain 0,
                        2 = Chain 1,
                        3 = Both Chains (0, 1)
   
   \param iOlpcGainDelta = Pointer to 4 int array for Olpc Gain Delta

   \param iThermalCal = Pointer to 4 unsigned int array for Thermal Cal Value

   \param cmacOlpc = Pointer to 4 unsigned long array for cmacOlpc

   \param cmacPsat = Pointer to 4 unsigned long array for cmacPsat

   \param iCmacOlpcPcdac = Pointer to 4 unsigned int array for cmacPsat

   \param iCmacPsatPcdac = Pointer to 4 unsigned int array for cmacPsat

   \param iPsatParm = Pointer to 4 unsigned int array for cmacPsat

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_SELF_INIT_CAL_RESULT
   ( 
     HANDLE hResourceContext, 
     int iChannel, 
     int iChainMask,
     int* iOlpcGainDelta,
     unsigned int* iThermalCal,
     unsigned long* cmacOlpc, 
     unsigned long* cmacPsat, 
     unsigned int* iCmacOlpcPcdac, 
     unsigned int* iCmacPsatPcdac, 
     unsigned int* iPsatParm
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to commit self init data

   \param iWriteOption = Write option
                     3 = Write to OTP
                     5 = Write to File

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SELF_INIT_COMMIT
   (
      HANDLE hResourceContext, 
      unsigned char iWriteOption
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to enable/disable transmit

   Note: Enable/disable continuous tx test cmd works only when target is awake.

   \param iMode =	0: TX_OFF
   					1: TX_SINE
   					2: TX_FRAME
   					3: TX_TX99
   					4: TX100


   	   iFreq = Transmit Frequency in MHz or Channel number
   	           For example, 2412 for channel 1 in 802.11g, or 1

   	   iDataRate =  0 - 1Mbps
					1 - 2Mbps LONG
					2 - 2Mbps SHORT
					3 - 5.5Mbps LONG
					4 - 5.5Mbps SHORT
					5 - 11Mbps LONG
					6 - 11Mbps SHORT
					7 - 11Mbps -- can be ignored
					8 - 6Mbps
					9 - 9Mbps
					10 - 12Mbps
					11 - 18Mbps
					12 - 24Mbps
					13 - 36Mbps
					14 - 48Mbps
					15 - 54Mbps
					16 - 6.5Mbps
					17 - 13Mbps
					18 - 19.5Mbps
					19 - 26Mbps
					20 - 39Mbps
					21 - 52Mbps
					22 - 58.5Mbps
					23 - 65Mbps
					24 - HT40 13.5Mbps
					25 - HT40 27Mbps
					26 - HT40 40.5Mbps
					27 - HT40 54Mbps
					28 - HT40 81Mbps
					29 - HT40 108Mbps
					30 - HT40 121.5Mbps
					31 - HT40 135Mbps

   	  dTxPwr = Tx Power in dBm (0.5 dBm steps)

         iAntenna 1 = antenna one
   	           2 = antenna two

   	  iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   			   1 = Anti-noise immunity (ANI) algorithm is enabled

   	  iScramblerOff = 1 Scrambler is disabled
   					= 0 Scrambler is enabled.  Scrambler is used with CCK frames

   	   iAifsn = value from 0 to 252,

   	   iPktSz = packet size, default is 1500.  Vary from 32 to 1500

   	   iTxPattern = Applies to CONT_TX_TX99, CONT_TX_TX100, and CONT_TX_MODE

   			0: ZEROES_PATTERN
   			1: ONES_PATTERN
   			2: REPEATING_10
   			3: PN7_PATTERN
   			4: PN9_PATTERN
   			5: PN15_PATTERN

   		iShortGuard = 1: Use short guard band, 0: Don't use

   		iNumPackets = number of packets to send, 0-65535

   		iWlanMode = WLAN channel mode:

   			0: NOHT
   			1: HT20 (20Mhz channel BW)
   			2: HT40PLUS (40MHz channel, primary channel high)
   			3: HT40MINUS (40MHz channel, primary channel low)

   		iTpcm = Tx Power Control Mode

   			0: Tx Power
   			1: Forced Gain
   			2: Target Power

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx
   (
      HANDLE hResourceContext,
	  int iMode,
	  int iFreq,
	  int iDataRate,
	  double dTxPwr,
	  int iAntenna,
	  int iEnANI,
	  int iScramblerOff,
	  int iAifsn,
	  int iPktSz,
	  int iTxPattern,
	  int iShortGuard,
	  int iNumPackets,
	  int iWlanMode,
	  int iTpcm, 
	  int txChain
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to enable continuous receive

   \param iAction =	0 = promiscuous mode (accept all incoming frames)
   				1 = filter mode (accept only frames with dest address equal specified mac address

   		iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   				 1=  Anti-noise immunity (ANI) algorithm is enabled

   		iFreq = Receive Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

   		iAntenna 1 = antenna one
				 2 = antenna two

   		iRxChain 1 = rx chain one
				 2 = rx chain two


   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Rx
   (
      HANDLE hResourceContext,
      unsigned long iAction,
      unsigned long iEnANI,
      unsigned long iFreq,
      unsigned long iAntenna,
      unsigned long iRxChain
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command to get receive report

   \param iTotalPkt =	Total packet received

   	   iRSSIinDbm = RSSI in dBm

   	   iCRCErrPkt = CRC Error Pkt

   	   iSECErrPkt = SEC Error Pkt

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Rx_Report
   (
      HANDLE hResourceContext,
      unsigned long* iTotalPkt,
      double*		 rssi,
      unsigned long* iCRCErrPkt,
      unsigned long* iSECErrPkt
   );

	   /******************************************************************************/
   /**

   Atheros WLAN FW command to read a register

   \param	iRegAddr  = register address

   			iRegValue = pointer to the register value read from register

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_ReadReg
   (
      HANDLE hResourceContext,
      unsigned long iRegAddr,
      unsigned long* iRegValue
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command to write a register

   \param	iRegAddr  = register address

   			iRegValue = register value to be written

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_WriteReg
   (
      HANDLE hResourceContext,
      unsigned long iRegAddr,
      unsigned long iRegValue
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command for reading data from OTP into a user supplied buffer.
   Maximum of 8 bytes can be read

   \param	iOffset_OTP =	offset from start of OTP

			iLength_OTP =	pointer to count of number of bytes to read, will be updated with 
				            number of bytes actually read on return.

   			iDataBuf_OTP =	pointer to OTP output data buffer, to be fill with bytes read
         
   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_ReadSegmentOTP
   (
      HANDLE hResourceContext,
	  unsigned long iOffset_OTP,
	  unsigned long* iLength_OTP,
	  unsigned char* iDataBuf_OTP
   );


   /******************************************************************************/
   /**

   Atheros WLAN command to enable/disable continuous transmit

   Note: Enable/disable continuous tx test cmd works only when target is awake.

   \param iMode =	0 = Disabling continuous Tx,
   				1 = Enabling continuous unmodulated Tx
   				2 = Enabling continuous modulated Tx

   	   iFreq = Transmit Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

   	   iDataRate =  0 - 1 Mbps
   					1 - 2 Mbps
   					2 - 5.5 Mbps
   					3 - 11 Mbps
   					4 - 6 Mbps
   					5 - 9 Mbps
   					6 - 12 Mbps
   					7 - 18 Mbps
   					8 - 24 Mbps
   					9 - 36 Mbps
   					10 - 48 Mbps
   					11 - 54 Mbps

   	  iTxPwr = Tx Power in dBm
   				5 - 11 dBm for unmodulated Tx
   				5 - 14 dBm for modulated Tx

         iAntenna 1 = antenna one
   	           2 = antenna two

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CONT_TRANSMIT
   (
      HANDLE hResourceContext,
      unsigned long iMode,
      unsigned long iFreq,
      unsigned long iDataRate,
      unsigned long iTxPwr,
      unsigned long iAntenna
   );


   /******************************************************************************/
   /**

   Atheros WLAN command to enable continuous receive

   \param iAction =	0 = promiscuous mode (accept all incoming frames)
   				1 = filter mode (accept only frames with dest address equal specified mac address

   	   iFreq = Receive Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

         iAntenna 1 = antenna one
   	           2 = antenna two

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CONT_RECEIVE
   (
      HANDLE hResourceContext,
      unsigned long iAction,
      unsigned long iFreq,
      unsigned long iAntenna
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to get receive report

   \param iTotalPkt =	Total packet received

   	   iRSSI = RSSI

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_RECEIVE_REPORT
   (
      HANDLE hResourceContext,
      unsigned long* iTotalPkt,
      unsigned long* iRSSI
   );
   /******************************************************************************/
   /**

   Atheros WLAN command to set MAC address

   \param iMAC_Address  =	MAC address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_MAC_ADDRESS
   (
      HANDLE hResourceContext,
      unsigned char aiMAC_Address[6]
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to force SLEEP or AWAKE

   \param iMode  =	1 = Wake up target
   				2 = Force the target to sleep

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SLEEP_WAKE
   (
      HANDLE hResourceContext,
      unsigned char iMode
   );

   /******************************************************************************/
   /**

   Atheros WLAN command to force SLEEP or AWAKE

   \param iMode  =	1 = Wake up target
   				2 = Force the target to sleep
   				3 = Force the target into deep sleep

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_SLEEP_WAKE
   (
      HANDLE hResourceContext,
      unsigned char iMode
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V2 command to enable/disable continuous transmit

   Note: Enable/disable continuous tx test cmd works only when target is awake.

   \param iMode =	0 = Disabling continuous Tx,
   				1 = Enabling continuous unmodulated Tx
   				2 = Enabling continuous modulated Tx

   	   iFreq = Transmit Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

   	   iDataRate =  0 - 1 Mbps
   					1 - 2 Mbps
   					2 - 5.5 Mbps
   					3 - 11 Mbps
   					4 - 6 Mbps
   					5 - 9 Mbps
   					6 - 12 Mbps
   					7 - 18 Mbps
   					8 - 24 Mbps
   					9 - 36 Mbps
   					10 - 48 Mbps
   					11 - 54 Mbps

   	  iTxPwr = Tx Power in dBm
   				5 - 11 dBm for unmodulated Tx
   				5 - 14 dBm for modulated Tx

         iAntenna 1 = antenna one
   	           2 = antenna two

   	  iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   			   1=  Anti-noise immunity (ANI) algorithm is enabled

   	  iScramblerOff = 1 Scrambler is disabled
   					= 0 Scrambler is enabled.  Scrambler is used with CCK frames

   	   iAifsn = value from 0 to 252,

   	   iPktSz = packet size, default is 1500.  Vary from 32 to 1500

   	   iTxPattern = Applies to CONT_TX_TX99, CONT_TX_TX100, and CONT_TX_MODE

   			ZEROES_PATTERN = 0,
   			ONES_PATTERN,
   			REPEATING_10,
   			PN7_PATTERN,
   			PN9_PATTERN,
   			PN15_PATTERN



   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V2_CONT_TRANSMIT
   (
      HANDLE hResourceContext,
      unsigned long iMode,
      unsigned long iFreq,
      unsigned long iDataRate,
      unsigned long iTxPwr,
      unsigned long iAntenna,
      unsigned long iEnANI,
      unsigned long iScramblerOff,
      unsigned long iAifsn,
      unsigned short  iPktSz,
      unsigned short  iTxPattern
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to enable/disable continuous transmit

   Note: Enable/disable continuous tx test cmd works only when target is awake.

   \param iMode =		0: TX_OFF
   					1: TX_SINE
   					2: TX_FRAME
   					3: TX_TX99
   					4: TX100


   	   iFreq = Transmit Frequency in MHz or Channel number
   	           For example, 2412 for channel 1 in 802.11g, or 1

   	   iDataRate =  0 - 1 Mbps
   					1 - 2 Mbps
   					2 - 5.5 Mbps
   					3 - 11 Mbps
   					4 - 6 Mbps
   					5 - 9 Mbps
   					6 - 12 Mbps
   					7 - 18 Mbps
   					8 - 24 Mbps
   					9 - 36 Mbps
   					10 - 48 Mbps
   					11 - 54 Mbps
   					12 - HT20 MCS0 6.5 Mbps
   					13 - HT20 MCS1 13  Mbps
   					14 - HT20 MCS2 19.5  Mbps
   					15 - HT20 MCS3 26  Mbps
   					16 - HT20 MCS4 39  Mbps
   					17 - HT20 MCS5 52  Mbps
   					18 - HT20 MCS6 58.5  Mbps
   					19 - HT20 MCS7 65  Mbps
   					20 - HT40 MCS0 13.5  Mbps
   					21 - HT40 MCS1 27.0  Mbps
   					22 - HT40 MCS2 40.5  Mbps
   					23 - HT40 MCS3 54  Mbps
   					24 - HT40 MCS4 81  Mbps
   					25 - HT40 MCS5 108  Mbps
   					26 - HT40 MCS6 121.5  Mbps
   					27 - HT40 MCS7 135  Mbps

   	  iTxPwr = Tx Power in dBm (0.5 dBm steps)

         iAntenna 1 = antenna one
   	           2 = antenna two

   	  iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   			   1 = Anti-noise immunity (ANI) algorithm is enabled

   	  iScramblerOff = 1 Scrambler is disabled
   					= 0 Scrambler is enabled.  Scrambler is used with CCK frames

   	   iAifsn = value from 0 to 252,

   	   iPktSz = packet size, default is 1500.  Vary from 32 to 1500

   	   iTxPattern = Applies to CONT_TX_TX99, CONT_TX_TX100, and CONT_TX_MODE

   			0: ZEROES_PATTERN
   			1: ONES_PATTERN
   			2: REPEATING_10
   			3: PN7_PATTERN
   			4: PN9_PATTERN
   			5: PN15_PATTERN

   		iShortGuard = 1: Use short guard band, 0: Don't use

   		iNumPackets = number of packets to send, 0-65535

   		iWlanMode = WLAN channel mode:

   			0: NOHT
        		1: HT20 (20Mhz channel BW)
   			2: HT40PLUS (40MHz channel, primary channel high)
   			3: HT40MINUS (40MHz channel, primary channel low)


   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_CONT_TRANSMIT(
      HANDLE hResourceContext,
      unsigned long  iMode,
      unsigned long  iFreq,
      unsigned long  iDataRate,
      unsigned long  iTxPwr,
      unsigned long  iAntenna,
      unsigned long  iEnANI,
      unsigned long  iScramblerOff,
      unsigned long  iAifsn,
      unsigned long  iPktSz,
      unsigned long  iTxPattern,
      unsigned long  iShortGuard,
      unsigned long  iNumPackets,
      unsigned long  iWlanMode);

   /******************************************************************************/
   /**

   Atheros WLAN FW V4 command to enable/disable continuous transmit

   Note: Enable/disable continuous tx test cmd works only when target is awake.

   \param iMode =		0: TX_OFF
   					1: TX_SINE
   					2: TX_FRAME
   					3: TX_TX99
   					4: TX100


   	   iFreq = Transmit Frequency in MHz or Channel number
   	           For example, 2412 for channel 1 in 802.11g, or 1

   	   iDataRate =  0 - 1 Mbps
   					1 - 2 Mbps
   					2 - 5.5 Mbps
   					3 - 11 Mbps
   					4 - 6 Mbps
   					5 - 9 Mbps
   					6 - 12 Mbps
   					7 - 18 Mbps
   					8 - 24 Mbps
   					9 - 36 Mbps
   					10 - 48 Mbps
   					11 - 54 Mbps
   					12 - HT20 MCS0 6.5 Mbps
   					13 - HT20 MCS1 13  Mbps
   					14 - HT20 MCS2 19.5  Mbps
   					15 - HT20 MCS3 26  Mbps
   					16 - HT20 MCS4 39  Mbps
   					17 - HT20 MCS5 52  Mbps
   					18 - HT20 MCS6 58.5  Mbps
   					19 - HT20 MCS7 65  Mbps
   					20 - HT40 MCS0 13.5  Mbps
   					21 - HT40 MCS1 27.0  Mbps
   					22 - HT40 MCS2 40.5  Mbps
   					23 - HT40 MCS3 54  Mbps
   					24 - HT40 MCS4 81  Mbps
   					25 - HT40 MCS5 108  Mbps
   					26 - HT40 MCS6 121.5  Mbps
   					27 - HT40 MCS7 135  Mbps

   	  dTxPwr = Tx Power in dBm (0.5 dBm steps)

         iAntenna 1 = antenna one
   	           2 = antenna two

   	  iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   			   1 = Anti-noise immunity (ANI) algorithm is enabled

   	  iScramblerOff = 1 Scrambler is disabled
   					= 0 Scrambler is enabled.  Scrambler is used with CCK frames

   	   iAifsn = value from 0 to 252,

   	   iPktSz = packet size, default is 1500.  Vary from 32 to 1500

   	   iTxPattern = Applies to CONT_TX_TX99, CONT_TX_TX100, and CONT_TX_MODE

   			0: ZEROES_PATTERN
   			1: ONES_PATTERN
   			2: REPEATING_10
   			3: PN7_PATTERN
   			4: PN9_PATTERN
   			5: PN15_PATTERN

   		iShortGuard = 1: Use short guard band, 0: Don't use

   		iNumPackets = number of packets to send, 0-65535

   		iWlanMode = WLAN channel mode:

   			0: NOHT
        		1: HT20 (20Mhz channel BW)
   			2: HT40PLUS (40MHz channel, primary channel high)
   			3: HT40MINUS (40MHz channel, primary channel low)

   		iTpcm = Tx Power Control Mode

   			0: Tx Power
   			1: Forced Gain
   			2: Target Power

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V4_CONT_TRANSMIT(
      HANDLE hResourceContext,
      unsigned long  iMode,
      unsigned long  iFreq,
      unsigned long  iDataRate,
      double  dTxPwr,
      unsigned long  iAntenna,
      unsigned long  iEnANI,
      unsigned long  iScramblerOff,
      unsigned long  iAifsn,
      unsigned long  iPktSz,
      unsigned long  iTxPattern,
      unsigned long  iShortGuard,
      unsigned long  iNumPackets,
      unsigned long  iWlanMode,
      unsigned long  iTpcm);

   /******************************************************************************/
   /**

   Atheros WLAN FW V2 command to enable continuous receive

   \param iAction =	0 = promiscuous mode (accept all incoming frames)
   				1 = filter mode (accept only frames with dest address equal specified mac address

   		iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   				 1=  Anti-noise immunity (ANI) algorithm is enabled

   	   iFreq = Receive Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

         iAntenna 1 = antenna one
   	           2 = antenna two

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V2_CONT_RECEIVE
   (
      HANDLE hResourceContext,
      unsigned long iAction,
      unsigned long iEnANI,
      unsigned long iFreq,
      unsigned long iAntenna
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to enable continuous receive

   \param iAction =	0 = promiscuous mode (accept all incoming frames)
   				1 = filter mode (accept only frames with dest address equal specified mac address

   		iEnANI = 0 = Anti-noise immunity (ANI) algorithm is disabled
   				 1=  Anti-noise immunity (ANI) algorithm is enabled

   	   iFreq = Receive Frequency in MHz
   	           For example, 2412 for channel 1 in 802.11g

         iAntenna 1 = antenna one
   	           2 = antenna two

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_CONT_RECEIVE
   (
      HANDLE hResourceContext,
      unsigned long iAction,
      unsigned long iEnANI,
      unsigned long iFreq,
      unsigned long iAntenna
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V2 command to get receive report

   \param iTotalPkt =	Total packet received

   	   iRSSI = RSSI

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V2_RECEIVE_REPORT
   (
      HANDLE hResourceContext,
      unsigned long* iTotalPkt,
      unsigned long* iRSSI
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V4 command to get receive report

   \param iTotalPkt =	Total packet received

   	   iRSSIinDbm = RSSI in dBm

   	   iCRCErrPkt = CRC Error Pkt

   	   iSECErrPkt = SEC Error Pkt

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V4_RECEIVE_REPORT
   (
      HANDLE hResourceContext,
      unsigned long* iTotalPkt,
      double*		   iRSSI,
      unsigned long* iCRCErrPkt,
      unsigned long* iSECErrPkt
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to get receive report

   \param iTotalPkt =	Total packet received

   	   iRSSI = RSSI

   	   iCRCErrPkt = CRC Error Pkt

   	   iSECErrPkt = SEC Error Pkt

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_RECEIVE_REPORT
   (
      HANDLE hResourceContext,
      unsigned long* iTotalPkt,
      unsigned long* iRSSI,
      unsigned long* iCRCErrPkt,
      unsigned long* iSECErrPkt
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command to get therm adc value

   \param iThermAdc = Therm ADC value

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_THERM
   (
      HANDLE hResourceContext,
      unsigned char* iThermAdc
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command to kickstart self init

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SELF_INIT_START
   (
     HANDLE hResourceContext
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW command to get self init results

   \param pPSatResultBuffer = Pointer to PSAT_CAL_RESULTS defined in QLib_Defines.h

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_SELF_INIT_RESULT
   (
	  HANDLE hResourceContext, 
      void* pPSatResultBuffer
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to set the RX filtering MAC address

   \param iMAC_Address  =	MAC address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_SET_MAC_ADDRESS
   (
      HANDLE hResourceContext,
      unsigned char aiMAC_Address[6]
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to set the NIC MAC address

   \param iMAC_Address  =	MAC address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_SET_NIC_MAC_ADDRESS
   (
      HANDLE hResourceContext,
      unsigned char aiMAC_Address[6]
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V3 command to set the cal file index

   \param iMAC_Address  =	MAC address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V3_SET_CALIBRATION_FILE_INDEX
   (
      HANDLE hResourceContext,
      int calFileIndex
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V2 command to set MAC address

   \param iMAC_Address  =	MAC address

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V2_SET_MAC_ADDRESS
   (
      HANDLE hResourceContext,
      unsigned char aiMAC_Address[6]
   );

   /******************************************************************************/
   /**

   Atheros WLAN FW V2 command to force SLEEP or AWAKE

   \param iMode  =	1 = Wake up target
   				2 = Force the target to sleep

   \return true if successful, false if failure

   \warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_V2_SLEEP_WAKE
   (
      HANDLE hResourceContext,
      unsigned long iMode
   );



#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QLIB_WLAN_ATHEROS_H)
